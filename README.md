# OpenGLWorkshop (for Assessment 2)  
  
This OpenGL project demonstrates a 3D scene with instanced rendering and a moving object created by Ayoub for GD1P04 3D Graphics Programming Assessment 2.  
  
  
## Table of Contents  
  
- [Description](#Description)  
- [Features](#Features)  
- [Requirements](#Requirements)  
- [Installation](#Installation)  
- [Controls](#Controls)  
- [Issues](#Issues)
- [Disclaimer](#Disclaimer)  
- [Credits](#Credits)  
  
  
## Description  
  
This OpenGL project demonstrates a 3D scene with instanced rendering and a moving object, including camera controls.  
A Visual Studio 2022 solution is provided, however "main.cpp" and individual source files "/src/" and include (header) files "/include/" can be accessed individually in their respective folders.  
Dependencies and resources are situated in their respective folders.  
  
  
## Features  
- Instanced Rendering: Efficiently renders a large number of instances of a model with unique transformations  
- Moving Object: An object that can be moved using keyboard controls  
- Dynamic Camera: Supports both automatic and manual control modes  
  
  
## Requirements  
  
- Visual Studio 2022 (v143)  
- Windows SDK 10.0  
- C++ Language Standard: ISO C++20 Standard (/std:c++20)  
- C Language Standard: ISO C17 (2018) Standard (/std:c17)  
- Windows x64  
- OpenGL dependencies (GLEW, GLFW, GLM)  
- STB dependency  
- tinyobjloader dependency  
  
  
## Installation  
  
Download and extract from the .zip file.  
This program can be run from the .exe file in the release folder. Resources are provided in their respective folder and dependencies are provided.  
This program can also be run by opening the "OpenGLWorkshop.sln" file in the source code folder and built in Debug or Release mode with Windows x64.  
  
  
## Controls  
  
The program has been designed to be operated with standard keyboard and mouse controls.  
  
OpenGL uses the right-handed coordinate system. The controls correspond to the direction the object faces (towards you).  
#### Object Movement  
- W: +Z axis movement  
- A: +X axis movement  
- S: -Z axis movement  
- D: -X axis movement  
- Q: -Y axis movement  
- E: +Y axis movement  
  
#### Camera Movement  
- Up Arrow: Zoom in (shortens distance to 0, 0, 0)  
- Down Arrow: Zoom out (increases distance from 0, 0, 0)  
- Left Arrow: Rotation clockwise around (0, 0, 0) along the (X, Z) plane  
- Right Arrow: Rotation anti-clockwise around (0, 0, 0) along the (X, Z) plane  
- Left Shift: Increases camera movement speed while held  
- Spacebar: Toggles automatic camera rotation  
  
#### Functions  
- 1: Toggles cursor visibility  
- 2: Toggles wire frame mode  
- 3: Print cursor coordinates to the console  
  
  
## Issues  
  
- Some mines still overlap.  
- Textured quad UI element has not been implemented.  
- Object movement directions derived from camera view continued to be problematic. Axis movement have been fixed to respective directions.  
- Was not able to rename the project without breaking it.  
DLL ISSUES - If there are issues with .dll files not being found, they must be moved into the correct directories.  
  
  
## Disclaimer  
  
This program is as complete as possible for submission. Errors should not be present and warnings should only come from STB and tinyobjloader.  
All code was written by Shiko based off my own knowledge from classes with lecturers and self driven research of the C++ coding language.  
This program has been cleaned for submission. Known issues have been searched for and fixed, unknown bugs may still be present.  
.vs/OpenGLWorkshop/v17/.suo is included for ease of solution loadup purposes due to the solution being manually configured. The solution should start with x64 Release, with main.cpp and FragmentShader.frag in side by side docks.  
  
  
## Credits  
  
Shikomisen (Ayoub) 2024
Media Design School  
GD1P04 - 3D Graphics Programming  
Written for Assignment 2  