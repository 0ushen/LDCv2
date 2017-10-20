#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LDCv2.h"
#include "client.h"

//Fonction qui permet la lecture d'un message.

void read_msg(char **ppMsg)
{
    char sUserInput[255];

    fflush(stdin);
    fgets(sUserInput, 255, stdin);

    *ppMsg = malloc((strlen(sUserInput) + 1) * sizeof(char));

    strcpy(*ppMsg, sUserInput);

}

/*  Vérifie si l'action est un NOMBRE compris entre 1 et le nombre de possibilités
    permises (dNPos) */

void action(int *dAction, int dNPos)
{
    char sAction[20];

    do
    {
        fflush(stdin);
        fgets(sAction, 20, stdin);

        if (strlen(sAction) > 2)
        {
            printf("Too many characters , try again!\n");
            continue;
        }

        *dAction = atoi(sAction);

        if(*dAction < 1 || *dAction > dNPos)
        {
            printf("Wrong Number!\nTry again\n");
        }

    } while(*dAction < 1 || *dAction > dNPos);
}

// Affiche le client stocké à l'adresse pointée par le pointeur générique pData.
void display(void *pData)
{
    Client *client = pData;

    printf("ID client : %d\nLast name : %sFirst name : %s\n",client->id, client->lastName, client->firstName);

}

/* Compare l'id du client stocké à l'adresse pointée par le pointeur générique pData avec
    l'idclient rentré par l'utilisateur. L'idclient recherché par l'utilisateur a été passé au module LDCv2
    sous forme de pointeur générique et revient donc ici en tant que pointeur générique. */

bool compare_id (void *pData, void *pToCompare)
{
    Client *client = pData;
    int *clientID = pToCompare;

    if(client->id == *clientID)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int main()
{
    int dAction, clientID;
    int i = 0;
    bool stop = false;

    DLL *list = NULL;
    Client *client = NULL;

    //Boucle infinie pour pouvoir tester mon module LDCv2.

    while(!stop)
    {
        if(i == 0)
        {
            printf("What to do?\n\
               1.Initialize a DLL.\n");
            action(&dAction, 1);
        }
        else
        {
            printf("What to do?\n\
               1.Initialize a DLL.\n\
               2.Reinitialize a DLL.\n\
               3.Add a client to DLL.\n\
               4.Display all clients.\n\
               5.Display a specific client.\n\
               6.Browse through clients.\n");
            action(&dAction, 6);
        }

        switch(dAction)
        {
            case 1 :
                list = dll_init(list);
                if(list == NULL)
                {
                    printf("Error : can't initialize DLL.\n");
                }
                else
                {
                    printf("DLL initialized!\n");
                }
                break;
            case 2 :
                list = dll_init(list);
                if(list == NULL)
                {
                    printf("Error : can't reinitialize DLL.\n");
                }
                else
                {
                    printf("DLL reinitialized!\n");
                }
                break;
            case 3 :
                client = malloc(sizeof(Client));
                printf("Client id : ");
                scanf("%d", &client->id);
                printf("First Name : ");
                read_msg(&client->firstName);
                printf("Second Name : ");
                read_msg(&client->lastName);
                if(dll_add(list, client) == DLL_OK)
                {
                    client->isRead = false;
                    printf("Client was added to DLL\n");
                }
                else
                {
                    printf("Error : can't add client to DLL\n");
                }
                break;
            case 4 :
                printf("1.Display all clients\n\
                        2.Unread clients only\n\
                        3.Already read clients only.\n");
                action(&dAction, 3);
                switch(dAction)
                {
                    case 1 :
                        if(dll_display(list, DLL_ALL, ASC, display) == DLL_ERROR)
                        {
                            printf("Error occured when trying do display all clients\n");
                        }
                        break;
                    case 2 :
                        if(dll_display(list, DLL_UNREAD, ASC, display) == DLL_ERROR)
                        {
                            printf("Error occured when trying do display unread clients\n");
                        }
                        break;
                    case 3 :
                        if(dll_display(list, DLL_READ, ASC, display) == DLL_ERROR)
                        {
                            printf("Error occured when trying do display already read clients\n");
                        }
                        break;
                    default :
                        printf("Wrong number!\nTry again\n");
                }
                break;
            case 5 :
                printf("Client id number : ");
                scanf("%d", &clientID);
                if(display_specific_data(list, &clientID, compare_id, display) == DLL_ERROR)
                {
                    printf("Error occured when trying to display client with id number %d", clientID);
                }
                break;
            case 6 :
                sequential_access(list, display);
                break;
            default :
                printf("Wrong number!\n");

        }
        i++;
    }

    free_dll(list);

}
