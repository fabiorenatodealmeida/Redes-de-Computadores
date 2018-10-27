#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFFER_SIZE 50
#define DEBUG

SOCKET escuta, conexao;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];

void registra_winsock() { // Inicializa o uso da DLL Winsock
	WSADATA wsadata;
	if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
		printf("Winsock Error\n");
		exit(1);
	}
}

void cria_socket_escuta() {
	escuta = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if (escuta < 0) {
		printf("Socket Error\n");
		exit(1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor
	server_address.sin_port = htons(33000); // Número da porta
	int len = sizeof(server_address);
	if (bind(escuta, (struct sockaddr *)&server_address, len) < 0) {
		printf("Bind Error\n");
		exit(1);
	}
	if (listen(escuta, 5) < 0) {
		printf("Listen Error\n");
		exit(1);
	}
}

void aguarda_conexao() {
	int len = sizeof(client_address);
	conexao = accept(escuta, (struct sockaddr *)&client_address, &len);
	if (conexao < 0) {
		printf("Accept Error\n");
		exit(1);
	}
}

void le_mensagem() {
	recv(conexao, mensagem, BUFFER_SIZE, 0);
}

void processa_PAR_OU_IMPAR() {
	// Mensagem: PAR-OU-IMPAR n
	// Respostas: PAR
	//            IMPAR
	//            ERRO
	char numerostr[10];
	strcpy(numerostr, &mensagem[13]);
	int numero = atoi(numerostr);
	if (numero % 2 == 0) {
		strcpy(resposta, "PAR");
	} else {
		strcpy(resposta, "IMPAR");
	}
}

void processa_DOBRO() {
	char numerostr[10];
	strcpy(numerostr, &mensagem[6]);
	int numero = atoi(numerostr);
	int dobro = numero * 2;
	char dobrostr[10];
	itoa(dobro, dobrostr, 10); // 10 = Base 10
	strcpy(resposta, dobrostr);
}

void processa() {
	#ifdef DEBUG
	printf("%s\n", mensagem);
	#endif
	char comando[20];
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		if (mensagem[i] == ' ') {
			comando[i] = '\0';
			break;
		} else {
			comando[i] = mensagem[i];
		}
	}
	if (strcmp(comando, "PAR-OU-IMPAR") == 0) processa_PAR_OU_IMPAR();
	else if (strcmp(comando, "DOBRO") == 0) processa_DOBRO();
	else {
		strcpy(resposta, "ERRO");
	}
}

void envia_resposta() {
	send(conexao, resposta, BUFFER_SIZE, 0);
}

void fecha_conexao() {
	closesocket(conexao);
}

int main(int argc, char *argv[]) {
	printf("Servidor...\n");
	registra_winsock();
	cria_socket_escuta();
	while (1) {
		aguarda_conexao();
		le_mensagem();
		processa(); // Trabalhar aqui...
		envia_resposta();
		fecha_conexao();
	}
	return 0;
}

