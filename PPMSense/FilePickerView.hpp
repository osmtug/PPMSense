#pragma once

#include "IView.hpp"
#include <string>

class FilePickerView : public IView {
public:
    FilePickerView();
    void render() override;

private:
    char filePath[1024]; // <-- Chemin du fichier sélectionné
};