/*
 * =====================================================================================
 *       Filename:  csi_fun.h
 *
 *    Description:  head file for csi processing fucntion  
 *        Version:  1.0
 *
 *         Author:  Yaxiong Xie  
 *         Email :  <xieyaxiongfly@gmail.com>
 *   Organization:  WANDS group @ Nanyang Technological University
 *
 *   Copyright (c)  WANDS group @ Nanyang Technological University
 * =====================================================================================
 */
#include <stdbool.h>
#define Kernel_CSI_ST_LEN 23 
typedef struct
{
    int real;
    int imag;
}COMPLEX;

typedef struct
{
    uint64_t tstamp;         /* h/w assigned time stamp */
    
    uint16_t channel;        /* wireless channel (represented in Hz)*/
    uint8_t  chanBW;         /* channel bandwidth (0->20MHz,1->40MHz)*/

    uint8_t  rate;           /* transmission rate*/
    uint8_t  nr;             /* number of receiving antenna*/
    uint8_t  nc;             /* number of transmitting antenna*/
    uint8_t  num_tones;      /* number of tones (subcarriers) */
    uint8_t  noise;          /* noise floor (to be updated)*/

    uint8_t  phyerr;          /* phy error code (set to 0 if correct)*/

    uint8_t    rssi;         /*  rx frame RSSI */
    uint8_t    rssi_0;       /*  rx frame RSSI [ctl, chain 0] */
    uint8_t    rssi_1;       /*  rx frame RSSI [ctl, chain 1] */
    uint8_t    rssi_2;       /*  rx frame RSSI [ctl, chain 2] */

    uint16_t   payload_len;  /*  payload length (bytes) */
    uint16_t   csi_len;      /*  csi data length (bytes) */
    uint16_t   buf_len;      /*  data length in buffer */
}csi_struct;

bool  is_big_endian();
int   open_csi_device();
void  close_csi_device(int fd);
int   read_csi_buf(unsigned char* buf_addr,int fd, int BUFSIZE);
void  record_status(unsigned char* buf_addr, int cnt, csi_struct* csi_status);
void  record_csi_payload(unsigned char* buf_addr, csi_struct* csi_status,unsigned char* data_buf, COMPLEX(* csi_buf)[3][114]);
void  porcess_csi(unsigned char* data_buf, csi_struct* csi_status,COMPLEX(* csi_buf)[3][114]);
