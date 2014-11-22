/**
*  \file socket.h
*  \brief Funções de gerenciamento das conexões UDP
*/

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__
// Defines p�blicos ========================================
#define SOCK_BUFF_LEN 64

// Types P�blicos ==========================================
typedef struct{
	unsigned int pack_count;
	unsigned int buff_size;
}pack_control_type;


typedef struct{
	
	pack_control_type ctrl;
	char buff[SOCK_BUFF_LEN];
}packet_type;

// Prot�tipos p�blicos ======================================
int initSocket (short port);
int startClient (char *server_ip,short server_port);
int connectToServer();
int waitClient (void);
int getPacket (packet_type &pack);
int sendPacket(packet_type &p);
// ==========================================================


#endif
