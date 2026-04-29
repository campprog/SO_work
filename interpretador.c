#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parse_comando(char *input, char *args[]);
void executar_simples(char *args[]);
void redirecionar_saida(char *args[], char *ficheiro);
void redirecionar_entrada(char *args[], char *ficheiro);
void executar_pipe(char *args1[], char *args2[]);

int main() {
    char input[256];

    while (1) {
        printf("%% ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // remover \n
        input[strcspn(input, "\n")] = 0;

        // terminar
        if (strcmp(input, "termina") == 0) {
            break;
        }

        // PIPE
        if (strchr(input, '|')) {
            char *parte1 = strtok(input, "|");
            char *parte2 = strtok(NULL, "|");

            // limpar espaços
            while (*parte1 == ' ') parte1++;
            while (*parte2 == ' ') parte2++;

            char *args1[20];
            char *args2[20];

            parse_comando(parte1, args1);
            parse_comando(parte2, args2);

            executar_pipe(args1, args2);
        }

        // REDIRECIONAMENTO SAÍDA >
        else if (strchr(input, '>')) {
            char *parte1 = strtok(input, ">");
            char *ficheiro = strtok(NULL, ">");

            while (*parte1 == ' ') parte1++;
            while (*ficheiro == ' ') ficheiro++;

            char *args[20];
            parse_comando(parte1, args);

            redirecionar_saida(args, ficheiro);
        }

        // REDIRECIONAMENTO ENTRADA <
        else if (strchr(input, '<')) {
            char *parte1 = strtok(input, "<");
            char *ficheiro = strtok(NULL, "<");

            while (*parte1 == ' ') parte1++;
            while (*ficheiro == ' ') ficheiro++;

            char *args[20];
            parse_comando(parte1, args);

            redirecionar_entrada(args, ficheiro);
        }

        // COMANDO NORMAL
        else {
            char *args[20];
            parse_comando(input, args);

            executar_simples(args);
        }
    }

    return 0;
}

void parse_comando(char *input, char *args[]) {
    int i = 0;

    // separa por espaços
    char *token = strtok(input, " ");

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL; // MUITO IMPORTANTE
}

void executar_simples(char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Erro no fork");
        return;
    }

    if (pid == 0) {
        // FILHO
        execvp(args[0], args);

        // só chega aqui se der erro
        perror("Erro no exec");
        exit(1);
    } else {
        // PAI
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Terminou comando com código %d\n", WEXITSTATUS(status));
        } else {
            printf("Comando terminou de forma anormal\n");
        }
    }
}

void redirecionar_saida(char *args[], char *ficheiro) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Erro no fork");
        return;
    }

    if (pid == 0) {
        // FILHO

        int fd = open(ficheiro, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Erro a abrir ficheiro");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO); // stdout → ficheiro
        close(fd);

        execvp(args[0], args);
        perror("Erro no exec");
        exit(1);
    } else {
        // PAI
        int status;
        wait(&status);
        printf("Terminou comando com código %d\n", WEXITSTATUS(status));
    }
}

void redirecionar_entrada(char *args[], char *ficheiro) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Erro no fork");
        return;
    }

    if (pid == 0) {
        // FILHO

        int fd = open(ficheiro, O_RDONLY);
        if (fd < 0) {
            perror("Erro a abrir ficheiro");
            exit(1);
        }

        dup2(fd, STDIN_FILENO); // stdin ← ficheiro
        close(fd);

        execvp(args[0], args);
        perror("Erro no exec");
        exit(1);
    } else {
        // PAI
        int status;
        wait(&status);
        printf("Terminou comando com código %d\n", WEXITSTATUS(status));
    }
}

void executar_pipe(char *args1[], char *args2[]) {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Erro no pipe");
        return;
    }

    pid_t pid1 = fork();

    if (pid1 == 0) {
        // FILHO 1 → escreve no pipe

        dup2(fd[1], STDOUT_FILENO); // stdout → pipe
        close(fd[0]); // não usa leitura
        close(fd[1]);

        execvp(args1[0], args1);
        perror("Erro exec 1");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) {
        // FILHO 2 → lê do pipe

        dup2(fd[0], STDIN_FILENO); // stdin ← pipe
        close(fd[1]); // não usa escrita
        close(fd[0]);

        execvp(args2[0], args2);
        perror("Erro exec 2");
        exit(1);
    }

    // PAI
    close(fd[0]);
    close(fd[1]);

    int status1, status2;
    wait(&status1);
    wait(&status2);

    printf("Terminou pipe com código %d\n", WEXITSTATUS(status2));
}