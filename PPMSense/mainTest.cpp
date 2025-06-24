// test_image_functions.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "fonction.h"

using namespace std;

int mainTEST() {
  // Test de la fonction affiche
  Image i1(
  //Code couleur Rouge
   {{0,0,0},
    {0,0,255},
    {0,255,255}},
   //Code couleur Vert
   {  {0,0,255},
      {0,255,255},
      {255,255,255}},
   //Code couleur Bleu
   {  {255,255,0},
      {255,0,0},
      {0,0,0}});
  i1.affiche();

  // Test de la fonction getPix
  cout<<"pixel en haut à gauche : ";
  i1.getPix(0,0);
  cout<<endl;
  cout<<"pixel en bas à droite : ";
  i1.getPix(2,2);
  cout<<endl;

  // Test de la fonction composante rouge
  Image i2({{1},{2},{3}},{{1},{2},{3}},{{1},{2},{3}});
  if(i2.composanteRouge() == Image ({{1},{2},{3}},{{0},{0},{0}},{{0},{0},{0}})){
    cout<<"test composante rouge : OK"<<endl;
  }
  else{
    cout<<"test composante rouge : RATE"<<endl;
    i2.composanteRouge().affiche();
  }

  // Test de la fonction detection
  if(i2.detection(0, 0, 0)&&!i2.detection(0, 0, 1)){
    cout<<"test detection : OK"<<endl;
  }
  else{
    cout<<"test detection : RATE"<<endl;
  }

  // Test de la fonction NiveauGris

  if(i2.NiveauGris() == Image({{1},{2},{3}},{{1},{2},{3}},{{1},{2},{3}})){
    cout<<"test niveau gris : OK"<<endl;
  }
  else{
    cout<<"test niveau gris : RATE"<<endl;
    i2.NiveauGris().affiche();
  }

  // Test de la fonction visiondeuteranopie
  if(i2.visionDeuteranopie() == Image({{1},{2},{3}},{{0},{0},{0}},{{1},{2},{3}})){
    cout<<"test vision deuteranopie : OK"<<endl;
  }
  else{
    cout<<"test vision deuteranopie : RATE"<<endl;
    i2.visionDeuteranopie().affiche();
  }

  // Test de la fonction visiontritanopie
  if(i2.visionTritanopie() == Image({{1},{2},{3}},{{1},{2},{3}},{{0},{0},{0}})){
    cout<<"test vision tritanopie : OK"<<endl;
  }
  else{
    cout<<"test vision tritanopie : RATE"<<endl;
    i2.visionTritanopie().affiche();
  }

  // Test de la fonction visionprotanopie
  if(i2.visionProtanopie() == Image({{0},{0},{0}},{{1},{2},{3}},{{1},{2},{3}})){
    cout<<"test vision protanopie : OK"<<endl;
  }
  else{
    cout<<"test vision protanopie : RATE"<<endl;
    i2.visionProtanopie().affiche();
  }

  // Test de la fonction noiretblanc
  if(i2.noirEtBlanc(4) == Image({{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}}) && i2.noirEtBlanc(0)== Image({{255},{255},{255}},{{255},{255},{255}},{{255},{255},{255}})){
    cout<<"test noiretblanc : OK"<<endl;
  }
  else{
    cout<<"test noiretblanc : RATE"<<endl;
    i2.noirEtBlanc(4).affiche();
    i2.noirEtBlanc(0).affiche();
  }

  // Test de la fonction luminosityUp
  if(i2.luminosityUp(100) == Image({{2},{4},{6}},{{2},{4},{6}},{{2},{4},{6}})){
    cout<<"test luminosityUp : OK"<<endl;
  }
  else{
    cout<<"test luminosityUp : RATE"<<endl;
    i2.luminosityUp(100).affiche();
  }

  // Test de la fonction luminosityDown
  Image i3({{2},{4},{6}},{{2},{4},{6}},{{2},{4},{6}});
  if(i3.luminosityDown(50) == Image({{1},{2},{3}},{{1},{2},{3}},{{1},{2},{3}})){
    cout<<"test luminosityDown : OK"<<endl;
  }
  else{
    cout<<"test luminosityDown : RATE"<<endl;
    i3.luminosityDown(50).affiche();
  }

  // Test de la fonction contrasteUp
  Image i4({{118},{128},{138}},{{118},{128},{138}},{{118},{128},{138}});
  if(i4.contrasteUp(100) == Image({{108},{128},{148}},{{108},{128},{148}},{{108},{128},{148}})){
    cout<<"test contrasteUp : OK"<<endl;
  }
  else{
    cout<<"test contrasteUp : RATE"<<endl;
    i4.contrasteUp(100).affiche();
  }

  // Test de la fonction contrasteDown
  if(i4.contrasteDown(50) == Image({{123},{128},{133}},{{123},{128},{133}},{{123},{128},{133}})){
    cout<<"test contrasteDown : OK"<<endl;
  }
  else{
    cout<<"test contrasteDown : RATE"<<endl;
    i4.contrasteDown(50).affiche();
  }

  // Test de la fonction rognerH
  if(i1.rognerH(1) == Image({{0,0,255},{0,255,255}},{{0,255,255},{255,255,255}}, {{255,0,0},{0,0,0}})){
    cout<<"test rognerH : OK"<<endl;
  }
  else{
    cout<<"test rognerH : RATE"<<endl;
    i1.rognerH(1).affiche();
  }

  // Test de la fonction rognerB
  if(i1.rognerB(1) == Image({{0,0,0},{0,0,255}},{{0,0,255},{0,255,255}}, {{255,255,0},{255,0,0}})){
    cout<<"test rognerB : OK"<<endl;
  }
  else{
    cout<<"test rognerB : RATE"<<endl;
    i1.rognerB(1).affiche();
  }

  // Test de la fonction rognerD
  if(i1.rognerD(1) == Image({{0,0},{0,0},{0,255}},{{0,0},{0,255},{255,255}},{{255,255},{255,0},{0,0}})){
    cout<<"test rognerD : OK"<<endl;
  }
  else{
    cout<<"test rognerD : RATE"<<endl;
    i1.rognerD(1).affiche();
  }

  // Test de la fonction rognerG
  if(i1.rognerG(2) == Image({{0},{255},{255}},{{255},{255},{255}},{{0},{0},{0}})){
    cout<<"test rognerG : OK"<<endl;
  }
  else{
    cout<<"test rognerG : RATE"<<endl;
    i1.rognerG(2).affiche();
  }

  








    return 0;
}



