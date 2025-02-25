#include "EngineCondition.h"
#include "ProjectSettings/ProjectSettings.h"
#include <iostream>

namespace EngineCondition {
// Initialize the current state to standalone running by default
State currentState = State::STANDALONE_RUNNING;

// Functions to check the current state
bool IsInEditor() {
  return currentState == State::IN_EDITOR_EDITING ||
         currentState == State::IN_EDITOR_PLAYING ||
         currentState == State::IN_EDITOR_COMPILING;
}

bool IsInEditorEditing() { return currentState == State::IN_EDITOR_EDITING; }

bool IsInEditorPlaying() { return currentState == State::IN_EDITOR_PLAYING; }

bool IsInEditorCompiling() {
  return currentState == State::IN_EDITOR_COMPILING;
}

bool IsDebugBuild() {
  return currentState == State::DEBUG_BUILD_STATE ||
         (IsInEditor() && currentState != State::RELEASE_BUILD_STATE);
}

bool IsReleaseBuild() { return currentState == State::RELEASE_BUILD_STATE; }

bool IsStandaloneRunning() { return currentState == State::STANDALONE_RUNNING; }

// Function to set the current state
void SetState(State newState) {
// Log state change for debugging
#ifdef DEBUG_BUILD
  std::cout << "Engine state changing from ";
  switch (currentState) {
  case State::IN_EDITOR_EDITING:
    std::cout << "IN_EDITOR_EDITING";
    break;
  case State::IN_EDITOR_PLAYING:
    std::cout << "IN_EDITOR_PLAYING";
    break;
  case State::IN_EDITOR_COMPILING:
    std::cout << "IN_EDITOR_COMPILING";
    break;
  case State::DEBUG_BUILD_STATE:
    std::cout << "DEBUG_BUILD_STATE";
    break;
  case State::RELEASE_BUILD_STATE:
    std::cout << "RELEASE_BUILD_STATE";
    break;
  case State::STANDALONE_RUNNING:
    std::cout << "STANDALONE_RUNNING";
    break;
  default:
    std::cout << "UNKNOWN";
    break;
  }

  std::cout << " to ";

  switch (newState) {
  case State::IN_EDITOR_EDITING:
    std::cout << "IN_EDITOR_EDITING";
    break;
  case State::IN_EDITOR_PLAYING:
    std::cout << "IN_EDITOR_PLAYING";
    break;
  case State::IN_EDITOR_COMPILING:
    std::cout << "IN_EDITOR_COMPILING";
    break;
  case State::DEBUG_BUILD_STATE:
    std::cout << "DEBUG_BUILD_STATE";
    break;
  case State::RELEASE_BUILD_STATE:
    std::cout << "RELEASE_BUILD_STATE";
    break;
  case State::STANDALONE_RUNNING:
    std::cout << "STANDALONE_RUNNING";
    break;
  default:
    std::cout << "UNKNOWN";
    break;
  }

  std::cout << std::endl;
#endif

  // Update project settings based on the new state
  auto &settings = ProjectSettings::GetInstance();
  if (newState == State::DEBUG_BUILD_STATE) {
    settings.SetDebugSymbols(true);
    settings.SetOptimization(false);
  } else if (newState == State::RELEASE_BUILD_STATE) {
    settings.SetDebugSymbols(false);
    settings.SetOptimization(true);
  }

  // Update the state
  currentState = newState;
}

// Helper functions for common state transitions
void EnterEditMode() { SetState(State::IN_EDITOR_EDITING); }

void EnterPlayMode() { SetState(State::IN_EDITOR_PLAYING); }

void EnterCompileMode() { SetState(State::IN_EDITOR_COMPILING); }

void ExitEditor() { SetState(State::STANDALONE_RUNNING); }
} // namespace EngineCondition
