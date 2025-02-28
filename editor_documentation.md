# GameEngineSavi Editor Documentation

The GameEngineSavi editor provides a visual interface for creating and editing game scenes. This document provides detailed information on how to use the editor effectively.

## Editor Interface

The editor interface consists of several panels:

```
+----------------------------------------------------------------------------------------------------------+
|                                       GameEngineSavi Editor                                              |
+------------------+-----------------------------------------------+-----------------------------------+
|  Hierarchy       |                                               |  Inspector                        |
|                  |                                               |                                   |
|  > Default Light |                  Scene View                   |  Transform                        |
|  > Default Cube  |                                               |  Position: X: 0  Y: 0  Z: 0       |
|                  |                                               |  Rotation: X: 0  Y: 0  Z: 0       |
|                  |                                               |  Scale:    X: 1  Y: 1  Z: 1       |
|                  |                                               |                                   |
|                  |                                               |  Material                         |
|                  |                                               |  Albedo: [Default]                |
|                  |                                               |  Normal Map: [None]               |
|                  |                                               |  Opacity: 1.0                     |
|                  |                                               |                                   |
|                  |                                               |  Physics                          |
|                  |                                               |  [x] Use Gravity                  |
|                  |                                               |  [ ] Is Kinematic                 |
|                  |                                               |  Friction: 0.5                    |
+------------------+-----------------------------------------------+-----------------------------------+
|                                                                                                      |
|  Project                                                                                             |
|  [Models] [Textures] [Scenes] [Scripts] [Prefabs] [Audio]                                            |
|                                                                                                      |
+----------------------------------------------------------------------------------------------------------+
```

### Panels

#### Hierarchy Panel
The Hierarchy Panel lists all objects in the current scene. By default, a new scene includes:
- Default Light: A point light positioned at (0, 5, 0)
- Default Cube: A cube positioned at (0, 0, 0)

You can:
- Click on an object to select it
- Right-click to create new objects
- Drag objects to reorder them
- Use the context menu to duplicate, delete, or rename objects

#### Scene View
The Scene View displays the 3D scene with the default cube and lighting. This is where you can:
- View and manipulate objects in 3D space
- Use the gizmos to move, rotate, and scale objects
- Navigate the scene using camera controls

#### Inspector Panel
The Inspector Panel shows properties of the selected object, including:
- Transform: Position, rotation, and scale
- Material: Albedo texture, normal map, opacity
- Physics: Gravity, kinematic state, friction
- Components: Any attached components and their properties

#### Project Panel
The Project Panel displays project assets organized by type:
- Models: 3D models (.obj files)
- Textures: Image files for materials
- Scenes: Saved scene files
- Scripts: C++ script files
- Prefabs: Reusable object templates
- Audio: Sound files

## Camera Controls

The editor camera is positioned at (0, 2, 5) by default, looking at the origin (0, 0, 0) where the default cube is placed.

- **WASD**: Move the camera
  - W: Move forward
  - A: Move left
  - S: Move backward
  - D: Move right
- **Mouse**: Look around by clicking and dragging in the Scene View
- **Shift**: Hold to move the camera faster
- **Space**: Play/Pause the scene to test game logic

## Creating Objects

1. Right-click in the Hierarchy panel
2. Select "Create" and choose an object type:
   - Empty GameObject
   - Cube
   - Sphere
   - Light
   - Camera
3. The new object will appear in the scene

## Editing Objects

1. Select an object in the Hierarchy panel
2. Modify its properties in the Inspector panel:
   - Transform (position, rotation, scale)
   - Material properties (albedo, normal map, opacity)
   - Physics settings (gravity, kinematic, friction)
3. Changes are applied immediately to the scene

## Saving and Loading Scenes

- **Ctrl+S**: Save the current scene
- **Ctrl+O**: Open a scene
- **Ctrl+N**: Create a new scene

## Editor Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl+N | New Scene |
| Ctrl+O | Open Scene |
| Ctrl+S | Save Scene |
| Ctrl+Shift+S | Save Scene As |
| F | Focus Selected |
| W | Move Tool |
| E | Rotate Tool |
| R | Scale Tool |
| Q | No Tool (Selection Only) |
| Delete | Delete Selected |
| Ctrl+D | Duplicate Selected |
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| Space | Play/Stop |
| Ctrl+1 | Hierarchy Panel Focus |
| Ctrl+2 | Scene View Focus |
| Ctrl+3 | Inspector Panel Focus |
| Ctrl+4 | Project Panel Focus |

## Default Scene

When you start the editor, a default scene is created with:

1. A default cube at position (0, 0, 0)
2. A default point light at position (0, 5, 0)
3. The editor camera at position (0, 2, 5) looking at the origin

This provides a good starting point for creating new scenes.

## Troubleshooting

If you encounter issues with the editor:

1. **White or blank scene view**: This may be due to:
   - Camera position being too close to objects
   - Missing OpenGL drivers
   - Graphics card compatibility issues

2. **Objects not visible**: Check:
   - Camera position and orientation
   - Object position (may be outside camera view)
   - Object scale (may be too small)

3. **Editor crashes**: Ensure:
   - Your graphics drivers are up to date
   - You have sufficient RAM
   - You're using a compatible GPU

## Building from Source

If you want to build the editor from source:

### Linux
```bash
# Build the editor
./build_editor.sh

# Run the editor
./run_editor.sh
```

### Windows
```batch
# Using Visual Studio
# Open GameEngineSavi.sln and build the solution

# Using MinGW
mingw32-make -f Makefile.mingw editor

# Run the editor
run_editor.bat
```
