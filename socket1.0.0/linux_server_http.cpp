#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::cin;

// struct hostent {
//     char    *h_name;                                //主机的规范名       
//     char    **h_aliases;                            //主机的别名
//     int     h_addrtype;                              //主机ip地址的类型
//     int     h_length;                                    //主机ip地址的长度
//     char    **h_addr_list;                        //主机ip地址,以网络字节序存储的
//     #define h_addr h_addr_list[0]
// };

int main(int argc, char  **argv) {
    int serv_fd, clnt_fd;
    char buf[2048], inbuf[2048];
    char *serv_port;
    int n;
    struct sockaddr_in serv_addr, clnt_addr;

    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("server: socket create failed!\n");
        exit(errno);
    }
    else
        cout << "server: socket created!" << endl; 

    serv_port = argv[1];
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(serv_port));
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serv_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("server: bind failed!");
        exit(errno);
    }
    else
        cout << "server: binded!" << endl;

    if(listen(serv_fd, 20) == -1) {
        perror("server: listen failed!");
        exit(errno);
    }
    else
        cout << "server: begin listen!" << endl;   

    snprintf(buf, sizeof(buf),
       "HTTP/1.0 200 OK\r\n"
       "Content-Length: 20\r\n"
       "Content-Type:text/html\r\n"
       "\r\n"
       "HELLO");
    
    while(1){
        socklen_t clnt_size = sizeof(sockaddr);
        if((clnt_fd = accept(serv_fd, (struct sockaddr*) &clnt_addr, &clnt_size)) == -1) {
            perror("server: accept failed!");
            exit(errno);
        }
        else
            printf("server: got connection from %s, port %d, socket %d\n", 
            inet_ntoa(clnt_addr.sin_addr), 
            ntohs(clnt_addr.sin_port),
            clnt_fd);
        
        n = read(clnt_fd, inbuf, sizeof(inbuf));
        write(fileno(stdout), inbuf, n);
        write(clnt_fd, buf, (int)strlen(buf));
        close(clnt_fd);
    }
    close(serv_fd);
    return 0;
}