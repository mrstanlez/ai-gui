# 🚀 C UI for Stable Diffusion C++

> A clean, fast, and lightweight User Interface (UI) written in C for the `stable-diffusion.cpp` backend.

---

## 📌 Prerequisites

Before running the application, you need to download the required AI model and the backend repository:

1. **Download the AI Model:** [DreamShaper 8 LCM (.safetensors)](https://huggingface.co)
2. **Download the Backend:** [stable-diffusion.cpp](https://github.com)

---

## 🛠️ Installation Guide (Linux)

Select your Linux distribution and GPU type below to reveal the specific installation commands:

### 📐 Arch / Manjaro / CachyOS

<details>
<summary>💻 Integrated GPU (Vulkan)</summary>

```bash
# Update system and install required build tools and Vulkan headers
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-intel vulkan-radeon

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with Vulkan support enabled
cmake -B build -DSD_VULKAN=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>❤️ AMD Radeon (Vulkan)</summary>

```bash
# Update system and install Vulkan drivers optimized for AMD Radeon
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-radeon

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with Vulkan support enabled
cmake -B build -DSD_VULKAN=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>💙 AMD Radeon (ROCm / HIP)</summary>

```bash
# Update system and install AMD ROCm and HIP SDK for hardware acceleration
sudo pacman -Syu --needed base-devel cmake git rocm-hip-sdk rocblas

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Set the environment path for ROCm and configure the HIPBLAS build
export ROCM_PATH=/opt/rocm
cmake -B build -DSD_HIPBLAS=ON -DCMAKE_BUILD_TYPE=Release

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>💚 NVIDIA (CUDA)</summary>

```bash
# Update system and install official NVIDIA toolkit and CUDA libraries
sudo pacman -Syu --needed base-devel cmake git cuda nvidia-utils

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with CUDA acceleration enabled
cmake -B build -DSD_CUDA=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

---

### 🐧 Debian / Ubuntu / Linux Mint

<details>
<summary>💻 Integrated GPU (Vulkan)</summary>

```bash
# Update package list and install system build essentials and Vulkan drivers
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools mesa-vulkan-drivers

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with Vulkan support enabled
cmake -B build -DSD_VULKAN=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>❤️ AMD Radeon (Vulkan)</summary>

```bash
# Update package list and install Vulkan libraries for AMD
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with Vulkan support enabled
cmake -B build -DSD_VULKAN=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>💚 NVIDIA (CUDA)</summary>

```bash
# Update package list and install NVIDIA CUDA toolkit for Linux
sudo apt update
sudo apt install -y build-essential cmake git pkg-config nvidia-cuda-toolkit

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure the build with CUDA acceleration enabled
cmake -B build -DSD_CUDA=ON

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

---

## 📺 Video Tutorial
For a step-by-step visual guide, watch this [YouTube Video](https://youtu.be).

---

## 🤝 Support & Acknowledgements
Thank you for using this project and supporting its development!
