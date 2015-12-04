#include <stdio.h>   
#include <string.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <sys/time.h>   
#include <stdlib.h>   

#define PORT 		8888        //服务器端口   
#define BACKLOG 	5           //listen队列中等待的连接数   
#define MAXDATASIZE 1024        //缓冲区大小   

typedef struct _CLIENT  
{  
	int fd;                     //客户端socket描述符   
	char name[20];              //客户端名称   
	struct sockaddr_in addr;    //客户端地址信息结构体   
	char data[MAXDATASIZE];     //客户端私有数据指针   
} CLIENT;  

void process_client(CLIENT * client, char *recvbuf, int len);   //客户请求处理函数   

/************************************************* 
 * Function    : main() 
 * Description :  
 * Calls       : process_client() 
 * Called By   :  
 * Input       :  
 * Output      :  
 * Return      :  
 *************************************************/  
void main(int argc ,char **argv)  
{  
	int i, maxi, maxfd, sockfd;  
	int nready;  
	ssize_t n;  
	fd_set rset, allset;        //select所需的文件描述符集合   
	int listenfd, connectfd;    //socket文件描述符   
	struct sockaddr_in server;  //服务器地址信息结构体   

	CLIENT client[FD_SETSIZE];  //FD_SETSIZE为select函数支持的最大描述符个数   
	char recvbuf[MAXDATASIZE];  //缓冲区   
	int sin_size;               //地址信息结构体大小   

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{                           //调用socket创建用于监听客户端的socket   
		perror("Creating socket failed.");  
		exit(1);  
	}  

	int opt = SO_REUSEADDR;  
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  //设置socket属性   

	bzero(&server, sizeof(server));  
	server.sin_family = AF_INET;  
	server.sin_port = htons(PORT);  
	server.sin_addr.s_addr = htonl(INADDR_ANY);  

	if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)  
	{                           //调用bind绑定地址   
		perror("Bind error.");  
		exit(1);  
	}  

	if (listen(listenfd, BACKLOG) == -1)  
	{                           //调用listen开始监听   
		perror("listen() error\n");  
		exit(1);  
	}  

	//初始化select   
	maxfd = listenfd;  
	maxi = -1;  
	for (i = 0; i < FD_SETSIZE; i++)  
	{  
		client[i].fd = -1;  
	}  
	FD_ZERO(&allset);           //清空   
	FD_SET(listenfd, &allset);  //将监听socket加入select检测的描述符集合   

	while (1)  
	{  
		struct sockaddr_in addr;  
		rset = allset;  
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);    //调用select   
		printf("Select got ready=%d. \n", nready);  

		sleep(1);

		//检测是否有新客户端请求   
		if (FD_ISSET(listenfd, &rset))  
		{                       
			printf("Accept a connection.\n");  
			//调用accept，返回服务器与客户端连接的socket描述符   
			sin_size = sizeof(struct sockaddr_in);  
			if ((connectfd = accept(listenfd, (struct sockaddr *)&addr, (socklen_t *) & sin_size)) == -1)  
			{  
				perror("Accept() error\n");  
				continue;  
			}  

			//将新客户端的加入数组   
			for (i = 0; i < FD_SETSIZE; i++)  
			{  
				if (client[i].fd < 0)  
				{  
					char buffer[20];  
					client[i].fd = connectfd;   //保存客户端描述符   
					memset(buffer, '0', sizeof(buffer));  
					sprintf(buffer, "Client[NO:%.2d][FD:%d]", i, connectfd);  
					memcpy(client[i].name, buffer, strlen(buffer));  
					client[i].addr = addr;  
					memset(buffer, '0', sizeof(buffer));  
					sprintf(buffer, "Data...");  
					memcpy(client[i].data, buffer, strlen(buffer));  
					printf("You got a connection from %s:%d.\n", inet_ntoa(client[i].addr.sin_addr),ntohs(client[i].addr.sin_port));  
					printf("Add a new connection:%s\n",client[i].name);  
					break;  
				}  
			}  

			if (i == FD_SETSIZE)  
				printf("Too many clients\n");  
			FD_SET(connectfd, &allset); //将新socket连接放入select监听集合   
			if (connectfd > maxfd)  
				maxfd = connectfd;  //确认maxfd是最大描述符   
			if (i > maxi)       //数组最大元素值   
				maxi = i;  
			if (--nready <= 0)  
				continue;       //如果没有新客户端连接，继续循环   
		}  

		// 检查所有老用户
		for (i = 0; i <= maxi; i++)  
		{  
			if ((sockfd = client[i].fd) < 0)    //如果客户端描述符小于0，则没有客户端连接，检测下一个   
				continue;  

			// 有客户连接，检测是否有数据   
			if (FD_ISSET(sockfd, &rset))  
			{  
				printf("Receive from connect fd[%d].\n", i);  
				if ((n = recv(sockfd, recvbuf, MAXDATASIZE, 0)) == 0)  
				{   
					//从客户端socket读数据，等于0表示网络中断   
					close(sockfd);  //关闭socket连接   
					printf("%s closed. User's data: %s\n", client[i].name, client[i].data);  
					FD_CLR(sockfd, &allset);    //从监听集合中删除此socket连接   
					client[i].fd = -1;  //数组元素设初始值，表示没客户端连接   
				}  
				else  
					process_client(&client[i], recvbuf, n); //接收到客户数据，开始处理   
				if (--nready <= 0)  
					break;      //如果没有新客户端有数据，跳出for循环回到while循环   
			}  
		}  
	}  
	close(listenfd);            //关闭服务器监听socket        
}  

/************************************************* 
 * Function    : process_client() 
 * Description : 处理客户端连接函数 
 * Calls       :  
 * Called By   : main() 
 * Input       :  
 * Output      :  
 * Return      :  
 *************************************************/  
void process_client(CLIENT * client, char *recvbuf, int len)  
{  
	char sendbuf[MAXDATASIZE];  
	int i;  

	printf("Received client( %s ) message: %s\n", client->name, recvbuf);  

	for (i = 0; i < len - 1; i++)  
	{  
		sendbuf[i] = recvbuf[len - i - 2];  
	}  

	sendbuf[len - 1] = '\0';  

	send(client->fd, sendbuf, strlen(sendbuf), 0);  
}  
