#include "ViewManager.hpp"

void ViewManager::pushView(std::unique_ptr<IView> view) {
    view->setViewManager(this);
    viewStack.push_back(std::move(view));
}

void ViewManager::popView() {
    if (!viewStack.empty()) {
        viewStack.pop_back();
    }
}

void ViewManager::goBack() {
    if (!viewStack.empty()) {
        viewStack.pop_back();
    }
}

IView* ViewManager::getCurrentView() {
    if (viewStack.empty()) return nullptr;
    return viewStack.back().get();
}
