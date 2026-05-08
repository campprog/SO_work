#include <unistd.h>   
#include <errno.h>    

int main(int argc, char *argv[]) {

    if (argc != 2) { //número de argumentos passados ao programa for diferente de 2. Implica ser 2(programa + ficheiro)
        write(2, "Uso: apaga <ficheiro>\n", 22); // escreve mensagem de erro
        return 1; // termina o programa com erro
    }

    
    if (unlink(argv[1]) < 0) {
        // tenta apagar o ficheiro indicado em argv[1]
        // se falhar, retorna erro

        
        if (errno == ENOENT) {
            // verifica se o ficheiro não existe
            write(2, "Aviso: ficheiro nao existe\n", 28);
            return 0;
        } else {
            // qualquer outro erro
            write(2, "Erro ao apagar ficheiro\n", 24);
            return 1; // termina com erro
        }
    }

    
    return 0; //o ficheiro foi apagado com sucesso
}