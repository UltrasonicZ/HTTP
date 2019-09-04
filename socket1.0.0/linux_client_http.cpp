#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
using namespace std;

//using std::cout;
//using std::endl;
//using std::cin;

// struct hostent {
//     char    *h_name;                                //主机的规范名       
//     char    **h_aliases;                            //主机的别名
//     int     h_addrtype;                              //主机ip地址的类型
//     int     h_length;                                    //主机ip地址的长度
//     char    **h_addr_list;                        //主机ip地址,以网络字节序存储的
//     #define h_addr h_addr_list[0]
// };

int main(int argc, char  **argv) {
    int clnt_fd;
    char str[32];
    char buf[32];
    char *host_name;
    char *serv_ip, *serv_port;
    struct hostent *remoteHost;
    struct sockaddr_in serv_addr;

    if ((clnt_fd = socket(AF_INET, SOCK_STREAM, 0)) ==-1) {
        perror("client: socket create failed!\n");
        exit(errno);
    }
    else
        cout << "client: socket created!" << endl;

    host_name = argv[1];
    if ((remoteHost = gethostbyname(host_name)) == NULL) {
        printf("client: gethostbyname error for host:%s\n", host_name);
        return 0;
    }
    else
        cout << "client: get host succeed!" << endl;

    serv_ip = (char *)inet_ntop(remoteHost->h_addrtype, remoteHost->h_addr_list[1], str, sizeof(str));
    serv_port = argv[2];
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(atoi(serv_port));

    if(connect(clnt_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("client: connect failed!");
        exit(errno);
    }
    else
        cout << "client: server connect!" << endl;

    snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");
    // snprintf(buf, sizeof(buf), "GET HTTP/1.1\r\n\r\n");

    // char http_str[1024] = {0};
    // cin.getline(http_str, 24);
    // snprintf(buf, sizeof(buf), http_str);

    int n = write(clnt_fd, buf, (int)strlen(buf));

    while (n > 0) {
        n = read(clnt_fd, buf, sizeof(buf));
        write(fileno(stdout), buf, n);
    }

    close(clnt_fd);
    return 0;
}