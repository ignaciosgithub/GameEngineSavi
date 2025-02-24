# Linux Physics Demo Executables

## Available Executables
- `SuperSimplePhysicsDemo` - A simplified physics demo that doesn't require OpenGL

## Running the Demos
To run the physics demos on Linux:
```
cd bin/linux
./SuperSimplePhysicsDemo
```

## Building from Source
If you want to build the demos from source:
```
# For the simplified demo
g++ -o bin/linux/SuperSimplePhysicsDemo SuperSimplePhysicsDemo.cpp Vector3.cpp -std=c++14

# For the full demo (requires OpenGL)
make physics_demo
```

## Demo Features
The physics demos demonstrate:
- Rigid body physics with variable mass
- Collision detection and response
- Variable friction coefficients
- Configurable gravity
- Torque calculation and application
