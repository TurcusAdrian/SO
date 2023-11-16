#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define SIZE 4096

char *obtine_permisiuni(mode_t mode) {
    static char permisiuni[10];
    strcpy(permisiuni, "---------");
    if (mode & S_IRUSR) permisiuni[0] = 'R';
    if (mode & S_IWUSR) permisiuni[1] = 'W';
    if (mode & S_IXUSR) permisiuni[2] = 'X';
    if (mode & S_IRGRP) permisiuni[3] = 'R';
    if (mode & S_IWGRP) permisiuni[4] = 'W';
    if (mode & S_IXGRP) permisiuni[5] = 'X';
    if (mode & S_IROTH) permisiuni[6] = 'R';
    if (mode & S_IWOTH) permisiuni[7] = 'W';
    if (mode & S_IXOTH) permisiuni[8] = 'X';
    return permisiuni;
}


int main(int argc, char **argv) {

    
    if (argc > 2) {
        perror("Usage: ./program <input_file>");
        exit(-1);
    }

    char *file_name = argv[1];

   
    if (strstr(file_name, ".bmp") == NULL) {
        perror("File extension must be '.bmp'");
        exit(-1);
    }

    
    int file_in = open(file_name, O_RDONLY);

    
    int file_out = open("statistica.txt", O_WRONLY);

    if (file_in == -1) {
        perror("Could not open the file!\n");
        exit(-1);
    }

    char bytes_to_read[SIZE]; 
    int nr = 0;
    

     //while((nr=read(file_in,bytes_to_read,SIZE))){
        //printf("%s",bytes_to_read);
     //}

    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        close(file_out);
        exit(EXIT_FAILURE);
    }


    char nume_fisier[50];
    sprintf(nume_fisier, "nume fisier: %s\n",file_name);
    write(file_out,nume_fisier,strlen(nume_fisier));



    char inaltime_imagine[50];
    sprintf(inaltime_imagine, "inaltime: %d\n",?);
    write(file_out,inaltime_imagine,strlen(inaltime_imagine));



    char lungime_imagine[50];
    sprintf(lungime_imagine, "lungime: %d\n",?);
    write(file_out,lungime_imagine,strlen(lungime_imagine));



    char dimensiune_imagine[50];
    sprintf(dimensiune_imagine, "dimensiune: %d\n",?);
    write(file_out,dimensiune_imagine,strlen(dimensiune_imagine));



    char identificator_utilizator[50];
    sprintf(identificator_utilizator, "identificatorul utilizatorului: %d\n",file_stat.st_uid);
    write(file_out,identificator_utilizator,strlen(identificator_utilizator));



    struct tm *timp_modificare = localtime(&file_stat.st_mtime);
    char timp_modificare_string[20];
    strftime(timp_modificare_string, sizeof(timp_modificare_string), "%d", timp_modificare);
    char timpul_ultimei_modificari[50];
    sprintf(timpul_ultimei_modificari, "timpul ultimei modificari: %d\n",timp_modificare_string);
    write(file_out,timpul_ultimei_modificari,strlen(timpul_ultimei_modificari));
        


    char contorul_de_legaturi[50];
    sprintf(contorul_de_legaturi, "contorul de legaturi: %d\n",file_stat.st_nlink);
    write(file_out,contorul_de_legaturi,strlen(contorul_de_legaturi));
    


    char drepturi_de_acces_user[50];
    sprintf(drepturi_de_acces_user, "Drepturi de acces user: %s\n", obtine_permisiuni(file_stat.st_mode & S_IRWXU));
    write(file_out, drepturi_de_acces_user, strlen(drepturi_de_acces_user));



    char drepturi_de_acces_grup[50];
    sprintf(drepturi_de_acces_grup, "Drepturi de acces grup: %s\n", obtine_permisiuni(file_stat.st_mode & S_IRWXG));
    write(file_out, drepturi_de_acces_grup, strlen(drepturi_de_acces_grup));



    char drepturi_de_acces_altii[50];
    sprintf(drepturi_de_acces_altii, "Drepturi de acces altii: %s\n", obtine_permisiuni(file_stat.st_mode & S_IRWXO));
    write(file_out, drepturi_de_acces_altii, strlen(drepturi_de_acces_altii));



    if(close(file_in)==-1){
        printf("Nu s-a putut inchide fisierul bmp primit ca argument!");
    }

    if(close(file_out)==-1){
        printf("Nu s-a putut inchide fisierul statistica.txt in care scriem!");
    }

    return 0;
}
