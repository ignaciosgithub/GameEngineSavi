#ifndef ENGINE_CONDITION_H
#define ENGINE_CONDITION_H

// Engine condition flags
// These flags define the current state of the engine

// Build configuration flags
#ifndef NDEBUG
    #define DEBUG_BUILD
#else
    #define RELEASE_BUILD
#endif

// Editor state flags - these would be set by the editor
// Default to not in editor for standalone builds
#ifndef IN_EDITOR
    #define STANDALONE_BUILD
#endif

namespace EngineCondition {
    // Engine condition states
    enum class State {
        // Editor states
        IN_EDITOR_EDITING,      // Game is being edited in the editor
        IN_EDITOR_PLAYING,      // Game is being played in a window in the editor
        IN_EDITOR_COMPILING,    // Game is being compiled in the editor
        
        // Build states
        DEBUG_BUILD_STATE,      // Debug build with additional checks and logging
        RELEASE_BUILD_STATE,    // Release build optimized for performance
        
        // Standalone states
        STANDALONE_RUNNING      // Game is running as a standalone application
    };
    
    // Current state of the engine - default to standalone running
    extern State currentState;
    
    // Functions to check the current state
    bool IsInEditor();
    bool IsInEditorEditing();
    bool IsInEditorPlaying();
    bool IsInEditorCompiling();
    bool IsDebugBuild();
    bool IsReleaseBuild();
    bool IsStandaloneRunning();
    
    // Functions to set the current state
    void SetState(State newState);
    
    // Helper functions for common state transitions
    void EnterEditMode();
    void EnterPlayMode();
    void EnterCompileMode();
    void ExitEditor();
}

#endif // ENGINE_CONDITION_H
