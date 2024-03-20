#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char nome [30];
    char cognome [30];
    char sesso;
    int annoNascita;
}persona;

void checkFile(FILE*f);
void readFile(FILE*f, persona**p);
void inserisci(char*b,persona**p);
void stampaArray(persona**p,int n);

int main(int argc,char *argv[]){
    int n=5;
    persona *persone[n];
    FILE *f = fopen("people.txt","r");

    checkFile(f);
    readFile(f,persone);
    stampaArray(persone,n);

    return 0;
}

void checkFile(FILE*f){
    if(!f){
        printf("ERRORE nell'apertura del file\n");
        exit(-1);
    } 
}

void readFile(FILE*f,persona**p){
    while(!feof(f)){
        char buffer[100];
        strcpy(buffer,fgets(buffer,100,f));
        inserisci(buffer,p);
        memset(buffer,0,sizeof(buffer));
    }
}

void inserisci(char*b,persona**p){
    static int i=0;
    p[i]=malloc(sizeof(persona));
    char*tok=strtok(b," \n");
    strncpy(p[i]->cognome,tok,sizeof((*p)->cognome));
    tok=strtok(NULL," \n");
    strncpy(p[i]->nome,tok,sizeof((*p)->nome));
    tok=strtok(NULL," \n");
    p[i]->sesso=*tok;
    tok=strtok(NULL," \n");
    p[i]->annoNascita=atoi(tok);
    tok=strtok(NULL," \n");
    i++;
}

void stampaArray(persona**p,int n){
    for(int i=0;i<n;i++){
        if(p[i])
            printf("%s %s %c %d\n",p[i]->cognome,p[i]->nome,p[i]->sesso,p[i]->annoNascita);
    }
}