#include <unistd.h>
#include <fcntl.h>

int main(int numero_argumentos, char *argumentos[])
{
    if (numero_argumentos != 2)
    {
        write(STDERR_FILENO, "Falta indicar o ficheiro.\n", sizeof("Falta indicar o ficheiro.\n") - 1);
        return 1;
    }

    int descritor_ficheiro = open(argumentos[1], O_RDONLY);

    if (descritor_ficheiro == -1)
    {
        write(STDERR_FILENO, "O ficheiro nao existe.\n", sizeof("O ficheiro nao existe.\n") - 1);
        return 1;
    }

    char bloco[1024];
    ssize_t bytes_lidos;

    while ((bytes_lidos = read(descritor_ficheiro, bloco, sizeof(bloco))) > 0)
    {
        if (write(STDOUT_FILENO, bloco, bytes_lidos) != bytes_lidos)
        {
            write(STDERR_FILENO, "Erro ao mostrar o conteudo do ficheiro.\n", sizeof("Erro ao mostrar o conteudo do ficheiro.\n") - 1);
            close(descritor_ficheiro);
            return 1;
        }
    }

    if (bytes_lidos == -1)
    {
        write(STDERR_FILENO, "Erro ao ler o conteudo do ficheiro.\n", sizeof("Erro ao ler o conteudo do ficheiro.\n") - 1);
    }

    close(descritor_ficheiro);

    return 0;
}
