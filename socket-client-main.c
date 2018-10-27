#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFFER_SIZE 50

SOCKET conexao;
struct sockaddr_in server_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];

void registra_winsock() { // Inicializa o uso da DLL Winsock
	WSADATA wsadata;
	if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    	printf("Winsock Error\n");
    	exit(1);
	}
}

void cria_socket_conexao() {
	conexao = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if (conexao < 0) {
		printf("Socket Error\n");
		exit(1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor
	server_address.sin_port = htons(33000); // Número da porta
}

void abre_conexao() {
	int len = sizeof(server_address);
	int result = connect(conexao, (struct sockaddr *)&server_address, len);
	if (result < 0) {
		printf("Connect Error");
		exit(1);
	}
}

void envia_mensagem() {
	send(conexao, mensagem, BUFFER_SIZE, 0);
}

void le_resposta() {
	recv(conexao, resposta, BUFFER_SIZE, 0);
}

void fecha_conexao() {
	closesocket(conexao);
}

void transmite() {
	cria_socket_conexao();
	abre_conexao();
	envia_mensagem();
	le_resposta();
	fecha_conexao();
}

void opc_par_ou_impar() {
	int numero;
	char numerostr[10];
	printf("Numero: ");
	scanf("%i", &numero);
	itoa(numero, numerostr, 10); // 10 = Base 10
	strcpy(mensagem, "PAR-OU-IMPAR ");
	strcpy(&mensagem[13], numerostr);
	transmite();
	printf("%s\n", resposta);
}

void opc_dobro() {
	int numero;
	char numerostr[10];
	printf("Numero: ");
	scanf("%i", &numero);
	itoa(numero, numerostr, 10); // 10 = Base 10
	strcpy(mensagem, "DOBRO ");
	strcpy(&mensagem[6], numerostr);
	transmite();
	printf("%s\n", resposta);
}

int main(int argc, char *argv[]) {
	registra_winsock();
	while (1) {
		printf("1. Informa se e par ou impar\n");
		printf("2. Calcula o dobro\n");
		printf("3. Sair\n");
		printf("Opcao: ");
		int opc;
		scanf("%i", &opc);
		switch (opc) {
			case 1:
				opc_par_ou_impar();
				break;
			case 2:
				opc_dobro();
				break;
			case 3:
				return 0;
		}
	}
}

