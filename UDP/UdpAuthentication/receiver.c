#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef struct{
    char username[20];
    char password[20];
}user;

int main(int argc,char*argv[]){
    int flagEOF;
    int sockfd;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[50], msg[40];
    user utente;

    if(argc < 2){
        printf("Errore! Inserisci il numero di porta\n");
        return -1;
    }

    sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("Errore nell'apertura del socket\n");
        return -1;
    }

    memset(&local_addr,0,len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&local_addr,len) < 0){
        printf("Errore! Inserisci un numero di porta differente\n");
        return -1;
    }

    while(1){
        flagEOF=0;
        memset(buffer,0,sizeof(buffer));
        recvfrom(sockfd,buffer,49,0,
            (struct sockaddr*)&remote_addr,&len);
        strcpy(utente.password,buffer);

        char*tok=strtok(buffer,"-");
        strcpy(utente.username,tok);
        tok=strtok(NULL,"-");
        strcpy(utente.password,tok);

        printf("IP = %s, port = %d, MSG = %s %s\n",
            inet_ntoa(remote_addr.sin_addr),
            ntohs(remote_addr.sin_port),
            utente.username,
            utente.password);

        if(!strcmp(utente.username,"1"))
            break;
        
        FILE *fusers = fopen("Users.txt","r");
        FILE *flog = fopen("Log.txt","a");

        fseek(fusers,0,0);
        char b[50];

        char token[50];
        do{
            if(feof(fusers)){
                strcpy(msg,"Utente non registrato");
                fprintf(flog,"%s Non registrato\n",utente.username);
                sendto(sockfd,msg,sizeof(msg)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
                fclose(flog);
                fclose(fusers);
                flagEOF=1;
                break;
            }
            fgets(b,50,fusers);
            strcpy(token,strtok(b," \n"));
        }while (strcmp(token,utente.username));
        
        if(flagEOF)
        continue;
        
        strcpy(token,strtok(NULL," \n"));
        if(!strcmp(token,utente.password)){
            strcpy(msg,"Accesso consentito");
            fprintf(flog,"%s ha effettuato il login\n",utente.username);
        }
        else{
            strcpy(msg,"Accesso negato");
                fprintf(flog,"%s accesso negato\n",utente.username);
            }
        

        fclose(flog);
        fclose(fusers);

        sendto(sockfd,msg,sizeof(msg)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
    }

    close(sockfd);
    
    return 0;
}