// SOCKET PROGRAMMING

#include <socketUtil.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

void comencarEscutarImprimirMensagems(int socket);
void imprimirMensagem(int socket);

void comencarEscutarImprimirMensagems(int socket)
{
    pthread_t id;
    pthread_create(&id, NULL, imprimirMensagem, socket);
}

void imprimirMensagem(int socket)
{
    char buffer[1024];
    
    while(true)
    {
        ssize_t contadorMensagemRecebida = recv(socket, buffer, 1024, 0);

        if(contadorMensagemRecebida>0)
        {
            buffer[contadorMensagemRecebida] = 0;
            printf("\r%s\n[Voce]: ", buffer);
            fflush(stdout);
        }

        if(contadorMensagemRecebida==0)
        {
            break;
        }
    }
    close(socket);
}

int main ()
{
    const char* ip = "127.0.0.1";
    int socketFD = socketTCP();
   
    struct sockaddr_in* address = enderecoIP("127.0.0.1", 3000);
    
    if(address == NULL)
    {
        printf("ADDRESS ASSIGNATION FAILED");
    }
    
    int result = connect(socketFD, address, sizeof(struct sockaddr_in));
    
    if(result == 0)
    {
        printf("Connection was successful\n");
    }
    
    char* nome = NULL;
    size_t nomeTamanho = 0;
    printf("Digite seu nome: \n");
    ssize_t nomeContador = getline(&nome, &nomeTamanho, stdin);
    nome[nomeContador - 1] = 0;

    char* mensagem = NULL;
    size_t mensagemTamanho = 0;

    printf("Insera uma mensagem (digite 'exit' se quer sair): \n");

    comencarEscutarImprimirMensagems(socketFD);

    char buffer[1024];
    bool first = false;
    printf("\n");
    while(true)
    {
        printf("[Voce]: ");
        fflush(stdout);

        ssize_t contador = getline(&mensagem, &mensagemTamanho, stdin);

        mensagem[contador - 1] = 0;
        sprintf(buffer, "[%s]: %s", nome, mensagem);
        if(contador>0)
        {
            if(strcmp(mensagem,"exit")==0)
            {
                break;
            }
        }
        ssize_t enviar = send(socketFD, buffer, strlen(buffer), 0);
    }

    close(socketFD);
    return 0;
}