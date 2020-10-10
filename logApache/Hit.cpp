/*************************************************************************
                           Hit  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Hit> (fichier Hit.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <cstdlib>
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Hit.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques

//------------------------------------------------- Surcharge d'opérateurs
ifstream &operator>>(ifstream &fichierLog, Hit &unHit)
// Lecture séquentielle des différentes parties d'une ligne du fichier log
{
    string garbage; // chaine de caractère qui stocke temporairement les inforamtions inutiles
    string hourAsString;
    unHit.cible = "";
    unHit.referer = "";
    unHit.hour = 0;

    getline(fichierLog, garbage, ':');
    // On verifie que la ligne n'est pas vide
    // si c'est le cas, on est alors en fin de fichier.
    if (garbage == "")
    {
        // fin de fichier
        return fichierLog;
    }

    getline(fichierLog, hourAsString, ':');

    // on est sûr que la chaîne contient un nombre (hypothèse du sujet)
    unHit.hour = atoi(hourAsString.c_str());

    getline(fichierLog, garbage, ' ');
    getline(fichierLog, garbage, ' ');
    getline(fichierLog, garbage, ' ');
    getline(fichierLog, unHit.cible, ' '); // on récupère la cible

    getline(fichierLog, garbage, ' ');
    getline(fichierLog, garbage, '"');
    getline(fichierLog, unHit.referer, '"'); // on récupère le referer

    // on ignore le reste de la ligne jusqu'à la fin de ligne
    getline(fichierLog, garbage);

    //cout << "I read this hour : " << unHit.hour << " and this cible " << unHit.cible << "and this referer : " << unHit.referer << endl; READING OK EXCEPT OTHER LINE?

    return fichierLog;
}

//-------------------------------------------- Constructeurs - destructeur
Hit::Hit()
{
#ifdef MAP
    cout << "Appel au constructeur de <Hit>" << endl;
#endif
} //----- Fin de Hit

Hit::~Hit()
{
#ifdef MAP
    cout << "Appel au destructeur de <Hit>" << endl;
#endif
} //----- Fin de ~Hit

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
