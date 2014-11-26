/**
*  \file socket.cpp
*  \brief Funções de gerenciamento das conexões UDP
*/
#include "socket.h"
int m_socket = 0;
sockaddr_in my_addr;
sockaddr_in other_addr;
int pack_counter = 0; /**< Contador de packeds enviados*/

short server_port;
char server_ip[16];

/**
 *  @brief Define as configurações do servidor (porta e ip)
 *
 *	@param [in] *ip define o ip do server
 *	@param [in] port define a porta do server
 */
void setServerConfig (char *ip, short port){
	server_port = port;
	
	strcpy(server_ip,ip);
}


/**
 *  @brief Função de inicialização dos sockets
 *  
 *  @return -1 erro
 */
int initSocket (){
	
	if(m_socket) return 0;
	
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != NO_ERROR) return -1;
	
	if(!(m_socket = socket(AF_INET,SOCK_DGRAM,0))) return -1;
	
	// Inicializa o endereço da máquina
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(server_port);
	
	// Inicializa o endereço da outra
	other_addr.sin_family = AF_INET;
	other_addr.sin_addr.s_addr = inet_addr(server_ip);
	other_addr.sin_port = htons(server_port);

	if(bind(m_socket,(struct sockaddr *)&my_addr , sizeof(my_addr)) == SOCKET_ERROR)
		return -1;
	
	DWORD nonBlocking = 1;
	if (ioctlsocket(m_socket, FIONBIO, &nonBlocking ) != 0)
		return -1;
	
	return 0;
}

/**
 *  @brief Valida a conexão entre Server e Client
 *  
 *  @return 1 houve recebimento de mensagem
 */
int connectToServer(){
	packet_type p;
	
	if(getPacket(p)){
		if(!(strcmp(p.buff,"CONNECT OK")))
			return 1;
	}
	
	strcpy(p.buff,"REQ CONECT");
	p.ctrl.buff_size = strlen("REQ CONECT") +1; // Tamanho da string + terminador 0
	p.ctrl.operation = CONNECTION_REQ;
	sendPacket(p);
	
	return 0;
}

/**
*  @brief aguarda confirmação de conexão
*  
*  @return 1 estabelece conexão
*/
int waitClient (void){
	int num_b;
	packet_type pack;
	if(num_b = getPacket(pack)){
		if(!strcmp(pack.buff,"REQ CONECT")){
			strcpy(pack.buff,"CONNECT OK");
			pack.ctrl.buff_size = strlen(pack.buff);
			pack.ctrl.operation = CONNECTION_REQ;
			sendPacket(pack);
			
			return 1;
		}
		
	}
	
	return 0;
}


/**
 *  @brief recebe conteúdo transferido
 *  
 *  @param [out] &pack buffer no qual será armazenado a mensagem a ser trasnferida
 */
int getPacket (packet_type &pack){
	int sz = sizeof(other_addr);
	//recvfrom (socket, buffer, lenght, flags, adrress, adress_lenght)
	int ret =  recvfrom(m_socket,(char *)&pack,(sizeof(packet_type)),0,(struct sockaddr *)&other_addr,&sz );
	
	if ((ret < 0) && (WSAGetLastError() == WSAEWOULDBLOCK)) return 0;
	
	return ret;
}

/**
 *  @brief transfere conteúdo
 *  
 *  @param [in/out] &p buffer com conteúdo a ser enviado
 */
int sendPacket(packet_type &p){
	p.ctrl.pack_count = ++pack_counter;
	//sendto (socket, message, length, flags, dest_address, dest_lenght)
	return sendto(m_socket,(char *)&p,sizeof(pack_control_type) + p.ctrl.buff_size,0,(LPSOCKADDR)&other_addr,sizeof(sockaddr));
}
