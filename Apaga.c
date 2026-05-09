#include <unistd.h>
#include <errno.h>

int main(int numero_argumentos, char *argumentos[]) {

    if (numero_argumentos != 2) { // verifica se foi indicado exatamente um ficheiro
        write(2, "Uso: apaga <ficheiro>\n", sizeof("Uso: apaga <ficheiro>\n") - 1); // escreve mensagem de erro
        return 1; // termina o programa com erro
    }


    if (unlink(argumentos[1]) < 0) {
        // tenta apagar o ficheiro indicado nos argumentos

        if (errno == ENOENT) {
            // verifica se o ficheiro não existe
            write(2, "Aviso: ficheiro nao existe\n", sizeof("Aviso: ficheiro nao existe\n") - 1);
            return 0;
        } else {
            // trata qualquer outro erro de remoção
            write(2, "Erro ao apagar ficheiro\n", sizeof("Erro ao apagar ficheiro\n") - 1);
            return 1; // termina com erro
        }
    }


    return 0; // o ficheiro foi apagado com sucesso
}
