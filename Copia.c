#include <fcntl.h>    // necessário para as opções de open()
#include <unistd.h>   // necessário para read, write, close e constantes
#include <sys/stat.h>
#include <sys/types.h>


#define TAMANHO_BLOCO 1024

int main(int numero_argumentos, char *argumentos[]) {
    // valida se foram passados exatamente 2 argumentos (origem e destino)
    if (numero_argumentos != 3) {
        write(STDERR_FILENO, "Erro: sao necessarios 2 ficheiros.\n", sizeof("Erro: sao necessarios 2 ficheiros.\n") - 1);
        return 1;
    }

    // abre o ficheiro de origem em modo de leitura
    int descritor_origem = open(argumentos[1], O_RDONLY);
    if (descritor_origem == -1) {
        write(STDERR_FILENO, "Erro: o ficheiro de origem nao existe.\n", sizeof("Erro: o ficheiro de origem nao existe.\n") - 1);
        return 1;
    }

    // abre o ficheiro de destino e falha se ele já existir
    int descritor_destino = open(argumentos[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (descritor_destino == -1)
    {
        write(STDERR_FILENO, "Erro: o ficheiro de destino ja existe ou nao pode ser criado.\n", sizeof("Erro: o ficheiro de destino ja existe ou nao pode ser criado.\n") - 1);
        close(descritor_origem);
        return 1;
    }

    // copia blocos de dados da origem para o destino
    char bloco[TAMANHO_BLOCO];
    ssize_t bytes_lidos;
    while ((bytes_lidos = read(descritor_origem, bloco, sizeof(bloco))) > 0)
    {
        if (write(descritor_destino, bloco, bytes_lidos) != bytes_lidos)
        {
            write(STDERR_FILENO, "Erro ao escrever no ficheiro de destino.\n", sizeof("Erro ao escrever no ficheiro de destino.\n") - 1);
            close(descritor_origem);
            close(descritor_destino);
            return 1;
        }
    }

    // fecha os descritores para libertar recursos
    close(descritor_origem);
    close(descritor_destino);

    return 0;
}
