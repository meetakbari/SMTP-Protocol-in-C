/*
	Author : Meet Akbari (AU1841073)
			 Mayankkumar Tank (AU1841057)
	Course : Computer Networks | 5th semester 
	Organization : School of Engineering and Applied Science, Ahmedabad University, India.
	Project : SMTP(Simple Mail Transfer Protocol) Implementation in C using Socket Programming
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define domain "server.smtp.com"
char mail_from[100];
char mail_to[100];  

void send_image(int socket)
{
    FILE *fpimage;
	int size;
	int readsize;
	int status;
	int packet_index = 1;
	char send_buffer[10239];
	char read_buffer[256];
    char f_name[100];

    printf("\nEnter Image Path/Image file name : ");
    scanf("%s",f_name);

    fpimage = fopen(f_name, "r");
	printf("\nCLIENT: \nGetting Image Size...\n");  
	
    if(fpimage == NULL){
		printf("Error occured while opening Image File!\n");
	}
	
    fseek(fpimage, 0, SEEK_END);
	size = ftell(fpimage);
	fseek(fpimage, 0, SEEK_SET);
	printf("Total Image size: %i\n",size);
    
	printf("Sending Image Size\n");
	write(socket, (void *)&size, sizeof(int));

	printf("Sending Image as a byte array...");

    do{ 
	    status=read(socket, &read_buffer , 255);
	    //printf("Bytes read: %i\n",status);
	}while(status < 0);

    //printf("Data successfully received in socket!\n");
	//printf("Socket Data: %s\n", read_buffer);

    while(!feof(fpimage))
    {
        readsize = fread(send_buffer, 1, sizeof(send_buffer)-1, fpimage);

	    do{
		    status = write(socket, send_buffer, readsize);  
	    }while(status<0);

            //printf("Packet number: %i\n",packet_index);
	    //printf("Packet size sent: %i\n",readsize);     
	    printf(" \n");
	    printf(" \n");

        packet_index++;
        bzero(send_buffer, sizeof(send_buffer));
    }
}


char * Mail_Header(char *from,char *to,char *sub,char *content)
{
    time_t t;
    time(&t); 

    char *header = NULL;
    char date[26];
    char DATE_h[8 + strlen(date)];
    char sender[8 + strlen(from)];   //FROM: sender's_email\r\n
    char recep[6 + strlen(to)];      //TO: recepient's_email\r\n
    char subject[11 + strlen(sub)];
    char content_a[1 + strlen(content) + 2 + 1 + 1];
    strftime(date, (33), "%a %d %b %Y %H:%M:%S",localtime(&t));
    sprintf(DATE_h,"DATE: %s\r\n",date);
    sprintf(sender,"FROM: %s\r\n",from);
    sprintf(subject,"Subject: %s\r\n",sub);
    sprintf(recep,"TO: %s\r\n",to);
    //extra \n is used to end the header part

    sprintf(content_a,"%s\r\n",content);

    int header_length = strlen(DATE_h) + strlen(sender) + strlen(subject) + strlen(recep) + strlen(content_a);

    header = (char *) malloc(header_length * sizeof(char));

    memcpy(&header[0], &DATE_h, strlen(DATE_h));
    memcpy(&header[0 + strlen(DATE_h)], &sender , strlen(sender));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender)], &subject , strlen(subject));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender) + strlen(subject)], &recep , strlen(recep));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender) + strlen(subject) + strlen(recep)], &content_a , strlen(content_a));
    
    return header;
}


int main(int argc, char *argv[])
{
    int socket_id,n;
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char cname[256];
    char buff[10240];

    if(argc < 3)
    {
        perror("\nPlease enter the hostname and port number.\n");
        exit(0);
    }
    portno = atoi(argv[2]);

    socket_id = socket(AF_INET,SOCK_STREAM, 0);

    if(socket_id < 0)
    {
        perror("\nError occured while opening the socket!\n");
        exit(0);
    }
    server = gethostbyname(argv[1]);

    if(server == NULL)
    {
        perror("\nError: No such host found!\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    
    //connect to the server
    int connect_id;
    connect_id = connect(socket_id,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(connect_id < 0)
    {
        perror("Error occured while connecting to server...\n");
    }
       
    //Implementing commands which are used b/w Client and SMTP Server for communication
    do
    {
        printf("Enter the command : ");
        intake: gets(cname);
        //cname[strlen(cname) + 1] = '\0';
        
        char code[4];       //to store the 3 digit response code received from server
        
        if(strcasecmp(cname,"HELO") == 0)
        {
            bzero(buff,10240);
            strcpy(buff,"HELO ");
            strcat(buff, domain);
            strcat(buff,"\r\n");
            n = write(socket_id,buff,strlen(buff));
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //HELO domain
            bzero(buff,10240);
            n = read(socket_id,buff,10239);
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("SERVER : %s\n",buff);    //250 Hello domain

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
        }
        else if(strcasecmp(cname,"MAIL FROM") == 0)
        {
            bzero(buff,10240);
            printf("\nEnter Sender Email id : ");
            scanf("%s",mail_from);
            strcpy(buff,"MAIL FROM:<");
            strcat(buff,mail_from);
            strcat(buff,">");
            strcat(buff,"\r\n");
            n = write(socket_id,buff,strlen(buff));
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //MAIL FROM:<your email id>
            bzero(buff,10240);
            n = read(socket_id,buff,10239);
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("SERVER : %s\n",buff);  //250 OK

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
        }
        else if(strcasecmp(cname,"RCPT TO") == 0)
        {
            bzero(buff,10240);
            printf("\nEnter Recipient Email id : ");
            scanf("%s",mail_to);
            strcpy(buff,"RCPT TO:<");
            strcat(buff,mail_to);
            strcat(buff,">");
            strcat(buff,"\r\n");
            n = write(socket_id,buff,strlen(buff));
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //RCPT TO:<your email id>
            bzero(buff,10240);
            n = read(socket_id,buff,10239);
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("SERVER : %s\n",buff);  //250 OK

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
        }
        else if(strcasecmp(cname,"DATA") == 0)          
        {
            bzero(buff,10240);
            strcpy(buff,"DATA");
            strcat(buff,"\r\n");
            n = write(socket_id,buff,strlen(buff));
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //DATA
            bzero(buff,10240);
            n = read(socket_id,buff,10239);
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("SERVER : %s\n",buff);  //354 Send message content; end with <CRLF>.<CRLF>

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"354") == 0)
            {
                printf("\nReady to send header data!\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }

            //creating a mail header
            char sub[150];
            char content[450];
            printf("\nEnter Subject : ");
            scanf("%[^\n]",sub);
            
            printf("\nEnter content : (Press Tab and Enter Key to end)\n");
            scanf("%[^\t]",content);

            bzero(buff,10240);
            //Mail_header function declared above
            strcpy(buff, Mail_Header(mail_from,mail_to,sub,content));  //assigning header to buffer
            
            n = write(socket_id,buff,strlen(buff));                     
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : ====| Mail header & content |====\n%s\n",buff);  //header content
            
            bzero(buff,10240);
            strcpy(buff,".\r\n");
            n = write(socket_id,buff,strlen(buff));                    
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            
            bzero(buff,10240);
            n = read(socket_id,buff,10239);  
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            } 
            printf("SERVER : %s\n",buff);     //250 OK

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {	
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);                                  
        }
        else if(strcasecmp(cname,"ATTACHMENT") == 0){
            bzero(buff,10240);
			strcpy(buff,"Attachment");
			strcat(buff,"\r\n");
			n = write(socket_id,buff,strlen(buff));
			if(n < 0)
			{
			    printf("\nError occured while writing to socket!\n");
			}
			printf("\nCLIENT : %s",buff);  //DATA
			bzero(buff,10240);
			n = read(socket_id,buff,10239);
			if(n < 0)
			{
			    printf("\nError occured while reading from socket!\n");
			}
			printf("SERVER : %s\n",buff);  //420 send Attachment

			code[0] = buff[0];
			code[1] = buff[1];
			code[2] = buff[2];
			code[3] = '\0';

			if(strcmp(code,"420") == 0)
			{
			    printf("\nReady to send image!\n\n");
			}
			else{
			    printf("\nError occured!\n\n");
			}


			send_image(socket_id);
			bzero(buff,10240);
			n = read(socket_id,buff,10239);
			if(n < 0)
			{
			    printf("\nError occured while reading from socket!\n");
			}
			printf("SERVER : %s\n\n",buff);
        }
        else if(strcasecmp(cname,"QUIT") == 0)
        {
            bzero(buff,10240);
            strcpy(buff,"QUIT");
            strcat(buff,"\r\n");
            n = write(socket_id,buff,strlen(buff));
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //QUIT
            bzero(buff,10240);
            n = read(socket_id,buff,10239);
            if(n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("SERVER : %s\n",buff);    //221 Bye

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"221") == 0)
            {
                printf("\nConnection closed successfully with SMTP Server!\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
        }
        else
        {	
        	strcpy(cname,"");
        	goto intake;
        }
        
    }while(strcmp(cname,"QUIT") != 0);   
}

