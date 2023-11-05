#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

// ./p file.txt out.txt caracter (in exemplu e ales Y)

int main(int argc, char** argv){
  char linie[BUFFER_SIZE];
  int f=0;
  int f_out=0;
  int countHigh=0;
  int countLow=0;
  int countCifre=0;
  int aparitiiCaracter=0;
  char character = argv[3][0];

  
  int nr=0;
  if(argc>4){
    printf("Prea multe argumente\n");
    exit(-1);
  }
  
  f=open(argv[1],O_RDONLY,S_IRUSR);
  
  if(f==-1){
    printf("Nu s-a putut deschide fisierul!\n");
    exit(-1);
  }
  while((nr=read(f,linie,BUFFER_SIZE))){

       for(int i=0;i<nr;i++){
	 
	if(isupper(linie[i]))
	  {countHigh++;}
	
	if(islower(linie[i]))
	  {countLow++;}
	
	if(isdigit(linie[i])){
	  {countCifre++;}
	}
	if(linie[i]==character){
	  aparitiiCaracter++;
	}
      }
  }
  
 f_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  if(f_out==-1){
    printf("Nu s-a putut deschide fisierul!\n");
    exit(-1);
  }
  
  char nrlitmic[50];
  sprintf(nrlitmic, "numar litere mici: %d\n",countLow);
  write(f_out,nrlitmic,strlen(nrlitmic));

  char nrlitmari[50];
  sprintf(nrlitmari, "numar litere mari: %d\n",countHigh);
  write(f_out,nrlitmari,strlen(nrlitmari));

  char nrcifre[50];
  sprintf(nrcifre, "numar cifre: %d\n",countCifre);
  write(f_out,nrcifre,strlen(nrcifre));

  char aparcar[50];
  sprintf(aparcar, "numar aparitii caracter: %d\n",aparitiiCaracter);
  write(f_out,aparcar,strlen(aparcar));

  
  
  close(f_out);
  close(f);
  return 0;
}
