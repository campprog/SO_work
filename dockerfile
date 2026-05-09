# Etapa 1
FROM alpine:latest AS construtor

# Instala o compilador C para o Alpine
RUN apk add --no-cache gcc musl-dev

WORKDIR /app
COPY . .

# Compila tudo de uma vez
RUN gcc Interpretador.c -o interpretador && \
    gcc Acrescenta.c -o acrescenta && \
    gcc Apaga.c -o apaga && \
    gcc Conta.c -o conta && \
    gcc Copia.c -o copia && \
    gcc Informa.c -o informa && \
    gcc Lista.c -o lista && \
    gcc Procura.c -o procura && \
    gcc Mostra.c -o mostra

# Etapa 2
FROM alpine:latest

WORKDIR /app

COPY --from=construtor /app/interpretador .
COPY --from=construtor /app/acrescenta .
COPY --from=construtor /app/apaga .
COPY --from=construtor /app/conta .
COPY --from=construtor /app/copia .
COPY --from=construtor /app/informa .
COPY --from=construtor /app/lista .
COPY --from=construtor /app/procura .
COPY --from=construtor /app/mostra .

ENV PATH="/app:${PATH}"

ENTRYPOINT ["./interpretador"]
