#include "SceneViewPanel.h"
#include "Editor.h"
#include "../Camera.h"
#include "../GameObject.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>

SceneViewPanel::SceneViewPanel(float x, float y, float w, float h, Editor* editor)
    : Panel(x, y, w, h), editor(editor), isDraggingCamera(false), isDraggingObject(false) {
    
}

void SceneViewPanel::Draw() {
    // Draw panel background
    Panel::Draw();
    
    // Draw panel title
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw "Scene" text (simplified for prototype)
    
    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // Set up viewport for scene rendering
    glViewport(static_cast<int>(x), static_cast<int>(y + 20), static_cast<int>(width), static_cast<int>(height - 20));
    
    // Clear depth buffer for 3D rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);  // Darker background for better contrast
    
    // Set material properties
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    // Render the scene
    if (editor && editor->GetScene()) {
        editor->GetScene()->Render();
    } else {
        // Fallback if scene is not available
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        // Manual implementation of gluPerspective since we might not have GLU
        float fovy = 45.0f;
        float aspect = width / (height - 20);
        float zNear = 0.1f;
        float zFar = 100.0f;
        
        float f = 1.0f / tan(fovy * 3.14159f / 360.0f);
        float m[16] = {0};
        
        m[0] = f / aspect;
        m[5] = f;
        m[10] = (zFar + zNear) / (zNear - zFar);
        m[11] = -1.0f;
        m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
        m[15] = 0.0f;
        
        glMultMatrixf(m);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Manual implementation of gluLookAt
        float eyex = 0.0f, eyey = 2.0f, eyez = 5.0f;
        float centerx = 0.0f, centery = 0.0f, centerz = 0.0f;
        float upx = 0.0f, upy = 1.0f, upz = 0.0f;
        
        // Calculate forward vector
        float forwardx = centerx - eyex;
        float forwardy = centery - eyey;
        float forwardz = centerz - eyez;
        
        // Normalize forward vector
        float forwardLen = sqrt(forwardx*forwardx + forwardy*forwardy + forwardz*forwardz);
        forwardx /= forwardLen;
        forwardy /= forwardLen;
        forwardz /= forwardLen;
        
        // Calculate right vector
        float rightx = forwardy * upz - forwardz * upy;
        float righty = forwardz * upx - forwardx * upz;
        float rightz = forwardx * upy - forwardy * upx;
        
        // Normalize right vector
        float rightLen = sqrt(rightx*rightx + righty*righty + rightz*rightz);
        rightx /= rightLen;
        righty /= rightLen;
        rightz /= rightLen;
        
        // Recalculate up vector
        float upx2 = righty * forwardz - rightz * forwardy;
        float upy2 = rightz * forwardx - rightx * forwardz;
        float upz2 = rightx * forwardy - righty * forwardx;
        
        // Create the look-at matrix
        float m2[16] = {
            rightx, upx2, -forwardx, 0,
            righty, upy2, -forwardy, 0,
            rightz, upz2, -forwardz, 0,
            0, 0, 0, 1
        };
        
        glMultMatrixf(m2);
        glTranslatef(-eyex, -eyey, -eyez);
        
        // Set light position
        GLfloat light_position[] = { 0.0f, 5.0f, 0.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        
        // Draw a gray background
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a cube
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidCube(1.0f);
    }
    
    // Restore OpenGL state
    glPopAttrib();
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    // Reset viewport to full window
    glViewport(0, 0, 800, 600);
    
    // Draw border around scene view
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y + 20);
    glVertex2f(x + width, y + 20);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw debug info
    DebugRenderState();
}

bool SceneViewPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!Panel::HandleInput(mouseX, mouseY, clicked)) {
        return false;
    }
    
    // Check if mouse is over the scene view
    if (mouseX >= x && mouseX <= x + width && mouseY >= y + 20 && mouseY <= y + height) {
        if (clicked) {
            // Start camera or object manipulation
            if (editor && editor->GetSelectedObject()) {
                // Start object manipulation
                isDraggingObject = true;
                dragStartPosition = editor->GetSelectedObject()->GetPosition();
            } else {
                // Start camera manipulation
                isDraggingCamera = true;
            }
            
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        } else {
            // Continue camera or object manipulation
            if (isDraggingCamera) {
                HandleCameraMovement(mouseX, mouseY);
            } else if (isDraggingObject) {
                HandleObjectManipulation(mouseX, mouseY);
            }
        }
        
        return true;
    }
    
    // Stop dragging if mouse is released
    if (!clicked) {
        isDraggingCamera = false;
        isDraggingObject = false;
    }
    
    return false;
}

void SceneViewPanel::HandleCameraMovement(int mouseX, int mouseY) {
    if (!editor || !editor->GetEditorCamera()) {
        return;
    }
    
    // Calculate mouse delta
    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;
    
    // Update camera rotation
    Camera* camera = editor->GetEditorCamera();
    Vector3 rotation = camera->GetRotation();
    rotation.y += deltaX * 0.1f;
    rotation.x += deltaY * 0.1f;
    camera->SetRotation(rotation);
    
    // Update last mouse position
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    
    std::cout << "Camera moved: deltaX=" << deltaX << ", deltaY=" << deltaY << std::endl;
    std::cout << "Camera position: " << camera->GetPosition().x << ", " 
              << camera->GetPosition().y << ", " << camera->GetPosition().z << std::endl;
}

void SceneViewPanel::HandleObjectManipulation(int mouseX, int mouseY) {
    if (!editor || !editor->GetSelectedObject()) {
        return;
    }
    
    // Calculate mouse delta
    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;
    
    // Update object position
    // In a real implementation, we would use proper object manipulation
    // For now, we'll just update the object's position
    GameObject* object = editor->GetSelectedObject();
    Vector3 position = object->GetPosition();
    position.x += deltaX * 0.01f;
    position.y -= deltaY * 0.01f;
    object->SetPosition(position);
    
    // Update last mouse position
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void SceneViewPanel::DebugRenderState() {
    if (!editor) {
        std::cout << "Editor is null in SceneViewPanel::DebugRenderState" << std::endl;
        return;
    }
    
    std::cout << "\n===== Scene View Panel Debug Information =====\n" << std::endl;
    std::cout << "Panel position: x=" << x << ", y=" << y << ", width=" << width << ", height=" << height << std::endl;
    std::cout << "Viewport: x=" << x << ", y=" << (y + 20) << ", width=" << width << ", height=" << (height - 20) << std::endl;
    
    // Debug OpenGL state
    std::cout << "\n----- OpenGL State -----" << std::endl;
    GLboolean depthTestEnabled;
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
    std::cout << "Depth test enabled: " << (depthTestEnabled ? "YES" : "NO") << std::endl;
    
    GLboolean lightingEnabled;
    glGetBooleanv(GL_LIGHTING, &lightingEnabled);
    std::cout << "Lighting enabled: " << (lightingEnabled ? "YES" : "NO") << std::endl;
    
    GLfloat clearColor[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    std::cout << "Clear color: (" << clearColor[0] << ", " << clearColor[1] << ", " 
              << clearColor[2] << ", " << clearColor[3] << ")" << std::endl;
    
    // Debug camera info
    std::cout << "\n----- Camera Information -----" << std::endl;
    Camera* camera = editor->GetEditorCamera();
    if (camera) {
        Vector3 camPos = camera->GetPosition();
        std::cout << "Camera position: (" << camPos.x << ", " << camPos.y << ", " << camPos.z << ")" << std::endl;
        
        // Debug camera look direction
        Vector3 lookDir = camera->lookDirection;
        std::cout << "Camera look direction: (" << lookDir.x << ", " << lookDir.y << ", " << lookDir.z << ")" << std::endl;
        
        // Debug camera field of view and other parameters
        std::cout << "Camera FOV: " << camera->fieldOfView << " degrees" << std::endl;
        std::cout << "Camera near plane: " << camera->nearPlane << std::endl;
        std::cout << "Camera far plane: " << camera->farPlane << std::endl;
        std::cout << "Camera aspect ratio: " << camera->aspectRatio << std::endl;
        
        // Calculate distance to origin (where the default cube should be)
        float distanceToOrigin = sqrt(camPos.x*camPos.x + camPos.y*camPos.y + camPos.z*camPos.z);
        std::cout << "Distance to origin: " << distanceToOrigin << " units" << std::endl;
        
        // Check if camera is too close to origin
        if (distanceToOrigin < 0.5f) {
            std::cout << "WARNING: Camera is very close to origin! This might cause rendering issues." << std::endl;
        }
        
        // Calculate view matrix for debugging
        Matrix4x4 viewMatrix = camera->GetViewMatrix();
        std::cout << "View matrix determinant: " << CalculateMatrixDeterminant(viewMatrix) << std::endl;
    } else {
        std::cout << "Editor camera is null" << std::endl;
    }
    
    // Debug scene info
    std::cout << "\n----- Scene Information -----" << std::endl;
    Scene* scene = editor->GetScene();
    if (scene) {
        std::cout << "Scene has " << scene->GetGameObjectCount() << " game objects" << std::endl;
        
        // Check for default cube
        bool foundDefaultCube = false;
        
        // List the first few objects
        int maxObjectsToList = 5;
        int count = 0;
        for (int i = 0; i < scene->GetGameObjectCount() && count < maxObjectsToList; i++) {
            GameObject* obj = scene->GetGameObject(i);
            if (obj) {
                Vector3 objPos = obj->GetPosition();
                std::cout << "Object " << i << ": " << obj->GetName() 
                          << " at (" << objPos.x << ", " << objPos.y << ", " << objPos.z << ")" << std::endl;
                
                // Check if this is the default cube
                if (obj->GetName() == "Default Cube") {
                    foundDefaultCube = true;
                    
                    // Calculate distance from camera to cube
                    if (camera) {
                        Vector3 camPos = camera->GetPosition();
                        float dx = camPos.x - objPos.x;
                        float dy = camPos.y - objPos.y;
                        float dz = camPos.z - objPos.z;
                        float distanceToCube = sqrt(dx*dx + dy*dy + dz*dz);
                        
                        std::cout << "Distance from camera to cube: " << distanceToCube << " units" << std::endl;
                        
                        // Check if cube is too close to camera
                        if (distanceToCube < camera->nearPlane * 1.5f) {
                            std::cout << "WARNING: Default cube is very close to camera's near plane!" << std::endl;
                            std::cout << "This might cause the cube to be clipped or not visible." << std::endl;
                        }
                    }
                    
                    // Check if cube has a mesh
                    std::cout << "Cube has " << obj->GetMeshCount() << " meshes" << std::endl;
                    if (obj->GetMeshCount() == 0) {
                        std::cout << "WARNING: Default cube has no meshes!" << std::endl;
                    }
                }
                
                count++;
            }
        }
        
        if (!foundDefaultCube) {
            std::cout << "WARNING: Default cube not found in scene!" << std::endl;
        }
        
        // Debug light info
        std::cout << "\n----- Light Information -----" << std::endl;
        const std::vector<PointLight>& lights = scene->GetPointLights();
        std::cout << "Scene has " << lights.size() << " point lights" << std::endl;
        
        for (size_t i = 0; i < lights.size() && i < 3; i++) {
            std::cout << "Light " << i << " position: (" 
                      << lights[i].position.x << ", " 
                      << lights[i].position.y << ", " 
                      << lights[i].position.z << ")" << std::endl;
            std::cout << "Light " << i << " color: (" 
                      << lights[i].color.x << ", " 
                      << lights[i].color.y << ", " 
                      << lights[i].color.z << ")" << std::endl;
            std::cout << "Light " << i << " intensity: " << lights[i].intensity << std::endl;
        }
    } else {
        std::cout << "Scene is null" << std::endl;
    }
    
    // Debug GUI colors
    std::cout << "\n----- GUI Color Information -----" << std::endl;
    std::cout << "Panel background color: Used in Panel::Draw()" << std::endl;
    std::cout << "Text color: Set with glColor3f(0.0f, 0.0f, 0.0f) in SceneViewPanel::Draw()" << std::endl;
    std::cout << "If these colors are similar, text might not be visible against the background" << std::endl;
    
    std::cout << "\n============================================\n" << std::endl;
}

// Helper function to calculate matrix determinant (simplified for 4x4)
float SceneViewPanel::CalculateMatrixDeterminant(const Matrix4x4& matrix) {
    // This is a simplified calculation that just checks if the matrix is valid
    // A full determinant calculation would be more complex
    float sum = 0.0f;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sum += fabs(matrix.elements[i][j]);
        }
    }
    return sum > 0.0f ? sum : 0.0f;
}
