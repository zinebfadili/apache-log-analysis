/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <App> (fichier App.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

//------------------------------------------------------ Include personnel
#include "App.h"
#include "Cible.h"
#include "Hit.h"
#include "Util.h"

//------------------------------------------------------------- Constantes
const string PROG_NAME = "analog"; // nom de l'executable

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
int App::Run(int argc, char const *argv[])
// Algorithme :
// Lire les arguments en ligne de commande
// Ouvrir le fichier d'entrée
//
//
{
    const int optionsReadingStatus = readOptions(argc, argv);
    if (optionsReadingStatus == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    ifstream logfile(options.inputFilename);
    if (logfile.fail()) // si le fichier ne s'ouvre pas correctement
    {
        cerr << "Erreur: impossible d'ouvrir le fichier d'entrée." << endl;
        return EXIT_FAILURE;
    }

    readFromFile(logfile); // appel à la fonction de lecture du fichier

    logfile.close();

    if (options.shouldFilterByTime)
    {
        cout << "Warning: only hits between " << options.filterTime << "h and " << (options.filterTime + 1) << "h have been taken into account" << endl;
    }

    if (options.shouldOutputDot) // génération du fichier .dot si demandé
    {
        if (writeDotGraph() == EXIT_FAILURE)
        {
            // En cas d'erreur, on interrompt l'exécution du programme
            return EXIT_FAILURE;
        }
    }

    showStatistics();

    // on détruit les cibles car c'est la fin du programme, on ne les utilise plus
    Cibles::iterator it = cibles.begin();
    const Cibles::iterator end = cibles.end();
    while (it != end)
    {
        delete it->second;
        it++;
    }

    return EXIT_SUCCESS;
} //----- Fin de App::Run

int App::writeDotGraph() const
// Algorithme :
// On ouvre le fichier de sortie.
// On y écrit l'en-tête “digraph {”
// Pour chaque Cible :
//      Si le nœud de même nom n'a pas déjà été créé, alors :
//          On crée un nœud. On écrit ce nœud dans le fichier de sortie
//          On enregistre le nom du nouveau nœud dans une variable $nom
//      Sinon :
//          On récupère le nom de l'ancien nœud dans une variable $nom
//      Fin Si
//
//      Pour chaque referer de la cible :
//          Si le nœud de même nom n'a pas déjà été créé, alors :
//              On crée un nœud. On écrit ce nœud dans le fichier de sortie
//              On enregistre le nom du nouveau nœud dans une variable $nom
//          Sinon :
//              On récupère le nom de l'ancien nœud dans une variable $nom
//          Fin Si
//          On écrit dans le fichier la liaison entre les deux nœud.
//      Fin Pour
// Fin Pour
// On écrit la fin “}” dans le fichier.
{
    ofstream dotfile(options.outputDotFilename);
    if (dotfile.fail()) // si le fichier ne s'ouvre pas correctement
    {
        cerr << "Erreur: impossible d'ouvrir le fichier de sortie." << endl;
        return EXIT_FAILURE;
    }

    dotfile << "digraph {" << endl;

    unsigned int nodeCount = 0;

    typedef map<string, string> NodeMap;
    NodeMap mapCibleToNodeName;

    Cibles::const_iterator it = cibles.begin();
    const Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        const string cibleName = it->first;
        const CibleReferersMap refs = it->second->referers;

        string nodeName1;

        // on vérifie si un noeud a déjà été constitué par la cible en cours
        const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(cibleName);
        if (cibleToNodeName == mapCibleToNodeName.end())
        {
            // clé non-trouvée, donc la cible est donnée comme nouveau noeud au graph
            nodeName1 = "node" + std::to_string(nodeCount);
            nodeCount++;
            mapCibleToNodeName.insert(pair<string, string>(cibleName, nodeName1));
            dotfile << nodeName1 << " [label=\"" << cibleName << "\"];" << endl;
        }
        else
        {
            // sinon on travaille avec le nom de noeud déjà existant pour cette cible
            nodeName1 = cibleToNodeName->second;
        }

        // on parcourt les referer de la cible etudiée
        CibleReferersMap::const_iterator it2 = refs.begin();
        const CibleReferersMap::const_iterator end2 = refs.end();
        while (it2 != end2)
        {
            const string refName = it2->first;
            const unsigned int count = it2->second;

            string nodeName2;

            // on vérifie si un noeud existe déjà ayant comme nom celui du referer en ours
            const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(refName);
            if (cibleToNodeName == mapCibleToNodeName.end())
            {
                // clé non-trouvée, donc le referer est donné comme nouveau noeud au graph

                nodeName2 = "node" + std::to_string(nodeCount);
                nodeCount++;
                mapCibleToNodeName.insert(pair<string, string>(refName, nodeName2));
                dotfile << nodeName2 << " [label=\"" << refName << "\"];" << endl;
            }
            else
            {
                //sinon on travaille avec le nom de noeud déjà donné pour ce referer
                nodeName2 = cibleToNodeName->second;
            }

            // on insère dans le fichier la nouvelle liaison
            dotfile << nodeName2 << " -> " << nodeName1 << " [label=\"" << count << "\"];" << endl;
            it2++;
        }

        it++;
    }

    dotfile << "}" << endl;

    cout << "Dot-file " << options.outputDotFilename << " generated" << endl;
    return EXIT_SUCCESS;
}

void App::showStatistics() const
// Algorithme :
// On crée une map de clé “nombre total de hits”, et de valeur : la Cible.
// On itère la map dans le sens inverse (donc décroissant selon la clé) :
//      On affiche la valeur.
//      Si on a itéré 10 fois, on quitte la boucle.
// Fin Itération
{
    // une multimap pour permettre les doublons sur les nombres de hits total qui est clé
    // on utilise la fonction compare std::greater pour classer par ordre décroissant,
    // on garde l'allocator, car on ne s'en sert pas
    // std::multimap<unsigned int, Cible*> ciblesMap;

    std::multimap<unsigned int, Cible *> ciblesMap;

    Cibles::const_iterator it = cibles.begin();
    Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        // on insère la pair <nbHitsTotal, Pointeur vers la cible> dans la multimap
        ciblesMap.insert(make_pair(it->second->nbHitsTotal, it->second));
        it++;
    }

    std::multimap<unsigned int, Cible *>::const_reverse_iterator itMap = ciblesMap.rbegin();
    std::multimap<unsigned int, Cible *>::const_reverse_iterator endMap = ciblesMap.rend();

    // affichage d'au plus 10 documents les plus consultés
    unsigned int i = 0;
    while (itMap != endMap && i < options.topStatsCount)
    {
        cout << itMap->second->nomCible << " "
             << "(" << itMap->first << " hits)" << endl;
        itMap++;
        i++;
    }
}

// méthode qui permet d'analyser le fichier
void App::readFromFile(ifstream &logfile)
// Algorithme :
// Tant que l'on a pas atteint la fin du fichier :
//      On lit un hit
//      Si le hit est invalide (c'est à dire qu'on est à la fin du fichier)
//          Alors on quitte la boucle.
//      Fin Si
//
//      Si l'option d'exclusion d'images/CSS/JS est activée
//          Si la cible ou le referer se termine par l'extension correspondante
//              Alors on passe à la prochaine ligne
//          Fin Si
//      Fin Si
//
//      Si l'option d'exclusion selon l'heure du hit est activée
//          Si l'heure du hit n'est pas comprise dans le bon intervalle
//              Alors on passe à la prochaine ligne
//          Fin Si
//      Fin Si
//
//      Si le referer du hit commence par le referer local
//          Alors on supprime le début du referer du hit
//          (c'est à dire on retire le referer local du referer du hit)
//      Fin Si
//
//      Si la cible n'existe pas dans la liste des cibles
//          Alors on l'ahjute à la liste des cibles
//      Fin Si
//
//      Dans tous les cas, on incrémente le nombre de hits total la cible
//      Et on incrémente le nombre de hits de la liaison referer -> cible
// Fin Tant que
//
{
    const string LOCAL_ADDRESS = options.serverReferer;
    Hit newHit;

    while (!logfile.eof())
    {
        logfile >> newHit; // lecture du hit grace à la surcharge d'opérateur >>

        if ((newHit.referer == "") && (newHit.cible == "")) // on vérifie la validité du hit
        {
            break; // le hit non valide signifie la fin du fichier
        }

        if (options.shouldExcludeOthers)
        {
            // On détermine si la cible est une ressource image/CSS/JS, …
            // En pratique, on a seulement besoin de vérifier la cible car le referer
            // sera toujours une page HTML.
            if (endsWithFilteredExtension(newHit.cible))
            {
                // Le fichier n'est pas du type souhaité
                continue; // on n'ajoute pas le hit
            }
        }

        if (options.shouldFilterByTime)
        {
            if (newHit.hour != options.filterTime)
            {
                continue; // on n'ajoute pas le hit s'il est d'une autre heure que celle désirée
            }
        }

        // si l'adresse locale INSA se trouve au début de referer on l'enleve
        if (Util::StartsWith(newHit.referer, LOCAL_ADDRESS))
        {
            // on retire l'adresse locale
            newHit.referer = newHit.referer.substr(LOCAL_ADDRESS.length());
        }

        const string key = newHit.cible;
        Cible *value;

        // On vérifie si la cible existe déjà
        Cibles::iterator oldCibleIt = cibles.find(key);
        if (oldCibleIt != cibles.end())
        {
            // si la cible existe déjà
            value = oldCibleIt->second; // on récupère le pointeur vers la cible déjà existante
        }
        else
        {
            // sinon on crée une nouvelle cible et on l'ajoute à la map
            value = new Cible(key);
            cibles.insert(CiblesContainerPair(key, value));
        }

        // appel à la méthode Increment qui on incrémente le nombre de hits total de la cibles
        //et soit ajoute le referer s'il ne fait pas déjà partie des referer de cette cible
        // ou incremente le nombre de liaison entre cette cible et ce referer
        value->Increment(newHit.referer);
    }
}

bool App::endsWithFilteredExtension(string filename) const
// Algorithme :
// Pour toutes les extensions filtrées
//      Si le nom en paramètre se termine par l'extension
//          Alors on retourne VRAI
//      Fin Si
// Fin Pour
// On retourne FAUX
{
    // on vérifie si la chaine de caractère paramètre effectif a pour terminaison l'une des extensions interdites
    const std::vector<std::string> exts = options.filteredExtensions;
    for (unsigned int i = 0; i < exts.size(); i++)
    {
        if (Util::EndsWith(filename, exts[i]))
        {
            return true;
        }
    }
    return false;
}

void App::usage()
// Affiche le message d'usage
{
    cerr << "Usage: " << PROG_NAME << " [-e] [-t 0-23] [-g out.dot] in.log" << endl;
}

int App::readOptions(int argc, char const *argv[])
{
    if (argc == 1)
    {
        App::usage();
        return EXIT_FAILURE;
    }

    int i = 1;
    while (i < argc)
    {
        //traitment de l'option "-g"
        if (!strcmp(argv[i], "-g"))
        {
            if (i == argc - 1)
            {
                App::usage();
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            // On lit ensuite le nom du fichier de sortie
            i++;
            options.shouldOutputDot = true;

            options.outputDotFilename = argv[i];
        }
        //traitement de l'option "-e"
        else if (!strcmp(argv[i], "-e"))
        {
            options.shouldExcludeOthers = true;
        }
        //traitement de l'option "-t"
        else if (!strcmp(argv[i], "-t"))
        {
            if (i == argc - 1)
            {
                App::usage();
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            i++;
            int hour = Util::Atoi(argv[i]);
            if (hour > 23 || hour < 0)
            {
                cerr << "Erreur: paramètre ‘heure’ de l'option -t"
                     << " doit être un nombre entre 0 et 23 (bornes incluses)." << endl;
                App::usage();
                return EXIT_FAILURE;
            }

            options.filterTime = (unsigned int)hour;
            options.shouldFilterByTime = true;
        }

        //possibilité d'évolution ci-dessous en commentaire : ajout de l'option pour que
        //l'utilisateur fournisse l'adresse locale
        /*  else if (!strcmp(argv[i], "-R"))
        {
            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            options.serverReferer = argv[i];
        }*/
        else
        {
            // Si le paramètre commence par un tiret - et qu'il n'a satisfait aucune des conditions précédentes
            // Alors c'est une option invalide
            if (!strncmp(argv[i], "-", 1))
            {
                cerr << "Erreur: option non reconnue ‘" << argv[i] << "’." << endl;
                App::usage();
                return EXIT_FAILURE;
            }
            else if (options.inputFilename == "")
            {
                // On n'a pas encore lu de nom de fichier d'entrée.
                options.inputFilename = argv[i];
            }
            else
            {
                cerr << "Erreur: trop d'arguments." << endl;
                App::usage();
                return EXIT_FAILURE;
            }
        }

        i++;
    }

    if (options.inputFilename == "") // si aucun fichier d'entrée n'est donné
    {
        cerr << "Erreur: argument manquant du nom de fichier d'entrée." << endl;
        App::usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//------------------------------------------------- Surcharges d'opérateurs

//-------------------------------------------- Constructeurs - destructeurs
App::App()
{
#ifdef MAP
    cout << "Appel au constructeur de <App>" << endl;
#endif
} //----- Fin de App

App::~App()
{
#ifdef MAP
    cout << "Appel au destructeur de <App>" << endl;
#endif
} //----- Fin de ~App
