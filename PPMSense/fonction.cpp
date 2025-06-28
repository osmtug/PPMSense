#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "fonction.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;

///-----------------partie 1 : NBGC-----------------

Image::Image(const vector<vector<int>> &rouge,const vector<vector<int>> &vert, const vector<vector<int>> &bleu)
{
    if (rouge.size() != vert.size() || rouge.size() != bleu.size() || bleu.size() != vert.size() || rouge[0].size() != vert[0].size() || rouge[0].size() != bleu[0].size() || bleu[0].size() != vert[0].size())
    {
        throw(invalid_argument("le vecteur des couleur n'ont pas la meme taille"));
    }
    _rouge = rouge;
    _vert = vert;
    _bleu = bleu;
    _longueur = rouge.size();
    _largeur = rouge[0].size();

}

Image::Image()
{
    _rouge = {{0}};
    _vert = {{0}};
    _bleu = {{0}};
    _longueur = _rouge.size();
    _largeur = _rouge[0].size();
}



Image::Image(const string nomFichier)
{
    if (!loadPicture(nomFichier, _rouge, _vert, _bleu)) {
		throw(invalid_argument("le fichier n'existe pas ou n'est pas au format ppm"));
    }
    _longueur = _rouge.size();
    _largeur = _rouge[0].size();
}

sf::Image Image::genererSFImage() {
    sf::Image img;
    img.create(_largeur, _longueur);

    for (int y = 0; y < _longueur; ++y) {
        for (int x = 0; x < _largeur; ++x) {
            
            sf::Color pixel(
                static_cast<sf::Uint8>(_rouge[y][x]),
                static_cast<sf::Uint8>(_vert[y][x]),
                static_cast<sf::Uint8>(_bleu[y][x])
            );
            img.setPixel(x, y, pixel);
        }
    }

    return img;
}

void Image::affiche() const
{
    cout << "rouge : " << endl;
    for(int i=0; i<_rouge.size(); i++)
    {
        cout << "ligne " << i << " : ";
        for(int j=0; j<_rouge[i].size(); j++)
        {
            cout << _rouge[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "bleu : " << endl;
    for(int i=0; i<_bleu.size(); i++)
    {
        cout << "ligne " << i<< " : ";
        for(int j=0; j<_bleu[i].size(); j++)
        {
            cout << _bleu[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "vert : " << endl;
    for(int i=0; i<_vert.size(); i++)
    {
        cout << "ligne " << i<< " : ";
        for(int j=0; j<_vert[i].size(); j++)
        {
            cout << _vert[i][j] << " ";
        }
        cout << endl;
    }
    if(_rouge == _vert)
        cout<<"egal"<<endl;
    else
        cout<<"pas egal"<<endl;
}

void Image::getPix(int x, int y) const
{
    cout << "rouge : " << _rouge[x][y] << endl;
    cout << "vert : " << _vert[x][y] << endl;
    cout << "bleu : " << _bleu[x][y] << endl;
}

Image Image::composanteRouge() const
{

    vector<vector<int>> vide(_rouge.size(), vector<int>(_rouge[0].size(), 0));
    return (Image(_rouge, vide, vide));
}

bool Image::detection(int r,int v,int b) const
{
    bool trouver = false;
    int i = 0;
    while(!trouver && i < _rouge.size())
    {

        int j = 0;
        while(!trouver && j < _rouge[0].size())
        {
            if(_rouge[i][j] == r && _vert[i][j] == v && _bleu[i][j] == b)
            {
                trouver = true;
            }
            j++;
        }
        i++;
    }
    return (trouver);
}

Image Image::NiveauGris() const
{
    vector<vector<int>> gris;

    for(int i=0; i<_rouge.size(); i++)
    {
        vector<int> l;
        for(int j=0; j<_rouge[i].size(); j++)
        {
            int moyenne = (_rouge[i][j] + _vert[i][j] + _bleu[i] [j]) / 3;
            l.push_back(moyenne);
        }
        gris.push_back(l);
    }
    return(Image(gris, gris, gris));
}

Image Image::visionDeuteranopie() const
{
    vector<vector<int>> vert(_rouge.size(), vector<int>
                             (_rouge[0].size(), 0));
    return(Image(_rouge, vert, _bleu));
}

Image Image::visionProtanopie() const
{
    vector<vector<int>> rouge(_rouge.size(), vector<int>(_rouge[0].size(), 0));
    return(Image(rouge, _vert, _bleu));
}

Image Image::visionTritanopie() const
{
    vector<vector<int>> bleu(_rouge.size(), vector<int>(_rouge[0].size(), 0));
    return(Image(_rouge, _vert, bleu));
}

Image Image::noirEtBlanc(int seuil) const
{
    vector<vector<int>> noirBlanc(_rouge.size(), vector<int>(_rouge[0].size(), 0));
    Image igris = NiveauGris();

    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[0].size(); j++)
        {
            if(igris._rouge[i][j] < seuil)
            {
                noirBlanc[i][j] = 0;
            }
            else
            {
                noirBlanc[i][j] = 255;
            }
        }
    }
    return(Image(noirBlanc, noirBlanc, noirBlanc));
}


vector<int> Image::histogrammeGris() const
{
    vector<int> histogram(256, 0);
    Image igris = NiveauGris();
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            histogram[igris._rouge[i][j]] ++;
        }
    }
    return histogram;
}

vector<vector<int>> Image::histogrammeCouleur() const
{
    vector<vector<int>> histogram(3, vector<int>(256, 0));
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            histogram[0][_rouge[i][j]] ++;
            histogram[1][_vert[i][j]] ++;
            histogram[2][_bleu[i][j]] ++;
        }
    }
    return histogram;
}

Image Image::luminosityUp(float factor) const
{
    factor = factor / 100+1;
    vector<vector<int>> rouge(_rouge.size());
    vector<vector<int>> vert(_rouge.size());
    vector<vector<int>> bleu(_rouge.size());
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            rouge[i].push_back(calculLumi(_rouge[i][j], factor));
            vert[i].push_back(calculLumi(_vert[i][j], factor));
            bleu[i].push_back(calculLumi(_bleu[i][j], factor));
        }
    }
    return(Image(rouge, vert, bleu));
}

Image Image::luminosityDown(float factor) const
{
    factor = (100 - factor)/100;
    vector<vector<int>> rouge(_rouge.size());
    vector<vector<int>> vert(_rouge.size());
    vector<vector<int>> bleu(_rouge.size());
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            rouge[i].push_back(calculLumi(_rouge[i][j], factor));
            vert[i].push_back(calculLumi(_vert[i][j], factor));
            bleu[i].push_back(calculLumi(_bleu[i][j], factor));
        }
    }
    return(Image(rouge, vert, bleu));
}

int calculLumi(int couleur, float factor)
{
    int result = couleur * factor;
    if(result > 255)
    {
        return(255);
    }
    else
    {
        if(result < 0)
        {
            return(0);
        }
        else
            return(result);
    }
}

Image Image::contrasteUp(float factor) const
{
    factor = (factor / 100)+1;
    vector<vector<int>> rouge(_rouge.size());
    vector<vector<int>> vert(_rouge.size());
    vector<vector<int>> bleu(_rouge.size());
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            rouge[i].push_back(calculContraste(_rouge[i][j], factor));
            vert[i].push_back(calculContraste(_vert[i][j], factor));
            bleu[i].push_back(calculContraste(_bleu[i][j], factor));
        }
    }
    return(Image(rouge, vert, bleu));
}

Image Image::contrasteDown(float factor) const
{
    factor = (100 - factor)/100;
    vector<vector<int>> rouge(_rouge.size());
    vector<vector<int>> vert(_rouge.size());
    vector<vector<int>> bleu(_rouge.size());
    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[i].size(); j++)
        {
            rouge[i].push_back(calculContraste(_rouge[i][j], factor));
            vert[i].push_back(calculContraste(_vert[i][j], factor));
            bleu[i].push_back(calculContraste(_bleu[i][j], factor));
        }
    }
    return(Image(rouge, vert, bleu));
}

int calculContraste(int couleur, float factor)
{
    int result = (couleur-128)*factor+128;
    if(result > 255)
    {
        return(255);
    }
    else
    {
        if(result < 0)
        {
            return(0);
        }
        else
            return(result);
    }
}

Image Image::reglageAuto() const
{
    float moyenne = 0;
    int valmin = _rouge[0][0];
    int valmax = _rouge[0][0];

    Image imgregler(_rouge, _vert, _bleu);

    for(int i=0; i<_rouge.size(); i++)
    {
        for(int j=0; j<_rouge[0].size(); j++)
        {

            //calcul de la moyenne
            moyenne += _rouge[i][j] + _vert[i][j] + _bleu[i][j];

            //calcul de la valeur minimal
            if(_rouge[i][j] < valmin)
            {
                valmin = _rouge[i][j];
            }
            if(valmin > _vert[i][j])
            {
                valmin = _vert[i][j];
            }
            if(valmin > _bleu[i][j])
            {
                valmin = _bleu[i][j];
            }

            //calcul de la valeur maximal
            if(_rouge[i][j] > valmax)
            {
                valmax = _rouge[i][j];
            }
            if(valmax < _vert[i][j])
            {
                valmax = _vert[i][j];
            }
            if(valmax < _bleu[i][j])
            {
                valmax = _bleu[i][j];
            }
        }
    }

    //modification de la luminositer en fonction de la moyenne
    moyenne = moyenne / ((_rouge.size() * _rouge[0].size())*3);

    float factmoy = (128.0/moyenne);

    if(factmoy >1)
    {
        imgregler = imgregler.luminosityUp((factmoy-1)*100);
    }
    else
    {
        imgregler = imgregler.luminosityDown((1-factmoy)*100);
    }


    //modification du contraste en fonction des valeurs minimal et maximal
    valmax = valmax*factmoy;
    valmin = valmin*factmoy;

    if(valmax>255)
        valmax = 255;

    float factmax = 128/(valmax-128);
    float factmin = (-valmin+(valmin-128))/(valmin-128);


    if(factmax < factmin)
    {
        imgregler = imgregler.contrasteUp((factmax-1)*100);
    }
    else
    {
        imgregler = imgregler.contrasteUp((factmin-1)*100);
    }


    return(imgregler);

}

///-----------------partie 2 : Le menu/les fichiers-----------------

string saisirfichier()
{
    string nomFichier;
    cout << "Entrez le nom du fichier : ";
    cin >> nomFichier;
    mettreFormatPpm(nomFichier);
    return nomFichier;
}

void afficheMenu()
{
    cout << "que voulez vous faire ?" << endl;
    cout <<AFFICHE<< ". affiche l'image" << endl;
    cout <<COMPO_ROUGE<< ". composante rouge" << endl;
    cout <<NIVEAU_GRIS<< ". niveau gris" << endl;
    cout <<VISION_DEUTE<< ". vision Deuteranopie" << endl;
    cout <<VISION_PROTA<< ". vision Protanopie" << endl;
    cout <<VISION_TRITA<< ". vision Tritanopie" << endl;
    cout <<NOIR_BLANC<< ". noir et blanc" << endl;
    cout <<LUMI_UP<< ". augmenter la luminosite" << endl;
    cout <<LUMI_DOWN<< ". diminuer la luminosite" << endl;
    cout <<CONTRASTE_UP<< ". augmenter le contraste" << endl;
    cout <<CONTRASTE_DOWN<< ". diminuer le contraste" << endl;
    cout <<REGL_AUTO<< ". reglage auto" << endl;
    cout <<ROGNER_H<< ". rogner le haut" << endl;
    cout <<ROGNER_B<< ". rogner le bas" << endl;
    cout <<ROGNER_G<< ". rogner la gauche" << endl;
    cout <<ROGNER_D<< ". rogner la droite" << endl;
    cout <<ROTATION_G<< ". tourner vers la gauche(90 degres)" << endl;
    cout <<ROTATION_D<< ". tourner vers la droite(90 degres)" << endl;
    cout <<RETOURN_H<< ". retourne l'image horizontalement" << endl;
    cout <<RETOURN_V<< ". retourne l'image verticalement" << endl;
    cout <<AGRANDIS<< ". agrandis l'image" << endl;
    cout <<RETRECIS<< ". retrcis l'image" << endl;
    cout <<ZOOM<< ". zoom l'image" << endl;
    cout <<FLOUEG3<< ". applique un filtre floue G3 a l'image" << endl;
    cout <<FLOUEG5<< ". applique un filtre floue G5 a l'image" << endl;
    cout <<SOBEL<< ". applique un filtre floue contour sobel a l'image" << endl;
    cout <<RECULE_HISTORIQUE<< ". retourner en arriere, annule des modification" << endl;
    cout <<QUIT<< ". quitter" << endl;
}

int choix(Image& i, int c)
{
    cin >> c;
    int entierUser;
    switch(c)
    {
    case AFFICHE:
        i.affiche();
        break;

    case COMPO_ROUGE:
        i = i.composanteRouge();
        break;

    case NIVEAU_GRIS:
        i = i.NiveauGris();
        break;

    case VISION_DEUTE:
        i = i.visionDeuteranopie();
        break;

    case VISION_PROTA:
        i = i.visionProtanopie();
        break;

    case VISION_TRITA:
        i = i.visionTritanopie();
        break;

    case NOIR_BLANC:
        cout << "Entrez le seuil : ";
        cin >> entierUser;
        i = i.noirEtBlanc(entierUser);
        break;

    case LUMI_UP:
        float factor;
        cout << "Entrez le facteur luminositée : ";
        cin >> factor;
        i = i.luminosityUp(factor);
        break;

    case LUMI_DOWN:
        cout << "Entrez le facteur luminositée : ";
        cin >> factor;
        i = i.luminosityDown(factor);
        break;

    case CONTRASTE_UP:
        cout << "Entrez le facteur contraste : ";
        cin >> factor;
        i = i.contrasteUp(factor);
        break;

    case CONTRASTE_DOWN:
        cout << "Entrez le facteur contraste : ";
        cin >> factor;
        i = i.contrasteDown(factor);
        break;

    case REGL_AUTO:
        i = i.reglageAuto();
        break;

    case ROGNER_H:
        cout<<"Entrez le nombre de ligne a rogner : "<<endl;
        cin>>entierUser;
        i = i.rognerH(entierUser);
        break;

    case ROGNER_B:
        cout<<"Entrez le nombre de ligne a rogner : "<<endl;
        cin>>entierUser;
        i = i.rognerB(entierUser);
        break;

    case ROGNER_G:
        cout<<"Entrez le nombre de colonne a rogner : "<<endl;
        cin>>entierUser;
        i = i.rognerG(entierUser);
        break;

    case ROGNER_D:
        cout<<"Entrez le nombre de colonne a rogner : "<<endl;
        cin>>entierUser;
        i = i.rognerD(entierUser);
        break;

    case ROTATION_G:
        i = i.rotationG();
        break;

    case ROTATION_D:
        i = i.rotationD();
        break;

    case RETOURN_H:
        i = i.retournementH();
        break;

    case RETOURN_V:
        i = i.retournementV();
        break;

    case AGRANDIS:
        cout<<"Entrez le facteur de la taille ? "<<endl;
        cin>>entierUser;
        i = i.agrandissement(entierUser);
        break;

    case RETRECIS:
        cout<<"Entrez le diviseur de la taille ? "<<endl;
        cin>>entierUser;
        i = i.retrecissement(entierUser);
        break;

    case ZOOM:
        cout<<"Entrez le facteur zoom(agrandissement si k>1 et rétrécissement si 0<k<1)"<<endl;
        cin>>factor;
        i = i.zoom(factor);
        break;
    case FLOUEG3:
        i = G3.application(i);
        break;
    case FLOUEG5:
        i = G5.application(i);
        break;
    case SOBEL:
        i = i.contourSobel(GRADX, GRADY);
        i.normalisation();
        break;
    case FCONTRASTE:
        i = CONTRASTE.application(i);
        i.normalisation();
        break;
    default :
        cout<<"veuillez saisire un nombre valide"<<endl;
    }

    return(c);


}

bool loadPicture(const string &picture, vector<vector<int>> &red,
                 vector<vector<int>> &green,
                 vector <vector<int>> &blue)
{
    // Declaration des variables
    string line; // pour recuperer les lignes du fichier image au format .ppm, qui est code en ASCII.
    string format; //pour recuperer le format de l'image : celui-ci doit être de la forme P3
    string name; // au cas où l'utilisateur se trompe dans le nom de l'image a charger, on redemande le nom.
    int taille;
    vector <int> mypixels; // pour recuperer les donnees du fichier de maniere lineaire. On repartira ensuite ces donnees dans les tableaux correspondants
    ifstream entree; // Declaration d'un "flux" qui permettra ensuite de lire les donnees de l'image.
    int hauteur; // pour bien verifier que l'image est carree, et de taille respectant les conditions fixees par l'enonce
    // Initialisation des variables
    name = picture;
    // Permet d'ouvrir le fichier portant le nom picture
    // ouverture du fichier portant le nom picture
    entree.open(name);
    // On verifie que le fichier a bien ete ouvert. Si cela n'est pas le cas, on redemande un nom de fichier valide
    while (!entree)
    {
        //cin.rdbuf(oldbuf);
        cerr << "Erreur! Impossible de lire de fichier " << name << " ! " << endl;
        return false;
    }
    // Lecture du nombre definissant le format (ici P3)
    entree >> format;
    // Lecture du commentaire
    while (std::getline(entree, line)) {
        // Si la ligne est vide, on continue
        if (line.empty()) continue;

        // Si ce n'est pas un commentaire, on suppose que c'est la ligne des dimensions
        if (line[0] != '#') {
            std::istringstream iss(line);
            iss >> taille >> hauteur;
            break;
        }
        // Sinon, on ignore la ligne car c'est un commentaire
    }

    //// Lecture du commentaire
    //getline(entree, line); // on lit la ligne du commentaire, qui est optionnelle dans le format PPM
    ////lecture des dimensions
    //entree >> taille >> hauteur;
    
    

    // On verifie que l'image a une taille qui verifie bien les conditions requises par l'enonce.
    // Si cela n'est pas le cas, on redemande un fichier valide, et ce, tant que necessaire.
    while (format != "P3")
    {
        if (format != "P3")
        {
            cerr << "Erreur! L'image que vous nous avez donnee a un format ne verifiant pas les conditions requises." << endl;
            cerr << "L'image que vous nous avez donnee doit etre codee en ASCII et non en brut." << endl;
        }
        entree.close();
        // On va redemander un nom de fichier valide.
		return false;
    }
    //Lecture de la valeur max
    getline(entree, line);
    //Lecture des donnees et ecriture dans les tableaux :
    // Pour plus de simplicite, on stocke d'abord toutes les donnees dans mypixels
    // dans l'ordre de lecture puis ensuite on les repartira dans les differents tableaux.
    //Les donnees stockees dans mypixels sont de la forme RGB RGB RGB ....
    // Il faudra donc repartir les valeurs R correspondant a la composante rouge de l'image
    // dans le tableau red, de même pour G et B.
    int pix;
    mypixels.resize(3*taille*hauteur); // taille fixe : on alloue une fois pour toutes
    for (int i = 0; i < 3*taille*hauteur; i++)
    {
        entree >> pix;
        mypixels[i]=pix;
    }
    // Remplissage des 3 tableaux : on repartit maintenant les valeurs dans les bonnes composantes
    // Comme dans mypixels, les donnees sont stockees de la maniere suivante : RGB RGB RGB, il faut mettre
    // les valeurs correspondant a la composante rouge dans red, ...
    // Ainsi, les valeurs de la composante rouge correspondent aux valeurs stockes aux indices
    // congrus a 0 mod 3 dans mypixels, que les valeurs de la composante verte correspond aux valeurs
    // stockes aux indices sont congrus a 1 mod 3, ...
    // les valeurs d'une ligne
    int val;
    red.resize(hauteur);
    green.resize(hauteur);
    blue.resize(hauteur);
    for (int i = 0; i < hauteur; i++)
    {
        vector <int> ligneR(taille);
        vector <int> ligneB(taille);  // les lignes ont toutes la même taille
        vector <int> ligneG(taille);
        for (int j = 0; j < taille; j++)
        {
            val =  mypixels[3*j + 3*taille*i];
            ligneR[j]=val;
            val = mypixels[3*j + 1 + 3*taille*i];
            ligneG[j]=val;
            val = mypixels[3*j + 2 + 3*taille*i];
            ligneB[j]=val;
        }
        red[i]=ligneR;
        green[i]=ligneG;
        blue[i]=ligneB;
    }
    // Informations a l'utilisateur pour dire que tout s'est bien passe
    cout << " L'image " << name << " a bien ete chargee dans les tableaux ." << endl;
    entree.close();
}

void Image::faitImage() const
{
    string nom;
    cout << "enregistrer l'image sous quelle nom ? "<<endl;
    cin >> nom;
    mettreFormatPpm(nom);
    ofstream sortie;
    sortie.open(nom);
    sortie << "P3" << endl;
    sortie << "# " << nom << endl;
    sortie << _largeur << " " << _longueur<< endl;
    sortie << "255" << endl;
    for (int i = 0; i < _longueur; i++)
    {
        for (int j = 0; j < _largeur; j++)
        {
            sortie << _rouge[i][j] << " " << _vert[i][j] << " " << _bleu[i][j] << " ";
        }
        sortie << endl;
    }
}

void Image::faitImage(string nom) const
{
    mettreFormatPpm(nom);
    ofstream sortie;
    sortie.open(nom);
    sortie << "P3" << endl;
    sortie << "# " << nom << endl;
    sortie << _largeur << " " << _longueur << endl;
    sortie << "255" << endl;
    for (int i = 0; i < _longueur; i++)
    {
        for (int j = 0; j < _largeur; j++)
        {
            sortie << _rouge[i][j] << " " << _vert[i][j] << " " << _bleu[i][j] << " ";
        }
        sortie << endl;
    }
}

///-----------------partie 3 : la geometrie/la taille-----------------

Image Image::rognerH(int n) const
{
    vector<vector<int>> r(_longueur-n);
    vector<vector<int>> v( _longueur-n);
    vector<vector<int>> b( _longueur-n);
    for(int i = n; i < _rouge.size(); i++)
    {
        for(int j = 0; j < _rouge[i].size(); j++)
        {
            r[i-n].push_back(_rouge[i][j]);
            v[i-n].push_back(_vert[i][j]);
            b[i-n].push_back(_bleu[i][j]);
        }
    }
    return(Image(r, v, b));
}

Image Image::rognerG(int n) const
{
    vector<vector<int>> r(_longueur);
    vector<vector<int>> v( _longueur);
    vector<vector<int>> b( _longueur);
    for(int i = 0; i < _rouge.size(); i++)
    {
        for(int j = n; j < _rouge[i].size(); j++)
        {
            r[i].push_back(_rouge[i][j]);
            v[i].push_back(_vert[i][j]);
            b[i].push_back(_bleu[i][j]);
        }
    }
    return(Image(r, v, b));
}

Image Image::rognerB(int n) const
{
    vector<vector<int>> r(_longueur-n);
    vector<vector<int>> v( _longueur-n);
    vector<vector<int>> b( _longueur-n);
    for(int i = 0; i < _rouge.size() - n; i++)
    {
        for(int j = 0; j < _rouge[i].size(); j++)
        {
            r[i].push_back(_rouge[i][j]);
            v[i].push_back(_vert[i][j]);
            b[i].push_back(_bleu[i][j]);
        }
    }
    return(Image(r, v, b));
}

Image Image::rognerD(int n) const
{
    vector<vector<int>> r(_longueur);
    vector<vector<int>> v( _longueur);
    vector<vector<int>> b( _longueur);
    for(int i = 0; i < _rouge.size(); i++)
    {
        for(int j = 0; j < _rouge[i].size() - n; j++)
        {
            r[i].push_back(_rouge[i][j]);
            v[i].push_back(_vert[i][j]);
            b[i].push_back(_bleu[i][j]);
        }
    }
    return(Image(r, v, b));
}

Image Image::rotationD() const
{
    vector<vector<int>> r(_largeur);
    vector<vector<int>> v( _largeur);
    vector<vector<int>> b( _largeur);
    for(int i = 0; i <= _rouge[0].size()-1; i++)
    {
        for(int j = _rouge.size()-1; j >= 0; j--)
        {
            r[i].push_back(_rouge[j][i]);
            v[i].push_back(_vert[j][i]);
            b[i].push_back(_bleu[j][i]);
        }
    }
    return(Image(r, v, b));
}

Image Image::rotationG() const
{
    vector<vector<int>> r(_largeur);
    vector<vector<int>> v( _largeur);
    vector<vector<int>> b( _largeur);
    int cpt = 0;
    for(int i = _rouge[0].size()-1; i >= 0; i--)
    {
        for(int j = 0; j < _rouge.size(); j++)
        {
            r[cpt].push_back(_rouge[j][i]);
            v[cpt].push_back(_vert[j][i]);
            b[cpt].push_back(_bleu[j][i]);
        }
        cpt++;
    }
    return(Image(r, v, b));
}

Image Image::retournementV() const
{
    vector<vector<int>> r(_rouge.size());
    vector<vector<int>> v(_rouge.size());
    vector<vector<int>> b(_rouge.size());
    for(int i = 0; i <= _rouge.size()-1; i++)
    {
        for(int j = _rouge[0].size()-1; j >= 0; j--)
        {
            r[i].push_back(_rouge[i][j]);
            v[i].push_back(_vert[i][j]);
            b[i].push_back(_bleu[i][j]);
        }
    }
    return(Image(r, v, b));
}

Image Image::retournementH() const
{
    vector<vector<int>> r(_rouge.size());
    vector<vector<int>> v(_rouge.size());
    vector<vector<int>> b(_rouge.size());
    int cpt = 0;
    for(int i = _rouge.size()-1; i >= 0; i--)
    {
        for(int j = 0; j < _rouge[0].size() ; j++)
        {
            r[cpt].push_back(_rouge[i][j]);
            v[cpt].push_back(_vert[i][j]);
            b[cpt].push_back(_bleu[i][j]);
        }
        cpt++;
    }
    return(Image(r, v, b));
}

Image Image::agrandissement(int n) const
{
    vector<vector<int>> r(_rouge.size()*n);
    vector<vector<int>> v(_rouge.size()*n);
    vector<vector<int>> b(_rouge.size()*n);
    vector<int> lr(_rouge[0].size()*n);
    vector<int> lv(_rouge[0].size()*n);
    vector<int> lb(_rouge[0].size()*n);
    for(int i = 0; i < _rouge.size(); i++)
    {
        for(int j = 0; j < _rouge[0].size(); j++)
        {
            for(int k = (j*n); k < (n+j*n); k++)
            {
                lr[k] = _rouge[i][j];
                lv[k] = _vert[i][j];
                lb[k] = _bleu[i][j];
            }
        }
        for(int l = (i*n); l < (n+i*n); l++)
        {
            r[l] = lr;
            v[l] = lv;
            b[l] = lb;
        }
    }
    return(Image(r, v, b));
}

Image Image::retrecissement(int n) const
{
    vector<vector<int>> r(_rouge.size()/n);
    vector<vector<int>> v(_rouge.size()/n);
    vector<vector<int>> b(_rouge.size()/n);
    int moyenneR, moyenneV, moyenneB;
    for(int i=0; i<_largeur/n; i++)
    {
        for(int j=0; j<_longueur/n; j++)
        {
            moyenneR = 0;
            moyenneV = 0;
            moyenneB = 0;
            for(int k = (i*n); k < (n+i*n); k++)
            {
                for(int l=j*n; l<n+j*n; l++)
                {
                    moyenneR += _rouge[k][l];
                    moyenneV += _vert[k][l];
                    moyenneB += _bleu[k][l];
                }
            }
            r[i].push_back(moyenneR /(n*n));
            v[i].push_back(moyenneV /(n*n));
            b[i].push_back(moyenneB /(n*n));
        }
    }
    return(Image(r, v, b));
}


Image Image::zoom(float k)
{

    vector<vector<int>> r(_longueur * k, vector<int>(_largeur * k));
    vector<vector<int>> v(_longueur * k, vector<int>(_largeur * k));
    vector<vector<int>> b(_longueur * k, vector<int>(_largeur * k));

    for (int i = 0; i < _longueur * k; ++i)
    {
        for (int j = 0; j < _largeur * k; ++j)
        {
            int original_i = i / k;
            int original_j = j / k;

            r[i][j] = _rouge[original_i][original_j];
            v[i][j] = _vert[original_i][original_j];
            b[i][j] = _bleu[original_i][original_j];
        }
    }
    return(Image(r,v,b));
}

void mettreFormatPpm(string& s)
{
    if(s.size()>4)
    {
        if(s[s.size()-1]!='m'||s[s.size()-2]!='p'||s[s.size()-3]!='p'||s[s.size()-4]!='.')
        {
            s.push_back('.');
            s.push_back('p');
            s.push_back('p');
            s.push_back('m');
        }
    }
    else
    {
        s.push_back('.');
        s.push_back('p');
        s.push_back('p');
        s.push_back('m');
    }
}


///-----------------partie 4 : les filtre-----------------

Filtre::Filtre(const vector<vector<float>> &action){
    _action = action;
    _rayon = (action.size()-1)/2;

}

int Image::getLongueur() const{
    return _longueur;
}

int Image::getLargeur() const{
    return _largeur;
}

int Image::getCouleurPix(int c,int x, int y) const{
    if(x<0 || x>_longueur-1 || y<0 || y>_largeur-1)
        return 0;
    switch(c){
        case 0: return _rouge[x][y];
        case 1: return _vert[x][y];
        case 2: return _bleu[x][y];
    }
  return 0;
}

Image Filtre::application(const Image &img) const{
    vector<vector<int>> rouge(img.getLongueur());
    vector<vector<int>> vert(img.getLongueur());
    vector<vector<int>> bleu(img.getLongueur());
    int pixr, pixv, pixb;
    for(int i=0;i<img.getLongueur(); i++){
        for(int j=0;j<img.getLargeur(); j++){
            pixr = 0;
            pixv = 0;
            pixb = 0;
            for(int k=-_rayon; k<=_rayon; k++){
                for(int l=-_rayon; l<=_rayon; l++){
                    pixr += img.getCouleurPix(0,i+k,j+l)*_action[k+_rayon][l+_rayon];
                    pixv += img.getCouleurPix(1,i+k,j+l)*_action[k+_rayon][l+_rayon];
                    pixb += img.getCouleurPix(2,i+k,j+l)*_action[k+_rayon][l+_rayon];
                }
            }


          rouge[i].push_back(pixr);
          vert[i].push_back(pixv);
          bleu[i].push_back(pixb);

        }
    }
    return(Image(rouge, vert, bleu));
}

Image Image::contourSobel(const vector<vector<float>> &vx, const vector<vector<float>> &vy) const{
    vector<vector<int>> rouge(_longueur);
    vector<vector<int>> vert(_longueur);
    vector<vector<int>> bleu(_longueur);
    Filtre fx(vx);
    Filtre fy(vy);
    Image ix = fx.application(*this);
    Image iy = fy.application(*this);
    for(int i=0; i<_longueur; i++){
        for(int j=0; j<_largeur; j++){
            rouge[i].push_back(sqrt(pow(ix._rouge[i][j],2)+pow(iy._rouge[i][j],2)));
            vert[i].push_back(sqrt(pow(ix._vert[i][j],2)+pow(iy._vert[i][j],2)));
            bleu[i].push_back(sqrt(pow(ix._bleu[i][j],2)+pow(iy._bleu[i][j],2)));
        }
    }
    return(Image(rouge, vert, bleu));
}

void Image::normalisation(){
    for(int i=0; i<_longueur; i++){
        for(int j=0; j<_largeur; j++){
            if(_rouge[i][j]>255){
                _rouge[i][j] = 255;
            }
            else{
                if( _rouge[i][j]<0){
                    _rouge[i][j] = 0;
                }
            }

            if(_vert[i][j]>255){
                _vert[i][j] = 255;
            }
            else{
                if( _vert[i][j]<0){
                    _vert[i][j] = 0;
                }
            }

            if(_bleu[i][j]>255){
                _bleu[i][j] = 255;
            }
            else{
                if( _bleu[i][j]<0){
                    _bleu[i][j] = 0;
                }
            }
        }
    }
}





bool Image::operator==(const Image &i) const{
    return(_rouge==_vert && _rouge==_bleu);
}







