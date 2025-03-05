#include "EngineCondition.h"

bool EngineCondition::isInEditor = true;

bool EngineCondition::IsInEditor() {
    return isInEditor;
}

void EngineCondition::SetInEditor(bool value) {
    isInEditor = value;
}
