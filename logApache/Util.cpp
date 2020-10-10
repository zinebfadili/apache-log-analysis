/*************************************************************************
                           Util  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//--------- Réalisation de la classe <Util> (fichier Util.cpp) -----------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <string>

//------------------------------------------------------ Include personnel
#include "Util.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
bool Util::EndsWith(std::string subject, std::string end)
{
    return subject.find(end) == (subject.length() - end.length());
}

bool Util::StartsWith(std::string subject, std::string start)
{
    return subject.find(start) == 0;
}

int Util::Atoi(const char *str)
// Algorithme :
// (Méthode peu robuste pour convertir char* en entier.)
// SORTIE : l'entier N, positif ou nul si la conversion est réussie.
//          sinon -1 en cas d'erreur
// Pour chaque caractère C de la chaîne de caractère
//      Si C n'est pas un chiffre, on retourne -1 (fin de la boucle et de la fonction).
//
//      Donner à X la valeur entière entre 0 et 9 associée au caractère C
//        où '0' => 0, ..., '9' => 9 (abstrait, concrètement on utilise
//        les valeurs ASCII des caractères).
//      Donner à N la valeur N * 10 + X
// Fin Pour
// Renvoyer N (il n'y a pas eu de valeur invalide)
{
    // la méthode convertie une chaine de caractère en entier qu'elle retourne
    int num = 0;

    if (str[0] == '\0')
    {
        return -1; // chaîne vide en paramètre
    }

    for (unsigned int i = 0; str[i] != 0; i++)
    {
        char c = str[i];
        if (c >= '0' && c <= '9')
        {
            num = num * 10 + (c - '0');
        }
        else
        {
            return -1;
        }
    }

    return num;
}
//------------------------------------------------- Surcharge d'opérateurs

//-------------------------------------------- Constructeurs - destructeur
// Pas de constructeur car la classe est virtuelle pure

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
