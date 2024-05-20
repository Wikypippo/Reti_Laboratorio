#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct prodotto{
    int Id;
    char nome[20];
    float prezzo;
    int quantità;
    struct prodotto *next;
};

void insert(struct prodotto**,struct prodotto*);
void stampaProdotti(struct prodotto**);

int main(int argc,char**argv){
    int sockfd;
    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(remote_addr);
    char sendLine[50];
    struct prodotto**head = NULL;
    char chose;

    if(argc!=3){
        perror("Inserire IP e numero di porta\n");
        exit(-1);
    }

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("Errore nella creazione della Socket\n");
        exit(-1);
    }

    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(argv[2]));
    remote_addr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sockfd,(struct sockaddr*)&remote_addr,len);

    while(1){
        struct prodotto*tmp = malloc(sizeof(struct prodotto));
        if(!tmp->Id){
            free(tmp);
            break;
        }
        else{
            insert(head,tmp);
        }
    }
    stampaProdotti(head);
    do{
        printf("Inserire ID_prodotto, Quantità_prodotto\n");
        fgets(sendLine,50,stdin);
        send(sockfd,sendLine,50,0);
        printf("c : continua gli acquisti\nq : concludi gli acquisti");
        scanf("%c",&chose);
    }while(chose=='c');
    
    close(sockfd);
    
    return 0;
}

void insert(struct prodotto**head,struct prodotto*prd){
    if(head == NULL){
        prd->next = NULL;
        *head = prd;
    }
    else{
        struct prodotto*ptr = *head;
        while (ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = prd;
        ptr->next->next = NULL;
    }
}

void deleteList(struct prodotto**head){
    struct prodotto *ptr = *head;
    struct prodotto *tmp = NULL;
    while(ptr){
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    head = NULL;
}

void stampaProdotti(struct prodotto**head){
    struct prodotto *ptr = *head;
    while (ptr){
        printf("ID: %d, nome: %s, prezzo: %f, quantità: %d\n",ptr->Id,ptr->nome,ptr->prezzo,ptr->quantità);
    }
    
}