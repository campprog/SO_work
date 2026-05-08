#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(STDERR_FILENO, "Filename is needed.\n", 20);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd == -1)
    {
        write(STDERR_FILENO, "File doesnt exist.\n", 19);
        return 1;
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytesRead) != bytesRead)
        {
            write(STDERR_FILENO, "Error trying to show file content.\n", 35);
            close(fd);
            return 1;
        }
    }

    if (bytesRead == -1)
    {
        write(STDERR_FILENO, "Error trying to read the file content.\n", 39);
    }

    close(fd);

    return 0;
}