#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

void escrever_numero(long numero)
{
    char digitos[20];
    int indice = 0;
    if (numero == 0)
    {
        write(STDOUT_FILENO, "0", 1);
        return;
    }
    while (numero > 0)
    {
        digitos[indice++] = (numero % 10) + '0';
        numero /= 10;
    }
    while (indice > 0)
        write(STDOUT_FILENO, &digitos[--indice], 1);
}

int main(int numero_argumentos, char *argumentos[])
{
    struct stat estado;

    if (numero_argumentos != 2)
    {
        write(STDERR_FILENO, "Falta indicar o ficheiro.\n", sizeof("Falta indicar o ficheiro.\n") - 1);
        return 1;
    }

    if (lstat(argumentos[1], &estado) == -1)
    {
        write(STDERR_FILENO, "Ficheiro inexistente.\n", sizeof("Ficheiro inexistente.\n") - 1);
        return 1;
    }

    // mostra o tipo de ficheiro
    write(STDOUT_FILENO, "Tipo: ", 6);
    if (S_ISREG(estado.st_mode))
        write(STDOUT_FILENO, "Ficheiro normal\n", 16);
    else if (S_ISDIR(estado.st_mode))
        write(STDOUT_FILENO, "Diretoria\n", 10);
    else if (S_ISLNK(estado.st_mode))
        write(STDOUT_FILENO, "Ligacao simbolica\n", 18);
    else if (S_ISCHR(estado.st_mode))
        write(STDOUT_FILENO, "Dispositivo de caracteres\n", 26);
    else if (S_ISBLK(estado.st_mode))
        write(STDOUT_FILENO, "Dispositivo de bloco\n", 21);
    else if (S_ISFIFO(estado.st_mode))
        write(STDOUT_FILENO, "FIFO (canal com nome)\n", 22);
    else if (S_ISSOCK(estado.st_mode))
        write(STDOUT_FILENO, "Socket\n", 7);
    else
        write(STDOUT_FILENO, "Desconhecido\n", 13);

    // mostra o i-node e o tamanho
    write(STDOUT_FILENO, "I-node: ", 8);
    escrever_numero(estado.st_ino);
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, "Tamanho: ", 9);
    escrever_numero(estado.st_size);
    write(STDOUT_FILENO, " bytes\n", 7);

    // mostra o dono do ficheiro
    struct passwd *utilizador = getpwuid(estado.st_uid);
    write(STDOUT_FILENO, "Dono: ", 6);
    if (utilizador)
    {
        int comprimento = 0;
        while (utilizador->pw_name[comprimento] != '\0')
            comprimento++;
        write(STDOUT_FILENO, utilizador->pw_name, comprimento);
        write(STDOUT_FILENO, "\n", 1);
    }
    else
    {
        escrever_numero(estado.st_uid); // mostra o ID se não houver nome
        write(STDOUT_FILENO, "\n", 1);
    }

    // mostra as permissões simbólicas
    write(STDOUT_FILENO, "Permissoes: ", 12);
    if (S_ISDIR(estado.st_mode))
        write(1, "d", 1);
    else if (S_ISLNK(estado.st_mode))
        write(1, "l", 1);
    else
        write(1, "-", 1);

    write(STDOUT_FILENO, (estado.st_mode & S_IRUSR) ? "r" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IWUSR) ? "w" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IXUSR) ? "x" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IRGRP) ? "r" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IWGRP) ? "w" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IXGRP) ? "x" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IROTH) ? "r" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IWOTH) ? "w" : "-", 1);
    write(STDOUT_FILENO, (estado.st_mode & S_IXOTH) ? "x" : "-", 1);
    write(STDOUT_FILENO, "\n", 1);

    // mostra as datas de acesso, alteração e mudança de estado
    char *texto_data;
    int comprimento_data;

    texto_data = ctime(&estado.st_atime);
    comprimento_data = 0;
    while (texto_data[comprimento_data] != '\0')
        comprimento_data++;
    write(1, "Leitura: ", 9);
    write(1, texto_data, comprimento_data);

    texto_data = ctime(&estado.st_mtime);
    comprimento_data = 0;
    while (texto_data[comprimento_data] != '\0')
        comprimento_data++;
    write(1, "Modif:   ", 9);
    write(1, texto_data, comprimento_data);

    texto_data = ctime(&estado.st_ctime);
    comprimento_data = 0;
    while (texto_data[comprimento_data] != '\0')
        comprimento_data++;
    write(1, "Estado:  ", 9);
    write(1, texto_data, comprimento_data);

    return 0;
}
