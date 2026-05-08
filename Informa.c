#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

void print_num(long n)
{
    char buf[20];
    int i = 0;
    if (n == 0)
    {
        write(STDOUT_FILENO, "0", 1);
        return;
    }
    while (n > 0)
    {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    while (i > 0)
        write(STDOUT_FILENO, &buf[--i], 1);
}

int main(int argc, char *argv[])
{
    struct stat st;

    if (argc != 2)
    {
        write(STDERR_FILENO, "Filename is needed.\n", 20);
        return 1;
    }

    if (lstat(argv[1], &st) == -1)
    {
        write(STDERR_FILENO, "Ficheiro inexistente.\n", 22);
        return 1;
    }

    // 1. Tipo de ficheiro (Ajustado com Link e Socket)
    write(STDOUT_FILENO, "Tipo: ", 6);
    if (S_ISREG(st.st_mode))
        write(STDOUT_FILENO, "Ficheiro normal\n", 16);
    else if (S_ISDIR(st.st_mode))
        write(STDOUT_FILENO, "Diretoria\n", 10);
    else if (S_ISLNK(st.st_mode))
        write(STDOUT_FILENO, "Link simbolico\n", 15);
    else if (S_ISCHR(st.st_mode))
        write(STDOUT_FILENO, "Dispositivo de caracteres\n", 26);
    else if (S_ISBLK(st.st_mode))
        write(STDOUT_FILENO, "Dispositivo de bloco\n", 21);
    else if (S_ISFIFO(st.st_mode))
        write(STDOUT_FILENO, "FIFO (named pipe)\n", 18);
    else if (S_ISSOCK(st.st_mode))
        write(STDOUT_FILENO, "Socket\n", 7);
    else
        write(STDOUT_FILENO, "Desconhecido\n", 13);

    // 2. Inode e Tamanho (Com quebras de linha)
    write(STDOUT_FILENO, "I-node: ", 8);
    print_num(st.st_ino);
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, "Tamanho: ", 9);
    print_num(st.st_size);
    write(STDOUT_FILENO, " bytes\n", 7);

    // 3. Dono
    struct passwd *pw = getpwuid(st.st_uid);
    write(STDOUT_FILENO, "Dono: ", 6);
    if (pw)
    {
        int len = 0;
        while (pw->pw_name[len] != '\0')
            len++;
        write(STDOUT_FILENO, pw->pw_name, len);
        write(STDOUT_FILENO, "\n", 1);
    }
    else
    {
        print_num(st.st_uid); // Mostra o ID se não houver nome
        write(STDOUT_FILENO, "\n", 1);
    }

    // 4. Permissões simbólicas (Incluindo o caractere de tipo inicial)
    write(STDOUT_FILENO, "Permissoes: ", 12);
    if (S_ISDIR(st.st_mode))
        write(1, "d", 1);
    else if (S_ISLNK(st.st_mode))
        write(1, "l", 1);
    else
        write(1, "-", 1);

    write(STDOUT_FILENO, (st.st_mode & S_IRUSR) ? "r" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IWUSR) ? "w" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IXUSR) ? "x" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IRGRP) ? "r" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IWGRP) ? "w" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IXGRP) ? "x" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IROTH) ? "r" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IWOTH) ? "w" : "-", 1);
    write(STDOUT_FILENO, (st.st_mode & S_IXOTH) ? "x" : "-", 1);
    write(STDOUT_FILENO, "\n", 1);

    // 5. Datas (O teu código de contagem manual está ótimo)
    char *data_str;
    int data_len;

    data_str = ctime(&st.st_atime);
    data_len = 0;
    while (data_str[data_len] != '\0')
        data_len++;
    write(1, "Leitura: ", 9);
    write(1, data_str, data_len);

    data_str = ctime(&st.st_mtime);
    data_len = 0;
    while (data_str[data_len] != '\0')
        data_len++;
    write(1, "Modif:   ", 9);
    write(1, data_str, data_len);

    data_str = ctime(&st.st_ctime);
    data_len = 0;
    while (data_str[data_len] != '\0')
        data_len++;
    write(1, "Estado:  ", 9);
    write(1, data_str, data_len);

    return 0;
}