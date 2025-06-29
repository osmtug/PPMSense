#pragma once

#include "IView.hpp"

class NewImageView : public IView {
public:
    NewImageView();
    void render() override;

private:
    int largeur;
    int hauteur;
};