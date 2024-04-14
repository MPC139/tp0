#include"utils.h"

t_log* logger;
static int addrlen;

int iniciar_servidor(void)
{
    int server_fd;
    struct sockaddr_in server_addr;
    int opt = 1;
    addrlen = sizeof(server_addr);

	// Create socket 
    if ((server_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("Socket failed.");
		log_error(logger,"Socket failed.");
        exit(EXIT_FAILURE);
    }

	// Set socket options
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1){
        perror("setsockopt failed.");
		log_error(logger,"setsockopt failed.");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))< 0)
    {
        perror("bind failed.");
		log_error(logger,"bind failed.");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd,MAX_CLIENTS) < 0)
    {
        perror("listen failed.");
		log_error(logger,"listen failed.");
        exit(EXIT_FAILURE);
    }

	log_trace(logger, "Listo para escuchar a mi cliente\n");
	log_info(logger,"Server listening on port %d\n",PORT);

	return server_fd;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente;
    struct sockaddr_in client_addr;
	char client_ip[INET_ADDRSTRLEN];
	


	if ((socket_cliente = accept(socket_servidor,(struct sockaddr *)&client_addr,(socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed.");
			log_error(logger,"accept failed.");
            exit(EXIT_FAILURE);
        }


	// Convert client address to string
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	printf("Client connected from IP: %s\n", client_ip);

	log_info(logger, "Se conecto un cliente!");
	log_info(logger, "IP %s",client_ip);


	// Send response to client
	log_info(logger,"Sending response to client...\n");
	char * message = "Hello from server Linux.";
	
	if ((send(socket_cliente,message,strlen(message),0)) == -1)
	{

		perror("send message failed.");
		log_error(logger,"send message failed.");
		exit(EXIT_FAILURE);		
	}
	
	log_info(logger,"Response sent to client\n");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
