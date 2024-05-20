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
void deleteList(struct prodotto**);
void stampaProdotti(struct prodotto**);
void buy(int,int,struct prodotto**);
void writeFile(struct prodotto**);

int main(int argc,char**argv){
    int sockfd, newsock, n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    struct prodotto**head = malloc(sizeof(struct prodotto*));
    char chose = 'c';
    char buff[50];

    if(argc!=2){
        printf("Inserire il numero di porta\n");
        exit(-1);
    }

    if( (sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        printf("Errore nella creazione della socket\n");
        exit(-1);
    }

    memset(&local_addr,0,len);
    local_addr.sin_family=AF_INET;
    local_addr.sin_port=htons(atoi(argv[1]));
    local_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sockfd,(struct sockaddr*)&local_addr,len) < 0){
        printf("Errore nella bind\n");
        exit(-1);
    }

    listen(sockfd,5);

    while(1){
        newsock=accept(sockfd,(struct sockaddr*)&remote_addr,&len);
        if(fork()){
            close(sockfd);
            FILE*f = fopen("prodotti.bin","r");
            struct prodotto*tmp;
            while(!feof(f)){
                tmp = malloc(sizeof(struct prodotto));
                tmp->next = NULL;
                fscanf(f,"%d %s %f %d",&(tmp->Id),tmp->nome,&(tmp->prezzo),&(tmp->quantità));
                insert(head,tmp);
                send(newsock,tmp,sizeof(tmp),0);
            }
            tmp = malloc(sizeof(struct prodotto));
            tmp->Id = 0;
            send(newsock,tmp,sizeof(tmp),0);
            fclose(f);
            stampaProdotti(head);
            do{
                n = recv(newsock,buff,49,0);
                buff[n] = 0;
                char*Id = strtok(buff,", ");
                char*Qta = strtok(NULL,", ");
                buy(atoi(Id),atoi(Qta),head);
                recv(newsock,&chose,sizeof(char),0);
            }while(chose == 'c');
            stampaProdotti(head);
            writeFile(head);
            deleteList(head);
            close(newsock);
        }
        else{
            close(newsock);
        }
    }
    
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

void buy(int Id_p,int qta,struct prodotto**head){
    struct prodotto* tmp = *head;
    while(Id_p!=tmp->Id && tmp != NULL){
        tmp = tmp->next;
    }
    if(!tmp){
        perror("Prodotto non disponibile\n");
    }
    else{
        tmp->quantità -= qta;
    }
}

void writeFile(struct prodotto**head){
    FILE*f = fopen("prodotti.bin","w");
    struct prodotto*prod = *head;
    while (prod){
        fprintf(f,"%d %s %f %d \n",prod->Id,prod->nome,prod->prezzo,prod->quantità);
    }
    fclose(f);
}