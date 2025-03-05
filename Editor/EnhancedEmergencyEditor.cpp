/**
 * EnhancedEmergencyEditor.cpp
 * Enhanced emergency editor for GameEngineSavi that saves frames to a folder.
 * 
 * This file contains a modified version of the editor that renders each frame
 * to a PNG file instead of directly to the screen, with support for panel traversal
 * and hotkeys.
 */

#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../ThirdParty/OpenGL/include/GL/gl_types.h"
#include "../FrameCapture_png.h"
#include "../Camera.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For system()
#include <fstream>

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include "../ThirdParty/OpenGL/include/GL/platform_gl.h"
#else
    #include "../ThirdParty/OpenGL/include/GL/platform_gl.h"
    #include <X11/keysym.h>
    #include <X11/Xlib.h>
#endif

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Global editor instance
std::unique_ptr<Editor> editor;

// Frame counter
int frameCount = 0;

// Panel traversal
enum PanelType {
    SCENE_VIEW_PANEL = 0,
    HIERARCHY_PANEL = 1,
    INSPECTOR_PANEL = 2,
    PROJECT_PANEL = 3,
    PANEL_COUNT
};

// Current active panel (default to scene view)
int activePanel = SCENE_VIEW_PANEL;

// Hotkey state
struct HotkeyState {
    bool wPressed = false;
    bool aPressed = false;
    bool sPressed = false;
    bool dPressed = false;
    bool shiftPressed = false;
    bool spacePressed = false;
    bool ctrlPressed = false;
    bool panel1Pressed = false;
    bool panel2Pressed = false;
    bool panel3Pressed = false;
    bool panel4Pressed = false;
    bool escapePressed = false;
    bool f12Pressed = false;
};

HotkeyState hotkeyState;

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    return ss.str();
}

// Function to save hotkey state to a file
void SaveHotkeyState() {
    std::ofstream file("frames/hotkey_state.txt");
    if (file.is_open()) {
        file << "Active Panel: " << activePanel << std::endl;
        file << "W: " << (hotkeyState.wPressed ? "Pressed" : "Released") << std::endl;
        file << "A: " << (hotkeyState.aPressed ? "Pressed" : "Released") << std::endl;
        file << "S: " << (hotkeyState.sPressed ? "Pressed" : "Released") << std::endl;
        file << "D: " << (hotkeyState.dPressed ? "Pressed" : "Released") << std::endl;
        file << "Shift: " << (hotkeyState.shiftPressed ? "Pressed" : "Released") << std::endl;
        file << "Space: " << (hotkeyState.spacePressed ? "Pressed" : "Released") << std::endl;
        file << "Ctrl: " << (hotkeyState.ctrlPressed ? "Pressed" : "Released") << std::endl;
        file << "Ctrl+1: " << (hotkeyState.panel1Pressed ? "Pressed" : "Released") << std::endl;
        file << "Ctrl+2: " << (hotkeyState.panel2Pressed ? "Pressed" : "Released") << std::endl;
        file << "Ctrl+3: " << (hotkeyState.panel3Pressed ? "Pressed" : "Released") << std::endl;
        file << "Ctrl+4: " << (hotkeyState.panel4Pressed ? "Pressed" : "Released") << std::endl;
        file << "Escape: " << (hotkeyState.escapePressed ? "Pressed" : "Released") << std::endl;
        file << "F12: " << (hotkeyState.f12Pressed ? "Pressed" : "Released") << std::endl;
        file.close();
    }
}

// Function to save panel info to a file
void SavePanelInfo() {
    std::ofstream file("frames/panel_info.txt");
    if (file.is_open()) {
        file << "Active Panel: " << activePanel << std::endl;
        switch (activePanel) {
            case SCENE_VIEW_PANEL:
                file << "Scene View Panel" << std::endl;
                break;
            case HIERARCHY_PANEL:
                file << "Hierarchy Panel" << std::endl;
                break;
            case INSPECTOR_PANEL:
                file << "Inspector Panel" << std::endl;
                break;
            case PROJECT_PANEL:
                file << "Project Panel" << std::endl;
                break;
        }
        file.close();
    }
}

// Common initialization function
void InitializeEditor() {
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Create frames directory
    system("mkdir -p frames");
    
    // Set engine condition to editor mode
    EngineCondition::isInEditor = true;
    
    // Create panel info file
    SavePanelInfo();
    
    // Create hotkey state file
    SaveHotkeyState();
}

// Process keyboard input
void ProcessKeyboard(unsigned char key, bool pressed) {
    // Update hotkey state
    switch (key) {
        case 'w':
        case 'W':
            hotkeyState.wPressed = pressed;
            break;
        case 'a':
        case 'A':
            hotkeyState.aPressed = pressed;
            break;
        case 's':
        case 'S':
            hotkeyState.sPressed = pressed;
            break;
        case 'd':
        case 'D':
            hotkeyState.dPressed = pressed;
            break;
        case ' ':
            hotkeyState.spacePressed = pressed;
            break;
        case 27: // Escape
            hotkeyState.escapePressed = pressed;
            break;
    }
    
    // Panel traversal (only on key press, not release)
    if (pressed && hotkeyState.ctrlPressed) {
        switch (key) {
            case '1':
                hotkeyState.panel1Pressed = true;
                activePanel = HIERARCHY_PANEL;
                std::cout << "Switched to Hierarchy Panel" << std::endl;
                SavePanelInfo();
                break;
            case '2':
                hotkeyState.panel2Pressed = true;
                activePanel = SCENE_VIEW_PANEL;
                std::cout << "Switched to Scene View Panel" << std::endl;
                SavePanelInfo();
                break;
            case '3':
                hotkeyState.panel3Pressed = true;
                activePanel = INSPECTOR_PANEL;
                std::cout << "Switched to Inspector Panel" << std::endl;
                SavePanelInfo();
                break;
            case '4':
                hotkeyState.panel4Pressed = true;
                activePanel = PROJECT_PANEL;
                std::cout << "Switched to Project Panel" << std::endl;
                SavePanelInfo();
                break;
        }
    }
    
    // Save hotkey state
    SaveHotkeyState();
}

// Process special keys
void ProcessSpecialKey(int key, bool pressed) {
    switch (key) {
        case 0xFFE1: // X11 Shift key
#ifdef PLATFORM_WINDOWS
        case 0x10: // Windows Shift key (VK_SHIFT)
#endif
            hotkeyState.shiftPressed = pressed;
            break;
        case 0xFFE3: // X11 Control key
#ifdef PLATFORM_WINDOWS
        case 0x11: // Windows Control key (VK_CONTROL)
#endif
            hotkeyState.ctrlPressed = pressed;
            break;
        case 0xFFC9: // X11 F12 key
#ifdef PLATFORM_WINDOWS
        case 0x7B: // Windows F12 key (VK_F12)
#endif
            hotkeyState.f12Pressed = pressed;
            if (pressed) {
                // Capture screenshot
                std::string filename = "frames/screenshot_" + std::to_string(frameCount) + ".png";
                FrameCapture_PNG::CaptureViewportToFile(filename);
                std::cout << "Screenshot captured to " << filename << std::endl;
            }
            break;
    }
    
    // Save hotkey state
    SaveHotkeyState();
}

// Main entry point
int main(int argc, char** argv) {
    std::cout << "Starting enhanced emergency editor on Linux..." << std::endl;
    
    // Initialize editor
    InitializeEditor();
    
    // Open X display
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Failed to open X display" << std::endl;
        
        // Even without a display, we can still render to files
        std::cout << "Continuing with headless rendering..." << std::endl;
        
        // Main loop for headless rendering
        bool running = true;
        TimeManager timeManager;
        
        while (running && frameCount < 100) { // Limit to 100 frames for testing
            // Update time
            timeManager.Update();
            float deltaTime = timeManager.GetDeltaTime();
            
            // Update editor
            if (editor) {
                editor->Update(deltaTime);
                editor->Render();
                
                // Capture frame to file
                std::string filename = GetFrameFilename();
                FrameCapture_PNG::CaptureViewportToFile(filename);
                std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
                
                // Increment frame counter
                frameCount++;
                
                // Small delay to not overwhelm the system
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        
        std::cout << "Enhanced emergency editor complete. " << frameCount << " frames rendered." << std::endl;
        std::cout << "Run the Python viewer script to view the rendered frames." << std::endl;
        
        return 0;
    }
    
    // Get root window
    Window root = DefaultRootWindow(display);
    
    // Get visual info
    XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), NULL);
    if (!vi) {
        std::cerr << "Failed to get visual info" << std::endl;
        XCloseDisplay(display);
        return 1;
    }
    
    // Create colormap
    Visual* visual = (Visual*)vi->visual;
    Colormap cmap = XCreateColormap(display, root, visual, AllocNone);
    
    // Set up window attributes
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    
    // Create window
    Window window = XCreateWindow(
        display, root,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        vi->depth, InputOutput, visual,
        CWColormap | CWEventMask, &swa
    );
    
    // Set window title
    XStoreName(display, window, "Enhanced Emergency Editor");
    
    // Map window
    XMapWindow(display, window);
    
    // Create OpenGL context
    GLXContext context = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, context);
    
    // Force software rendering
    putenv((char*)"LIBGL_ALWAYS_SOFTWARE=1");
    std::cout << "Forcing software rendering" << std::endl;
    
    // Initialize graphics API
    GraphicsAPIFactory::GetInstance().Initialize();
    
    // Set up OpenGL for 3D rendering
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        graphics->SetDepthTest(true);
        graphics->SetDepthFunc(GL_LEQUAL);
        graphics->SetCullFace(true);
        graphics->SetCullFaceMode(GL_BACK);
    }
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running && frameCount < 100) { // Limit to 100 frames for testing
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file
            std::string filename = GetFrameFilename();
            FrameCapture_PNG::CaptureViewportToFile(filename);
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Process events
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);
            
            switch (event.type) {
                case Expose:
                    // Redraw
                    if (editor) {
                        editor->Render();
                    }
                    break;
                    
                case KeyPress:
                    // Handle key press
                    {
                        KeySym keysym = XLookupKeysym(&event.xkey, 0);
                        if (keysym >= XK_a && keysym <= XK_z) {
                            ProcessKeyboard((unsigned char)keysym, true);
                        } else if (keysym >= XK_A && keysym <= XK_Z) {
                            ProcessKeyboard((unsigned char)(keysym - XK_A + 'a'), true);
                        } else if (keysym >= XK_0 && keysym <= XK_9) {
                            ProcessKeyboard((unsigned char)keysym, true);
                        } else if (keysym == XK_space) {
                            ProcessKeyboard(' ', true);
                        } else if (keysym == XK_Escape) {
                            ProcessKeyboard(27, true);
                        } else {
                            ProcessSpecialKey((int)keysym, true);
                        }
                    }
                    break;
                    
                case KeyRelease:
                    // Handle key release
                    {
                        KeySym keysym = XLookupKeysym(&event.xkey, 0);
                        if (keysym >= XK_a && keysym <= XK_z) {
                            ProcessKeyboard((unsigned char)keysym, false);
                        } else if (keysym >= XK_A && keysym <= XK_Z) {
                            ProcessKeyboard((unsigned char)(keysym - XK_A + 'a'), false);
                        } else if (keysym >= XK_0 && keysym <= XK_9) {
                            ProcessKeyboard((unsigned char)keysym, false);
                        } else if (keysym == XK_space) {
                            ProcessKeyboard(' ', false);
                        } else if (keysym == XK_Escape) {
                            ProcessKeyboard(27, false);
                        } else {
                            ProcessSpecialKey((int)keysym, false);
                        }
                    }
                    break;
            }
        }
        
        // Swap buffers
        auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
        if (graphics) {
            graphics->SwapBuffers();
        } else {
            glXSwapBuffers(display, window);
        }
    }
    
    // Clean up
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    
    std::cout << "Enhanced emergency editor complete. " << frameCount << " frames rendered." << std::endl;
    std::cout << "Run the Python viewer script to view the rendered frames." << std::endl;
    
    return 0;
}
