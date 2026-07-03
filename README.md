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
<summary>🟥 AMD Radeon (Vulkan)</summary>

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
<summary>🟥 AMD Radeon (ROCm / HIP)</summary>

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
<summary>🟦 Intel Arc / Graphics (SYCL / oneAPI)</summary>

```bash
# Update system and install Intel oneAPI Base Toolkit packages
sudo pacman -Syu --needed base-devel cmake git intel-oneapi-base-toolkit

# Activate the Intel environment variables
source /opt/intel/oneapi/setvars.sh

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure using Intel C/C++ compilers and enable SYCL acceleration
cmake -B build -DSD_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟩 NVIDIA (CUDA)</summary>

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
<summary>🟥 AMD Radeon (Vulkan)</summary>

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
<summary>🟦 Intel Arc / Graphics (SYCL / oneAPI)</summary>

```bash
# Update package list and install build prerequisites
sudo apt update
sudo apt install -y build-essential cmake git pkg-config

# Note: Ensure you have installed the Intel oneAPI Base Toolkit before running this
source /opt/intel/oneapi/setvars.sh

# Clone the repository with all submodules
git clone --recursive https://github.com
cd stable-diffusion.cpp

# Configure using Intel C/C++ compilers and enable SYCL acceleration
cmake -B build -DSD_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx

# Compile the project using all available CPU cores
cmake --build build --config Release --parallel
```
</details>

<details>
<summary>🟩 NVIDIA (CUDA)</summary>

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
