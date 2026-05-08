#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        write(STDERR_FILENO, "Filename missing.\n", 18);
        return 1;
    }

    int fdOrigin = open(argv[1], O_RDONLY);
    if (fdOrigin == -1)
    {
        write(STDERR_FILENO, "File doesnt exist.\n", 19);
        return 1;
    }

    int fdDestination = open(argv[2], O_WRONLY | O_APPEND);
    if (fdDestination == -1)
    {
        write(STDERR_FILENO, "File doesnt exist.\n", 19);
        close(fdOrigin);
        return 1;
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fdOrigin, buffer, sizeof(buffer))) > 0)
    {
        if (write(fdDestination, buffer, bytesRead) != bytesRead)
        {
            write(STDERR_FILENO, "Erro ao escrever no ficheiro destino.\n", 38);
            close(fdOrigin);
            close(fdDestination);
            return 1;
        }
    }

    if (bytesRead == -1)
    {
        write(STDERR_FILENO, "Erro ao ler o ficheiro origem.\n", 31);
    }

    /* 5. Fechar tudo */
    close(fdOrigin);
    close(fdDestination);

    return 0;
}