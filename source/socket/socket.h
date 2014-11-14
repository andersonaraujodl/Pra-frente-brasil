/**
*  \file socket.h
*  \brief Funções de gerenciamento das conexões UDP
*/

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__

// Types Públicos ==========================================

typedef struct{
	int socket;
	int port;
	sockaddr_in address;
}socket_type;

typedef struct{
	unsigned char pack_count;
	unsigned int buff_size;
	char *buffer;
}packet_type;
// ==========================================================
#endif