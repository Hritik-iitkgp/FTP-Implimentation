#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
int main(){
 
  char ip[15] ;
  int port;
  int no_of_req;
  int time_interval;
  printf("Enter the ip address as like 127.0.0.1: ");
  scanf("%s",ip);
  printf("Enter the port number: ");
  scanf("%d",&port);
  printf("Enter the number of requests: ");
  scanf("%d",&no_of_req);
  printf("Enter the time interval in s: ");
  scanf("%d",&time_interval);
  
  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  
  int n;
 struct timeval start, end;
 // send a number of request
for(int i=0;i<no_of_req;i++){
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
 memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);
  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
 
  char buffer[1024];
  bzero(buffer, 1024);
  strcpy(buffer, "ping to server");
  //printf("Enter the message to send :- ");
  //scanf("%[^\n]s",buffer);
  printf("Client: %s\n", buffer);
  gettimeofday(&start, NULL);
  send(sock, buffer, strlen(buffer), 0);
  recv(sock, buffer, sizeof(buffer), 0);
  gettimeofday(&end, NULL);
  send(sock, buffer, strlen(buffer), 0);
  printf("Server: %s\n", buffer);
  float rtt = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("RTT: %f ms\n\n", rtt/1000);
  // wait for time interval for next request
  sleep(time_interval);
 }
  
  close(sock);
  printf("Disconnected from the server.\n");
  
  return 0;
}

