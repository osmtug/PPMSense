#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "fonction.h"

using namespace std;

int main()
{

    /*Image i1(
            //Code couleur Rouge
             {{0,0,0,0},
              {0,0,255,255},
              {0,255,255,255},
              {255,255,255,255}},
             //Code couleur Vert
             {  {0,0,255,255},
                {0,255,255,255},
                {255,255,255,0},
                {255,255,0,0}},
             //Code couleur Bleu
             {  {255,255,0,0},
                {255,0,0,0},
                {0,0,0,0},
                {0,0,0,0}});


    i1.affiche();
    cout<<endl;

    (i1.NiveauGris()).affiche();

    Image i2( //Code couleur Rouge
             {{0,0,0,0,255},
              {0,0,255,255,255},
              {0,255,255,255,255},
              {255,255,255,255,255}},
             //Code couleur Vert
             {  {0,0,255,255,255},
                {0,255,255,255,0},
                {255,255,255,0,0},
                {255,255,0,0,0}},
             //Code couleur Bleu
             {  {255,255,0,0,0},
                {255,0,0,0,0},
                {0,0,0,0,0},
                {0,0,0,0,0}});*/
    //l'utilisateur saisie l'image a modifier et l'on declare un objet Image avec ses valeurs
    Image i(saisirfichier());

    //declaration de la variable pour le choix de l'utilisateur
    int c=0;

    //declaration du vecteur pour stocker l'historique
    vector<Image> vi(10);
    vi[0] = i;

    //declaration et initialisation des variables pour gerer l'historique
    int cpt = 0;
    int historique;

    //boucle qui effectue des modification tant que l'utilisateur le souhaite
    while(c!=QUIT)
    {

        //incrementation du compteur pour l'historique (sauf si on affiche les vecteur car alors il n'y a pas de modification
        if(c!=AFFICHE)
            cpt++;

        //affichage et saise des choix de l'utilisateur
        afficheMenu();
        c = choix(i,c);

        //stockage de la nouvelle image dans les image pour les historiques sauf si l'utilisateur veut acceder a l'historique
        if(c!=RECULE_HISTORIQUE)
        {
            vi[cpt%10] = i;
        }
        //l'utilisateur accède a l'historique et saise le nombre de mofication a annulé
        else
        {
            cout<<"combien de modification a annule ? (10 max)"<<endl;
            cin>>historique;
            cpt-=(historique+1);
            //l'image de l'utilisateur prend les valeurs de l'image de l'historique attendue par l'utilisateur
            i = vi[cpt%10];
        }
    }
    //creation de l'image dans un nouveau fichier
    i.faitImage();






    return 0;
}
