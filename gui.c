/*
Author: Stanislav petrek
Date: 1.july 2026

Project: AI-GUI for stable diffussion
  Build: gcc gui.c -o ai-gui `pkg-config --cflags --libs gtk+-3.0`
    Run: ./ai-gui

*/


#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

GtkWidget *text_view_prompt;
GtkWidget *text_view_neg_prompt;
GtkWidget *entry_path;
GtkWidget *scale_steps;
GtkWidget *scale_cfg;
GtkWidget *scale_seed;
GtkWidget *spin_steps;
GtkWidget *spin_cfg;
GtkWidget *spin_seed;
GtkWidget *image_view;
GtkWidget *progress_bar;
GtkWidget *btn_generate;
GtkWidget *btn_cancel;
GtkWidget *lbl_status_time;
GtkWidget *lbl_current_time;
GtkWidget *lbl_processing_time;

pid_t child_pid = 0;
pthread_t thread_id;
int total_steps_global = 6;
int elapsed_seconds = 0;
guint processing_timer_id = 0;

struct GenArgs {
    char command[4096];
    char exe_path[1024];
};

struct ProgressUpdate {
    int finished;
    char exe_path[1024];
};

gboolean update_current_time(gpointer data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time_str[64];
    snprintf(time_str, sizeof(time_str), "Current Time: %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    gtk_label_set_text(GTK_LABEL(lbl_current_time), time_str);
    return TRUE;
}

gboolean update_processing_time(gpointer data) {
    elapsed_seconds++;
    char proc_str[64];
    snprintf(proc_str, sizeof(proc_str), "Processing: %d sec", elapsed_seconds);
    gtk_label_set_text(GTK_LABEL(lbl_processing_time), proc_str);

    double est_total = total_steps_global * 6.0; 
    if (est_total < 10.0) est_total = 30.0;
    double fraction = (double)elapsed_seconds / est_total;
    if (fraction > 0.95) fraction = 0.95; 
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), fraction);    
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "");
    
    return TRUE;
}

gboolean gui_update_progress(gpointer data) {
    struct ProgressUpdate *up = (struct ProgressUpdate *)data;
    
    if (processing_timer_id > 0) {
        g_source_remove(processing_timer_id);
        processing_timer_id = 0;
    }
    
    if (up->finished == 1) {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "");
        
        gtk_image_set_from_file(GTK_IMAGE(image_view), "temp_output.png");
        
        int gen_min = elapsed_seconds / 60;
        int gen_sec = elapsed_seconds % 60;
        
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char auto_filename[1024];
        snprintf(auto_filename, sizeof(auto_filename), "%04d-%02d-%02d_%02d-%02d-%02d-(gen.time %dmin%02dsec).png",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, gen_min, gen_sec);
        
        char final_path[2048];
        snprintf(final_path, sizeof(final_path), "%s%s", up->exe_path, auto_filename);
        
        char cp_command[4096];
        snprintf(cp_command, sizeof(cp_command), "cp temp_output.png \"%s\"", final_path);
        system(cp_command);
        unlink("temp_output.png");
        
        char stats_str[2048];
        snprintf(stats_str, sizeof(stats_str), "Saved: %s", auto_filename);
        gtk_label_set_text(GTK_LABEL(lbl_status_time), stats_str);
        
        char proc_str[256];
        snprintf(proc_str, sizeof(proc_str), "Finished: %d sec", elapsed_seconds);
        gtk_label_set_text(GTK_LABEL(lbl_processing_time), proc_str);
    } else {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "");
        gtk_label_set_text(GTK_LABEL(lbl_status_time), "Canceled.");
        gtk_label_set_text(GTK_LABEL(lbl_processing_time), "Time: 0 sec");
    }
    
    gtk_widget_set_sensitive(btn_generate, TRUE);
    gtk_widget_set_sensitive(btn_cancel, FALSE);
    child_pid = 0;
    g_free(up);
    return FALSE;
}

void *worker_thread(void *data) {
    struct GenArgs *args = (struct GenArgs *)data;
    
    child_pid = 1; 
    int status = system(args->command);
    
    struct ProgressUpdate *up = g_new0(struct ProgressUpdate, 1);
    strcpy(up->exe_path, args->exe_path);
    
    if (status == 0) {
        up->finished = 1;
    } else {
        up->finished = -1;
    }
    
    g_main_context_invoke(NULL, gui_update_progress, up);
    g_free(args);
    return NULL;
}

void on_generate_clicked(GtkWidget *widget, gpointer data) {
    GtkTextBuffer *buffer_p = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_prompt));
    GtkTextIter start_p, end_p;
    gtk_text_buffer_get_bounds(buffer_p, &start_p, &end_p);
    const gchar *prompt = gtk_text_buffer_get_text(buffer_p, &start_p, &end_p, FALSE);

    GtkTextBuffer *buffer_n = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_neg_prompt));
    GtkTextIter start_n, end_n;
    gtk_text_buffer_get_bounds(buffer_n, &start_n, &end_n);
    const gchar *neg_prompt = gtk_text_buffer_get_text(buffer_n, &start_n, &end_n, FALSE);
    
    const gchar *sd_path = gtk_entry_get_text(GTK_ENTRY(entry_path));
    double steps = gtk_range_get_value(GTK_RANGE(scale_steps));
    double cfg = gtk_range_get_value(GTK_RANGE(scale_cfg));
    double seed_val = gtk_range_get_value(GTK_RANGE(scale_seed));
    
    total_steps_global = (int)steps;
    elapsed_seconds = 0;
    
    gtk_widget_set_sensitive(btn_generate, FALSE);
    gtk_widget_set_sensitive(btn_cancel, TRUE);
    gtk_label_set_text(GTK_LABEL(lbl_status_time), "Processing...");
    gtk_label_set_text(GTK_LABEL(lbl_processing_time), "Processing: 0 sec");

    struct GenArgs *args = g_new0(struct GenArgs, 1);

    int final_seed = (int)seed_val;
    if (final_seed == -1) {
        srand(time(NULL) ^ (getpid() << 16));
        final_seed = rand() % 100000;
    }

    snprintf(args->command, sizeof(args->command), 
             "\"%s\" -m \"/mnt/data/Programy/Linux/LM_AI/Image generating/stable-diffusion-cpp/dreamshaper_8LCM.safetensors\" -p \"%s\" --negative-prompt \"%s\" --steps %d --cfg-scale %.1f --seed %d -W 512 -H 512 -o temp_output.png > /dev/null 2>&1", 
             sd_path, prompt, neg_prompt, (int)steps, cfg, final_seed);
    
    ssize_t len = readlink("/proc/self/exe", args->exe_path, sizeof(args->exe_path) - 1);
    if (len != -1) {
        args->exe_path[len] = '\0';
        char *last_slash = strrchr(args->exe_path, '/');
        if (last_slash != NULL) {
            *(last_slash + 1) = '\0';
        }
    } else {
        strcpy(args->exe_path, "./");
    }

    processing_timer_id = g_timeout_add_seconds(1, update_processing_time, NULL);

    pthread_create(&thread_id, NULL, worker_thread, args);
    pthread_detach(thread_id);
}

void on_cancel_clicked(GtkWidget *widget, gpointer data) {
    system("killall sd-cli 2>/dev/null");
}

void on_scale_steps_changed(GtkRange *range, gpointer data) {
    double val = gtk_range_get_value(range);
    g_signal_handlers_block_by_func(spin_steps, G_CALLBACK(gtk_spin_button_set_value), NULL);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_steps), val);
    g_signal_handlers_unblock_by_func(spin_steps, G_CALLBACK(gtk_spin_button_set_value), NULL);
}

void on_spin_steps_changed(GtkSpinButton *spin, gpointer data) {
    double val = gtk_spin_button_get_value(spin);
    g_signal_handlers_block_by_func(scale_steps, G_CALLBACK(gtk_range_set_value), NULL);
    gtk_range_set_value(GTK_RANGE(scale_steps), val);
    g_signal_handlers_unblock_by_func(scale_steps, G_CALLBACK(gtk_range_set_value), NULL);
}

void on_scale_cfg_changed(GtkRange *range, gpointer data) {
    double val = gtk_range_get_value(range);
    g_signal_handlers_block_by_func(spin_cfg, G_CALLBACK(gtk_spin_button_set_value), NULL);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cfg), val);
    g_signal_handlers_unblock_by_func(spin_cfg, G_CALLBACK(gtk_spin_button_set_value), NULL);
}

void on_spin_cfg_changed(GtkSpinButton *spin, gpointer data) {
    double val = gtk_spin_button_get_value(spin);
    g_signal_handlers_block_by_func(scale_cfg, G_CALLBACK(gtk_range_set_value), NULL);
    gtk_range_set_value(GTK_RANGE(scale_cfg), val);
    g_signal_handlers_unblock_by_func(scale_cfg, G_CALLBACK(gtk_range_set_value), NULL);
}

void on_scale_seed_changed(GtkRange *range, gpointer data) {
    double val = gtk_range_get_value(range);
    g_signal_handlers_block_by_func(spin_seed, G_CALLBACK(gtk_spin_button_set_value), NULL);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_seed), val);
    g_signal_handlers_unblock_by_func(spin_seed, G_CALLBACK(gtk_spin_button_set_value), NULL);
}

void on_spin_seed_changed(GtkSpinButton *spin, gpointer data) {
    double val = gtk_spin_button_get_value(spin);
    g_signal_handlers_block_by_func(scale_seed, G_CALLBACK(gtk_range_set_value), NULL);
    gtk_range_set_value(GTK_RANGE(scale_seed), val);
    g_signal_handlers_unblock_by_func(scale_seed, G_CALLBACK(gtk_range_set_value), NULL);
}

void create_input_panel(GtkWidget *vbox_panel, GtkWidget *window) {
    GtkWidget *lbl_prompt, *scrolled_window_p, *lbl_neg_prompt, *scrolled_window_n;
    GtkWidget *lbl_steps, *hbox_steps, *lbl_cfg, *hbox_cfg, *lbl_seed, *hbox_seed, *lbl_path;
    GtkWidget *hbox_status_row;
    GtkTextBuffer *buffer_p, *buffer_n;

    lbl_prompt = gtk_label_new("Prompt:");
    gtk_widget_set_halign(lbl_prompt, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(vbox_panel), lbl_prompt, FALSE, FALSE, 0);
    scrolled_window_p = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_p), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window_p, -1, 90);
    gtk_box_pack_start(GTK_BOX(vbox_panel), scrolled_window_p, FALSE, FALSE, 0);
    text_view_prompt = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_prompt), GTK_WRAP_WORD_CHAR);
    buffer_p = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_prompt));
    gtk_text_buffer_set_text(buffer_p, "beautiful sandy beach, sunny day, turquoise ocean wave, small amount of palm trees", -1);
    gtk_container_add(GTK_CONTAINER(scrolled_window_p), text_view_prompt);

    lbl_neg_prompt = gtk_label_new("Negative Prompt:");
    gtk_widget_set_halign(lbl_neg_prompt, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(vbox_panel), lbl_neg_prompt, FALSE, FALSE, 0);
    scrolled_window_n = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_n), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window_n, -1, 70);
    gtk_box_pack_start(GTK_BOX(vbox_panel), scrolled_window_n, FALSE, FALSE, 0);
    text_view_neg_prompt = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_neg_prompt), GTK_WRAP_WORD_CHAR);
    buffer_n = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_neg_prompt));
    gtk_text_buffer_set_text(buffer_n, "blurry, low quality, distorted", -1);
    gtk_container_add(GTK_CONTAINER(scrolled_window_n), text_view_neg_prompt);

    hbox_steps = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_panel), hbox_steps, FALSE, FALSE, 2);
    lbl_steps = gtk_label_new("Steps:     ");
    gtk_box_pack_start(GTK_BOX(hbox_steps), lbl_steps, FALSE, FALSE, 0);
    scale_steps = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 20, 1);
    
    gtk_scale_add_mark(GTK_SCALE(scale_steps), 1.0, GTK_POS_BOTTOM, "1");
    for (int i = 2; i <= 18; i += 2) {
        char tick_str[8];
        snprintf(tick_str, sizeof(tick_str), "%d", i);
        gtk_scale_add_mark(GTK_SCALE(scale_steps), (double)i, GTK_POS_BOTTOM, tick_str);
    }
    gtk_scale_add_mark(GTK_SCALE(scale_steps), 20.0, GTK_POS_BOTTOM, "20");
    
    gtk_range_set_value(GTK_RANGE(scale_steps), 6);

    gtk_box_pack_start(GTK_BOX(hbox_steps), scale_steps, TRUE, TRUE, 0);
    spin_steps = gtk_spin_button_new_with_range(1, 20, 1);
    gtk_widget_set_valign(spin_steps, GTK_ALIGN_CENTER);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_steps), 6);
    gtk_box_pack_start(GTK_BOX(hbox_steps), spin_steps, FALSE, FALSE, 0);
    g_signal_connect(scale_steps, "value-changed", G_CALLBACK(on_scale_steps_changed), NULL);
    g_signal_connect(spin_steps, "value-changed", G_CALLBACK(on_spin_steps_changed), NULL);

    hbox_cfg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_panel), hbox_cfg, FALSE, FALSE, 2);
    lbl_cfg = gtk_label_new("CFG:        ");
    gtk_box_pack_start(GTK_BOX(hbox_cfg), lbl_cfg, FALSE, FALSE, 0);
    scale_cfg = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1.0, 10.0, 0.5);
    gtk_scale_add_mark(GTK_SCALE(scale_cfg), 2.0, GTK_POS_BOTTOM, "2.0");
    gtk_range_set_value(GTK_RANGE(scale_cfg), 2.0);
    gtk_box_pack_start(GTK_BOX(hbox_cfg), scale_cfg, TRUE, TRUE, 0);
    spin_cfg = gtk_spin_button_new_with_range(1.0, 10.0, 0.5);
    gtk_widget_set_valign(spin_cfg, GTK_ALIGN_CENTER);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cfg), 2.0);
    gtk_box_pack_start(GTK_BOX(hbox_cfg), spin_cfg, FALSE, FALSE, 0);
    g_signal_connect(scale_cfg, "value-changed", G_CALLBACK(on_scale_cfg_changed), NULL);
    g_signal_connect(spin_cfg, "value-changed", G_CALLBACK(on_spin_cfg_changed), NULL);

    hbox_seed = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_panel), hbox_seed, FALSE, FALSE, 2);
    lbl_seed = gtk_label_new("Seed:      ");
    gtk_box_pack_start(GTK_BOX(hbox_seed), lbl_seed, FALSE, FALSE, 0);
    scale_seed = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -1, 999999, 1);
    gtk_range_set_value(GTK_RANGE(scale_seed), -1);
    gtk_box_pack_start(GTK_BOX(hbox_seed), scale_seed, TRUE, TRUE, 0);
    spin_seed = gtk_spin_button_new_with_range(-1, 999999, 1);
    gtk_widget_set_valign(spin_seed, GTK_ALIGN_CENTER);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_seed), -1);
    gtk_box_pack_start(GTK_BOX(hbox_seed), spin_seed, FALSE, FALSE, 0);
    g_signal_connect(scale_seed, "value-changed", G_CALLBACK(on_scale_seed_changed), NULL);
    g_signal_connect(spin_seed, "value-changed", G_CALLBACK(on_spin_seed_changed), NULL);

    gtk_widget_set_tooltip_text(scale_steps, "Steps are a linear range, depending on processing the whole image generation loop. Default value is 6.");
    gtk_widget_set_tooltip_text(spin_steps, "Steps are a linear range, depending on processing the whole image generation loop. Default value is 6.");

    gtk_widget_set_tooltip_text(scale_cfg, "CFG Scale adjusts how closely the model follows your text prompt. Default value is 2.0.");
    gtk_widget_set_tooltip_text(spin_cfg, "CFG Scale adjusts how closely the model follows your text prompt. Default value is 2.0.");

    gtk_widget_set_tooltip_text(scale_seed, "Seed controls the generation randomness. Set to -1 for a completely random image every time.");
    gtk_widget_set_tooltip_text(spin_seed, "Seed controls the generation randomness. Set to -1 for a completely random image every time.");


    lbl_path = gtk_label_new("Path to sd-cli:");
    gtk_widget_set_halign(lbl_path, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(vbox_panel), lbl_path, FALSE, FALSE, 0);
    entry_path = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_path), "/mnt/data/Programy/Linux/LM_AI/Image generating/stable-diffusion-cpp/bin/sd-cli");
    gtk_box_pack_start(GTK_BOX(vbox_panel), entry_path, FALSE, FALSE, 0);

    btn_generate = gtk_button_new_with_label("Generate");
    g_signal_connect(btn_generate, "clicked", G_CALLBACK(on_generate_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox_panel), btn_generate, FALSE, FALSE, 8);

    btn_cancel = gtk_button_new_with_label("Cancel");
    gtk_widget_set_sensitive(btn_cancel, FALSE);
    g_signal_connect(btn_cancel, "clicked", G_CALLBACK(on_cancel_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox_panel), btn_cancel, FALSE, FALSE, 4);

    progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox_panel), progress_bar, FALSE, FALSE, 4);

    hbox_status_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_panel), hbox_status_row, FALSE, FALSE, 4);

    lbl_processing_time = gtk_label_new("Processing: 0 sec");
    gtk_box_pack_start(GTK_BOX(hbox_status_row), lbl_processing_time, TRUE, TRUE, 0);

    lbl_current_time = gtk_label_new("Time: 00:00:00");
    gtk_box_pack_start(GTK_BOX(hbox_status_row), lbl_current_time, TRUE, TRUE, 0);

    lbl_status_time = gtk_label_new("Ready");
    gtk_box_pack_start(GTK_BOX(vbox_panel), lbl_status_time, FALSE, FALSE, 4);

    g_timeout_add_seconds(1, update_current_time, window);
    update_current_time(window);
}

void setup_css_styles(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        "button { background-color: #FF8C00; color: white; background-image: none; font-weight: bold; } "
        "button:active { background-color: #CC7000; } "
        "button:disabled { background-color: #D66F00; color: #E0E0E0; } "
        "entry { min-height: 24px; } "
        "spinbutton { min-height: 24px; } "
        "label { font-size: 11px; }", -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(btn_generate);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *hbox, *vbox_panel;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "AI-GUI for stable-diffusion-cpp");
    gtk_window_set_default_size(GTK_WINDOW(window), 1080, 720);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    image_view = gtk_image_new();
    gtk_image_set_from_icon_name(GTK_IMAGE(image_view), "image-missing", GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start(GTK_BOX(hbox), image_view, TRUE, TRUE, 10);

    vbox_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox), vbox_panel, FALSE, FALSE, 10);
    gtk_widget_set_size_request(vbox_panel, 360, -1);

    create_input_panel(vbox_panel, window);
    setup_css_styles();

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
