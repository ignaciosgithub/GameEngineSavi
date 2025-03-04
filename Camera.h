#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

class Camera {
public:
    Camera() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), 
               lookDirection(0, 0, -1), fieldOfView(60.0f), aspectRatio(16.0f / 9.0f), 
               nearPlane(0.1f), farPlane(1000.0f), viewportX(0.0f), viewportY(0.0f), 
               viewportWidth(1.0f), viewportHeight(1.0f), enabled(true) {}
               
    // Constructor for compatibility with previous Camera.cpp definition
    Camera(Vector3 pos, Vector3 dir, float fov) : position(pos), lookDirection(dir), fieldOfView(fov),
               rotation(0, 0, 0), scale(1, 1, 1), aspectRatio(16.0f / 9.0f), nearPlane(0.1f), farPlane(1000.0f),
               viewportX(0.0f), viewportY(0.0f), viewportWidth(1.0f), viewportHeight(1.0f), enabled(true) {}
    
    // Position
    void SetPosition(const Vector3& position) { this->position = position; }
    Vector3 GetPosition() const { return position; }
    
    // Rotation
    void SetRotation(const Vector3& rotation) { this->rotation = rotation; }
    Vector3 GetRotation() const { return rotation; }
    
    // Scale
    void SetScale(const Vector3& scale) { this->scale = scale; }
    Vector3 GetScale() const { return scale; }
    
    // Field of view
    void SetFieldOfView(float fov) { fieldOfView = fov; }
    float GetFieldOfView() const { return fieldOfView; }
    
    // Aspect ratio
    void SetAspectRatio(float ratio) { aspectRatio = ratio; }
    float GetAspectRatio() const { return aspectRatio; }
    
    // Clipping planes
    void SetNearPlane(float near) { nearPlane = near; }
    float GetNearPlane() const { return nearPlane; }
    
    void SetFarPlane(float far) { farPlane = far; }
    float GetFarPlane() const { return farPlane; }
    
    // Viewport
    void SetViewport(float x, float y, float width, float height) {
        viewportX = x;
        viewportY = y;
        viewportWidth = width;
        viewportHeight = height;
    }
    
    float GetViewportX() const { return viewportX; }
    float GetViewportY() const { return viewportY; }
    float GetViewportWidth() const { return viewportWidth; }
    float GetViewportHeight() const { return viewportHeight; }
    
    // Enabled
    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool IsEnabled() const { return enabled; }
    
    // Look at a target
    void LookAt(const Vector3& target) {
        // Calculate the direction to look at
        Vector3 direction = target - position;
        direction.normalize();
        
        // Calculate the rotation angles
        float yaw = atan2(direction.x, direction.z) * 180.0f / 3.14159f;
        float pitch = -asin(direction.y) * 180.0f / 3.14159f;
        
        // Set the rotation
        rotation = Vector3(pitch, yaw, 0.0f);
    }
    
    // Update the camera
    void Update(float deltaTime) {
        // Update camera logic here
    }
    
    // Get the view matrix
    Matrix4x4 GetViewMatrix() const {
        // Calculate the look direction
        float pitch = rotation.x * 3.14159f / 180.0f;
        float yaw = rotation.y * 3.14159f / 180.0f;
        
        // Create a local lookDirection for this method
        Vector3 viewDirection;
        viewDirection.x = sin(yaw) * cos(pitch);
        viewDirection.y = sin(pitch);
        viewDirection.z = cos(yaw) * cos(pitch);
        
        // Calculate the view matrix
        Vector3 up(0, 1, 0);
        Vector3 forward = viewDirection;
        forward.normalize();
        
        Vector3 right = up.cross(forward);
        right.normalize();
        
        up = forward.cross(right);
        up.normalize();
        
        Matrix4x4 view;
        view.elements[0][0] = right.x;
        view.elements[0][1] = right.y;
        view.elements[0][2] = right.z;
        view.elements[1][0] = up.x;
        view.elements[1][1] = up.y;
        view.elements[1][2] = up.z;
        view.elements[2][0] = -forward.x;  // Negate forward direction for OpenGL convention
        view.elements[2][1] = -forward.y;
        view.elements[2][2] = -forward.z;
        view.elements[3][0] = -right.dot(position);
        view.elements[3][1] = -up.dot(position);
        view.elements[3][2] = forward.dot(position);  // Positive dot product with forward
        view.elements[3][3] = 1.0f;
        
        return view;
    }
    
    // Get the projection matrix
    Matrix4x4 GetProjectionMatrix() const {
        // Calculate the projection matrix
        float f = 1.0f / tan(fieldOfView * 0.5f * 3.14159f / 180.0f);
        float rangeInv = 1.0f / (nearPlane - farPlane);
        
        Matrix4x4 projection;
        projection.elements[0][0] = f / aspectRatio;
        projection.elements[1][1] = f;
        projection.elements[2][2] = (nearPlane + farPlane) * rangeInv;
        projection.elements[2][3] = -1.0f;
        projection.elements[3][2] = 2.0f * nearPlane * farPlane * rangeInv;
        projection.elements[3][3] = 0.0f;
        
        return projection;
    }
    
private:
    Vector3 rotation;
    Vector3 scale;
    
public:
    Vector3 position;
    Vector3 lookDirection;
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    
    float viewportX;
    float viewportY;
    float viewportWidth;
    float viewportHeight;
    
    bool enabled;
};
