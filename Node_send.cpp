#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "remote_fork.h"

using namespace std;

int main() {
	string send_ip = "127.0.0.1";
  	//string send_ip = "10.65.192.223";
  	int send_port = 8050;
	
  	int e;
	int want_to_fork = 1;
	int pid;
	
	//Sender
	if(want_to_fork){
		int send_sockfd;
  		struct sockaddr_in sender_addr;
  	
  		send_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  		if(send_sockfd < 0) {
    		perror("[-]Error in Sender socket");
    		exit(1);
  		}
  		printf("[+]Sender socket created successfully.\n");

  		sender_addr.sin_family = AF_INET;
  		sender_addr.sin_port = send_port;
  		sender_addr.sin_addr.s_addr = inet_addr(send_ip.c_str());

  		e = connect(send_sockfd, (struct sockaddr*)&sender_addr, sizeof(sender_addr));
  		if(e == -1) {
    		perror("[-]Error Sender in socket");
    		exit(1);
  		}
 		printf("[+]Connected to Sender.\n");
  		
  		//driver
  		printf("I'M In Parent node");
		if(want_to_fork){
			pid = myfork(send_sockfd);
		}
  
  		printf("[+]Closing the connection.\n");
  		close(send_sockfd);
	}
	
	return 0;
}


