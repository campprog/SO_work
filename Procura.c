#include <unistd.h>   // para usar read, write, close
#include <fcntl.h>    // para usar open
#include <string.h>   // para usar strstr (procurar texto dentro de texto)

#define BUFFER 1024   // tamanho do buffer de leitura

// função simples para imprimir números (copiada do conta)
void write_int(int n) {
    char buf[20];   // array para guardar os dígitos do número
    int i = 0;      // contador

    if (n == 0) {   // caso especial se o número for 0
        write(1, "0", 1);
        return;
    }

    // transformar o número em caracteres (ao contrário)
    while (n > 0) {
        buf[i++] = (n % 10) + '0';  // pega no último dígito
        n /= 10;                    // remove o último dígito
    }

    // escreve os dígitos na ordem correta
    while (i--) write(1, &buf[i], 1);
}

int main(int argc, char *argv[]) {
    // verificar se o número de argumentos está correto
    if (argc != 3) {
        write(2, "Uso: procura <ficheiro> <padrao>\n", 33);
        return 1;
    }

    // abrir o ficheiro em modo leitura
    int fd = open(argv[1], O_RDONLY);

    // verificar se houve erro ao abrir
    if (fd < 0) {
        write(2, "Erro: ficheiro nao existe\n", 27);
        return 1;
    }

    char buffer[BUFFER];  // buffer para ler do ficheiro
    int bytes;            // número de bytes lidos

    char linha[1024];     // guardar uma linha completa
    int pos = 0;          // posição atual na linha
    int num_linha = 1;    // contador de linhas
    int encontrou = 0;    // flag para saber se encontrou alguma coisa

    // ler o ficheiro em blocos
    while ((bytes = read(fd, buffer, BUFFER)) > 0) {
        for (int i = 0; i < bytes; i++) {
            char c = buffer[i];  // caractere atual

            linha[pos++] = c;    // adicionar à linha

            // se encontrar fim de linha
            if (c == '\n') {
                linha[pos] = '\0';  // terminar string

                // ver se a linha contém o padrão
                if (strstr(linha, argv[2]) != NULL) {
                    encontrou = 1;

                    // imprimir linha encontrada
                    write(1, "Linha ", 6);
                    write_int(num_linha);
                    write(1, ": ", 2);
                    write(1, linha, pos);
                }

                pos = 0;        // reset da linha
                num_linha++;    // próxima linha
            }
        }
    }

    // tratar última linha (caso não termine com \n)
    if (pos > 0) {
        linha[pos] = '\0';

        if (strstr(linha, argv[2]) != NULL) {
            encontrou = 1;

            write(1, "Linha ", 6);
            write_int(num_linha);
            write(1, ": ", 2);
            write(1, linha, pos);
            write(1, "\n", 1);
        }
    }

    // se não encontrou nada
    if (!encontrou) {
        write(1, "Nenhuma linha encontrada\n", 25);
    }

    close(fd);  // fechar o ficheiro

    return 0;
}