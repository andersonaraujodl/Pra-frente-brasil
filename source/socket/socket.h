/**
*  \file socket.h
*  \brief Funções de gerenciamento das conexões UDP
*/

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__
// Defines públicos ========================================
#define SOCK_BUFF_LEN 512

// Types Públicos ==========================================

typedef struct{
	int port;
	sockaddr_in address;
}socket_type;

typedef union{
	struct{
		unsigned int pack_count;
		unsigned int buff_size;
	};
	char buff[SOCK_BUFF_LEN +8];
}packet_type;
// ==========================================================
#endif
