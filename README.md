# Urho3D-Color-Lines

The "bin" folder contains a compiled version of the game.

![screencolorlines](https://cloud.githubusercontent.com/assets/13021826/9157246/4c0a8fb0-3f07-11e5-9ec6-c963a93e6228.jpg)

Esc - open/close Menu

Compiling in Visual Studio:
* In CMakeLists.txt change paths
   * set (CMAKE_MODULE_PATH [Urho3D sources]/CMake/Modules)
   * set (ENV{URHO3D_HOME} [Urho3D compiled verison])
* CMake: all parametrs (OpenGL, static runtime, etc) must be the same as that of the engine
* Open Urho3DColorLines.sln and set "Lines" as startup project
* Build

License: MIT

http://3dgra.blogspot.com


