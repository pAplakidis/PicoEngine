# LearnOpenGL Project

Cross-platform OpenGL project for **Ubuntu/Linux** and **Windows with Visual Studio**.

GLFW, GLAD, stb_image, and ImGui are included in the repository where applicable.

## Requirements

### Ubuntu / Linux

Install:

```bash
sudo apt update

sudo apt install \
  build-essential \
  cmake \
  libgl1-mesa-dev \
  libx11-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev
```

GLFW itself does **not** need to be installed with `apt`.

The project uses the bundled static library:

```text
Dependencies/GLFW/lib-linux-x86_64/libglfw3.a
```

### Windows

Install:

* Visual Studio 2022
* `Desktop development with C++` workload
* Windows SDK
* CMake tools for Windows

The project uses the bundled GLFW library:

```text
Dependencies/GLFW/lib-vc2022/glfw3.lib
```

No separate GLFW installation is required.

---

## Setup

Clone the repository:

```bash
git clone <repository-url>
cd LearnOpenGL
```

The expected dependency structure is:

```text
Dependencies/
├── GLFW/
│   ├── include/
│   │   └── GLFW/
│   ├── lib-linux-x86_64/
│   │   └── libglfw3.a
│   └── lib-vc2022/
│       └── glfw3.lib
│
└── GLAD/
    ├── include/
    └── src/
        └── glad.c
```

Application and Core code is located under:

```text
Application/
├── res/
└── src/

Core/
├── res/
└── src/
    └── vendor/
        ├── stb_image/
        └── imgui/
```

---

## Build on Ubuntu / Linux

Configure a debug build:

```bash
CC=clang CXX=clang++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Build:

```bash
cmake --build build -j$(nproc)
```

Run:

```bash
./build/bin/LearnOpenGL
```

For a release build:

```bash
CC=clang CXX=clang++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

To completely regenerate the build:

```bash
rm -rf build

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
```

---

## Build on Windows

Open **Developer PowerShell for Visual Studio 2022**.

Generate a Visual Studio project:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

Build Debug:

```powershell
cmake --build build --config Debug
```

Build Release:

```powershell
cmake --build build --config Release
```

The executable will normally be placed under:

```text
build/bin/Debug/
```

or:

```text
build/bin/Release/
```

You can also open the generated Visual Studio solution from the `build/` directory and build/run the project directly from Visual Studio.

Alternatively, Visual Studio can open the repository root directly as a CMake project:

```text
File → Open → Folder
```

Select the project root containing:

```text
CMakeLists.txt
```

