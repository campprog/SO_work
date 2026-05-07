# Stage 1
FROM alpine:latest AS builder

# Instala o compilador C para o Alpine
RUN apk add --no-cache gcc musl-dev

WORKDIR /app
COPY . .

# Compila tudo de uma vez
RUN gcc interpretador.c -o interpretador && \
    gcc AddDestination.c -o AddDestination && \
    gcc apaga.c -o apaga && \
    gcc conta.c -o conta && \
    gcc CopyFile.c -o CopyFile && \
    gcc Info.c -o Info && \
    gcc ListFiles.c -o ListFiles && \
    gcc procura.c -o procura && \
    gcc Show.c -o Show

# Stage 2
FROM alpine:latest

WORKDIR /app

COPY --from=builder /app/interpretador .
COPY --from=builder /app/AddDestination .
COPY --from=builder /app/apaga .
COPY --from=builder /app/conta .
COPY --from=builder /app/CopyFile .
COPY --from=builder /app/Info .
COPY --from=builder /app/ListFiles .
COPY --from=builder /app/procura .
COPY --from=builder /app/Show .

ENV PATH="/app:${PATH}"

ENTRYPOINT ["./interpretador"]