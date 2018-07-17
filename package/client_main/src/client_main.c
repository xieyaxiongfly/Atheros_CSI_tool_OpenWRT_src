/*
 * =====================================================================================
 *
 *       Filename:  client_main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/03/2017 14:21:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yaxiong Xie, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "csi_func.h"

#define BUFSIZE 4096
unsigned char buf_addr[BUFSIZE];
unsigned char data_buf[1500];

int quit;
int sock;
int fd;
int log_flag;
FILE*  fp;
COMPLEX csi_matrix[3][3][114];
csi_struct*   csi_status;

void sig_handler(int signo){
    if(signo == SIGINT){
        quit = 1;
    }
}
void exit_program(){
    printf("!!! WE EXIT !!!\n");
    if (log_flag == 1){
        fclose(fp);
    }
    close(fd);
    close(sock);
    sock = -1;
}

int checkCPUendian(){
    int num = 1;
    if(*((char*)&num) == 1){
        printf("Little-endian\n");
        return 0;
    }
    else{ 
        printf("Big-endian\n");
        return 1;
    }
}

int main(int argc, char* argv[])
{
    int    i,total_msg_cnt,cnt;
    int    data_len,data_len_local;
    int    byte_cnt,send_cnt;

    char   *hostname;
    int    port;
    struct hostent *hp;
    struct sockaddr_in pin;
    int    ret;
    int    CPUendian;
    int    log_flag = 0;
    
    fd_set readfds,writefds,exceptfds; 

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    u_int8_t    tmp_int8;
    u_int16_t   buf_len;

    log_flag = 0;

    csi_status = (csi_struct*)malloc(sizeof(csi_struct));
    /* check usage*/
    if (1 == argc){
    /*  If you want to log the CSI for off-line processing,
     * you need to specify the name of the output file
     */
        log_flag    = 0;
        printf("/* ********************************************************/\n");
        printf("/*    Usage: recv_csi serverIP serverPort   */\n");
        printf("/* ********************************************************/\n");
    }
   if (2 == argc){
        printf("The serverIP and serverPort must be set simutaneously! \n");
        fclose(fp);
        return 0; 
    }
    if (3 == argc){
        hostname = argv[1];
        port     = atoi(argv[2]);
    }else{
         /* connect to the default server*/
        hostname = "155.69.151.130"; // the server IP
        port = 6767;      // port 
    }
    
    if (argc > 3){
        printf(" Too many input arguments !\n");
        return 0;
    }

    fd = open_csi_device();
    if (fd < 0){
        perror("Failed to open the CSI device...");
        return errno;
    }
    
    memset(&pin,0,sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_port   = htons(port);

    if ((hp = gethostbyname(hostname))!= NULL){
        pin.sin_addr.s_addr =
                        ((struct in_addr *)hp->h_addr)->s_addr;
    }else{
        pin.sin_addr.s_addr = inet_addr(hostname);
    }

    if((sock = (int)socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("socket"); 
        return 0;
    } 

    printf("Waiting for the connection!\n");

    ret = connect(sock,(const struct sockaddr *)&pin,sizeof(pin));
  //  printf("ret is :%d!\n",ret);

    //if(connect(sock,(const struct sockaddr *)&pin,sizeof(pin)) == -1) {
    if(ret == -1){
        perror("connect");
        exit_program();
        return 0;
    }
    printf("Connection with server is built!\n");

    FD_SET(sock,&readfds);
    FD_SET(sock,&writefds);
    FD_SET(sock,&exceptfds);
    
    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
        printf("Can't catch SIGINT\n");
        exit_program(); 
        return 1;
    }

    /* Now receive the message*/
    quit = 0;
    printf("# Receiving data! Press Ctrl+c to quit!\n");
    while(quit == 0){
        if (quit == 1){
            exit_program();
            return 0;
        }
       /*  keep listening to the kernel and waiting for the csi report */
        cnt = read_csi_buf(buf_addr,fd,BUFSIZE);
        if(cnt){
            printf("cnt is:%d\n",cnt);
            total_msg_cnt += 1;
            data_len        = cnt;
            data_len_local  = data_len; 
    
            CPUendian = checkCPUendian();
            if(CPUendian == 1){ 
                printf("CPU is BigEndian and we SWAP!\n");
                unsigned char *tmp = (unsigned char *)&data_len;
                unsigned char t;
                t = tmp[0];tmp[0] = tmp[3];tmp[3] = t;
                t = tmp[1];tmp[1] = tmp[2];tmp[2] = t;
            }
            printf("data len:%d\n",data_len);
            printf("data_len_local is:%d\n",data_len_local);

            send_cnt  = send(sock,(unsigned char *)&data_len,sizeof(int),0);
            if(send_cnt == -1){
                perror("send");
                exit_program();
                return 0;
            }
            
            byte_cnt = 0;
            while(byte_cnt < data_len_local){
                send_cnt = send(sock,buf_addr+byte_cnt,data_len_local-byte_cnt,0);
                if(send_cnt == -1){
                    perror("send"); 
                    exit_program();
                    return 0;
                }
                byte_cnt += send_cnt;
            }
   
            printf("We have sent %d bytes!\n",byte_cnt);
            
            record_status(buf_addr, cnt, csi_status);
            //record_csi_payload(buf_addr, csi_status, data_buf, csi_matrix);
            
            printf("Recv %dth msg with rate: 0x%02x | payload len: %d\n",total_msg_cnt,csi_status->rate,csi_status->payload_len);
            printf("========  Bsic Information of the Transmission ==========\n");
            printf("csi_len= %d |",csi_status->csi_len);
            printf("chanBW= %d   |",csi_status->chanBW);
            printf("num_tones= %d  |",csi_status->num_tones);
            printf("nr= %d  |",csi_status->nr);
            printf("nc= %d\n",csi_status->nc);
            printf("rssi= %d  |",csi_status->rssi);
            printf("rssi_0= %d  |",csi_status->rssi_0);
            printf("rssi_1= %d  |",csi_status->rssi_1);
            printf("rssi_2= %d\n",csi_status->rssi_2);

            /*  log the received data for off-line processing */
            if (log_flag){
                buf_len = csi_status->buf_len;
                fwrite(&buf_len,1,2,fp);
                fwrite(buf_addr,1,buf_len,fp);
            }
                                                            
        }
    }
   
    exit_program();
    free(csi_status);
    return 0;
}
