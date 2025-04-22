# Classic computer vision effects on your webcamera
A pet project desktop app that makes use of OpenCV, OpenGL and ImGui to apply various computer vision techniques to webcam stream. UI panels are dockable, you can "drag and drop" it and the configuration should be persistent between application runs. Performance is limited by your webcamera's maximum FPS (and it may change due to lighting conditions). The app has been tested on Windows 10 x64, OpenCV version 4.10.0

## Knows issues
1) Chromatic aberration produces artifacts and screen tearing at high offsets. Try to keep values closer to zero.

## How to build
CMake is used as a build system, so the process should be relatively straightforward. Though manual installation of OpenCV binaries is still required. Or you can try to build OpenCV from source. On Windows the process is as follows:
1) Download the latest LTS version of OpenCV binaries from [here](https://sourceforge.net/projects/opencvlibrary/files/). Unpack and run the installer.
2) Then create a system environment variable named OpenCV_DIR leading to the installed static libraries. For example, C:\PATHTOSTUFF\opencv\build\x64\vc16\lib
3) Clone this repository: git clone https://github.com/yomvol/CompVisionPostprocess.git
4) Into command prompt:
   ```
   mkdir build && cd build
   ```
   ```
   cmake .. -G "Visual Studio 17 2022"
   ```
   ```
   cmake --build . --config Release
   ```
