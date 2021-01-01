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
  	string recieve_ip = "127.0.0.1";
  	//string recieve_ip = "10.65.192.58";
	int recieve_port = 8051;
	
  	int e;
	
	//Reciever
	int recieve_sockfd, new_sock;
	struct sockaddr_in reciever_addr, new_addr;
	socklen_t addr_size;
  
  	recieve_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(recieve_sockfd < 0) {
		perror("[-]Error in socket");
   		exit(1);
  	}
  	printf("[+]Reciever socket created successfully.\n");

  	reciever_addr.sin_family = AF_INET;
  	reciever_addr.sin_port = recieve_port;
  	reciever_addr.sin_addr.s_addr = inet_addr(recieve_ip.c_str());

  	e = bind(recieve_sockfd, (struct sockaddr*)&reciever_addr, sizeof(reciever_addr));
  	if(e < 0) {
    	perror("[-]Error in Reciever binding");
    	exit(1);
  	}
  	printf("[+]Reciever Binding successfull.\n");

  	if(listen(recieve_sockfd, 10) == 0){
 		printf("[+]Reciever Listening....\n");
 	}
 	else{
 		perror("[-]Reciever Error in listening");
    	exit(1);
 	}
	
  	addr_size = sizeof(new_addr);
  	new_sock = accept(recieve_sockfd, (struct sockaddr*)&new_addr, &addr_size);
  	if(write_file(new_sock)){
  		printf("[+]Data written in the file successfully.\n");
  		restart();
  	}

	return 0;
}


