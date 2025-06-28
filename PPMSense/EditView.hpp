#pragma once

#include "IView.hpp"
#include <string>
#include "fonction.hpp"

enum class OutilActif {
	Aucun,
	Flou,
	noirEtBlanc,
	VisionDaltonien,
	LumiContraste,
	Rogner,
	
};

class EditView : public IView {
public:
    EditView(const std::string& filePath);
    void render() override;

    void refreshImage();

	void renderFloue();

	void renderNoirEtBlanc();

	void renderLumiContraste();

	void renderRogner();

	void renderDaltonien();

	Image getHistoriqueImage(int index) {
		if (index < 0 || index >= historique.size()) {
			return image; // Retourne l'image actuelle si l'index est invalide
		}
		int size = historique.size();
		for (int i = 0; i < size - index - 1; ++i) {
			historique.pop_back();
		}
		Image imageH = historique.back();
		historique.pop_back();
		return imageH;
	}

	void setImage(const Image& img) {
		historique.push_back(image);
		image = img;
		refreshImage();
	}

	void renderSidebar();

	OutilActif outilActif = OutilActif::Aucun;

private:
    std::string imagePath;
    Image image;
    sf::Texture texture;
    bool loaded = false;
	std::vector<Image> historique;
};
