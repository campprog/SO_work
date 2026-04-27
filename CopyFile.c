#include <fcntl.h>    // Necessário para as flags open() 
#include <unistd.h>   // Necessário para read, write, close e constantes
#include <string.h>   // Apenas para strlen() no cálculo das mensagens de erro
#include <stdlib.h>   // Para a função exit() 
#include <sys/stat.h>
#include <sys/types.h>


#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // 1. Validar se foram passados exatamente 2 argumentos (origem e destino)
    if (argc != 3) {
        write(STDERR_FILENO, "Error: Its needed 2 files.\n", 27);
        return 1;
    }

    // 2. Abrir o ficheiro de origem em modo de leitura (O_RDONLY)
    int fd_source = open(argv[1], O_RDONLY); 
    if (fd_source == -1) {
        write(STDERR_FILENO, "Error: Source file does not exist.\n", 35);
        return 1;
    }

    // 3. Abrir o ficheiro de destino
    // O_WRONLY: Modo de escrita 
    // O_CREAT: Cria o ficheiro se não existir 
    // O_EXCL: Garante que o comando falha se o ficheiro já existir (conforme enunciado parte 1b)
    // S_IRUSR | S_IWUSR: Define permissões de leitura/escrita para o dono
    int fd_destination = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd_destination == -1) 
    {
        write(STDERR_FILENO, "Error: Destination file already exists or can not be created\n", 61);
        close(fd_destination);
        return 1;
    }

    // 4. Ciclo de transferência (Read e Write)
    char buffer[BUFFER_SIZE];
    ssize_t ReadedBytes;
    // Lê da origem e coloca no buffer 
    while ((ReadedBytes = read(fd_source, buffer, sizeof(buffer))) > 0) 
    {
        // Escreve o que está no buffer para o destino
        if (write(fd_destination, buffer, ReadedBytes) != ReadedBytes) 
        {
            char *erro_critico = "Erro critico na escrita do ficheiro.\n";
            write(STDERR_FILENO, "Error writing to destination file.\n", 34);
            close(fd_source);
            close(fd_destination);
            return 1;
        }
    }

    // 5. Fechar os descritores para libertar recursos 
    close(fd_source);
    close(fd_destination);

    return 0;
}