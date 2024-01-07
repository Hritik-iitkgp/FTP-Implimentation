#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>
void *clientmanager(void *arg){
int client_sock=*(int *) arg;
char buffer[1024];
struct timeval start, end;
struct sockaddr_in server_addr, client_addr;
socklen_t addr_size;
addr_size = sizeof(client_addr);
int logFile = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (logFile == -1) {
        perror("Error opening log file");
        exit(1);
    }

    flock(logFile, LOCK_EX);
    FILE* fp = fdopen(logFile, "a");
    bzero(buffer, 1024);
    //receive the request
    
    recv(client_sock, buffer, sizeof(buffer), 0);
    gettimeofday(&start, NULL);
    send(client_sock, buffer, strlen(buffer), 0);
    recv(client_sock, buffer, sizeof(buffer), 0);
    gettimeofday(&end, NULL);
    // compute the rtt
    float rtt = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    // log the client ip , port and rtt
    getpeername(client_sock,(struct sockaddr*)&client_addr, &addr_size);
    fprintf(fp,"Client Ip Address: %s\n",inet_ntoa(client_addr.sin_addr));
    fprintf(fp,"Client Port No: %d\n",client_addr.sin_port);
    fprintf(fp,"Round Trip Time in ms = %f ms\n\n",rtt/1000);
    close(client_sock);
    printf("[+]Client disconnected.\n\n");
    fclose(fp);
    pthread_exit(NULL);

}
int main(){
 
  char *ip = "127.0.0.1";
  int port = 5596;
  
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  struct timeval start, end;
  socklen_t addr_size;
  char buffer[1024];
  int n;
 
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
 
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
 
  listen(server_sock, 5);
  printf("Listening...\n");
 
  while(1){
  

    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");
 
    pthread_t pid;
    
    if(pthread_create(&pid ,NULL,clientmanager,&client_sock)!=0){
    perror("Error");
    close(client_sock);
    }
    else{
    pthread_detach(pid);
    }
  }
  
  return 0;
}
