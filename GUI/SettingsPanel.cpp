#include "SettingsPanel.h"
#include "../ProjectSettings/ProjectSettings.h"
#include <iostream>
#include <sstream>

// TextField implementation is in TextField.cpp

// Checkbox implementation
Checkbox::Checkbox(float x, float y, float size, const std::string &label,
                   bool initialValue)
    : GUIElement(x, y, size, size), checked(initialValue), label(label),
      onCheckedChanged(nullptr) {}

void Checkbox::Draw() {
  if (!visible)
    return;

  // Draw label
  glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f(x + height + 5, y + height / 2);

  // Draw checkbox background
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(x, y);
  glVertex2f(x + height, y);
  glVertex2f(x + height, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Draw checkbox border
  glBegin(GL_LINE_LOOP);
  glColor3f(0.5f, 0.5f, 0.5f);
  glVertex2f(x, y);
  glVertex2f(x + height, y);
  glVertex2f(x + height, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Draw check mark if checked
  if (checked) {
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(x + 3, y + height / 2);
    glVertex2f(x + height / 2, y + height - 3);
    glVertex2f(x + height / 2, y + height - 3);
    glVertex2f(x + height - 3, y + 3);
    glEnd();
  }
}

bool Checkbox::HandleInput(int mouseX, int mouseY, bool clicked) {
  if (!enabled || !visible)
    return false;

  bool inside = mouseX >= x && mouseX <= x + height && mouseY >= y &&
                mouseY <= y + height;

  if (inside && clicked) {
    checked = !checked;
    if (onCheckedChanged) {
      onCheckedChanged(checked);
    }
    return true;
  }

  return false;
}

// Dropdown implementation
Dropdown::Dropdown(float x, float y, float w, float h, const std::string &label,
                   const std::vector<std::string> &options,
                   int initialSelection)
    : GUIElement(x, y, w, h), options(options), selectedIndex(initialSelection),
      label(label), expanded(false), onSelectionChanged(nullptr) {}

void Dropdown::Draw() {
  if (!visible)
    return;

  // Draw label
  glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f(x, y - 5);

  // Draw dropdown background
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Draw dropdown border
  glBegin(GL_LINE_LOOP);
  glColor3f(0.5f, 0.5f, 0.5f);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Draw selected option
  glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f(x + 5, y + height / 2);

  // Draw dropdown arrow
  glBegin(GL_TRIANGLES);
  glVertex2f(x + width - 15, y + height / 2 - 3);
  glVertex2f(x + width - 5, y + height / 2 - 3);
  glVertex2f(x + width - 10, y + height / 2 + 3);
  glEnd();

  // Draw expanded options if expanded
  if (expanded) {
    for (size_t i = 0; i < options.size(); i++) {
      float optionY = y + height + i * height;

      // Draw option background
      glBegin(GL_QUADS);
      if (i == selectedIndex) {
        glColor3f(0.8f, 0.8f, 1.0f);
      } else {
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      glVertex2f(x, optionY);
      glVertex2f(x + width, optionY);
      glVertex2f(x + width, optionY + height);
      glVertex2f(x, optionY + height);
      glEnd();

      // Draw option border
      glBegin(GL_LINE_LOOP);
      glColor3f(0.5f, 0.5f, 0.5f);
      glVertex2f(x, optionY);
      glVertex2f(x + width, optionY);
      glVertex2f(x + width, optionY + height);
      glVertex2f(x, optionY + height);
      glEnd();

      // Draw option text
      glColor3f(0.0f, 0.0f, 0.0f);
      glRasterPos2f(x + 5, optionY + height / 2);

      // Note: Actual text rendering would be implemented here
    }
  }
}

bool Dropdown::HandleInput(int mouseX, int mouseY, bool clicked) {
  if (!enabled || !visible)
    return false;

  bool insideMain =
      mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;

  if (insideMain && clicked) {
    expanded = !expanded;
    return true;
  }

  if (expanded) {
    for (size_t i = 0; i < options.size(); i++) {
      float optionY = y + height + i * height;
      bool insideOption = mouseX >= x && mouseX <= x + width &&
                          mouseY >= optionY && mouseY <= optionY + height;

      if (insideOption && clicked) {
        selectedIndex = i;
        expanded = false;
        if (onSelectionChanged) {
          onSelectionChanged(selectedIndex);
        }
        return true;
      }
    }
  }

  return false;
}

void Dropdown::SetSelectedIndex(int index) {
  if (index >= 0 && index < static_cast<int>(options.size())) {
    selectedIndex = index;
    if (onSelectionChanged) {
      onSelectionChanged(selectedIndex);
    }
  }
}

// SettingsPanel implementation
SettingsPanel::SettingsPanel(float x, float y, float w, float h)
    : Panel(x, y, w, h) {
  float currentY = y + 10;
  float labelWidth = 150;
  float controlWidth = 200;
  float controlHeight = 25;
  float spacing = 30;

  // Project settings
  projectNameField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Project Name"));
  AddElement(projectNameField.get());
  currentY += spacing;

  // Build settings
  debugSymbolsCheckbox = std::unique_ptr<Checkbox>(
      new Checkbox(x + labelWidth, currentY, controlHeight, "Debug Symbols"));
  AddElement(debugSymbolsCheckbox.get());
  currentY += spacing;

  optimizationCheckbox = std::unique_ptr<Checkbox>(
      new Checkbox(x + labelWidth, currentY, controlHeight, "Optimization"));
  AddElement(optimizationCheckbox.get());
  currentY += spacing;

  outputDirectoryField = std::unique_ptr<TextField>(
      new TextField(x + labelWidth, currentY, controlWidth, controlHeight,
                    "Output Directory"));
  AddElement(outputDirectoryField.get());
  currentY += spacing;

  // Physics settings
  fixedTimeStepField = std::unique_ptr<TextField>(
      new TextField(x + labelWidth, currentY, controlWidth, controlHeight,
                    "Fixed EngineTime Step"));
  AddElement(fixedTimeStepField.get());
  currentY += spacing;

  gravityField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Gravity"));
  AddElement(gravityField.get());
  currentY += spacing;

  enableCollisionsCheckbox = std::unique_ptr<Checkbox>(new Checkbox(
      x + labelWidth, currentY, controlHeight, "Enable Collisions"));
  AddElement(enableCollisionsCheckbox.get());
  currentY += spacing;

  // Rendering settings
  targetFPSField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Target FPS"));
  AddElement(targetFPSField.get());
  currentY += spacing;

  vsyncCheckbox = std::unique_ptr<Checkbox>(
      new Checkbox(x + labelWidth, currentY, controlHeight, "VSync"));
  AddElement(vsyncCheckbox.get());
  currentY += spacing;

  std::vector<std::string> msaaOptions = {"Off", "2x", "4x", "8x"};
  msaaDropdown = std::unique_ptr<Dropdown>(
      new Dropdown(x + labelWidth, currentY, controlWidth, controlHeight,
                   "MSAA", msaaOptions));
  AddElement(msaaDropdown.get());
  currentY += spacing;

  shadowsCheckbox = std::unique_ptr<Checkbox>(
      new Checkbox(x + labelWidth, currentY, controlHeight, "Shadows"));
  AddElement(shadowsCheckbox.get());
  currentY += spacing;

  // Asset paths
  modelsPathField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Models Path"));
  AddElement(modelsPathField.get());
  currentY += spacing;

  texturesPathField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Textures Path"));
  AddElement(texturesPathField.get());
  currentY += spacing;

  soundsPathField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Sounds Path"));
  AddElement(soundsPathField.get());
  currentY += spacing;

  scriptsPathField = std::unique_ptr<TextField>(new TextField(
      x + labelWidth, currentY, controlWidth, controlHeight, "Scripts Path"));
  AddElement(scriptsPathField.get());
  currentY += spacing;

  // Save button
  saveButton = std::unique_ptr<Button>(
      new Button(x + w / 2 - 50, currentY, 100, controlHeight, "Save"));
  saveButton->SetOnClick([this]() {
    SaveToSettings();
    std::cout << "Settings saved!" << std::endl;
  });
  AddElement(saveButton.get());

  // Initialize with current settings
  UpdateFromSettings();
}

void SettingsPanel::UpdateFromSettings() {
  ProjectSettings &settings = ProjectSettings::GetInstance();

  // Project settings
  projectNameField->SetText(settings.GetProjectName());

  // Build settings
  debugSymbolsCheckbox->SetChecked(settings.GetDebugSymbols());
  optimizationCheckbox->SetChecked(settings.GetOptimization());
  outputDirectoryField->SetText(settings.GetOutputDirectory());

  // Physics settings
  std::stringstream ss;
  ss << settings.GetFixedTimeStep();
  fixedTimeStepField->SetText(ss.str());

  ss.str("");
  ss << settings.GetGravity();
  gravityField->SetText(ss.str());

  enableCollisionsCheckbox->SetChecked(settings.GetEnableCollisions());

  // Rendering settings
  ss.str("");
  ss << settings.GetTargetFPS();
  targetFPSField->SetText(ss.str());

  vsyncCheckbox->SetChecked(settings.GetVSync());

  int msaaIndex = 0;
  switch (settings.GetMSAA()) {
  case 0:
    msaaIndex = 0;
    break;
  case 2:
    msaaIndex = 1;
    break;
  case 4:
    msaaIndex = 2;
    break;
  case 8:
    msaaIndex = 3;
    break;
  default:
    msaaIndex = 0;
    break;
  }
  msaaDropdown->SetSelectedIndex(msaaIndex);

  shadowsCheckbox->SetChecked(settings.GetShadows());

  // Asset paths
  modelsPathField->SetText(settings.GetAssetPath("models"));
  texturesPathField->SetText(settings.GetAssetPath("textures"));
  soundsPathField->SetText(settings.GetAssetPath("sounds"));
  scriptsPathField->SetText(settings.GetAssetPath("scripts"));
}

void SettingsPanel::SaveToSettings() {
  ProjectSettings &settings = ProjectSettings::GetInstance();

  // Project settings
  settings.SetProjectName(projectNameField->GetText());

  // Build settings
  settings.SetDebugSymbols(debugSymbolsCheckbox->IsChecked());
  settings.SetOptimization(optimizationCheckbox->IsChecked());
  settings.SetOutputDirectory(outputDirectoryField->GetText());

  // Physics settings
  try {
    settings.SetFixedTimeStep(std::stof(fixedTimeStepField->GetText()));
    settings.SetGravity(std::stof(gravityField->GetText()));
  } catch (const std::exception &e) {
    std::cerr << "Error parsing numeric values: " << e.what() << std::endl;
  }

  settings.SetEnableCollisions(enableCollisionsCheckbox->IsChecked());

  // Rendering settings
  try {
    settings.SetTargetFPS(std::stoi(targetFPSField->GetText()));
  } catch (const std::exception &e) {
    std::cerr << "Error parsing numeric values: " << e.what() << std::endl;
  }

  settings.SetVSync(vsyncCheckbox->IsChecked());

  int msaa = 0;
  switch (msaaDropdown->GetSelectedIndex()) {
  case 0:
    msaa = 0;
    break;
  case 1:
    msaa = 2;
    break;
  case 2:
    msaa = 4;
    break;
  case 3:
    msaa = 8;
    break;
  default:
    msaa = 0;
    break;
  }
  settings.SetMSAA(msaa);

  settings.SetShadows(shadowsCheckbox->IsChecked());

  // Asset paths
  settings.SetAssetPath("models", modelsPathField->GetText());
  settings.SetAssetPath("textures", texturesPathField->GetText());
  settings.SetAssetPath("sounds", soundsPathField->GetText());
  settings.SetAssetPath("scripts", scriptsPathField->GetText());

  // Save to file
  settings.SaveToFile(settings.GetProjectPath() + "/project.json");
}
