#include <socketUtil.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

struct usuarioAceitado
{
    int socketUsuario;
    struct sockaddr_in endereco;
    int erro;
    bool aceitado;
};

struct usuarioAceitado* aceitarUsuario(int serverSocket);
void recebirImprimirMensagem(int socketCliente);
void comecarAceitarUsuarios(int serverSocket);
void aceitarNovoUsuario(int serverSocket);
void usuarioNoOutroThread(struct usuarioAceitado* usuario);
void enviarMensagemRecebida(char* buffer, int socket);

// para multiplos usuarios

struct usuarioAceitado usuariosAceitados[10];
int contadorUsuarios = 0;


int main()
{
    int serverSocket = socketTCP();

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(serverSocket);
        return -1;
    }

    struct sockaddr_in* serverAddress = enderecoIP("", 3000);


    int resultado = bind(serverSocket, serverAddress, sizeof(struct sockaddr_in));

    if(resultado == 0)
    {
        printf("Server encendido.\n");
    }else
    {
        printf("A porta está ocupada.\n");
        close(serverSocket);
        return -1;
    }


    int listenSource = listen(serverSocket, 10);
    
    
    comecarAceitarUsuarios(serverSocket);
    
    shutdown(serverSocket, SHUT_RDWR);
    return 0;
}

void comecarAceitarUsuarios(int serverSocket)
{
    while (true)
    {
        struct usuarioAceitado* novoCliente = aceitarUsuario(serverSocket);

        usuariosAceitados[contadorUsuarios++] = *novoCliente;
        usuarioNoOutroThread(novoCliente);    
    }
}

void usuarioNoOutroThread(struct usuarioAceitado* usuario)
{
    pthread_t id;
    pthread_create(&id, NULL, recebirImprimirMensagem, usuario->socketUsuario);
}

struct usuarioAceitado* aceitarUsuario(int serverSocket)
{
    struct sockaddr_in enderecoCliente;
    int tamanhoEnderecoCliente = sizeof(enderecoCliente);
    int clienteSocket = accept(serverSocket, &enderecoCliente, &tamanhoEnderecoCliente);

    struct usuarioAceitado* usuario = malloc(sizeof(struct usuarioAceitado));
    usuario->endereco = enderecoCliente;
    usuario->socketUsuario = clienteSocket;
    usuario->aceitado = clienteSocket>0;

    if(!usuario->aceitado)
    {
        usuario->erro = clienteSocket;
    }
    
    return usuario;
}

void recebirImprimirMensagem(int socketCliente)
{
    char buffer[1024];
    
    while(true)
    {
        ssize_t contadorMensagemRecebida = recv(socketCliente, buffer, 1024, 0);

        if(contadorMensagemRecebida>0)
        {
            buffer[contadorMensagemRecebida] = 0;
            printf("%s\n", buffer);
            enviarMensagemRecebida(buffer, socketCliente);
        }

        if(contadorMensagemRecebida==0)
        {
            break;
        }
    }
    close(socketCliente);
}

void enviarMensagemRecebida(char* buffer, int socket)
{
    for(int i = 0; i < contadorUsuarios; i++)
    {
        if(usuariosAceitados[i].socketUsuario != socket)
        {
            send(usuariosAceitados[i].socketUsuario, buffer, 1024, 0);
        }
    }
}