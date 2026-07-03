*Front end User Interface in C for AI Stable diffusion C++*
Steps after downloading:
  1. you need model:https://huggingface.co/Lykon/dreamshaper-8-lcm/resolve/main/DreamShaper8_LCM.safetensors?download=true
  2. you need stable diffusion cpp: https://github.com/leejet/stable-diffusion.cpp
\
\
# Linux:\
(https://youtu.be/vENyZvjF5oY)
\
**Arch/Manjaro/Cachy:**
***Integrated GPU Vulkan***
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-intel vulkan-radeon
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
\
***AMD Radeon Vulkan***
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-radeon
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
\
***AMD Radeon ROCm/HIP***
sudo pacman -Syu --needed base-devel cmake git rocm-hip-sdk rocblas
git clone --recursive https://github.com
cd stable-diffusion.cpp
export ROCM_PATH=/opt/rocm
cmake -B build -DSD_HIPBLAS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
\
***NVIDIA(CUDA)***
sudo pacman -Syu --needed base-devel cmake git cuda nvidia-utils
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_CUDA=ON
cmake --build build --config Release --parallel
\
**Debian/Mint/Ubuntu:**
***Integrated GPU Vulkan***
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools mesa-vulkan-drivers
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
\
***AMD Radeon Vulkan***
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
\
***NVIDIA(CUDA)***
sudo apt update
sudo apt install -y build-essential cmake git pkg-config nvidia-cuda-toolkit
git clone --recursive https://github.com
cd stable-diffusion.cpp
cmake -B build -DSD_CUDA=ON
cmake --build build --config Release --parallel
\
\
\
\
\
Thank you for your support
