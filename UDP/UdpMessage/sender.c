#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char*argv[]){
    
    int n;
    int sockfd;
    struct sockaddr_in remote_addr;
    char sendline[1000];
    char recline[1000];
    socklen_t len = sizeof(struct sockaddr_in);

    if(argc!=3){
        printf("Errore! Inserisci IP e porta\n");
        return -1;
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        printf("Errore nell'apertura del socket\n");
        return -1;
    }

    memset(&remote_addr,0,len);
    remote_addr.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&remote_addr.sin_addr);
    remote_addr.sin_port=htons(atoi(argv[2]));

    while (fgets(sendline,1000,stdin)!=NULL){
        sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&remote_addr,len);

        n = recvfrom(sockfd,recline,999,0,(struct sockaddr*)&remote_addr,&len);

        recline[n] = 0;

        printf("IP = %s, p = %d, msg = %s\n",inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port),recline);

        if(strcmp(sendline,"fine\n")==0){
            printf("arrivederci\n");
            break;
        }

    }
    
    close(sockfd);

    return 0;
}