#pragma once

class EngineCondition {
public:
    static bool IsInEditor() {
        return isInEditor;
    }
    
    static void SetInEditor(bool inEditor) {
        isInEditor = inEditor;
    }
    
private:
    static bool isInEditor;
};
