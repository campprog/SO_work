FROM ubuntu:latest

RUN apt-get update && apt-get install -y gcc

WORKDIR /app

COPY . .

RUN gcc interpretador.c -o interpretador

RUN gcc AddDestination.c -o AddDestination
RUN gcc apaga.c -o apaga
RUN gcc conta.c -o conta
RUN gcc CopyFile.c -o CopyFile
RUN gcc Info.c -o Info
RUN gcc ListFiles.c -o ListFiles
RUN gcc procura.c -o procura
RUN gcc Show.c -o Show

ENTRYPOINT ["./interpretador"]