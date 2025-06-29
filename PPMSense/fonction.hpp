#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED


#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <imgui.h>


//constante pour les choix de l'utilisateur
const int AFFICHE=1;
const int COMPO_ROUGE=2;
const int NIVEAU_GRIS=3;
const int VISION_DEUTE=4;
const int VISION_PROTA=5;
const int VISION_TRITA=6;
const int NOIR_BLANC=7;
const int LUMI_UP=8;
const int LUMI_DOWN=9;
const int CONTRASTE_UP=10;
const int CONTRASTE_DOWN=11;
const int REGL_AUTO=12;
const int ROGNER_H=13;
const int ROGNER_B=14;
const int ROGNER_G=15;
const int ROGNER_D=16;
const int ROTATION_G=17;
const int ROTATION_D=18;
const int RETOURN_H=19;
const int RETOURN_V=20;
const int AGRANDIS=21;
const int RETRECIS=22;
const int ZOOM=23;
const int FLOUEG3 = 24;
const int FLOUEG5 = 25;
const int SOBEL = 26;
const int FCONTRASTE = 27;
const int RECULE_HISTORIQUE=28;
const int QUIT=29;





using namespace std;

class Image
{
    

public:

    vector<vector<int>> _rouge;
    vector<vector<int>> _vert;
    vector<vector<int>> _bleu;
    int _longueur;
    int _largeur;

    ///constructeur avec des vecteurs
    Image(const vector<vector<int>> &rouge, const vector<vector<int>> &vert, const vector<vector<int>> &bleu);

    ///constructeur vide
    Image();

    Image(int, int);

    ///constructeur qui lit un fichier
    Image(const string nomFichier);

    void setPixel(int x, int y, const ImVec4& color);

    sf::Image genererSFImage();

    ///affichage des vecteur de l'image cible
    void affiche() const;

    ///affichage des valeurs du pixel au coordonné passé en paramètre
    void getPix(int x, int y) const;

    ///retourne une copie de l'image cible avec seulement le vecteur rouge de remplir
    Image composanteRouge() const;

    ///retourne vrai si il y a les valeur passé en paramètre dans l'image cible
    bool detection(int r,int v,int b) const;

    ///retourne une copie de l'image cible avec seulement des nuance de gris
    Image NiveauGris() const;

    ///retourne une copie de l'image cible en simulant la vision d'un daltonien deuteranopie
    Image visionDeuteranopie() const;

    ///retourne une copie de l'image cible en simulant la vision d'un daltonien protanopie
    Image visionProtanopie() const;

    ///retourne une copie de l'image cible en simulant la vision d'un daltonien tritanopie
    Image visionTritanopie() const;

    ///retourne une copie de l'image cible en noir et blanc avec un seuil pour delimité
    Image noirEtBlanc(int seuil) const;

    ///retourne l'histogramme gris de l'image cible
    vector<int> histogrammeGris() const;

    ///retourne l'histogramme de l'image cible
    vector<vector<int>> histogrammeCouleur() const;

    ///retourne une copie de l'image cible en augmentant la luminosité de "factor"(avec "factor" en pourcentage)
    Image luminosityUp(float factor) const;

    ///retourne une copie de l'image cible en baissant la luminosité de "factor"(avec "factor" en pourcentage)
    Image luminosityDown(float factor) const;

    ///retourne une copie de l'image cible en augmentant le contraste de "factor"(avec "factor" en pourcentage)
    Image contrasteUp(float factor) const;

    ///retourne une copie de l'image cible en baissant le contraste de "factor"(avec "factor" en pourcentage)
    Image contrasteDown(float factor) const;

    ///retourne une copie de l'image cible en reglant automatiquement la luminosité et le contraste
    Image reglageAuto() const;

    //Image reglageAutoGris() const;

    ///ecrit dans un fichier, dont le nom est saisie par l'utilisateur, l'image cible
    void faitImage() const;

    void faitImage(string nom) const;

    ///retourne une copie de l'image cible en rognant le haut de n pixel
    Image rognerH(int n) const;

    ///retourne une copie de l'image cible en rognant le bas de n pixel
    Image rognerB(int n) const;

    ///retourne une copie de l'image cible en rognant la droite de n pixel
    Image rognerD(int n) const;

    ///retourne une copie de l'image cible en rognant la gauche de n pixel
    Image rognerG(int n) const;

    ///retourne une copie de l'image cible en la tournant de 90° vers la droite
    Image rotationD() const;

    ///retourne une copie de l'image cible en la tournant de 90° vers la gauche
    Image rotationG() const;

    ///retourne une copie de l'image cible en faisant une symétrie sur un axe verticale
    Image retournementV() const;

    ///retourne une copie de l'image cible en faisant une symétrie sur un axe horizontale
    Image retournementH() const;

    ///retourne une copie de l'image cible en l'agrandissant de *n
    Image agrandissement(int n) const;

    ///retourne une copie de l'image cible en la retrecissant de /n
    Image retrecissement(int n) const;

    ///retourne une copie de l'image cible en faisant un zoom de facteur k
    Image zoom(float k);

    ///retourne la longueur de l'image
    int getLongueur() const;

    ///retourne la largeur de l'image
    int getLargeur() const;

    ///retourne la valeur du pyxel passer en parametre de l'image
    int getCouleurPix(int c,int x, int y) const;

    ///verifie l'egaliter des trois vecteur de l'image
    bool operator==(const Image &i) const;

    ///retourne une image avec le filtre contourSobel
    Image contourSobel(const vector<vector<float>> &vx, const vector<vector<float>> &vy) const;

    ///retourne une image normaliser sans valeur negatif ou au dessus de 255
    void normalisation();
};

///calcule et retourne la valeur de la couleur par rapport au factor pour la luminosité tout en restant dans les borne de 0-255
int calculLumi(int couleur, float factor);

///calcule et retourne la valeur de la couleur par rapport au factor pour le contraste tout en restant dans les borne de 0-255
int calculContraste(int couleur, float factor);

///retourne au format ppm le nom de fichier saise par l'utilisateur
string saisirfichier();

///remplit les vecteur en paramètre avec les valeur du fichier "picture"
bool loadPicture(const string &picture, vector<vector<int>> &red,vector<vector<int>> &green,vector <vector<int>> &blue);

///affiche le menu des different choix de modification possible
void afficheMenu();

///saisie du choix puis application des modification
int choix( Image& i, int c);

///met une chaine de charactère au format ppm si il n'y est pas déjà
void mettreFormatPpm(string& s);



///----------------------classe filtre------------------------

class Filtre
{
    vector<vector<float>> _action;
    int _rayon;

public:

    ///constructeur de filtre
    Filtre(const vector<vector<float>> &action);

    ///créer une nouvelle image à partir de l’image passée en paramètre.
    ///L’image créée est l’image résultante de l’application du filtre sur la première image.
    Image application(const Image &img) const;
};

const Filtre G3({{1.0/16.0,1.0/8.0,1.0/16.0},{1.0/8.0,1.0/4.0,1.0/8.0},{1.0/16.0,1.0/8.0,1.0/16.0}});

const Filtre G5({{1.0/256.0, 4.0/256.0, 6.0/256.0, 4.0/256.0, 1.0/256.0},
                {4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0},
                {6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0, 6.0/256.0},
                {4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0},
                {1.0/256.0, 4.0/256.0, 6.0/256.0, 4.0/256.0, 1.0/256.0}});

const vector<vector<float>> GRADX = {{-1.0, 0.0, 1.0},{-2.0, 0.0, 2.0},{-1.0, 0.0, 1.0}};

const vector<vector<float>> GRADY = {{-1.0, -2.0, -1.0},{0.0, 0.0, 0.0},{1.0, 2.0, 1.0}};

const Filtre CONTRASTE({{0.0,-1.0,0.0},{-1.0,5.0,-1.0},{0.0,-1.0,0.0}});





#endif // FONCTION_H_INCLUDED
