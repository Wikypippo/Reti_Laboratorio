#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
    //i processi avranno entrambi la loro copia di queste variabili!!
    int sockfd, n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[1000];

    if(argc < 4) {
        printf("Errore! Inserisci i parametri corretti\n");
        return -1;
    }

    if(fork()) {
        //parte che gestisce la ricezione
        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            printf("Errore nell'apertura di socket!\n");
            return -1;
        }
        memset(&local_addr, 0, len);
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(atoi(argv[3]));

        if(bind(sockfd, (struct sockaddr*)&local_addr, len) < 0) {
            printf("Errore bind\n");
            return -1;
        }

        while(1) {
            n = recvfrom(sockfd, msg, sizeof(msg) - 1, 0, (struct sockaddr*)&remote_addr, &len);
            msg[n] = 0;
            printf("IP = %s,  port = %d, msg = %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), msg);

            if(strcmp(msg, "basta\n") == 0) {
                break;
            }
        }
        printf("Fine della ricezione\n");
        close(sockfd);
        return 0;
    }
    else {
        //parte che gestisce la trasmissione
        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            printf("Errore nell'apertura di socket!\n");
            return -1;
        }

        memset(&remote_addr, 0, len);
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(atoi(argv[2]));

        while(fgets(msg, sizeof(msg), stdin) != NULL) {
            sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&remote_addr, len);
            if(strcmp(msg, "basta\n") == 0) {
                break;
            }
        }
        printf("Fine della trasmissione\n");
        close(sockfd);
        return 0;
    }
    return 0;
}