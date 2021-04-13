#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>		// inet_ntoa
#include <time.h> 			// time
//======================================================================
int error( int ern, const char *msg ){
    perror( msg );
    return ern;
}
//======================================================================
int main( void ) {
	
	/* zmienne predefiniowane *****************************************/
	int  port   = 12345;
	
	/* zmienne niezainicjowane ****************************************/		
	int 		fd;
	char 		buffer[256];
	socklen_t 	socklen;
	char 		hex[200];
	
	struct 	sockaddr_in serv_addr, cli_addr;
	
	/* tworzymy gniazdo ***********************************************/
	fd = socket( AF_INET, SOCK_DGRAM, 0 );		// tworzymy nowe gniazdo
	if (fd < 0) 
        return error( 1, "socket()" );
        
    /* zapelniamy strukture zerami (inicjujemy) ***********************/
	bzero( ( char * )&serv_addr, sizeof(serv_addr));	// zapelniamy strukture zerami
	
	/* przypisujemy parametry *****************************************/
	serv_addr.sin_family 	  	= AF_INET;			// typ gniazda 
	serv_addr.sin_addr.s_addr 	= INADDR_ANY;		// oczekujemy polaczen na kazdym adresie
	serv_addr.sin_port 			= htons(port);		// port, na ktorym nasluchujemy
	
	/* mapujemy gniazdo ***********************************************/
	if( bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
		return error( 2, "bind()" );
	
	/* rozpoczynamy nasluchiwanie na gniezdzie ************************/
	listen( fd, 5 );
	
	/* kod obslugujacy nowe polaczenia ********************************/
	socklen = sizeof(cli_addr);
	
	while(1) 
	{
			
			bzero(buffer, 256);

			/* wypisujemy informacje o polaczeniu *********************/
			
			recvfrom(fd, (char *)buffer, 255, MSG_WAITALL, (struct sockaddr *)&cli_addr, &socklen);

			printf("Address: %s Port: %d\n",  inet_ntoa( cli_addr.sin_addr ), cli_addr.sin_port );

			pid_t PID = fork();

			char pid_number [6] = "";
			char msg[256];

			if ( PID == 0)  
			{					
				 for (int i = 0; i < 4; i++) 
				 {
				 	pid_number[i] = buffer[i];
				 }
				 printf("PID number: %s\n",pid_number);
				 for (int i = 0, j = 4; i < strlen(buffer); i++, j++)
				 	{
				 		msg[i]=buffer[j];
				 	}
				printf("Message received \n");
				FILE *file = fopen(pid_number, "w+");

				if (file != NULL) 
				{
					int len = strlen(msg);
					for (int i = 0, j = 0; i < len; ++i, j += 2)
					{
						sprintf(hex + j, "%02x", msg[i] & 0xff);
					}

					printf("File created \n");
					fputs(hex, file);
					fclose(file);
				}
				else
				{
					printf("error");
				}
			}
					
	}
	 /* zamykamy gniazdo **********************************************/
	close(fd);
	
	//------------------------------------------------------------------
	return 0; 
}
//======================================================================