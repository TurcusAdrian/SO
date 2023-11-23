#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define BMP_HEADER_SIZE 54

typedef struct {
    char signature[2];
    int size;
    int reserved;
    int offset;
} BMPHeader;
 
typedef struct {
    int header_size;
    int width;
    int height;
    int planes;
    int bpp;
    int compression;
    int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    int colors_used;
    int colors_important;
}InfoHeader;


char *obtine_permisiuni_user(mode_t mode) {
    static char permisiuni_user[3];
    strcpy(permisiuni_user, "---");
    if (mode & S_IRUSR) permisiuni_user[0] = 'R';
    if (mode & S_IWUSR) permisiuni_user[1] = 'W';
    if (mode & S_IXUSR) permisiuni_user[2] = 'X';
    return permisiuni_user;
}

char *obtine_permisiuni_grup(mode_t mode) {
    static char permisiuni_grup[3];
    strcpy(permisiuni_grup, "---");
    
    if (mode & S_IRGRP) permisiuni_grup[3] = 'R';
    if (mode & S_IWGRP) permisiuni_grup[4] = 'W';
    if (mode & S_IXGRP) permisiuni_grup[5] = 'X';
    return permisiuni_grup;
}

char *obtine_permisiuni_other(mode_t mode) {
    static char permisiuni_other[3];
    strcpy(permisiuni_other, "---");
    if (mode & S_IROTH) permisiuni_other[0] = 'R';
    if (mode & S_IWOTH) permisiuni_other[1] = 'W';
    if (mode & S_IXOTH) permisiuni_other[2] = 'X';
    return permisiuni_other;
}

char* get_file_name(char **argv){
    
    return argv[1];
}

void verificare_argumente(int argc, char **argv) {
     
    if (argc > 2) {
        perror("Usage: ./program <input_file>");
        exit(-1);
    }

   
    if (strstr(get_file_name(argv), ".bmp") == NULL) {
        perror("Fisierul trebuie sa fie unul de tipul '.bmp' ");
        exit(-1);
    }

}


void close_files(int file_input, int file_output){

    if(close(file_input)==-1){
        printf("Nu s-a putut inchide fisierul bmp primit ca argument!");
    }

    if(close(file_output)==-1){
        printf("Nu s-a putut inchide fisierul statistica.txt in care scriem!");
    }
}

void scriere_in_fisier(int file_in,int file_out,char **argv){


    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        close(file_out);
        exit(EXIT_FAILURE);
    }
    
    char bmp_header[BMP_HEADER_SIZE];
    int bytes_read = read(file_in, bmp_header, BMP_HEADER_SIZE);

    unsigned int width = *(unsigned int*)&bmp_header[18];  
    unsigned int height = *(unsigned int*)&bmp_header[22]; 
    unsigned int image_size = *(unsigned int*)&bmp_header[34]; 

    char nume_fisier[50];
    sprintf(nume_fisier, "nume fisier: %s\n",get_file_name(argv) );
    write(file_out,nume_fisier,strlen(nume_fisier));



    char inaltime_imagine[50];
    sprintf(inaltime_imagine, "inaltime: %d\n",height);
    write(file_out,inaltime_imagine,strlen(inaltime_imagine));



    char lungime_imagine[50];
    sprintf(lungime_imagine, "lungime: %d\n",width);
    write(file_out,lungime_imagine,strlen(lungime_imagine));



    char dimensiune_imagine[50];
    sprintf(dimensiune_imagine, "dimensiune: %d\n",image_size);
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
    sprintf(drepturi_de_acces_user, "Drepturi de acces user: %s\n", obtine_permisiuni_user(file_stat.st_mode & S_IRWXU));
    write(file_out, drepturi_de_acces_user, strlen(drepturi_de_acces_user));



    char drepturi_de_acces_grup[50];
    sprintf(drepturi_de_acces_grup, "Drepturi de acces grup: %s\n", obtine_permisiuni_grup(file_stat.st_mode & S_IRWXG));
    write(file_out, drepturi_de_acces_grup, strlen(drepturi_de_acces_grup));



    char drepturi_de_acces_altii[50];
    sprintf(drepturi_de_acces_altii, "Drepturi de acces altii: %s\n", obtine_permisiuni_other(file_stat.st_mode & S_IRWXO));
    write(file_out, drepturi_de_acces_altii, strlen(drepturi_de_acces_altii));

}

int main(int argc, char **argv) {

    verificare_argumente(argc,argv);
   
    
    int file_in = open(get_file_name(argv), O_RDONLY);

    
    int file_out = open("statistica.txt", O_WRONLY);

    if (file_in == -1) {
        perror("Nu se poate deschide fisierul!\n");
        exit(-1);
    }
    
    scriere_in_fisier(file_in,file_out,argv);
   
    close_files(file_in,file_out);

    return 0;
}
