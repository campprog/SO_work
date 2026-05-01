#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>   // Para opendir e readdir
#include <sys/stat.h> // Para distinguir tipo de ficheiro
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *path;

    // vê se é suposto mostrar a diretoria atual (por defeito) ou outra qualquer
    if (argc < 2) {
        path = "."; 
    } else {
        path = argv[1];
    }

    // abre a diretoria 
    DIR *dir = opendir(path);
    if (dir == NULL) {
        write(STDERR_FILENO, "Error trying to open the directory.\n", 37);
        return 1;
    }

    // 3. dirent é uma struct que contem as informações dos ficheiros de uma diretoria
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        
        // Ignorar os diretórios especiais "." e ".." para a lista ficar limpa
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // escreve o nome do ficheiro ou da pasta na consola
        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));

        // 4. Distinguir entre ficheiro e diretoria 
        // vê se o ficheiro é diretoria ou não (DT_DIR é uma constante). Se entry->d_type for DT_DIR é porque é uma diretoria
        if (entry->d_type == DT_DIR) {
            write(STDOUT_FILENO, "  (Directory)\n", 14);
        } else {
            write(STDOUT_FILENO, "   (File)\n", 10);
        }
    }

    // 5. Fechar a diretoria
    closedir(dir);

    return 0;
}