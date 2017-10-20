#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LDCv2.h"

/* Un node est une cellule de la liste doublement chaînées , chaque cellule contenant un pointeur vers la cellule précédente,
   un pointeur vers des données, un tag pour savoir si les données ont été lues ou non et un pointeur vers la cellule suivante. */

typedef struct Node_t
{
    struct Node_t *pPreviousNode;
    void *pData;
    bool isRead;
    struct Node_t *pNextNode;
}Node;

/* La structure DLL contient les informations pour pouvoir travailler sur la liste , cad le nombre de cellules (length),
   un pointeur vers le début de la liste (pTail) et un pointeur vers la fin de la liste (pHead). */

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
    /* Si dLL est différent de NULL ca veut dire que la fonction a été appellée pour une
       réinitialisation donc j'efface la liste passée en paramètre. */

    if (dLL != NULL)
    {
        free_dll(dLL);
    }

    // Allocation de mémoire pour la liste.

    dLL = malloc(sizeof(DLL));
    if(dLL == NULL)
    {
        printf("Error malloc in dll_init()");
    }

    // Initialisation des éléments de la structure représentant la liste (DLL).

    dLL->length = 0;
    dLL->pTail = NULL;
    dLL->pHead = NULL;

    /* Renvoi l'adresse à laquelle la liste a été créée. Si je ne fais pas ca,
       la valeur du pointeur passé en paramètre restera la même qu'avant l'appel
       de la fonction. La seule raison pour laquelle je demande un pointeur vers
       la liste en paramètre est pour détérminer si c'est une initilisation ou
       une réinitialisation. */

    return dLL;

}

int dll_add(DLL *dLL, void *pData)
{
    /* Sors de la fonction et affiche un message d'erreur si la liste passée en paramètre
     n'a pas été préalablement initialisée. */

    if(dLL == NULL)
    {
        printf("dLL = NULL in dll_add\n");
        return DLL_ERROR;
    }

    /* Si la longueur de la liste est égale à 0 cela veut dire qu'il n'y a pas encore de cellule
       dans la liste donc celle ci sera la première ce qui est un cas particulier.
       En effet cette cellule sera à la fois le début (pTail) et la fin (pHead) de la liste. */

    if(dLL->length == 0)
    {
        // Allocation de mémoire pour la cellule.

        dLL->pTail = malloc(sizeof(Node));
        if(dLL->pTail == NULL)
        {
            printf("Error malloc in dll_add()");

            return DLL_ERROR;
        }
        /* Cas particulier ou le pointeur sur la fin de la liste pHead pointe sur la meme cellule
           que le pointeur sur la début de la liste pTail. */

        dLL->pHead = dLL->pTail;

        // Initialisation des éléments de cette première cellule.

        dLL->pTail->pNextNode = NULL;
        dLL->pTail->pPreviousNode = NULL;
        dLL->pTail->pData = pData;
        dLL->pTail->isRead = false;

        // La taille de la liste est incrémentée.

        dLL->length++;

        return DLL_OK;
    }

    // Allocation de mémoire pour la  nouvelle cellule.

    dLL->pHead->pNextNode = malloc(sizeof(Node));
    if(dLL->pHead->pNextNode == NULL)
    {
        printf("Error malloc in dll_add()");

        return DLL_ERROR;
    }

    // Initialisation des éléments de la nouvelle cellule.

    dLL->pHead->pNextNode->pPreviousNode = dLL->pHead;
    dLL->pHead->pNextNode->pNextNode = NULL;
    dLL->pHead->pNextNode->pData = pData;
    dLL->pHead->pNextNode->isRead = false;

    // Le pointeur vers la fin de la liste pHead est changé pour pointer sur la nouvelle cellule ajoutée.

    dLL->pHead = dLL->pHead->pNextNode;

    // La taille de la liste est incrémentée.

    dLL->length++;

    return DLL_OK;
}

int dll_display(DLL *dLL, int mode, int order, void(*pdisplay)(void*))
{
    int i;
    Node *currentNode;

    /* Sors de la fonction et affiche un message d'erreur si la liste passée en paramètre
     n'a pas été préalablement initialisée. */

    if(dLL == NULL)
    {
        printf("dLL = NULL in dll_display\n");
        return DLL_ERROR;
    }

    /* Sors de la fonction et affiche un message d'erreur si la liste passée en paramètre
     ne contient pas au moins une celulle. */

    if(dLL->length == 0)
    {
        printf("No data in the DLL yet.\n");
        return DLL_ERROR;
    }

    /* Cette condition définit si les données seront affichées dans un ordre croissant (ASC)
       ou décroissant (DESC) */

    if(order == ASC)
    {
        currentNode = dLL->pTail;
    }
    else if(order == DESC)
    {
        currentNode = dLL->pHead;
    }

    // La fonction next_node() détermine la prochiane cellule affichée selon l'ordre passé en paramètre.

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
        /* Cette condition saute les messages déjà lus si la fonction a été appellée
           avec le mode "non lus uniquement". */

        if(mode == DLL_UNREAD && currentNode->isRead == true)
        {
            next_node(order);
            continue;
        }

        /* Cette condition saute les messages non lus si la fonction a été appellée
           avec le mode "déjà lus uniquement". */

        if(mode == DLL_READ && currentNode->isRead == false)
        {
            next_node(order);
            continue;
        }

        // Affiche le client et change le statut du message en "lu".

        pdisplay(currentNode->pData);
        currentNode->isRead = true;

        // On passe à la prochaine cellule.

        next_node(order);
    }

    return DLL_OK;
}

int display_specific_data(DLL *dLL, void *pToCompare, bool(*pcompare)(void*, void*), void(*pdisplay)(void*))
{
    /* Sors de la fonction et affiche un message d'erreur si la liste passée en paramètre
     n'a pas été préalablement initialisée. */

    if(dLL == NULL)
    {
        printf("DLL doesn't exist yet\n");
    }

    /* Sors de la fonction et affiche un message d'erreur si la liste passée en paramètre
     ne contient pas au moins une celulle. */

    if(dLL->length == 0)
    {
        printf("No data in the DLL yet.\n");
        return DLL_OK;
    }

    /* Cette boucle parcourt la liste à la recherche de données qui correspondent aux
       données pointées par pToCompare. Si une correspondance est trouvée la fonction
       pointée par pcompare doit renvoyer true, si c'est les cas les données sont affichées.
       La boucle s'arrête quand elle est arrivées à la dernière cellule de la liste. */

    bool isFound = false; // Variable utilisée pour savoir si la recherche a retourné un résulat.

    Node *currentNode = dLL->pTail; // La boucle commence par le début de la liste (pTail).

    while(currentNode != NULL)
    {
        if(pcompare(currentNode->pData, pToCompare) == true)
        {
            pdisplay(currentNode->pData);
            currentNode->isRead = true; // Tag les données comme "lues" vu qu'elles ont été affichées.
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

/* Cette fonction parcourt la liste de données en partant du début (pTail) et en
   affichant les données d'une celulle à la fois. Chaque fois que l'utilisateur appuie
   sur enter (avec un input vide) la cellule suivante est affichée. */

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
