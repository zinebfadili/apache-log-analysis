/*************************************************************************
                           Util  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <Util> (fichier Util.h) ----------------
#if !defined(UTIL_H)
#define UTIL_H

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <string>

//----------------------------------------------------------------- PUBLIC
class Util
{
    //----------------------------------------------------------------- PUBLIC
public:
    //----------------------------------------------------- Méthodes publiques

    // Mode d'emploi :
    // Retourne VRAI si la chaîne `subject` se termine par la chaîne end.
    // Renvoie FAUX sinon.
    // Une fonction très simple donc elle pourrait être inline.
    static bool EndsWith(std::string subject, std::string end);

    // Mode d'emploi :
    // Retourne VRAI si la chaîne `subject` commence par la chaîne start.
    // Renvoie FAUX sinon.
    // Une fonction très simple donc elle pourrait être inline.
    static bool StartsWith(std::string subject, std::string start);

    // Mode d'emploi :
    // Analyse une chaine de caractères et retourne la valeur numérique
    // du nombre entier positif qui s'y trouve potentiellemnt.
    // Retourne -1 si la chaine n'est pas dans le bon format.
    static int Atoi(const char *str);

    // sans constructeur
    Util() = delete;

    // ni destructeur
    ~Util() = delete;
}; // fin de class Util

#endif // UTIL_H
