#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
char HELLO_MSG[]="220 Hello user";
char WRONG_COMM[]="400";
char WRONG_USERNAME[]="430";
char userMas[]="Kostya";
char passMas[]="12345";
char commands[8][5]={"SEND","RECV","DELE","MKD ","CWD ","RMD ","LIST","QUIT"};
int makeDataConnection(char *recv){
    char ip[15];
    int points=0;
    for(int i=0;i<strlen(recv);i++,recv++){
        if((*recv>'0'&&*recv<'9')&&points!=4){
            ip[i]=*recv;
        }
        if(*recv==','&&points!=4){
            points++;
            if(points==4){
                break;
            }
            ip[i]='.';
        }
    }
    int listenfd = 0, sockData = 0;
    struct sockaddr_in serv_addr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_aton(ip, &serv_addr.sin_addr);
    serv_addr.sin_port = htons(30);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(listen(listenfd, 10)!=0){
        printf("Problem with port listening\n");
        exit(-1);
    }
    sockData = accept(listenfd, (struct sockaddr*)NULL, NULL);
    return sockData;
}
int checkUserName(char *recv,int type){
    char buff[64]="";
    strcpy(buff, &recv[5]);
    if(type==0&&strcmp(buff, userMas)==0){
        return 0;
    }
    if(type==1&&strcmp(buff, passMas)==0){
        return 0;
    }
    if(type==2){
        return makeDataConnection(recv);
    }
    return -2;
}
int sendall(int s, char *buf, int len, int flags){
    int total = 0;     int n=0;
    while(total < len){
        n = (int)send(s, buf+total, len-total, flags);
        if(n == -1)
        break;
        total += n;
        
    }
    return (n==-1 ? -1 : total);
    
}
int recvall(int s, char *buf, int len, int flags){
    int total = 0;     int n=0;
    while(total < len){
        n = (int)recv(s, buf+total, len-total, flags);
        if(n == -1)
        break;
        total += n;
        
    }
    return (n==-1 ? -1 : total);
}
int handleComm(int sockComm, int sockData){
    char recvBuff[255];
    int returnValue=-1;
    int recvStatus=(int)recv(sockComm,recvBuff,sizeof(recvBuff),0);
    printf("Recieved bytes:%d\n",recvStatus);
    char commType[5];
    strncpy(commType, recvBuff, 4);
    int commNumb=-1,tempCommNumb=-1;
    for(int i=0;i<8;i++){
        tempCommNumb=memcmp(commType, commands[i], 4);
        if(tempCommNumb==0){
            commNumb=i;
        }
    }
    if(commNumb!=-1){
        
    }
    if(strstr(recvBuff, commType)){
        
    }
    else{
        sendall(sockComm,WRONG_COMM,(int)strlen(WRONG_COMM),0);
        if(strstr(recvBuff, "QUIT")){
            return 1;
        }
        returnValue=handleComm(sockComm,sockData);
    }
    return 0;
}
int authoriseUser(int sock,int type){
    char recvBuff[255];
    int returnValue=-1;
    int recvStatus=(int)recv(sock,recvBuff,sizeof(recvBuff),0);
    printf("Recieved bytes:%d\n",recvStatus);
    char commType[5];
    switch (type) {
        case 0:
            strcpy(commType, "USER ");
            break;
        case 1:
            strcpy(commType, "PASS ");
            break;
        case 2:
            strcpy(commType, "PORT ");
            break;
        default:
            return -1;
            break;
    }
    if(strstr(recvBuff, commType)){
        int checkResult=checkUserName(recvBuff,type);
        if(checkResult==-2){
            sendall(sock,WRONG_USERNAME,(int)strlen(WRONG_USERNAME),0);
            returnValue=authoriseUser(sock,type);
        }
        if(type==2){
            return checkResult;
        }
    }
    else{
        sendall(sock,WRONG_COMM,(int)strlen(WRONG_COMM),0);
        if(strstr(recvBuff, "QUIT")){
            return 1;
        }
        returnValue=authoriseUser(sock,type);
    }
    return 0;
}
int main(int argc, char *argv[]) {
    int listenfd = 0, sockComm = 0;
    struct sockaddr_in serv_addr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(21);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(listen(listenfd, 10)!=0){
        printf("Problem with port listening\n");
        exit(-1);
    }
    int recvStatus, *recvMass=(int*)malloc(3*sizeof(int)),funcStatus;
    int sendStatus, *sendMas=(int*)malloc(recvMass[2]*sizeof(int));
    sockComm = accept(listenfd, (struct sockaddr*)NULL, NULL);
    sendStatus=sendall(sockComm,HELLO_MSG,(int)strlen(HELLO_MSG),0);
    funcStatus=authoriseUser(sockComm,0);
    if(funcStatus==-1){
        printf("Iternal server error\n");
        exit(-1);
    }
    if(funcStatus==1){
        
    }
    funcStatus=authoriseUser(sockComm,1);
    if(funcStatus==-1){
        printf("Iternal server error\n");
        exit(-1);
    }
    if(funcStatus==1){
        
    }
    int sockData=authoriseUser(sockComm,2);
    if(sockData==-1){
        printf("Iternal server error\n");
        exit(-1);
    }
    if(sockData==1){
        
    }
    
}
