/*************************************************************************
                           Cible  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <Cible> (fichier Cible.h) ----------------
#if !defined(CIBLE_H)
#define CIBLE_H

//--------------------------------------------------- Interfaces utilisées
#include <map>
#include <string>
using namespace std;

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

typedef map<string, unsigned int> CibleReferersMap;

//------------------------------------------------------------------------
// Rôle de la classe <Cible>
// Cette classe sert de conteneur pour mémoriser le nombre de hits de
// chaque referer en direction d'une cible particulière.
// Elle regroupe les informations d'un fichier accédé :
// - nom du fichier
// - referers : les origines des requêtes
// - nombre de d'accès à ce fichier
//------------------------------------------------------------------------

class Cible
{
    //----------------------------------------------------------------- PUBLIC
public:
    friend class App;

    //----------------------------------------------------- Méthodes publiques
    void Increment(string referer);
    // Mode d'emploi :
    //   `referer` est le nom du referer associé à un hit en particulier.
    // Contrat :
    // * Si le referer est déjà présent dans la CibleReferersMap, alors
    //   la valeur associée à la clé `referer` est incrémentée.
    // * Sinon, alors une nouvelle paire clé-valeur est ajoutée à la map
    //   et contient la valeur 1.

    unsigned int GetCount();
    // Mode d'emploi :
    //   Accesseur de la variable privée nbHitsTotal.

    //------------------------------------------------- Surcharge d'opérateurs
    Cible &operator=(const Cible &unCible) = delete;
    // On ne souhaite pas d'opérateur de copie
    // car la copie en profondeur est très coûteuse.

    //-------------------------------------------- Constructeurs - destructeur
    Cible(const Cible &unCible) = delete;
    // On ne souhaite pas de constructeur de copie
    // car la copie en profondeur est très coûteuse.

    Cible(string cible);
    // Contrat :

    virtual ~Cible();
    // Mode d'emploi :
    //
    // Contrat :
    //

    //------------------------------------------------------------------ PRIVE

protected:
    //----------------------------------------------------- Méthodes protégées

    //----------------------------------------------------- Attributs protégés

    string nomCible; // le nom de la cible

    unsigned int nbHitsTotal; // le nombre de hits total vers la cible

    // une map qui associe le nom du referer au nombre de hits vers la cible
    CibleReferersMap referers;
};

//-------------------------------- Autres définitions dépendantes de <Cible>

#endif // CIBLE_H
