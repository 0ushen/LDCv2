/**
 * LDCv2.h
 *
 * auteur : Samir Benlafya
 */

#include <stdbool.h>

/**
 * DLL_OK
 *
 * return code si une fonction s'est d�roul�e sans erreurs.
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
 * D�finit un param�tre "toutes les donn�es" pour la fonction d'affichage des donn�es.
 */
 #define DLL_ALL 2

 /**
 * DLL_UNREAD
 *
 * D�finit un param�tre "donn�es non lues uniquement" pour la fonction d'affichage des donn�es.
 */
 #define DLL_UNREAD 0

 /**
 * DLL_READ
 *
 * D�finit un param�tre "donn�es non lues uniquement" pour la fonction d'affichage des donn�es.
 */
 #define DLL_READ 1

 /**
 * ASC
 *
 * D�finit un ordre de tri ascendant pour la fonction d'affichage des donn�es.
 */
 #define ASC 1

 /**
 * DESC
 *
 * D�finit un ordre de tri descendant pour la fonction d'affichage des donn�es.
 */
 #define DESC 0


// D�claration de la structure DLL (type opaque)

typedef struct DLL_t DLL;

 /**
 * dll_init : cette fonction re(initialise) une liste doublement chain�es.
 *            ATTENTION : Lors de l'initialisation d'une liste le pointeur pass� en param�tre
 *            devra avoir �t� pr�alablement initialis� � NULL.
 *            Un 2e appel de la fonction sur une liste existante la r�initialise.
 *
 * @param *dLL : pointeur vers la liste � (r�)initialiser.
 *
 * @return : adresse � laquelle la liste a �t� cr��e.
 */
 DLL* dll_init(DLL *dLL);

 /**
 * free_dll : efface la liste pass�e en param�tre.
 *            A FAIRE ABSOLUMENT UNE FOIS QU'ON A PLUS BESOIN DE LA LISTE SOUS PEINE
 *            DE FUITES MEMOIRES.
 *
 * @param *dLL : pointeur vers la liste a effac� de la m�moire.
 *
 */
 void free_dll(DLL *dLL);

 /**
 * dll_add : ajoute les donn�es dans la liste pass�e en param�tre.
 *
 * @param *dLL : pointeur vers la liste dans laquelle les donn�es seront ajout�es.
 *
 * @param *pData : pointeur g�n�rique vers les donn�es � ajouter dans la liste.
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int dll_add(DLL *dLL, void *pData);;

 /**
 * dll_display : affiche toute les donn�es dans la liste selon le mode et l'ordre pass�s en param�tres.
 *
 * @param *dLL : pointeur vers la liste contenant les donn�es � afficher.
 *
 * @param mode : mode d'affichage des donn�es 3 possibilit�s (DLL_ALL = toutes les donn�es,
 *                DLL_UNREAD = donn�es non lues , DLL_READ = donn�es d�j� lues).
 *
 * @param order : ordre d'affichage des donn�es 2 possibilit�s (ASC = de la premi�re
 *                rentr�e � la derni�re, DESC = de la derni�re rentr�e � la premi�re).
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les donn�es (param1).
 *
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int dll_display(DLL *dLL, int mode, int order, void(*pdisplay)(void*));

 /**
 * display_specific_data : affiche des donn�es sp�cifiques selon le param�tre pToCompare.
 *
 * @param *dLL : pointeur vers la liste contenant les donn�es.
 *
 * @param *pToCompare : pointeur g�n�rique vers ce � quoi les donn�es de la liste divent �tre compar�es.
 *
 * @param *pcompare : pointeur vers une fonction permetant de comparer les donn�es en (param1) vis � vis
 *                    de l'�l�ment de comparaison pToCompare (param2).
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les donn�es (param1).
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int display_specific_data(DLL *dLL,void *pToCompare, bool(*pcompare)(void*, void*), void(*pdisplay)(void*));

 /**
 * sequential_access : affiche toutes les donn�es contenues dans la liste de facon s�quentielle
 *                     dans un ordre ascendant.
 *                     Chaque fois que l'utilisateur appuie sur la touche "enter" en laissant l'input vide
 *                     les donn�es suivantes sont affich�es.
 *                     Pour arreter l'affichage il faut entrer n'importe quel caract�re.
 *
 * @param *dLL : pointeur vers la liste contenant les clients.
 *
 * @param *pdisplay : pointeur vers une fonction permettant d'afficher les donn�es (param1).
 *
 * @return : DLL_OK ou DLL_ERROR
 */
 int sequential_access(DLL *dLL, void(*pdisplay)(void*));
