#include "platform.h"
#include <memory>
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "Pyramid.h"
#include "Vector3.h"
#include "PointLight.h"
#include "Face.h"
// Don't include Triangle.h here to avoid redefinition

// C++11 implementation of make_unique (since we're targeting C++11)
namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

// Create a red pyramid
std::unique_ptr<GameObject> CreateRedPyramid(const std::string& name, const Vector3& position) {
    // Create a pyramid mesh
    std::vector<Face> faces;
    
    // Base (square)
    Face baseFace;
    Triangle baseTriangle1;
    baseTriangle1.vertices[0] = Vector3(-0.5f, -0.5f, -0.5f);
    baseTriangle1.vertices[1] = Vector3(0.5f, -0.5f, -0.5f);
    baseTriangle1.vertices[2] = Vector3(0.5f, -0.5f, 0.5f);
    baseFace.triangles.push_back(baseTriangle1);
    
    Triangle baseTriangle2;
    baseTriangle2.vertices[0] = Vector3(-0.5f, -0.5f, -0.5f);
    baseTriangle2.vertices[1] = Vector3(0.5f, -0.5f, 0.5f);
    baseTriangle2.vertices[2] = Vector3(-0.5f, -0.5f, 0.5f);
    baseFace.triangles.push_back(baseTriangle2);
    faces.push_back(baseFace);
    
    // Front face (triangle)
    Face frontFace;
    Triangle frontTriangle;
    frontTriangle.vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
    frontTriangle.vertices[1] = Vector3(-0.5f, -0.5f, 0.5f);
    frontTriangle.vertices[2] = Vector3(0.5f, -0.5f, 0.5f);
    frontFace.triangles.push_back(frontTriangle);
    faces.push_back(frontFace);
    
    // Back face (triangle)
    Face backFace;
    Triangle backTriangle;
    backTriangle.vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
    backTriangle.vertices[1] = Vector3(0.5f, -0.5f, -0.5f);
    backTriangle.vertices[2] = Vector3(-0.5f, -0.5f, -0.5f);
    backFace.triangles.push_back(backTriangle);
    faces.push_back(backFace);
    
    // Left face (triangle)
    Face leftFace;
    Triangle leftTriangle;
    leftTriangle.vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
    leftTriangle.vertices[1] = Vector3(-0.5f, -0.5f, -0.5f);
    leftTriangle.vertices[2] = Vector3(-0.5f, -0.5f, 0.5f);
    leftFace.triangles.push_back(leftTriangle);
    faces.push_back(leftFace);
    
    // Right face (triangle)
    Face rightFace;
    Triangle rightTriangle;
    rightTriangle.vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
    rightTriangle.vertices[1] = Vector3(0.5f, -0.5f, 0.5f);
    rightTriangle.vertices[2] = Vector3(0.5f, -0.5f, -0.5f);
    rightFace.triangles.push_back(rightTriangle);
    faces.push_back(rightFace);
    
    // Create the pyramid
    Pyramid* pyramid = new Pyramid(faces, Vector3(0, 0, 0), position);
    
    // Create a game object with the pyramid
    std::vector<PointLight> lights;
    auto gameObject = std::make_unique<GameObject>(
        name,
        position,
        Vector3(0, 0, 0),
        Vector3(1, 1, 1),
        lights
    );
    
    // Add the pyramid as a mesh
    gameObject->AddMesh(pyramid);
    
    return gameObject;
}

// Create a physics demo scene
void CreatePhysicsDemo() {
    // Create a scene
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    
    // Load the scene (initializes Time)
    scene->Load();
    
    // Create physics and collision systems
    std::unique_ptr<PhysicsSystem> physicsSystem = std::make_unique<PhysicsSystem>();
    std::unique_ptr<CollisionSystem> collisionSystem = std::make_unique<CollisionSystem>();
    
    // Connect the systems
    physicsSystem->SetCollisionSystem(collisionSystem.get());
    
    // Set gravity
    physicsSystem->SetGravity(-9.81f);
    
    // Create top pyramid with gravity
    auto topPyramid = CreateRedPyramid("TopPyramid", Vector3(0, 2, 0));
    auto topRb = std::make_unique<RigidBody>();
    topRb->EnableGravity(true);
    topRb->SetFrictionCoefficient(0.5f);
    topRb->SetMass(1.0f);
    topRb->SetGameObject(topPyramid.get());
    
    // Add the rigid body to the physics system
    physicsSystem->AddBody(topRb.get());
    
    // Add the rigid body to the game object
    topPyramid->AddComponent(std::move(topRb));
    
    // Create bottom pyramid without gravity
    auto bottomPyramid = CreateRedPyramid("BottomPyramid", Vector3(0, 0, 0));
    auto bottomRb = std::make_unique<RigidBody>();
    bottomRb->EnableGravity(false);
    bottomRb->SetFrictionCoefficient(0.3f);
    bottomRb->SetMass(2.0f);
    bottomRb->SetGameObject(bottomPyramid.get());
    
    // Add the rigid body to the physics system
    physicsSystem->AddBody(bottomRb.get());
    
    // Add the rigid body to the game object
    bottomPyramid->AddComponent(std::move(bottomRb));
    
    // Add nearby object
    auto nearbyObject = CreateRedPyramid("NearbyObject", Vector3(2, 1, 0));
    
    // Add game objects to the scene
    scene->AddGameObject(std::move(topPyramid));
    scene->AddGameObject(std::move(bottomPyramid));
    scene->AddGameObject(std::move(nearbyObject));
    
    // Create and add a camera
    std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();
    scene->AddCamera(std::move(mainCamera));
    
    // Set the physics system in the scene
    scene->SetPhysicsSystem(std::move(physicsSystem));
    
    // Run the scene
    scene->Run();
}

// Entry point for the physics demo
int main(int argc, char** argv) {
    CreatePhysicsDemo();
    return 0;
}
