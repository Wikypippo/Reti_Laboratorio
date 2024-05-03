#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc,char*argv[]){

    int sockfd,n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[1000];

    if(argc < 2){
        printf("Errore! inserire il numero di porta\n");
        return -1;
    }

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        printf("Errore nell'apertura  di socket\n");
        return -1;
    }

    memset(&local_addr,0,len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&local_addr,len)<0){
        printf("Errore nella bind\n");
        return -1;
    }

    for(;;){
        n = recvfrom(sockfd,msg,999,0,(struct sockaddr*)&remote_addr,&len);

        msg[n]=0;
        sendto(sockfd,msg,n,0,(struct sockaddr*)&remote_addr,len);

        printf("IP = %s, p = %d, msg = %s\n",inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port),msg);

        if(strcmp(msg,"fine\n")==0){
            printf("arrivederci\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}