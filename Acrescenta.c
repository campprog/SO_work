#include <unistd.h>
#include <fcntl.h>

int main(int numero_argumentos, char *argumentos[])
{
    if (numero_argumentos != 3)
    {
        write(STDERR_FILENO, "Falta indicar ficheiros.\n", sizeof("Falta indicar ficheiros.\n") - 1);
        return 1;
    }

    int descritor_origem = open(argumentos[1], O_RDONLY);
    if (descritor_origem == -1)
    {
        write(STDERR_FILENO, "O ficheiro de origem nao existe.\n", sizeof("O ficheiro de origem nao existe.\n") - 1);
        return 1;
    }

    int descritor_destino = open(argumentos[2], O_WRONLY | O_APPEND);
    if (descritor_destino == -1)
    {
        write(STDERR_FILENO, "O ficheiro de destino nao existe.\n", sizeof("O ficheiro de destino nao existe.\n") - 1);
        close(descritor_origem);
        return 1;
    }

    char bloco[1024];
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

    if (bytes_lidos == -1)
    {
        write(STDERR_FILENO, "Erro ao ler o ficheiro de origem.\n", sizeof("Erro ao ler o ficheiro de origem.\n") - 1);
    }

    /* Fecha os ficheiros abertos antes de terminar. */
    close(descritor_origem);
    close(descritor_destino);

    return 0;
}
