#include <stdio.h>
#include <string.h>	//strlen
#include <stdlib.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write

#include <pthread.h> //for threading , link with lpthread
#include <wiringPi.h>

#define RF1 21
#define RF2 22
#define RF3 23
#define RF4 24
#define ON 0
#define OFF 1


void *connection_handler(void *);
int rfswitch(int c);
int daemone(void);
int mode;

int main(void)
{

         mode = 0;
         daemone();

	return(0);
}

int daemone()
{
        int socket_desc , new_socket , c , *new_sock;
        struct sockaddr_in server , client;
        char *message;

  wiringPiSetup () ;

      pinMode(RF1, OUTPUT);
      pinMode(RF2, OUTPUT);
      pinMode(RF3, OUTPUT);
      pinMode(RF4, OUTPUT);
      rfswitch(0);


        //Create socket
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1)
        {
                printf("Could not create socket");
        }

        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 9876 );

        //Bind
        if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        {
                return 1;
        }

        //Listen
        listen(socket_desc , 3);

        //Accept and incoming connection
        c = sizeof(struct sockaddr_in);
        while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
        {

                pthread_t sniffer_thread;
                new_sock = malloc(1);
                *new_sock = new_socket;

                if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
                {
                        return 1;
                }

                //Now join the thread , so that we dont terminate before the thread
                //pthread_join( sniffer_thread , NULL);
        }

        if (new_socket<0)
        {
                return 1;
        }

        return 0;

}

void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[20];
         int c;

        message = "\0";
	while( (read_size = recv(sock , client_message , 20 , 0)) > 0 )
	{
		//Send the ststus back to client


            if(strcmp(client_message, "RF1") == 0) 
                {
                
               c = rfswitch(1);
                 message = "RF1 ON";
                }
             else if(strcmp(client_message, "RF2") == 0)
              {
               c = rfswitch(2);
                 message = "RF2 ON";
             }
             else if (strcmp(client_message, "RF3") == 0)
             {

               c = rfswitch(3);
                 message = "RF3 ON";
             }
            else if (strcmp(client_message, "RF4") == 0 )
             {

               c = rfswitch(4);
                 message = "RF4 ON";
             }
            else if (strcmp(client_message, "RF0") == 0)
             {

                c = rfswitch(0);
                 message = "OFF";
             }
            else if (strcmp(client_message, "RF5") == 0)
            {
                mode = 1;
                c = rfswitch(5);
                message = "RF2+P";
            }
            else if (strcmp(client_message, "RF6") == 0)
            {
                mode = 1;
                c = rfswitch(6);
                message = "RF3+P";
                // with preamp inline
            }
            else if (strcmp(client_message, "MODE?") == 0)
            {
                if (mode == 0)
                {
                    message = "N";
                }
                else
                {
                    message = "P";
                }

                //c = rfswitch(7);
                // with preamp inline
            }

            else if (strcmp(client_message, "RF?") == 0)
             {

                c = rfswitch(7);

                 switch (c)
                   {
                  case 0:
                    message = "OFF";
                    break;
          
                  case 1:
                    message = "RF1 ON";

                    break;
                  case 2:
                    message = "RF2 ON";
                    break;

                  case 3:
                    message = "RF3 ON";

                    break;
                  case 4:
                    message = "RF4 ON";
                    break;
                  case 5:
                      message = "RF2+P";
                      break;
                  case 6:
                      message = "RF3+P";
                      break;


                  }

             }


        write(sock , message , strlen(message));
        
		
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
                rfswitch(0);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
		rfswitch(0);
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}



int rfswitch (int c)
{

     switch (c)
       {
       case 0:
         digitalWrite(RF1, OFF);
         digitalWrite(RF2, OFF);
         digitalWrite(RF3, OFF);
         digitalWrite(RF4, OFF);
             break;
       case 1:
         digitalWrite(RF1, ON);
         digitalWrite(RF2, OFF);
         digitalWrite(RF3, OFF);
         digitalWrite(RF4, OFF);

             break;
       case 2:
         digitalWrite(RF2, ON);
         digitalWrite(RF3, OFF);
         digitalWrite(RF4, OFF);
         digitalWrite(RF1, OFF);

             break;
       case 3:
         digitalWrite(RF3, ON);
         digitalWrite(RF4, OFF);
         digitalWrite(RF1, OFF);
         digitalWrite(RF2, OFF);

             break;
       case 4:
         digitalWrite(RF4, ON);
         digitalWrite(RF1, OFF);
         digitalWrite(RF2, OFF);
         digitalWrite(RF3, OFF);

             break;
       case 5:
           digitalWrite(RF1, OFF);
           digitalWrite(RF2, ON);
           digitalWrite(RF3, OFF);
           digitalWrite(RF4, OFF);

           break;
       case 6:
           digitalWrite(RF1, OFF);
           digitalWrite(RF2, ON);
           digitalWrite(RF3, ON);
           digitalWrite(RF4, OFF);

           break;

        case 7:
         
            if (mode == 0)
            {
                if (digitalRead(RF1) == ON)
                {
                    return 1;
                }
                else if (digitalRead(RF2) == ON)
                {
                    return 2;
                }
                else if (digitalRead(RF3) == ON)
                {
                    return 3;
                }
                else if (digitalRead(RF4) == ON)
                {
                    return 4;
                }
            }
            if (mode == 1)
            {
                if (digitalRead(RF1) == ON)
                {
                    return 1;
                }
                else if ((digitalRead(RF2) == ON) && (digitalRead(RF3) == OFF))
                {
                    return 5;
                }
                else if ((digitalRead(RF2) == ON) && (digitalRead(RF3) == ON))
                {
                    return 6;
                }
            }

           
           
             break;

       }


}
