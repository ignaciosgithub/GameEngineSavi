#pragma once

class EngineCondition {
public:
    static bool isInEditor;
    static bool IsInEditor();
    static void SetInEditor(bool value);
};
