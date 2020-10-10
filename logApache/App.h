/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <App> (fichier App.h) ----------------
#if !defined(APP_H)
#define APP_H

//--------------------------------------------------- Interfaces utilisées
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

//------------------------------------------------------ Include personnel
#include "Cible.h"

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
typedef std::map<string, Cible *> Cibles;
typedef std::pair<string, Cible *> CiblesContainerPair;

//------------------------------------------------------------------------
// Rôle de la classe <App>
// La classe App sert de chef d'orchestre de tout le programme.
//------------------------------------------------------------------------

class App
{
    //----------------------------------------------------------------- PUBLIC

public:
    //----------------------------------------------------- Méthodes publiques
    // Mode d'emploi :
    // Cette méthode est le point d'entrée dans le programme
    // Elle renvoie le code de retour du programme, qui est :
    //   * EXIT_SUCCESS s'il n'y a pas eu d'erreur
    //   * EXIT_FAILURE s'il y a eu une erreur fatale
    // Contrat :
    // La valeur de retour est soit EXIT_SUCCESS soit EXIT_FAILURE.
    int Run(int argc, char const *argv[]);

    App();

    ~App();

protected:
    //----------------------------------------------------- Méthodes protégées
    // Mode d'emploi :
    // affiche le message d'usage du programme sur la sortie standard
    static void usage();

    // Mode d'emploi :
    // Lit les hits du fichier {options.inputFilename}
    // dans la map Cibles.
    void readFromFile(ifstream &logfile);

    // Mode d'emploi :
    // Renvoie TRUE si la chaine se termine par une extension filtrées
    // Renvoie FALSE sinon
    bool endsWithFilteredExtension(string filename) const;

    // Mode d'emploi :
    // Cette méthode analyse les paramètres donnés en ligne de commande
    // Elle renvoie le code de retour du programme, qui est :
    //   * EXIT_SUCCESS s'il n'y a pas eu d'erreur
    //   * EXIT_FAILURE s'il y a eu une erreur
    // Contrat :
    // La valeur de retour est soit 0 soit EXIT_FAILURE.
    int readOptions(int argc, char const *argv[]);

    // Mode d'emploi :
    // Affiche les N={options.topStatsCount} meilleures cibles.
    void showStatistics() const;

    // Mode d'emploi :
    // Écrit dans le fichier {options.outputDotFilename} le graphe
    // de toutes les cibles et leurs referers.
    int writeDotGraph() const;

    struct Options
    {
        string inputFilename = "";

        string outputDotFilename = "";
        bool shouldOutputDot = false;

        bool shouldExcludeOthers = false;

        unsigned int filterTime = 0;
        bool shouldFilterByTime = false;

        string serverReferer = "http://intranet-if.insa-lyon.fr";

        unsigned int topStatsCount = 10;

        std::vector<std::string> filteredExtensions = {".jpg", ".png", ".bmp", ".css", ".js"};
    };

    //----------------------------------------------------- Attributs protégés
    Options options;
    Cibles cibles;
};

//-------------------------------- Autres définitions dépendantes de <App>

#endif // APP_H
