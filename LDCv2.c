#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LDCv2.h"

/* Un node est une cellule de la liste doublement cha�n�es , chaque cellule contenant un pointeur vers la cellule pr�c�dente,
   un pointeur vers des donn�es, un tag pour savoir si les donn�es ont �t� lues ou non et un pointeur vers la cellule suivante. */

typedef struct Node_t
{
    struct Node_t *pPreviousNode;
    void *pData;
    bool isRead;
    struct Node_t *pNextNode;
}Node;

/* La structure DLL contient les informations pour pouvoir travailler sur la liste , cad le nombre de cellules (length),
   un pointeur vers le d�but de la liste (pTail) et un pointeur vers la fin de la liste (pHead). */

struct DLL_t
{
    size_t length;
    Node *pTail;
    Node *pHead;
};

void free_dll(DLL *dLL)
{
    int i;

    for(i = 0 ; i < dLL->length ; i++)
    {
        free(dLL->pTail->pPreviousNode);
        free(dLL->pTail->pData);
        dLL->pTail = dLL->pTail->pNextNode;
    }
    free(dLL);
}

DLL* dll_init(DLL *dLL)
{
    /* Si dLL est diff�rent de NULL ca veut dire que la fonction a �t� appell�e pour une
       r�initialisation donc j'efface la liste pass�e en param�tre. */

    if (dLL != NULL)
    {
        free_dll(dLL);
    }

    // Allocation de m�moire pour la liste.

    dLL = malloc(sizeof(DLL));
    if(dLL == NULL)
    {
        printf("Error malloc in dll_init()");
    }

    // Initialisation des �l�ments de la structure repr�sentant la liste (DLL).

    dLL->length = 0;
    dLL->pTail = NULL;
    dLL->pHead = NULL;

    /* Renvoi l'adresse � laquelle la liste a �t� cr��e. Si je ne fais pas ca,
       la valeur du pointeur pass� en param�tre restera la m�me qu'avant l'appel
       de la fonction. La seule raison pour laquelle je demande un pointeur vers
       la liste en param�tre est pour d�t�rminer si c'est une initilisation ou
       une r�initialisation. */

    return dLL;

}

int dll_add(DLL *dLL, void *pData)
{
    /* Sors de la fonction et affiche un message d'erreur si la liste pass�e en param�tre
     n'a pas �t� pr�alablement initialis�e. */

    if(dLL == NULL)
    {
        printf("dLL = NULL in dll_add\n");
        return DLL_ERROR;
    }

    /* Si la longueur de la liste est �gale � 0 cela veut dire qu'il n'y a pas encore de cellule
       dans la liste donc celle ci sera la premi�re ce qui est un cas particulier.
       En effet cette cellule sera � la fois le d�but (pTail) et la fin (pHead) de la liste. */

    if(dLL->length == 0)
    {
        // Allocation de m�moire pour la cellule.

        dLL->pTail = malloc(sizeof(Node));
        if(dLL->pTail == NULL)
        {
            printf("Error malloc in dll_add()");

            return DLL_ERROR;
        }
        /* Cas particulier ou le pointeur sur la fin de la liste pHead pointe sur la meme cellule
           que le pointeur sur la d�but de la liste pTail. */

        dLL->pHead = dLL->pTail;

        // Initialisation des �l�ments de cette premi�re cellule.

        dLL->pTail->pNextNode = NULL;
        dLL->pTail->pPreviousNode = NULL;
        dLL->pTail->pData = pData;
        dLL->pTail->isRead = false;

        // La taille de la liste est incr�ment�e.

        dLL->length++;

        return DLL_OK;
    }

    // Allocation de m�moire pour la  nouvelle cellule.

    dLL->pHead->pNextNode = malloc(sizeof(Node));
    if(dLL->pHead->pNextNode == NULL)
    {
        printf("Error malloc in dll_add()");

        return DLL_ERROR;
    }

    // Initialisation des �l�ments de la nouvelle cellule.

    dLL->pHead->pNextNode->pPreviousNode = dLL->pHead;
    dLL->pHead->pNextNode->pNextNode = NULL;
    dLL->pHead->pNextNode->pData = pData;
    dLL->pHead->pNextNode->isRead = false;

    // Le pointeur vers la fin de la liste pHead est chang� pour pointer sur la nouvelle cellule ajout�e.

    dLL->pHead = dLL->pHead->pNextNode;

    // La taille de la liste est incr�ment�e.

    dLL->length++;

    return DLL_OK;
}

int dll_display(DLL *dLL, int mode, int order, void(*pdisplay)(void*))
{
    int i;
    Node *currentNode;

    /* Sors de la fonction et affiche un message d'erreur si la liste pass�e en param�tre
     n'a pas �t� pr�alablement initialis�e. */

    if(dLL == NULL)
    {
        printf("dLL = NULL in dll_display\n");
        return DLL_ERROR;
    }

    /* Sors de la fonction et affiche un message d'erreur si la liste pass�e en param�tre
     ne contient pas au moins une celulle. */

    if(dLL->length == 0)
    {
        printf("No data in the DLL yet.\n");
        return DLL_ERROR;
    }

    /* Cette condition d�finit si les donn�es seront affich�es dans un ordre croissant (ASC)
       ou d�croissant (DESC) */

    if(order == ASC)
    {
        currentNode = dLL->pTail;
    }
    else if(order == DESC)
    {
        currentNode = dLL->pHead;
    }

    // La fonction next_node() d�termine la prochiane cellule affich�e selon l'ordre pass� en param�tre.

    void next_node(int order)
    {
        if(order == ASC)
        {
            currentNode = currentNode->pNextNode;
        }
        else if(order == DESC)
        {
            currentNode = currentNode->pPreviousNode;
        }
    }

    // Boucle d'affichage

    for(i = 0 ; i < dLL->length ; i++)
    {
        /* Cette condition saute les messages d�j� lus si la fonction a �t� appell�e
           avec le mode "non lus uniquement". */

        if(mode == DLL_UNREAD && currentNode->isRead == true)
        {
            next_node(order);
            continue;
        }

        /* Cette condition saute les messages non lus si la fonction a �t� appell�e
           avec le mode "d�j� lus uniquement". */

        if(mode == DLL_READ && currentNode->isRead == false)
        {
            next_node(order);
            continue;
        }

        // Affiche le client et change le statut du message en "lu".

        pdisplay(currentNode->pData);
        currentNode->isRead = true;

        // On passe � la prochaine cellule.

        next_node(order);
    }

    return DLL_OK;
}

int display_specific_data(DLL *dLL, void *pToCompare, bool(*pcompare)(void*, void*), void(*pdisplay)(void*))
{
    /* Sors de la fonction et affiche un message d'erreur si la liste pass�e en param�tre
     n'a pas �t� pr�alablement initialis�e. */

    if(dLL == NULL)
    {
        printf("DLL doesn't exist yet\n");
    }

    /* Sors de la fonction et affiche un message d'erreur si la liste pass�e en param�tre
     ne contient pas au moins une celulle. */

    if(dLL->length == 0)
    {
        printf("No data in the DLL yet.\n");
        return DLL_OK;
    }

    /* Cette boucle parcourt la liste � la recherche de donn�es qui correspondent aux
       donn�es point�es par pToCompare. Si une correspondance est trouv�e la fonction
       point�e par pcompare doit renvoyer true, si c'est les cas les donn�es sont affich�es.
       La boucle s'arr�te quand elle est arriv�es � la derni�re cellule de la liste. */

    bool isFound = false; // Variable utilis�e pour savoir si la recherche a retourn� un r�sulat.

    Node *currentNode = dLL->pTail; // La boucle commence par le d�but de la liste (pTail).

    while(currentNode != NULL)
    {
        if(pcompare(currentNode->pData, pToCompare) == true)
        {
            pdisplay(currentNode->pData);
            currentNode->isRead = true; // Tag les donn�es comme "lues" vu qu'elles ont �t� affich�es.
            isFound = true;
        }

        currentNode = currentNode->pNextNode;
    }

    if(isFound == false)
    {
        printf("Can't find a match.\n");
    }

    return DLL_OK;
}

/* Cette fonction parcourt la liste de donn�es en partant du d�but (pTail) et en
   affichant les donn�es d'une celulle � la fois. Chaque fois que l'utilisateur appuie
   sur enter (avec un input vide) la cellule suivante est affich�e. */

int sequential_access(DLL *dLL, void(*pdisplay)(void*))
{
    Node *currentNode = dLL->pTail;

    do
    {
        pdisplay(currentNode->pData);
        currentNode->isRead = true;
        currentNode = currentNode->pNextNode;
    }while(currentNode != NULL && getchar() == '\n');

    return DLL_OK;
}
