#include <unistd.h>
#include <fcntl.h>

#define TAMANHO_BLOCO 1024

void escrever_texto(char *texto)
{
    while (*texto)
        write(1, texto++, 1);
}

void escrever_inteiro(int numero)
{
    char digitos[20];
    int indice = 0;

    if (numero == 0)
    {
        write(1, "0", 1);
        return;
    }

    while (numero > 0)
    {
        digitos[indice++] = (numero % 10) + '0';
        numero /= 10;
    }

    while (indice--)
        write(1, &digitos[indice], 1);
}

int main(int numero_argumentos, char *argumentos[])
{
    // Por defeito, lê da entrada padrão (teclado ou redirecionamento <)
    int descritor_ficheiro = 0;

    // Se receber argumentos a mais, mostra erro
    if (numero_argumentos > 2)
    {
        write(2, "Uso: conta [ficheiro]\n", sizeof("Uso: conta [ficheiro]\n") - 1);
        return 1;
    }

    // Se receber o nome do ficheiro, abre esse ficheiro
    if (numero_argumentos == 2)
    {
        descritor_ficheiro = open(argumentos[1], O_RDONLY);

        if (descritor_ficheiro < 0)
        {
            write(2, "Erro: ficheiro nao existe\n", sizeof("Erro: ficheiro nao existe\n") - 1);
            return 1;
        }
    }

    char bloco[TAMANHO_BLOCO];
    int bytes_lidos;

    int linhas = 0, palavras = 0, caracteres = 0;
    int dentro_palavra = 0;

    while ((bytes_lidos = read(descritor_ficheiro, bloco, TAMANHO_BLOCO)) > 0)
    {
        for (int indice = 0; indice < bytes_lidos; indice++)
        {
            char caracter = bloco[indice];

            caracteres++;

            if (caracter == '\n')
                linhas++;

            if (caracter == ' ' || caracter == '\n' || caracter == '\t')
            {
                dentro_palavra = 0;
            }
            else if (!dentro_palavra)
            {
                palavras++;
                dentro_palavra = 1;
            }
        }
    }

    // Só fecha o descritor se tivermos aberto um ficheiro específico
    if (descritor_ficheiro != 0)
    {
        close(descritor_ficheiro);
    }

    escrever_texto("Linhas: ");
    escrever_inteiro(linhas);

    escrever_texto("\nPalavras: ");
    escrever_inteiro(palavras);

    escrever_texto("\nCaracteres: ");
    escrever_inteiro(caracteres);

    escrever_texto("\n");

    return 0;
}