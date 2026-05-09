#include <unistd.h>   // para usar read, write, close
#include <fcntl.h>    // para usar open
#include <string.h>   // para usar strstr (procurar texto dentro de texto)

#define TAMANHO_BLOCO 1024   // tamanho do bloco de leitura

// função simples para imprimir números (copiada do conta)
void escrever_inteiro(int numero) {
    char digitos[20];   // vetor para guardar os dígitos do número
    int indice = 0;     // contador

    if (numero == 0) {  // caso especial se o número for 0
        write(1, "0", 1);
        return;
    }

    // transformar o número em caracteres (ao contrário)
    while (numero > 0) {
        digitos[indice++] = (numero % 10) + '0';  // obtém o último dígito
        numero /= 10;                             // remove o último dígito
    }

    // escreve os dígitos na ordem correta
    while (indice--) write(1, &digitos[indice], 1);
}

int main(int numero_argumentos, char *argumentos[]) {
    // verificar se o número de argumentos está correto
    if (numero_argumentos != 3) {
        write(2, "Uso: procura <ficheiro> <padrao>\n", sizeof("Uso: procura <ficheiro> <padrao>\n") - 1);
        return 1;
    }

    // abrir o ficheiro em modo leitura
    int descritor_ficheiro = open(argumentos[1], O_RDONLY);

    // verificar se houve erro ao abrir
    if (descritor_ficheiro < 0) {
        write(2, "Erro: ficheiro nao existe\n", sizeof("Erro: ficheiro nao existe\n") - 1);
        return 1;
    }

    char bloco[TAMANHO_BLOCO];    // bloco para ler do ficheiro
    int bytes_lidos;              // número de bytes lidos

    char linha[1024];     // guardar uma linha completa
    int posicao = 0;      // posição atual na linha
    int numero_linha = 1; // contador de linhas
    int encontrou = 0;    // indicador para saber se encontrou alguma coisa

    // ler o ficheiro em blocos
    while ((bytes_lidos = read(descritor_ficheiro, bloco, TAMANHO_BLOCO)) > 0) {
        for (int indice = 0; indice < bytes_lidos; indice++) {
            char caracter = bloco[indice];  // carácter atual

            linha[posicao++] = caracter;     // adicionar à linha

            // se encontrar fim de linha
            if (caracter == '\n') {
                linha[posicao] = '\0';  // terminar texto

                // ver se a linha contém o padrão
                if (strstr(linha, argumentos[2]) != NULL) {
                    encontrou = 1;

                    // imprimir linha encontrada
                    write(1, "Linha ", 6);
                    escrever_inteiro(numero_linha);
                    write(1, ": ", 2);
                    write(1, linha, posicao);
                }

                posicao = 0;       // reinício da linha
                numero_linha++;    // próxima linha
            }
        }
    }

    // tratar última linha (caso não termine com \n)
    if (posicao > 0) {
        linha[posicao] = '\0';

        if (strstr(linha, argumentos[2]) != NULL) {
            encontrou = 1;

            write(1, "Linha ", 6);
            escrever_inteiro(numero_linha);
            write(1, ": ", 2);
            write(1, linha, posicao);
            write(1, "\n", 1);
        }
    }

    // se não encontrou nada
    if (!encontrou) {
        write(1, "Nenhuma linha encontrada\n", 25);
    }

    close(descritor_ficheiro);  // fechar o ficheiro

    return 0;
}
