#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	if ((logger = iniciar_logger()) == NULL )
	{
		perror("No se pudo crear log.");
		exit(EXIT_FAILURE);
	}
	log_info(logger,"Hola! Soy un log creado por MATIAS");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if ((config = iniciar_config()) == NULL)
	{
		perror("No se pudo Iniciar config.");
		exit(EXIT_FAILURE);
	}
	if (config_has_property(config,"IP") && config_has_property(config,"PUERTO") && config_has_property(config,"CLAVE"))
	{
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		valor = config_get_string_value(config,"CLAVE");
	}else
	{
		perror("Error con property en config.");
		exit(-1);
	}	
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	log_info(logger,"Lei ip %s, puerto %s, clave %s",ip,puerto,valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	enviar_mensaje(valor,conexion);
	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log","TP0",1,LOG_LEVEL_INFO);	
	
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("./cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while (1)
	{

		leido = readline("> ");
		if (!leido)
		{
			break;
		}
		if (leido)		
		{
			log_info(logger,"%s\n",(leido));
		}
		if (!strncmp(leido,"exit",4))
		{
			free(leido);
			break;
		}
		
		
	}
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	while (1)
	{
		leido = readline("> ");
		if (strlen(leido) == 0)
			{
				break;
			}
		if (leido)		
			{
				agregar_a_paquete(paquete,leido,strlen(leido)+1);
			}
		if (!strncmp(leido,"exit",4))
			{
				enviar_paquete(paquete,conexion);
				eliminar_paquete(paquete);
				free(leido);
				liberar_conexion(conexion);
				break;
			}
	}
	

	// Leemos y esta vez agregamos las lineas al paquete
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
}
