#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef struct{
    char username[20];
    char password[20];
}user;

int main(int argc,char*argv[]){

    int sockfd;
    struct sockaddr_in dest_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    user utente;
    char msg[41],buffer[50];
    strcpy(utente.username,argv[3]);
    strcpy(utente.password,argv[4]);

    if (argc < 5){
        printf("Errore! Scrivere IP di destinazione, porta di destinazione, Username e Password");
        return -1;
    }

    sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("Errore nell'apertura del Socket");
        return -1;
    }

    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1],&(dest_addr.sin_addr));
    dest_addr.sin_port=htons(atoi(argv[2]));

    memset(msg,0,sizeof(msg));
    strcat(msg,utente.username);
    strcat(msg,"-");
    strcat(msg,utente.password);

    sendto(sockfd,msg,sizeof(msg)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));

    if(strcmp(utente.username,"1")){
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&remote_addr,&len);
        printf("%s\n",buffer);
    }

    close(sockfd);

    return 0;
}