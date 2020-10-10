/*************************************************************************
                           Cible  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Cible> (fichier Cible.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Cible.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
void Cible::Increment(string referer)
// Algorithme :
// Incrémenter le nombre de hits total de la cible.
//
// Trouver le referer qui porte le nom donné en paramètre.
// Si ce referer existe, alors on incrémente le nombre de hits entre
//   le referer et la cible.
// Sinon, on ajoute un nouveau referer dans la liste des referers
//   de la cible (en n'oubliant pas de compter 1 hit entre la cible
//   et le referer).
{
    // on incrémente le nombre de Hits total de la cible
    nbHitsTotal++;

    // on récupère le referer (s'il est déjà repertorié)
    CibleReferersMap::iterator it = referers.find(referer);

    if (it != referers.end())
    {
        // si le referer existe, on incrémente le nombre
        // de hits de la liaison referer -> cible
        it->second++;
    }
    else
    {
        // sinon on crée le referer
        referers.insert(pair<string, unsigned int>(referer, 1));
    }
} //----- Fin de Increment

unsigned int Cible::GetCount()
// Accesseur public de nbHitsTotal (privée)
{
    return nbHitsTotal;
}

//------------------------------------------------- Surcharge d'opérateurs

//-------------------------------------------- Constructeurs - destructeur
Cible::Cible(string cible) : nomCible(cible), nbHitsTotal(0), referers()
// Algorithme :
// Affecter à nomCible le nom de cette cible, donné en paramètre
//   au constructeur.
// Affecter au nombre total de hits la valeur 0.
// Initialiser la structure de donnée, initialement vide, qui regroupe
//   tous les referers et leurs nombres de hits spécifiques.
{
#ifdef MAP
    cout << "Appel au constructeur de <Cible>" << endl;
#endif
} //----- Fin de Cible

Cible::~Cible()
{
#ifdef MAP
    cout << "Appel au destructeur de <Cible>" << endl;
#endif
} //----- Fin de ~Cible

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
