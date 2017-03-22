#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

char TWITTER_CONSUMER_KEY[] = "gPFX6uLLPSq1YNV3UvOOmxBm9";
char TWITTER_CONSUMER_SECRET[] = "A9OFNbEcGFBfV0GNt9dwx2AWncPRrcGBbueVzdl8e3FEdd1EJk";
char TWITTER_ACCESS_TOKEN_KEY[] = "1486245986-QrZJp6vH6DDzjMJXUCQ0y5sl9eiCJVLRv30agdq";
char TWITTER_ACCESS_TOKEN_SECRET[] = "lzXe6UKt8vCPQOR5WesgErMJ8Ip0XpNvhhYLgEmStfF6r";


void error(const char *msg) { perror(msg); exit(0); }

int main(int argc,char *argv[])
{
    //print argv
    /*
    puts("ARGV");
    int index;
    for( index = 0; index < (sizeof(argv) / sizeof(argv[0])); index++){
        printf( "%d", argv[index]);
        printf( "\n" );
    }*/

    /* first what are we going to send and where are we going to send it? */
    int portno =        80;
    char *host =        "cs590.herokuapp.com";
    char *message_fmt = "%s %s HTTP/1.0\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024],response[4096];

    if (argc < 2) { puts("Parameters: <Request type> <command>"); exit(0); }

    /* fill in the parameters */
    sprintf(message,message_fmt,argv[1],argv[2]);
    printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    puts("SERVER");
    puts(server);

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        printf("RESPONSE: %s\n", response);
        // HANDLE RESPONSE CHUCK HERE BY, FOR EXAMPLE, SAVING TO A FILE.
         bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    /* process response */
    printf("Response:\n%s\n",response);

    return 0;
}