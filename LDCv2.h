/**
 * LDCv2.h
 *
 * auteur : Samir Benlafya
 */

#include <stdbool.h>

/**
 * DLL_OK
 *
 * return code si une fonction s'est déroulée sans erreurs.
 */
#define DLL_OK 1

 /**
 * DLL_ERROR
 *
 * return code si il y a eu une erreur dans une fonction.
 */
 #define DLL_ERROR 0

 /**
 * DLL_ALL
 *
 * Définit un paramètre "toutes les données" pour la fonction d'affichage des données.
 */
 #define DLL_ALL 2

 /**
 * DLL_UNREAD
 *
 * Définit un paramètre "données non lues uniquement" pour la fonction d'affichage des données.
 */
 #define DLL_UNREAD 0

 /**
 * DLL_READ
 *
 * Définit un paramètre "données non lues uniquement" pour la fonction d'affichage des données.
 */
 #define DLL_READ 1

 /**
 * ASC
 *
 * Définit un ordre de tri ascendant pour la fonction d'affichage des données.
 */
 #define ASC 1

 /**
 * DESC
 *
 * Définit un ordre de tri descendant pour la fonction d'affichage des données.
 */
 #define DESC 0


// Déclaration de la structure DLL (type opaque)

typedef struct DLL_t DLL;

 /**
 * dll_init : cette fonction re(initialise) une liste doublement chainées.
 *            ATTENTION : Lors de l'initialisation d'une liste le pointeur passé en paramètre
 *            devra avoir été préalablement initialisé à NULL.
 *            Un 2e appel de la fonction sur une liste existante la réinitialise.
 *
 * @param *dLL : pointeur vers la liste à (ré)initialiser.
 *
 * @return : adresse à laquelle la liste a été créée.
 */
 DLL* dll_init(DLL *dLL);

 /**
 * free_dll : efface la liste passée en paramètre.
 *            A FAIRE ABSOLUMENT UNE FOIS QU'ON A PLUS BESOIN DE LA LISTE SOUS PEINE
 *            DE FUITES MEMOIRES.
 *
 * @param *dLL : pointeur vers la liste a effacé de la mémoire.
 *
 */
 void free_dll(DLL *dLL);

 /**
 * dll_add : ajoute les données dans la liste passée en paramètre.
 *
 * @param *dLL : pointeur vers la liste dans laquelle les données seront ajoutées.
 *
 * @param *pData : pointeur générique vers les données à ajouter dans la liste.
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int dll_add(DLL *dLL, void *pData);;

 /**
 * dll_display : affiche toute les données dans la liste selon le mode et l'ordre passés en paramètres.
 *
 * @param *dLL : pointeur vers la liste contenant les données à afficher.
 *
 * @param mode : mode d'affichage des données 3 possibilités (DLL_ALL = toutes les données,
 *                DLL_UNREAD = données non lues , DLL_READ = données déjà lues).
 *
 * @param order : ordre d'affichage des données 2 possibilités (ASC = de la première
 *                rentrée à la dernière, DESC = de la dernière rentrée à la première).
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les données (param1).
 *
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int dll_display(DLL *dLL, int mode, int order, void(*pdisplay)(void*));

 /**
 * display_specific_data : affiche des données spécifiques selon le paramètre pToCompare.
 *
 * @param *dLL : pointeur vers la liste contenant les données.
 *
 * @param *pToCompare : pointeur générique vers ce à quoi les données de la liste divent être comparées.
 *
 * @param *pcompare : pointeur vers une fonction permetant de comparer les données en (param1) vis à vis
 *                    de l'élément de comparaison pToCompare (param2).
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les données (param1).
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int display_specific_data(DLL *dLL,void *pToCompare, bool(*pcompare)(void*, void*), void(*pdisplay)(void*));

 /**
 * sequential_access : affiche toutes les données contenues dans la liste de facon séquentielle
 *                     dans un ordre ascendant.
 *                     Chaque fois que l'utilisateur appuie sur la touche "enter" en laissant l'input vide
 *                     les données suivantes sont affichées.
 *                     Pour arreter l'affichage il faut entrer n'importe quel caractère.
 *
 * @param *dLL : pointeur vers la liste contenant les clients.
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les données (param1).
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int sequential_access(DLL *dLL, void(*pdisplay)(void*));
