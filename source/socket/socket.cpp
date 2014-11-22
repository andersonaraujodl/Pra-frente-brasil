/**
*  \file socket.cpp
*  \brief Funções de gerenciamento das conexões UDP
*/
#include "socket.h"
int m_socket;
sockaddr_in my_addr;
sockaddr_in other_addr;
int pack_counter = 0; /**< Contador de packeds enviados*/
 
/**
 *  @brief Função de inicialização dos sockets
 *  
 *  @return -1 erro
 */
int initSocket (short port){
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != NO_ERROR) return -1;
	
	if(!(m_socket = socket(AF_INET,SOCK_DGRAM,0))) return -1;
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(port);

	if(bind(m_socket,(struct sockaddr *)&my_addr , sizeof(my_addr)) == SOCKET_ERROR)
		return -1;
	
	DWORD nonBlocking = 1;
	if (ioctlsocket(m_socket, FIONBIO, &nonBlocking ) != 0)
		return -1;
	
	return 0;
}

/**
 *  @brief Brief
 *  
 *  @param [in] server_ip   IP do server no formato xxx.xxx.xxx.xxx
 *  @param [in] server_port Número da porta do server
 *  @return Return_Description
 */
int startClient (char *server_ip,short server_port){
		
	other_addr.sin_family = AF_INET;
	other_addr.sin_addr.s_addr = inet_addr(server_ip);
	other_addr.sin_port = htons(server_port);
}

int connectToServer(){
	packet_type p;
	
	strcpy(p.buff,"REQ CONECT");
	
	p.ctrl.buff_size = strlen("REQ CONECT");
	sendPacket(p);
	
	if(getPacket(p)){
		if(!(strcmp(p.buff,"CONNECT OK")))
			return 1;
	}

	return 0;
}

/**
*  \brief Brief
*  
*  \return Return_Description
*  
*  \details Details
*/
int waitClient (void){
	int num_b;
	packet_type pack;
	if(num_b = getPacket(pack)){
		if(!strcmp(pack.buff,"REQ CONECT")){
			strcpy(pack.buff,"CONNECT OK");
			pack.ctrl.buff_size = strlen(pack.buff);
			sendPacket(pack);
			
			return 1;
		}
		
	}
	
	return 0;
}


/**
 *  @brief Brief
 *  
 *  @param [in] p Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int getPacket (packet_type &pack){
	int sz = sizeof(other_addr);
	int ret =  recvfrom(m_socket,(char *)&pack,(sizeof(packet_type)),0,(struct sockaddr *)&other_addr,&sz );
	
	if ((ret < 0) && (WSAGetLastError() == WSAEWOULDBLOCK)) return 0;
	
	return ret;
}

/**
 *  @brief Brief
 *  
 *  @param [in] p Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int sendPacket(packet_type &p){
	p.ctrl.pack_count = ++pack_counter;
	return sendto(m_socket,(char *)&p,sizeof(pack_control_type) + p.ctrl.buff_size,0,(LPSOCKADDR)&other_addr,sizeof(sockaddr));
}
