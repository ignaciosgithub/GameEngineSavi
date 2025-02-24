# Physics System for GameEngineSavi

This document describes the physics system implementation for the GameEngineSavi game engine.

## Features

- **Rigid Body Physics**: Simulates physical objects with mass, velocity, and forces
- **Collision Detection**: Detects collisions between objects in the scene
- **Variable Friction**: Configurable friction coefficients that are multiplicative with normal force
- **Gravity**: Configurable gravity value that can be enabled/disabled per object
- **Torque**: Rotational forces applied at collision points
- **Cross-Platform**: Works on both Windows and Linux

## Components

### PhysicsSystem

The main system that updates all rigid bodies and handles collision detection/resolution.

```cpp
PhysicsSystem physicsSystem;
physicsSystem.SetGravity(-9.81f);
```

### RigidBody

Represents a physical object with properties like mass, friction, and velocity.

```cpp
RigidBody* body = new RigidBody();
body->SetMass(1.0f);
body->SetFrictionCoefficient(0.5f);
body->EnableGravity(true);
```

### CollisionSystem

Handles collision detection and resolution between rigid bodies.

```cpp
CollisionSystem collisionSystem;
physicsSystem.SetCollisionSystem(&collisionSystem);
```

## Building the Demo

### Linux

```bash
make
./LinuxPhysicsDemo
```

### Windows

```bash
build_windows.bat
PhysicsDemo.exe
```

## Demo Description

The physics demo shows two red pyramids:
- The top pyramid has gravity enabled and falls onto the bottom pyramid
- The bottom pyramid has gravity disabled but is affected by collision forces
- Both pyramids have different friction coefficients
- Torque is applied during collisions, causing rotation

## Implementation Details

- Uses explicit Euler integration for simplicity
- Friction coefficients are multiplicative with normal force
- Collision response uses impulse-based resolution
- Torque is calculated from collision point and force vector
