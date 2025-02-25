#ifndef GUI_H
#define GUI_H

#include "../platform.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

class GUIElement {
protected:
    float x, y, width, height;
    bool visible;
    bool enabled;
    std::string id;

public:
    GUIElement(float x, float y, float w, float h);
    virtual ~GUIElement() = default;
    
    virtual void Draw() = 0;
    virtual bool HandleInput(int x, int y, bool clicked) = 0;
    
    void SetPosition(float x, float y);
    void SetSize(float w, float h);
    void SetVisible(bool visible);
    void SetEnabled(bool enabled);
    
    bool IsVisible() const { return visible; }
    bool IsEnabled() const { return enabled; }
};

class Button : public GUIElement {
private:
    std::string text;
    std::function<void()> onClick;

public:
    Button(float x, float y, float w, float h, const std::string& text);
    void Draw() override;
    bool HandleInput(int x, int y, bool clicked) override;
    void SetOnClick(std::function<void()> callback);
};

class Panel : public GUIElement {
private:
    std::vector<std::unique_ptr<GUIElement>> children;

public:
    Panel(float x, float y, float w, float h);
    void Draw() override;
    bool HandleInput(int x, int y, bool clicked) override;
    void AddElement(std::unique_ptr<GUIElement> element);
};

class GUI {
private:
    std::vector<std::unique_ptr<GUIElement>> elements;
    bool visible;

public:
    GUI();
    void Draw();
    void HandleInput(int x, int y, bool clicked);
    void AddElement(std::unique_ptr<GUIElement> element);
    void SetVisible(bool visible);
};

#endif // GUI_H
