#pragma once
#include <vector>
#include <memory>
#include "IView.hpp"

class ViewManager {
public:
    static ViewManager& get() {
        static ViewManager instance;
        return instance;
    }
    void goBack();  
    void pushView(std::unique_ptr<IView> view);
    void popView();
    IView* getCurrentView();

private:
    std::vector<std::unique_ptr<IView>> viewStack;
};
