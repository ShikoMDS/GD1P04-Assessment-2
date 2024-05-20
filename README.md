ReadMe.txt
Project Overview
This OpenGL project demonstrates a 3D scene with instanced rendering and a moving object. It includes camera controls and a UI element that responds to user input.

Functionality
Instanced Rendering: Renders multiple instances of a model with random transformations.
Moving Object: An object that can be moved using keyboard controls.
Camera Controls: Allows the user to toggle between automatic and manual camera modes.
UI Element: Displays a UI quad that responds to mouse hover.
Controls
W/S/A/D: Move the object in the forward, backward, left, and right directions.
Q/E: Move the object up and down.
Arrow Keys: Control the camera manually.
1: Toggle cursor visibility.
2: Toggle wireframe mode.
3: Log the current mouse position.
Space: Toggle camera mode.
Escape: Exit the application.
Features
Instanced Rendering: Efficiently renders a large number of instances of a model with unique transformations.
Dynamic Camera: Supports both automatic and manual control modes.
Responsive UI: A UI element that changes texture when hovered over by the mouse.
Installation
Ensure you have the following dependencies installed:

OpenGL
GLFW
GLEW
GLM
stb_image
tinyobjloader
Clone the project repository:

git clone <repository-url>
Open the project in your preferred IDE (e.g., Visual Studio).

Build the project:

Ensure all dependencies are correctly linked.
Set the appropriate build configuration (e.g., Release or Debug).
Usage Instructions
Run the application. A window titled "OpenGL Demo 2" should appear.
Use the controls listed above to interact with the scene.
Observe the instanced rendering of models and the moving object.
Hover over the UI quad to see the texture change.
Additional Information
ShaderLoader: Handles loading and compiling shaders.
ModelLoader: Loads 3D models and textures.
Renderer: Manages rendering of the scene, including instanced models and the moving object.
Camera: Implements both automatic and manual camera controls.
UI: Manages the UI element and mouse interactions.