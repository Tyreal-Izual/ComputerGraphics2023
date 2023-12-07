# ComputerGraphics2023

## Overview
This project is a custom 3D rendering engine developed in C++. It supports different rendering modes including Wireframe, Rasterization, and Ray Tracing. The engine is capable of handling object transformations, camera control, light calculations, and texture mapping.

## Features
- **Multiple Rendering Modes**: Wireframe, Rasterisation, and Ray Tracing.
- **Camera Control**: Movement and rotation of the camera in 3D space.
- **Lighting**: Basic lighting models with support for shadows, reflection and refraction.
- **Object Loading**: Load 3D objects from `.obj` files with material `.mtl` support.
- **Photon Mapping**: For advanced lighting in Ray Tracing mode.

## Requirements
- C++ compiler
- SDL2 library for creating windows and handling events
- GLM library for mathematics

## Installation
1. Ensure you have SDL2 and GLM installed on your system.
2. Clone the repository to your local machine.
3. Compile the code using:  
  ```bash
  make 
  ```
4. You may need to use:  
  ```bash 
  make clean
  ```  
  first.  
5. Run the compiled executable.

## Usage
- Run the program. A window will open displaying the rendered scene.
- Use keyboard inputs to interact with the scene:
    - Arrow keys for camera movement.
    - `W` and `S` for moving the camera forward and backward.
    - `A` and `D` for camera rotation.
    - Number keys `1`, `2`, `3` to switch between Wireframe, Rasterisation, and Ray Tracing modes.
    - Other keys for specific actions like light movement and camera tilt.
- Close the window by `esc`or terminate the program to exit.

## Code Structure
- `main()`: The entry point of the program. Initializes the window and enters the render loop.
- Rendering Functions:
    - `drawWireframeScene()`: Renders the scene in wireframe mode.
    - `drawRasterisedScene()`: Renders the scene using rasterization.
    - `drawRayTracedScene()`: Renders the scene using ray tracing.
- Utility Functions:
    - `loadOBJWithMaterials()`: Loads a 3D object with materials.
    - `handleEvent()`: Handles user input events.


[//]: # (## License)

[//]: # (This project is licensed under the [MIT License]&#40;LICENSE&#41;.)


## The following is what I've done in CG2023:

### Improvement and new feature for CW:
Improved version:  
![improved.png](photos%2Fimproved.png)  
Applied photon map:  
![Screenshot 2023-12-05 at 5.32.13 AM.png](photos%2FScreenshot%202023-12-05%20at%205.32.13%E2%80%AFAM.png)  
Photon map:  
![photon map.png](photos%2Fphoton%20map.png)  
Improve soft shadow to make sure it's really soft and as well for the sphere:  
![soft shadow.png](photos%2Fsoft%20shadow.png)  
Environment map:  
![environment.png](photos%2Fenvironment.png)  
Add refractive material to sphere:  
![refractive.png](photos%2Frefractive.png)  
Smooth and elegant soft shadow (Area Lights with Jittered Sampling)  
![Smooth and elegant soft shadows.png](photos%2FSmooth%20and%20elegant%20soft%20shadows.png)  
Rough attempt at soft shadow with multipoint light sources (area light)  
![Rough attempt at soft shadows (multi-point light sources).png](photos%2FRough%20attempt%20at%20soft%20shadows%20%28multi-point%20light%20sources%29.png)  
Reflective materials for Blue Rectangle:  
![Reflective materials.png](photos%2FReflective%20materials.png)  
Add sphere to the rendering window:  
![sphere.png](photos%2Fsphere.png)  


### The procedure for week7:  
The following is 3 different light position of the object:  
(0,0.5,0):  
![(0,0.5,0).png](photos%2F%280%2C0.5%2C0%29.png)  
(0,0.5,0.5):  
![(0,0.5,0.5).png](photos%2F%280%2C0.5%2C0.5%29.png)  
(0,0.8,0):  
![(0,0.8,0).png](photos%2F%280%2C0.8%2C0%29.png)  
Movable light source (it is slow due to slow rendering after each move):  
![ezgif.com-video-to-gif.gif](photos%2Fezgif.com-video-to-gif.gif)
Task2-3  
![Task2.png](photos%2FTask2.png)  

### The procedure for week6:  
Ray Tracing the Scene:  
![RayTracingtheScene.png](photos%2FRayTracingtheScene.png)  
Shadow:  
![shadow.png](photos%2Fshadow.png)  
Changing:  
![Final.gif](photos%2FFinal.gif)

### The procedure for week5:  
Translations:
![translation.gif](photos%2Ftranslation.gif)  
Look up/down, right/left:  
![pan&tilt.gif](photos%2Fpan%26tilt.gif)  
Rotation:  
![rotation.gif](photos%2Frotation.gif)  
Front of the object:  
![front.png](photos%2Ffront.png)  

### The procedure for week4:  
Starting of the week4, stuck at task6(showing 'stars' but not same one as given).  
So i decided to connect the stars as the line first, and found the problem:  
![week4,1.JPG](photos%2Fweek4%2C1.JPG)  
![week4,2.jpg](photos%2Fweek4%2C2.jpg)  
I change the the code: float zi = vertexPosition.z - cameraPosition.z  to 
float zi = cameraPosition.z - vertexPosition.z; and fixed the problem.  
![week4,3.jpg](photos%2Fweek4%2C3.jpg)  
The filledTriangle is not fully right, I did some updates on that and fix the colour part.  
![week4,4.jpg](photos%2Fweek4%2C4.jpg)  
![week4,5.jpg](photos%2Fweek4%2C5.jpg)  
Starting of depth process:  
![week4,6.JPG](photos%2Fweek4%2C6.JPG)  
Final view :)   
![week4,7.jpeg](photos%2Fweek4%2C7.jpeg)  

### The procedure for week3 Task 5-6 as following:  
![1.JPG](photos%2F1.JPG)  
![2.JPG](photos%2F2.JPG)  
![3.jpeg](photos%2F3.jpeg)  
![4.jpeg](photos%2F4.jpeg)  
![5.jpeg](photos%2F5.jpeg)  


