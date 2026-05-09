#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int main(int numero_argumentos, char *argumentos[])
{
    char *caminho;

    // vê se é suposto mostrar a diretoria atual (por defeito) ou outra qualquer
    if (numero_argumentos < 2)
    {
        caminho = ".";
    }
    else
    {
        caminho = argumentos[1];
    }

    // abre a diretoria
    DIR *diretoria = opendir(caminho);
    if (diretoria == NULL)
    {
        write(STDERR_FILENO, "Erro ao abrir a diretoria.\n", sizeof("Erro ao abrir a diretoria.\n") - 1);
        return 1;
    }

    // dirent é uma estrutura que contém informações dos ficheiros de uma diretoria
    struct dirent *entrada;
    while ((entrada = readdir(diretoria)) != NULL)
    {

        // Ignorar os diretórios especiais "." e ".." para a lista ficar limpa
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
        {
            continue;
        }

        // escreve o nome do ficheiro ou da pasta na consola
        write(STDOUT_FILENO, entrada->d_name, strlen(entrada->d_name));

        // distingue entre ficheiro e diretoria
        if (entrada->d_type == DT_DIR)
        {
            write(STDOUT_FILENO, "  (Diretoria)\n", sizeof("  (Diretoria)\n") - 1);
        }
        else
        {
            write(STDOUT_FILENO, "   (Ficheiro)\n", sizeof("   (Ficheiro)\n") - 1);
        }
    }

    // fecha a diretoria
    closedir(diretoria);

    return 0;
}
