#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

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
    static char permisiuni_user[4];
    strcpy(permisiuni_user, "---");
    if (mode & S_IRUSR) permisiuni_user[0] = 'R';
    if (mode & S_IWUSR) permisiuni_user[1] = 'W';
    if (mode & S_IXUSR) permisiuni_user[2] = 'X';
    return permisiuni_user;
}

char *obtine_permisiuni_grup(mode_t mode) {
    static char permisiuni_grup[4];
    strcpy(permisiuni_grup, "---");
    
    if (mode & S_IRGRP) permisiuni_grup[0] = 'R';
    if (mode & S_IWGRP) permisiuni_grup[1] = 'W';
    if (mode & S_IXGRP) permisiuni_grup[2] = 'X';
    return permisiuni_grup;
}

char *obtine_permisiuni_other(mode_t mode) {
    static char permisiuni_other[4];
    strcpy(permisiuni_other, "---");
    if (mode & S_IROTH) permisiuni_other[0] = 'R';
    if (mode & S_IWOTH) permisiuni_other[1] = 'W';
    if (mode & S_IXOTH) permisiuni_other[2] = 'X';
    return permisiuni_other;
}

char* path_to_directory_input(char **argv){
    return argv[1];
}

char* path_to_directory_output(char **argv){
    return argv[2];
}

int e_director(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
        perror("Eroare la structura stat");
        exit(EXIT_FAILURE);
    }

    return S_ISDIR(path_stat.st_mode);
}

void verificare_argumente(int argc, char **argv) {
    
    if (argc != 3) {
        printf("Usage: %s <directory_input> %s <directory_output>\n", path_to_directory_input(argv));
        exit(EXIT_FAILURE);
    }
    

    if (!e_director(path_to_directory_input(argv)) || !e_director(path_to_directory_output(argv))) {
        perror("Unul din argumentele date nu e director!\n");
        exit(EXIT_FAILURE);
    }
}

void close_files(int file_input, int file_output){

    if(close(file_input)==-1){
        printf("Nu s-a putut inchide fisierul primit ca argument!");
    }

    if(close(file_output)==-1){
        printf("Nu s-a putut inchide fisierul statistica.txt in care scriem!");
    }
}

void close_directories(DIR *dir_input, DIR *dir_output){

    if (closedir(dir_input) == -1) {
        perror("Nu s-a putut inchide directorul de intrare!");
    }

    if (closedir(dir_output) == -1) {
        perror("Nu s-a putut inchide directorul de iesire!");
    }
}

void bmp_convert_to_grey(int file_in, char *entry_name) {
    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        exit(EXIT_FAILURE);
    }

    char bmp_header[BMP_HEADER_SIZE];
    int bytes_read = read(file_in, bmp_header, BMP_HEADER_SIZE);

    unsigned int width = *(unsigned int *)&bmp_header[18];
    unsigned int height = *(unsigned int *)&bmp_header[22];
    unsigned int image_size = *(unsigned int *)&bmp_header[34];

    lseek(file_in, BMP_HEADER_SIZE, SEEK_SET);

    if (bytes_read == BMP_HEADER_SIZE && width > 0 && height > 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                unsigned char pixel[3];
                read(file_in, pixel, sizeof(pixel));
                lseek(file_in, -3, SEEK_CUR);

                double grey = 0.299 * (double)pixel[0] + 0.587 * (double)pixel[1] + 0.114 * (double)pixel[2];
                unsigned char grey_pixel[3] = {(unsigned char)grey, (unsigned char)grey, (unsigned char)grey};
                write(file_in, grey_pixel, sizeof(grey_pixel));
            }
        }
    }

    close(file_in);
    exit(EXIT_SUCCESS);
}

void bmp_scriere_in_fisier(int file_in,int file_out,char* entry_name){


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
    sprintf(nume_fisier, "nume imagine: %s\n",entry_name);
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
    sprintf(timpul_ultimei_modificari, "timpul ultimei modificari: %s\n",timp_modificare_string);
    write(file_out,timpul_ultimei_modificari,strlen(timpul_ultimei_modificari));
        


    char contorul_de_legaturi[50];
    sprintf(contorul_de_legaturi, "contorul de legaturi: %ld\n",file_stat.st_nlink);
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

void normal_scriere_in_fisier(int file_in,int file_out, char* entry_name){
    
    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        close(file_out);
        exit(EXIT_FAILURE);
    }

     char nume_fisier[50];
    sprintf(nume_fisier, "nume fisier: %s\n",entry_name);
    write(file_out,nume_fisier,strlen(nume_fisier));


    char dimensiune_fisier[50];
    sprintf(dimensiune_fisier, "dimensiune: %ld\n",file_stat.st_size);
    write(file_out,dimensiune_fisier,strlen(dimensiune_fisier));



    char identificator_utilizator[50];
    sprintf(identificator_utilizator, "identificatorul utilizatorului: %d\n",file_stat.st_uid);
    write(file_out,identificator_utilizator,strlen(identificator_utilizator));



    struct tm *timp_modificare = localtime(&file_stat.st_mtime);
    char timp_modificare_string[20];
    strftime(timp_modificare_string, sizeof(timp_modificare_string), "%d", timp_modificare);
    char timpul_ultimei_modificari[50];
    sprintf(timpul_ultimei_modificari, "timpul ultimei modificari: %s\n",timp_modificare_string);
    write(file_out,timpul_ultimei_modificari,strlen(timpul_ultimei_modificari));
        


    char contorul_de_legaturi[50];
    sprintf(contorul_de_legaturi, "contorul de legaturi: %ld\n",file_stat.st_nlink);
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

void director_scriere_in_fisier(int file_in,int file_out, char* entry_name){
    
    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        close(file_out);
        exit(EXIT_FAILURE);
    }

    char nume_director[50];
    sprintf(nume_director, "nume director: %s\n",entry_name);
    write(file_out,nume_director,strlen(nume_director));


    char identificator_utilizator[50];
    sprintf(identificator_utilizator, "identificatorul utilizatorului: %d\n",file_stat.st_uid);
    write(file_out,identificator_utilizator,strlen(identificator_utilizator));



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

void link_scriere_in_fisier(int file_in,int file_out, char* entry_name, const char* file_path){
    
    struct stat file_stat;

    if (fstat(file_in, &file_stat) == -1) {
        perror("Eroare");
        close(file_in);
        close(file_out);
        exit(EXIT_FAILURE);
    }

    struct stat s_link;
    
    if(lstat(file_path,&s_link)!=0){
        perror("Eroare la lstat!");
        exit(EXIT_FAILURE);
    }
    
    char nume_legatura[50];
    sprintf(nume_legatura, "nume legatura: %s\n",entry_name);
    write(file_out,nume_legatura,strlen(nume_legatura));

    char dimensiune_legatura[50];
    sprintf(dimensiune_legatura, "dimensiune legatura: %ld\n",s_link.st_size);
    write(file_out,dimensiune_legatura,strlen(dimensiune_legatura));

    
    char dimensiune_fisier_target[50];
    sprintf(dimensiune_fisier_target,"dimensiune fisier target: %ld\n",file_stat.st_size);
    write(file_out,dimensiune_fisier_target,strlen(dimensiune_fisier_target));

    char identificator_utilizator[50];
    sprintf(identificator_utilizator, "identificatorul utilizatorului: %d\n",file_stat.st_uid);
    write(file_out,identificator_utilizator,strlen(identificator_utilizator));

    struct tm *timp_modificare = localtime(&file_stat.st_mtime);
    char timp_modificare_string[20];
    strftime(timp_modificare_string, sizeof(timp_modificare_string), "%d", timp_modificare);
    char timpul_ultimei_modificari[50];
    sprintf(timpul_ultimei_modificari, "timpul ultimei modificari: %s\n",timp_modificare_string);
    write(file_out,timpul_ultimei_modificari,strlen(timpul_ultimei_modificari));
        


    char contorul_de_legaturi[50];
    sprintf(contorul_de_legaturi, "contorul de legaturi: %ld\n",file_stat.st_nlink);
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
   
    DIR *directory_input=opendir(path_to_directory_input(argv));
    DIR *directory_output=opendir(path_to_directory_output(argv));
    
    struct dirent *entry;

    struct stat file_stat;

    char nume_fisier_statistica[500];

    int file_out_specific=0;

    while ((entry = readdir(directory_input)) != NULL) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { //sarim peste . si ..
            continue;
        }

        char file_path[5000];
        
        snprintf(file_path, sizeof(file_path), "%s/%s", path_to_directory_input(argv), entry->d_name);
        snprintf(nume_fisier_statistica, sizeof(nume_fisier_statistica), "%s/%s_statistica.txt", path_to_directory(argv), entry->d_name);
        file_out_specific = open(nume_fisier_statistica, O_WRONLY | O_APPEND | O_CREAT);
        
        if (lstat(file_path, &file_stat) == -1) {
            perror("Eroare la stat");
            continue;  //merge la urmatorul fisier
        }
	
	    if(S_ISLNK(file_stat.st_mode)){
            
            pid_t copil_link;

            copil_link=fork();

            if(copil_link==0){
                link_scriere_in_fisier(open(file_path, O_RDONLY),file_out_specific, entry->d_name,file_path);
                execlp("wc", "wc", "-l", file_path, NULL);
                exit(EXIT_SUCCESS);
            }
            else{
                perror("Eroare la procesul copil al symlink!\n");
                exit(EXIT_FAILURE);
            }

            int status_link=0;
            waitpid(copil_link, &status_link, 0);

            if (WIFEXITED(status_link)) {
            int lines = WEXITSTATUS(status_link);

            printf("Copilul a numarat %d linii.\n", lines);
	        continue;
        }

        if(S_ISREG(file_stat.st_mode) && strstr(entry->d_name, ".bmp")){
            
            pid_t copil_bmp_statis, copil_bmp_gri;

            copil_bmp_statis=fork();

            if(copil_bmp_statis==0){
                bmp_scriere_in_fisier(open(file_path, O_RDONLY), file_out_specific, entry->d_name);
                execlp("wc", "wc", "-l", file_path, NULL);
                exit(EXIT_SUCCESS);
            }
            else{
                perror("Eroare la procesul copil statistica .bmp!\n");
                exit(EXIT_FAILURE);
            }

            copil_bmp_gri=fork();
            if(copil_bmp_statis==0){
                bmp_convert_to_grey(open(file_path, O_RDONLY), entry->d_name);
                exit(EXIT_SUCCESS);
            }
            else{
                perror("Eroare la procesul copil conversie in gri .bmp!\n");
                exit(EXIT_FAILURE);
            }

            continue;

        }

        if(S_ISREG(file_stat.st_mode)){
            
            pid_t copil_regular;

            copil_regular=fork();

            if(copil_regular==0){    
                normal_scriere_in_fisier(open(file_path, O_RDONLY), file_out_specific, entry->d_name);
                execlp("wc", "wc", "-l", file_path, NULL);
                exit(EXIT_SUCCESS);
            }
            else{
                perror("Eroare la procesul copil al regular file!\n");
                exit(EXIT_FAILURE);
            }
            continue;

        }
        
        if(S_ISDIR(file_stat.st_mode)){

            pid_t copil_dir;

            copil_dir=fork();

            if(copil_dir==0){
                director_scriere_in_fisier(open(file_path, O_RDONLY), file_out_specific, entry->d_name);
                execlp("wc", "wc", "-l", file_path, NULL);
                exit(EXIT_SUCCESS);
            }
            else{
                perror("Eroare la procesul copil al director!\n");
                exit(EXIT_FAILURE);
            }
            continue;
        }

        else{
            printf("Tip de fisier necunoscut!\n");
            exit(EXIT_FAILURE);
        }
        close_files(open(file_path, O_RDONLY), file_out_specific);
        close_directories(directory_input,directory_output);
    }   
    closedir(directory_input);
    close(file_out_specific);

    return 0;
}