#include <unistd.h>   // permite usar read, write e close
#include <fcntl.h>    // permite usar open

#define BUFFER 1024   // tamanho do bloco de leitura

// escreve uma string para o ecrã (stdout)
void write_str(char *str) {
    while (*str) write(1, str++, 1); // escreve caracter a caracter
}

// escreve um número inteiro (substitui o printf)
void write_int(int n) {
    char buf[20];
    int i = 0;

    // caso especial: número 0
    if (n == 0) {
        write(1, "0", 1);
        return;
    }

    // converte o número para caracteres (ao contrário)
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    // escreve os caracteres na ordem correta
    while (i--) write(1, &buf[i], 1);
}

int main(int argc, char *argv[]) {

    // verifica se foi passado exatamente 1 argumento (programa + ficheiro)
    if (argc != 2) {
        write(2, "Uso: conta <ficheiro>\n", 22); // mensagem de erro
        return 1; // termina com erro
    }

    // abre o ficheiro em modo leitura
    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        // erro se o ficheiro não existir
        write(2, "Erro: ficheiro nao existe\n", 27);
        return 1;
    }

    char buf[BUFFER];
    int bytes;

    int linhas = 0, palavras = 0, caracteres = 0;
    int dentro_palavra = 0; // indica se estamos dentro de uma palavra

    // lê o ficheiro em blocos
    while ((bytes = read(fd, buf, BUFFER)) > 0) {
        for (int i = 0; i < bytes; i++) {
            char c = buf[i];

            caracteres++; // conta todos os caracteres

            if (c == '\n')
                linhas++; // conta linhas

            // verifica separadores (espaço, enter, tab)
            if (c == ' ' || c == '\n' || c == '\t') {
                dentro_palavra = 0;
            } else if (!dentro_palavra) {
                // início de nova palavra
                palavras++;
                dentro_palavra = 1;
            }
        }
    }

    close(fd); // fecha o ficheiro

    // mostra os resultados
    write_str("Linhas: ");
    write_int(linhas);

    write_str("\nPalavras: ");
    write_int(palavras);

    write_str("\nCaracteres: ");
    write_int(caracteres);

    write_str("\n");

    return 0; // terminou com sucesso
}