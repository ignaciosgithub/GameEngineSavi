#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../Animation/Animation.h"
#include "../Animation/KeyFrame.h"
#include "../Vector3.h"

// Simple test for the animation system
int main() {
    std::cout << "Simple Animation System Test" << std::endl;
    std::cout << "==========================" << std::endl;
    
    // Create a simple animation
    std::cout << "Creating animation..." << std::endl;
    auto animation = std::make_shared<Animation>("TestAnimation", 1.0f, true);
    
    // Create keyframes with vertex data
    std::vector<GLfloat> keyframe1Vertices = {
        // Cube in pose 1 (default position)
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f
    };
    
    std::vector<GLfloat> keyframe2Vertices = {
        // Cube in pose 2 (slightly moved)
        -1.0f, -1.0f, -0.8f,
        -1.0f, -1.0f,  1.2f,
        -1.0f,  1.0f, -0.8f,
        -1.0f,  1.0f,  1.2f,
         1.0f, -1.0f, -0.8f,
         1.0f, -1.0f,  1.2f,
         1.0f,  1.0f, -0.8f,
         1.0f,  1.0f,  1.2f
    };
    
    // Add keyframes to animation
    animation->AddKeyFrame(0.0f, keyframe1Vertices);
    animation->AddKeyFrame(0.5f, keyframe2Vertices);
    
    std::cout << "Animation created with " << animation->GetKeyFrames().size() << " keyframes" << std::endl;
    
    // Test interpolation at different times
    std::cout << "\nTesting interpolation at different times:" << std::endl;
    
    float testTimes[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.25f};
    for (float time : testTimes) {
        std::cout << "Time: " << time << std::endl;
        
        // Get interpolated vertices
        const std::vector<GLfloat>& interpolatedVertices = animation->GetInterpolatedVertices(time);
        
        // Print first vertex as sample
        if (!interpolatedVertices.empty()) {
            std::cout << "  First vertex: (" 
                      << interpolatedVertices[0] << ", " 
                      << interpolatedVertices[1] << ", " 
                      << interpolatedVertices[2] << ")" << std::endl;
        } else {
            std::cout << "  No vertices returned!" << std::endl;
        }
    }
    
    std::cout << "\nSimple animation test completed successfully!" << std::endl;
    
    return 0;
}
