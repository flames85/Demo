#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int client_sockfd;
    int len = 0;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    memset(&remote_addr, 0x0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=AF_INET; //设置为IP通信

    if(argc < 3) return 0;

    printf("connect ip:port = %s:%s\n", argv[1], argv[2]);
    
    remote_addr.sin_addr.s_addr = inet_addr(argv[1]);//服务器IP地址
    remote_addr.sin_port=htons(atoi(argv[2])); //服务器端口号
    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((client_sockfd = socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        return 1;
    }
    /*将套接字绑定到服务器的网络地址上*/
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        perror("connect");
        return 1;
    }
    printf("#connected \n");

    /*循环的发送接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数*/
    char send_buf[BUFSIZ] = {0};  //数据传送的缓冲区
    char recv_buf[BUFSIZ] = {0};  //数据传送的缓冲区
    int index = 1;
    string strTmp;
    for(;index < 5000; index++)
    {
        sprintf(send_buf, "GET /TMS/QueryAuthorization?Account=FixedPU%d@hyc HTTP/1.1\r\n"
                          "From:TAS_001@hyc\r\n"
                          "User-Agent: Version 11.9 Build 580 20151110\r\n"
                          "Host: 192.168.26.41\r\n\r\n", index);

        len = send(client_sockfd, send_buf, strlen(send_buf), 0);

        cout << "send" << endl;

        len = recv(client_sockfd, recv_buf, BUFSIZ,0);
        cout << "recv" << recv_buf << endl;

        recv_buf[len] = '\0';
        strTmp += recv_buf;
        memset(recv_buf, 0x0, sizeof(recv_buf));


        string::size_type pos = strTmp.find("Password=");
        if(pos != string::npos)
        {
            string sAccount = strTmp.substr(pos, 70);
            cout << sAccount << endl;
            strTmp.clear();
        }
    }

    close(client_sockfd);//关闭套接字
    return 0;
}
