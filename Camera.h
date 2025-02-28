#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

class Camera
{
public:
    Vector3 position;
    Vector3 lookDirection;
    float fieldOfView;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float aspectRatio = 16.0f / 9.0f;
    
    // Viewport properties (normalized 0-1 coordinates)
    float viewportX = 0.0f;
    float viewportY = 0.0f;
    float viewportWidth = 1.0f;
    float viewportHeight = 1.0f;
    bool enabled = true;
   
    Camera() : position(0, 0, 0), lookDirection(0, 1, -1), fieldOfView(90){}
   
    Camera(Vector3 pos, Vector3 dir, float fov) : position(pos), lookDirection(dir), fieldOfView(fov) {}
    
    // Get camera position
    const Vector3& GetPosition() const { return position; }
    
    // Set camera position
    void SetPosition(const Vector3& pos) { position = pos; }
    
    // Get camera rotation
    Vector3 GetRotation() const {
        // Convert look direction to Euler angles
        Vector3 normalized = lookDirection;
        normalized.normalize();
        
        // Calculate pitch (x-rotation)
        float pitch = asin(normalized.y) * 180.0f / M_PI;
        
        // Calculate yaw (y-rotation)
        float yaw = atan2(normalized.x, normalized.z) * 180.0f / M_PI;
        
        return Vector3(pitch, yaw, 0);
    }
    
    // Set camera rotation
    void SetRotation(const Vector3& rotation) {
        // Convert Euler angles to look direction
        float yaw = rotation.y * M_PI / 180.0f;   // Convert to radians
        float pitch = rotation.x * M_PI / 180.0f; // Convert to radians
        
        // Calculate new look direction
        lookDirection.x = sin(yaw) * cos(pitch);
        lookDirection.y = sin(pitch);
        lookDirection.z = cos(yaw) * cos(pitch);
        
        // Normalize the look direction
        lookDirection.normalize();
    }
    
    // Look at a specific point
    void LookAt(const Vector3& target) {
        lookDirection = (target - position);
        lookDirection.normalize();
    }
    
    // Get view matrix
    Matrix4x4 GetViewMatrix() const {
        Vector3 up(0, 1, 0);
        Vector3 forward = lookDirection;
        forward.normalize();
        
        Vector3 right = Vector3::Cross(up, forward);
        right.normalize();
        
        up = Vector3::Cross(forward, right);
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
        view.elements[3][0] = -Vector3::Dot(right, position);
        view.elements[3][1] = -Vector3::Dot(up, position);
        view.elements[3][2] = Vector3::Dot(forward, position);  // Positive dot product with forward
        view.elements[3][3] = 1.0f;
        
        return view;
    }
    
    // Get projection matrix
    Matrix4x4 GetProjectionMatrix() const {
        float f = 1.0f / tan(fieldOfView * 0.5f * M_PI / 180.0f);
        float nf = 1.0f / (nearPlane - farPlane);
        
        Matrix4x4 projection;
        projection.elements[0][0] = f / aspectRatio;
        projection.elements[1][1] = f;
        projection.elements[2][2] = (farPlane + nearPlane) * nf;
        projection.elements[2][3] = -1.0f;
        projection.elements[3][2] = 2.0f * farPlane * nearPlane * nf;
        projection.elements[3][3] = 0.0f;
        
        return projection;
    }
    
    // Set viewport properties
    void SetViewport(float x, float y, float width, float height) {
        viewportX = x;
        viewportY = y;
        viewportWidth = width;
        viewportHeight = height;
    }
    
    // Enable/disable camera
    void SetEnabled(bool isEnabled) {
        enabled = isEnabled;
    }
    
    // Check if camera is enabled
    bool IsEnabled() const {
        return enabled;
    }
    
    // Update aspect ratio based on viewport dimensions
    void UpdateAspectRatio(int windowWidth, int windowHeight) {
        if (windowWidth > 0 && windowHeight > 0) {
            float viewportActualWidth = viewportWidth * windowWidth;
            float viewportActualHeight = viewportHeight * windowHeight;
            if (viewportActualWidth > 0 && viewportActualHeight > 0) {
                aspectRatio = viewportActualWidth / viewportActualHeight;
            }
        }
    }
    
    // Update camera (called every frame)
    void Update(float deltaTime) {
        // This method is called by the editor to update the camera
        // The actual movement is handled in EditorMain.cpp through input handling
    }
};

#endif // CAMERA_H
