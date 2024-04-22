#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc,char**argv){

    int sockfd,n;
    struct sockaddr_in6 remote_addr;
    char sendline[1000], recline[1000];
    char ipv6_addr[INET6_ADDRSTRLEN]; //salveremo l'indirizzo ipv6 qui
    socklen_t len = sizeof(struct sockaddr);
    
    if(argc!=3){
        printf("Errore! inserire porta\n");
        return -1;
    }

    sockfd = socket(AF_INET6,SOCK_DGRAM,0);
    if(sockfd<0){
        printf("Errore nell'apertura del socket\n");
        return -1;
    }

    memset(&remote_addr,0,len);
    remote_addr.sin6_family=AF_INET6;
    inet_pton(AF_INET6,argv[1],&(remote_addr.sin6_addr));
    remote_addr.sin6_port=htons(atoi(argv[2]));

    while(fgets(sendline,1000,stdin)!=NULL){
        sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr*)&remote_addr,len);

        n = recvfrom(sockfd,recline,999,0,(struct sockaddr*)&remote_addr,&len);
        recline[n]=0;

        inet_ntop(AF_INET6,&(remote_addr.sin6_addr),ipv6_addr,INET6_ADDRSTRLEN);
        printf("IP = %s, p = %d, msg = %s",ipv6_addr,ntohs(remote_addr.sin6_port),recline);

        if(strcmp(recline,"fine")==0){
            printf("arrivederci\n");
            break;
        }
    }
    close(sockfd);
    return 0;
}