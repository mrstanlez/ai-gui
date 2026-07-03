**Front end User Interface in C for AI Stable diffusion C++**

Steps after downloading:
  1. you need model:https://huggingface.co/Lykon/dreamshaper-8-lcm/resolve/main/DreamShaper8_LCM.safetensors?download=true
  2. you need stable diffusion cpp: https://github.com/leejet/stable-diffusion.cpp

# Linux:
(https://youtu.be/vENyZvjF5oY)

**Arch/Manjaro/Cachy:**

<details>
<summary>💻 <i>Integrated GPU Vulkan</i></summary>

```bash
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-intel vulkan-radeon
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟥 <i>AMD Radeon Vulkan</i></summary>

```bash
sudo pacman -Syu --needed base-devel cmake git shaderc spirv-headers vulkan-headers vulkan-icd-loader vulkan-tools vulkan-radeon
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟥 <i>AMD Radeon ROCm/HIP</i></summary>

```bash
sudo pacman -Syu --needed base-devel cmake git rocm-hip-sdk rocblas
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
export ROCM_PATH=/opt/rocm
cmake -B build -DSD_HIPBLAS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟦 <i>Intel Arc / Graphics (SYCL / oneAPI)</i></summary>

```bash
sudo pacman -Syu --needed base-devel cmake git intel-oneapi-base-toolkit
source /opt/intel/oneapi/setvars.sh
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟩 <i>NVIDIA(CUDA)</i></summary>

```bash
sudo pacman -Syu --needed base-devel cmake git cuda nvidia-utils
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_CUDA=ON
cmake --build build --config Release --parallel
```
</details>

**Debian/Mint/Ubuntu:**

<details>
<summary>💻 <i>Integrated GPU Vulkan</i></summary>

```bash
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools mesa-vulkan-drivers
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟥 <i>AMD Radeon Vulkan</i></summary>

```bash
sudo apt update
sudo apt install -y build-essential cmake git glslc libvulkan-dev pkg-config spirv-headers vulkan-tools
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_VULKAN=ON
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟦 <i>Intel Arc / Graphics (SYCL / oneAPI)</i></summary>

```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config
source /opt/intel/oneapi/setvars.sh
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟩 <i>NVIDIA(CUDA)</i></summary>

```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config nvidia-cuda-toolkit
git clone --recursive https://github.com/leejet/stable-diffusion.cpp
cd stable-diffusion.cpp
cmake -B build -DSD_CUDA=ON
cmake --build build --config Release --parallel
```
</details>

Thank you for your support
