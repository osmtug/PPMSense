#pragma once

class ViewManager;

class IView {
public:
    virtual ~IView() = default;
    virtual void render() = 0;
    void setViewManager(ViewManager* vm) { viewManager = vm; }

protected:
    ViewManager* viewManager = nullptr;
};
