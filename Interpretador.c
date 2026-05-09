#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void analisar_comando(char *entrada, char *argumentos_comando[]);
void executar_simples(char *argumentos_comando[]);
void redirecionar_saida(char *argumentos_comando[], char *ficheiro);
void redirecionar_entrada(char *argumentos_comando[], char *ficheiro);
void executar_pipe(char *argumentos_primeiro[], char *argumentos_segundo[]);

int main()
{
    char entrada[256];

    while (1)
    {
        printf("%% ");
        fflush(stdout);

        if (fgets(entrada, sizeof(entrada), stdin) == NULL)
        {
            break;
        }

        // remove o fim de linha
        entrada[strcspn(entrada, "\n")] = 0;

        // termina o interpretador
        if (strcmp(entrada, "termina") == 0)
        {
            break;
        }

        // executa comandos ligados por canal
        if (strchr(entrada, '|'))
        {
            char *primeira_parte = strtok(entrada, "|");
            char *segunda_parte = strtok(NULL, "|");

            // limpar espaços
            while (*primeira_parte == ' ')
                primeira_parte++;
            while (*segunda_parte == ' ')
                segunda_parte++;

            char *argumentos_primeiro[20];
            char *argumentos_segundo[20];

            analisar_comando(primeira_parte, argumentos_primeiro);
            analisar_comando(segunda_parte, argumentos_segundo);

            executar_pipe(argumentos_primeiro, argumentos_segundo);
        }

        // executa comando com redirecionamento de saída
        else if (strchr(entrada, '>'))
        {
            char *parte_comando = strtok(entrada, ">");
            char *ficheiro = strtok(NULL, ">");

            while (*parte_comando == ' ')
                parte_comando++;
            while (*ficheiro == ' ')
                ficheiro++;

            char *argumentos_comando[20];
            analisar_comando(parte_comando, argumentos_comando);

            redirecionar_saida(argumentos_comando, ficheiro);
        }

        // executa comando com redirecionamento de entrada
        else if (strchr(entrada, '<'))
        {
            char *parte_comando = strtok(entrada, "<");
            char *ficheiro = strtok(NULL, "<");

            while (*parte_comando == ' ')
                parte_comando++;
            while (*ficheiro == ' ')
                ficheiro++;

            char *argumentos_comando[20];
            analisar_comando(parte_comando, argumentos_comando);

            redirecionar_entrada(argumentos_comando, ficheiro);
        }

        // executa comando simples
        else
        {
            char *argumentos_comando[20];
            analisar_comando(entrada, argumentos_comando);

            executar_simples(argumentos_comando);
        }
    }

    return 0;
}

void analisar_comando(char *entrada, char *argumentos_comando[])
{
    int indice = 0;

    // separa por espaços
    char *elemento = strtok(entrada, " ");

    while (elemento != NULL)
    {
        argumentos_comando[indice++] = elemento;
        elemento = strtok(NULL, " ");
    }

    argumentos_comando[indice] = NULL; // marca o fim da lista de argumentos
}

void executar_simples(char *argumentos_comando[])
{
    pid_t processo = fork();

    if (processo == -1)
    {
        perror("Erro no fork");
        return;
    }

    if (processo == 0)
    {
        // processo filho
        execvp(argumentos_comando[0], argumentos_comando);

        // só chega aqui se der erro
        perror("Erro ao executar");
        exit(1);
    }
    else
    {
        // processo pai
        int estado;
        wait(&estado);

        if (WIFEXITED(estado))
        {
            printf("\n");
            printf("Terminou comando com código %d\n", WEXITSTATUS(estado));
        }
        else
        {
            printf("\n");
            printf("Comando terminou de forma anormal\n");
        }
    }
}

void redirecionar_saida(char *argumentos_comando[], char *ficheiro)
{
    pid_t processo = fork();

    if (processo == -1)
    {
        perror("Erro no fork");
        return;
    }

    if (processo == 0)
    {
        // processo filho

        int descritor_ficheiro = open(ficheiro, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (descritor_ficheiro < 0)
        {
            perror("Erro a abrir ficheiro");
            exit(1);
        }

        dup2(descritor_ficheiro, STDOUT_FILENO); // saída padrão para ficheiro
        close(descritor_ficheiro);

        execvp(argumentos_comando[0], argumentos_comando);
        perror("Erro ao executar");
        exit(1);
    }
    else
    {
        // processo pai
        int estado;
        wait(&estado);

        if (WIFEXITED(estado))
        {
            printf("\n");
            printf("Terminou comando com código %d\n", WEXITSTATUS(estado));
        }
        else
        {
            printf("\n");
            printf("Comando terminou de forma anormal\n");
        }
    }
}

void redirecionar_entrada(char *argumentos_comando[], char *ficheiro)
{
    pid_t processo = fork();

    if (processo == -1)
    {
        perror("Erro no fork");
        return;
    }

    if (processo == 0)
    {
        // processo filho

        int descritor_ficheiro = open(ficheiro, O_RDONLY);
        if (descritor_ficheiro < 0)
        {
            perror("Erro a abrir ficheiro");
            exit(1);
        }

        dup2(descritor_ficheiro, STDIN_FILENO); // entrada padrão a partir do ficheiro
        close(descritor_ficheiro);

        execvp(argumentos_comando[0], argumentos_comando);
        perror("Erro ao executar");
        exit(1);
    }
    else
    {
        // processo pai
        int estado;
        wait(&estado);

        if (WIFEXITED(estado))
        {
            printf("\n");
            printf("Terminou comando com código %d\n", WEXITSTATUS(estado));
        }
        else
        {
            printf("\n");
            printf("Comando terminou de forma anormal\n");
        }
    }
}

void executar_pipe(char *argumentos_primeiro[], char *argumentos_segundo[])
{
    int descritores_canal[2];

    if (pipe(descritores_canal) == -1)
    {
        perror("Erro no canal");
        return;
    }

    pid_t primeiro_processo = fork();

    if (primeiro_processo == 0)
    {
        // primeiro filho escreve no canal

        dup2(descritores_canal[1], STDOUT_FILENO); // saída padrão para o canal
        close(descritores_canal[0]);               // não usa leitura
        close(descritores_canal[1]);

        execvp(argumentos_primeiro[0], argumentos_primeiro);
        perror("Erro ao executar 1");
        exit(1);
    }

    pid_t segundo_processo = fork();

    if (segundo_processo == 0)
    {
        // segundo filho lê do canal

        dup2(descritores_canal[0], STDIN_FILENO); // entrada padrão a partir do canal
        close(descritores_canal[1]);              // não usa escrita
        close(descritores_canal[0]);

        execvp(argumentos_segundo[0], argumentos_segundo);
        perror("Erro ao executar 2");
        exit(1);
    }

    // processo pai
    close(descritores_canal[0]);
    close(descritores_canal[1]);

    int estado_primeiro;
    int estado_segundo;
    wait(&estado_primeiro);
    wait(&estado_segundo);
    if (WIFEXITED(estado_segundo))
    {
        printf("\n");
        printf("Terminou encadeamento com código %d\n", WEXITSTATUS(estado_segundo));
    }
    else
    {
        printf("\n");
        printf("Comando terminou de forma anormal\n");
    }
}
