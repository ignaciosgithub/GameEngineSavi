#ifndef SETTINGS_PANEL_H
#define SETTINGS_PANEL_H

#include "../ProjectSettings/ProjectSettings.h"
#include "GUI.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class Checkbox;
class Dropdown;

// Use the TextField class from TextField.h
#include "TextField.h"

// Checkbox for editing boolean values
class Checkbox : public GUIElement {
private:
  bool checked;
  std::string label;
  std::function<void(bool)> onCheckedChanged;

public:
  Checkbox(float x, float y, float size, const std::string &label,
           bool initialValue = false);

  void Draw() override;
  bool HandleInput(int mouseX, int mouseY, bool clicked) override;

  bool IsChecked() const { return checked; }
  void SetChecked(bool value) { checked = value; }

  void SetOnCheckedChanged(std::function<void(bool)> callback) {
    onCheckedChanged = callback;
  }
};

// Dropdown for selecting from a list of options
class Dropdown : public GUIElement {
private:
  std::vector<std::string> options;
  int selectedIndex;
  std::string label;
  bool expanded;
  std::function<void(int)> onSelectionChanged;

public:
  Dropdown(float x, float y, float w, float h, const std::string &label,
           const std::vector<std::string> &options, int initialSelection = 0);

  void Draw() override;
  bool HandleInput(int mouseX, int mouseY, bool clicked) override;

  int GetSelectedIndex() const { return selectedIndex; }
  std::string GetSelectedOption() const { return options[selectedIndex]; }
  void SetSelectedIndex(int index);

  void SetOnSelectionChanged(std::function<void(int)> callback) {
    onSelectionChanged = callback;
  }
};

// Settings panel for editing project settings
class SettingsPanel : public Panel {
private:
  // Project settings
  std::unique_ptr<TextField> projectNameField;

  // Build settings
  std::unique_ptr<Checkbox> debugSymbolsCheckbox;
  std::unique_ptr<Checkbox> optimizationCheckbox;
  std::unique_ptr<TextField> outputDirectoryField;

  // Physics settings
  std::unique_ptr<TextField> fixedTimeStepField;
  std::unique_ptr<TextField> gravityField;
  std::unique_ptr<Checkbox> enableCollisionsCheckbox;

  // Rendering settings
  std::unique_ptr<TextField> targetFPSField;
  std::unique_ptr<Checkbox> vsyncCheckbox;
  std::unique_ptr<Dropdown> msaaDropdown;
  std::unique_ptr<Checkbox> shadowsCheckbox;

  // Asset paths
  std::unique_ptr<TextField> modelsPathField;
  std::unique_ptr<TextField> texturesPathField;
  std::unique_ptr<TextField> soundsPathField;
  std::unique_ptr<TextField> scriptsPathField;

  // Save button
  std::unique_ptr<Button> saveButton;

public:
  SettingsPanel(float x, float y, float w, float h);

  // Update the panel with current project settings
  void UpdateFromSettings();

  // Save the current panel values to project settings
  void SaveToSettings();
};

#endif // SETTINGS_PANEL_H
