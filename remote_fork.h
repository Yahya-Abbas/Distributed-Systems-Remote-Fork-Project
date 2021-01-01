#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <assert.h>

#include "dmtcp.h"

#define INTS_PER_LOOP 5
#define SIZE 1024

using namespace std;


bool has_suffix(const string& s, const string& suffix)
{
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());    
}


void send_file(FILE *fp, int sockfd) {
    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int write_file(int recieve_sockfd){

    int n, fp;
    string filename = "node_restart.dmtcp";
    char buffer[SIZE];
	
    fp = open(filename.c_str(), O_CREAT | O_WRONLY,S_IRUSR|S_IWUSR);
    
    do {
        n = read(recieve_sockfd, buffer, SIZE);
        if (n <= 0){
            break;
        }
        write(fp, buffer, n);
        bzero(buffer, SIZE);
    } while(n>0);
    
    close(fp);
    return 1;
}

void restart() {
    system("dmtcp_restart -p 0 node_restart.dmtcp");
}

int myfork(int ip) {

//  Checkpoint the calling process
	int original_generation, rr;
    if(dmtcp_is_enabled()){
        printf("DMTCP Coordinator is enabled\n");
        original_generation = dmtcp_get_generation();
    }else{
        printf("DMTCP Coordinator not enabled!\n");
    }
    if(dmtcp_is_enabled()){
        printf("\n");
        
        rr = dmtcp_checkpoint();
        if(rr == DMTCP_NOT_PRESENT){
            printf("***** Error, DMTCP not running; checkpoint skipped ***** \n");
        }
        
        if(rr == DMTCP_AFTER_CHECKPOINT){
            printf("***** after checkpoint *****\n");
            printf("I'm The Parent Node\n");
   			// Wait long enough for checkpoint request to be written out.
    		while (dmtcp_get_generation() == original_generation) {
      			sleep(1);
    		}
            
            //  Send the checkpoint file to the child node
            string filename;
            
            DIR *dir = opendir(".");
    		if(!dir){
        		printf("NO directory Found");
    		}
			else{
				dirent *entry;
				const char* name;
    			while((entry = readdir(dir)) != NULL){
    				;
        			if(has_suffix(entry->d_name, ".dmtcp")){
            			printf("%s\n", entry->d_name);
            			filename = string(entry->d_name);
        			}
    			}
				closedir(dir);
			}
 			
 			fstream file;
 			FILE *fp;
    		if (fp = fopen(filename.c_str(), "r")){
    			fclose(fp);
    			file.open(filename.c_str(), ios::in | ios::binary);
    			if (file.is_open()){
                	cout << "[LOG] : File is ready to Transmit.\n";
            	}
            	else
            	{
                	cout << "[ERROR] : File loading failed, Exititng.\n";
                	exit(EXIT_FAILURE);
            	}
            	string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            	cout << "[LOG] : Transmission Data Size " << contents.length() << " Bytes.\n";
            	
            	cout << "[LOG] : Sending...\n";
            	
            	int bytes_sent = send(ip, contents.c_str(), contents.length(), 0);
            	cout << "[LOG] : Transmitted Data Size " << bytes_sent << " Bytes.\n";
            	
            	cout << "[LOG] : File Transfer Complete.\n";
            	close(ip);
    		}
    		file.close();
			
    		printf("[+]File data sent successfully.\n");
    		system("dmtcp_command --quit");
    		exit(1);
    		
        }
        if(rr == DMTCP_AFTER_RESTART){
            printf("***** after restart *****\n");
            printf("I'm The Child Node\n");
        }
    }
    else{
        printf(" dmtcp disabled -- nevermind\n");
    }
    
    return rr;
}

