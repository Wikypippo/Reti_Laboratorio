#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc,char* argv[]){

    int sockfd;
    struct sockaddr_in dest_addr;
    int i,n;
    char buffer[100];

    if (argc < 3){
        printf("Errore! Scrivere IP di destinazione e porta di destinazione");
        return -1;
    }

    sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("Errore nell'apertura del Socket");
        return -1;
    }

    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&(dest_addr.sin_addr));
    dest_addr.sin_port=htons(atoi(argv[2]));

    printf("Insert integer:\n");
    scanf("%d",&n);

    for(int i=0; i<10; i++){
        sprintf(buffer,"%d",i+n);
        printf("Sending %s\n",buffer);
        sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    }

    close(sockfd);

    return 0;
}