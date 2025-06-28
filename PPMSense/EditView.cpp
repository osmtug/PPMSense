#include "EditView.hpp"
#include "ViewManager.hpp"
#include <imgui.h>
#include <Windows.h>
#include <gl/GL.h>
#include <functional>

int paramInt1 = 0;
int paramInt2 = 0;
int paramInt3 = 0;
int paramInt4 = 0;

EditView::EditView(const std::string& path)
    : imagePath(path) {
    try {
        image = Image(path); 
        sf::Image sfImage = image.genererSFImage(); 
        texture.loadFromImage(sfImage);
        loaded = true;
    }
    catch (const std::exception& e) {
        loaded = false;
        // Tu peux aussi afficher une erreur avec tinyfd_messageBox
    }
}

void EditView::render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Édition d'image", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    // Dimensions générales
    float largeurGauche = ImGui::GetContentRegionAvail().x * 0.65f;
    float hauteurTotale = ImGui::GetContentRegionAvail().y;
    float hauteurBas = 150.0f;
    float hauteurHaut = hauteurTotale - hauteurBas;

    // === ZONE GAUCHE ===
    ImGui::BeginChild("ZoneGauche", ImVec2(largeurGauche, 0), true);
    if (ImGui::Button("< Retour")) {
        viewManager->popView();
        outilActif = OutilActif::Aucun;
        ImGui::EndChild(); // ZoneGauche
        ImGui::End();      // Fenêtre
        return;
    }

    // Partie haute : affichage de l’image
    ImGui::BeginChild("ImageAffichage", ImVec2(0, hauteurHaut), true);

    ImGui::Text("Fichier : %s", imagePath.c_str());

	renderImageWithOverlay(outilActif == OutilActif::Rogner);
    

    ImGui::EndChild(); // Fin partie haute

    // Partie basse : les boutons (hauteur fixe)
    ImGui::BeginChild("BoutonsAction", ImVec2(0, 0), true); // hauteur 0 => prend le reste

    

    if (loaded) {

        float maxWidth = ImGui::GetContentRegionAvail().x;
        float currentX = 0.0f;

        const float spacing = ImGui::GetStyle().ItemSpacing.x;

        auto addButton = [&](const char* label, std::function<void()> onClick) {
            ImVec2 size = ImGui::CalcTextSize(label);
            size.x += 2 * ImGui::GetStyle().FramePadding.x;

            if (currentX + size.x > maxWidth) {
                ImGui::NewLine();
                currentX = 0.0f;
            }

            if (ImGui::Button(label)) {
                onClick();
            }

            currentX += size.x + spacing;
            ImGui::SameLine();
        };

        addButton("Nuance De Gris", [&] { setImage(image.NiveauGris()); });
        addButton("Filtre", [&] { outilActif = OutilActif::Flou; });
        addButton("Noir Et Blanc", [&] { outilActif = OutilActif::noirEtBlanc; paramInt1 = 0; });
        addButton("Vision Daltonnienne", [&] { outilActif = OutilActif::VisionDaltonien; });
        addButton("Luminosite & Contraste", [&] { outilActif = OutilActif::LumiContraste; paramInt1 = 0; paramInt2 = 0; });
        addButton("Rogner", [&] { outilActif = OutilActif::Rogner; paramInt1 = 0; paramInt2 = 0; paramInt3 = 0; paramInt4 = 0; });
        addButton("Sauvegarder", [&] { /* TODO */ });
        addButton("Annuler", [&] {
            image = getHistoriqueImage(historique.size() - 1);
            refreshImage();
        });
    }

    ImGui::EndChild(); // boutons

    ImGui::EndChild(); // ZoneGauche

    // === ZONE DROITE ===
    ImGui::SameLine();
    ImGui::BeginChild("ZoneDroite", ImVec2(0, 0), true); // largeur dynamique
    renderSidebar();
    ImGui::EndChild();

    ImGui::End();
}

void EditView::renderImageWithOverlay(bool modeRognage) {
    if (!loaded) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Image non chargée !");
        return;
    }

    sf::Vector2u imgSize = texture.getSize();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    // Calcul du facteur d’échelle pour garder les proportions
    float scaleX = avail.x / imgSize.x;
    float scaleY = avail.y / imgSize.y;
    float scale = std::min(scaleX, scaleY);

    ImVec2 finalSize = ImVec2(imgSize.x * scale, imgSize.y * scale);

    // Centrer l’image horizontalement
    float centerX = (avail.x - finalSize.x) * 0.5f;
    if (centerX < 0) centerX = 0;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerX);

    // Affichage de l’image
    GLuint textureID = texture.getNativeHandle();
    ImGui::Image((ImTextureID)(intptr_t)textureID, finalSize);

    if (modeRognage) {
        // Dessin des zones à rogner
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 imagePos = ImGui::GetItemRectMin();

        // Couleur semi-transparente noire
        ImU32 overlayColor = IM_COL32(0, 0, 0, 180);

        // Convertir les pixels rognés en taille écran
        float hTop = paramInt1 * scale;
        float hBottom = paramInt2 * scale;
        float wLeft = paramInt3 * scale;
        float wRight = paramInt4 * scale;

        // Rectangle haut
        drawList->AddRectFilled(
            imagePos,
            ImVec2(imagePos.x + finalSize.x, imagePos.y + hTop),
            overlayColor
        );

        // Rectangle bas
        drawList->AddRectFilled(
            ImVec2(imagePos.x, imagePos.y + finalSize.y - hBottom),
            ImVec2(imagePos.x + finalSize.x, imagePos.y + finalSize.y),
            overlayColor
        );

        // Rectangle gauche
        drawList->AddRectFilled(
            ImVec2(imagePos.x, imagePos.y + hTop),
            ImVec2(imagePos.x + wLeft, imagePos.y + finalSize.y - hBottom),
            overlayColor
        );

        // Rectangle droite
        drawList->AddRectFilled(
            ImVec2(imagePos.x + finalSize.x - wRight, imagePos.y + hTop),
            ImVec2(imagePos.x + finalSize.x, imagePos.y + finalSize.y - hBottom),
            overlayColor
        );

        // Bord blanc autour de la zone conservée
        drawList->AddRect(
            ImVec2(imagePos.x + wLeft, imagePos.y + hTop),
            ImVec2(imagePos.x + finalSize.x - wRight, imagePos.y + finalSize.y - hBottom),
            IM_COL32(255, 255, 255, 220),
            0.0f,
            0,
            2.0f
        );
    }
}


void EditView::refreshImage() {
	sf::Image sfImage = image.genererSFImage();
	texture.loadFromImage(sfImage);
}



void EditView::renderSidebar() {
    ImGui::Text("Outils");

    switch (outilActif) {
    case OutilActif::Aucun:
        ImGui::Text("Sélectionnez une action à gauche.");
        break;

    case OutilActif::Flou:
		renderFloue();
        break;

    case OutilActif::noirEtBlanc:
		renderNoirEtBlanc();
        break;

    case OutilActif::VisionDaltonien:
		renderDaltonien();
        break;

	case OutilActif::LumiContraste:
		renderLumiContraste();
		break;

	case OutilActif::Rogner:
        renderRogner();
		break;

    default:
        ImGui::Text("Outil inconnu.");
        break;
    }
}

void EditView::renderFloue() {
    ImGui::Text("Choisir un type de flou :");
    if (ImGui::Button("Flou G3")) {
        setImage(G3.application(image));
    }

    if (ImGui::Button("Flou G5")) {
        setImage(G5.application(image));
    }

    if (ImGui::Button("Contour Sobel")) {
        setImage(image.contourSobel(GRADX, GRADY));
    }

    if (ImGui::Button("Contraste")) {
        setImage(CONTRASTE.application(image));
    }
}

void EditView::renderNoirEtBlanc() {
    ImGui::Text("Paramètre pour le niveau de gris :");
    ImGui::InputInt("Valeur Noir Et Blanc", &paramInt1);

    if (ImGui::Button("Valider")) {
        setImage(image.noirEtBlanc(paramInt1));
    }
}

void EditView::renderDaltonien() {
    ImGui::Text("Choisir un type de daltonnisme :");
    if (ImGui::Button("Vision Deuteranopie")) {
        setImage(image.visionDeuteranopie());
    }

    if (ImGui::Button("Vision Protanopie")) {
        setImage(image.visionProtanopie());
    }

    if (ImGui::Button("Vision Tritanopie")) {
        setImage(image.visionTritanopie());
    }
}

void EditView::renderLumiContraste() {
    ImGui::Text("Parametre pour le niveau de luminosite (en pourcentage) :");
    ImGui::InputInt("Valeur Luminosite", &paramInt1);

    if (ImGui::Button("Augmenter")) {
        setImage(image.luminosityUp(paramInt1));
    }
    ImGui::SameLine();
    if (ImGui::Button("Diminiuer")) {
        setImage(image.luminosityDown(paramInt1));
    }

    ImGui::Text("Parametre pour le niveau de contraste (en pourcentage) :");
    ImGui::InputInt("Valeur Contraste", &paramInt2);

    if (ImGui::Button("Augmenter")) {
        setImage(image.contrasteUp(paramInt2));
    }
    ImGui::SameLine();
    if (ImGui::Button("Diminiuer")) {
        setImage(image.contrasteDown(paramInt2));
    }

    if (ImGui::Button("Reglage Automatique")) {
        setImage(image.reglageAuto());
    }
}

void EditView::renderRogner() {
    ImGui::Text("Rogner l'image :");

    ImGui::SliderInt("Haut", &paramInt1, 0, image.getLongueur() / 2);
    ImGui::SliderInt("Bas", &paramInt2, 0, image.getLongueur() / 2);
    ImGui::SliderInt("Gauche", &paramInt3, 0, image.getLargeur() / 2);
    ImGui::SliderInt("Droite", &paramInt4, 0, image.getLargeur() / 2);

    if (ImGui::Button("Appliquer Rognage")) {
        historique.push_back(image);
        image = image.rognerH(paramInt1);
        image = image.rognerB(paramInt2);
        image = image.rognerG(paramInt3);
        image = image.rognerD(paramInt4);
        refreshImage(); // Recharge la texture pour affichage
		paramInt1 = 0;
		paramInt2 = 0;
		paramInt3 = 0;
		paramInt4 = 0; 
    }
}


