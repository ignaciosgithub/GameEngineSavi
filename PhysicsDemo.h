#ifndef PHYSICS_DEMO_H
#define PHYSICS_DEMO_H

#include <memory>
#include "GameObject.h"
#include "Vector3.h"

// Create a red pyramid
std::unique_ptr<GameObject> CreateRedPyramid(const std::string& name, const Vector3& position);

// Create a physics demo scene
void CreatePhysicsDemo();

#endif // PHYSICS_DEMO_H
