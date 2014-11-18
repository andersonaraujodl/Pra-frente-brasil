/**
*  \file socket.cpp
*  \brief Funções de gerenciamento das conexões UDP
*/

int m_socket;
sockaddr_in my_addr;
sockaddr_in other_addr;
 
/**
 *  @brief Função de inicialização dos sockets
 *  
 *  @return -1 erro
 */
int initSocket (short port){
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != NO_ERROR) return -1;
	
	if(!(m_socket = socket(AF_INET,SOCK_DGRAM,0))) return -1;
	
	my_addr.cliAddr.sin_family = AF_INET;
	my_addr.cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.cliAddr.sin_port = htons(port);
	
	bind(my_sock.socket,(struct sockaddr *)&my_addr , sizeof(my_addr));
	
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
	other_addr.cliAddr.sin_port = htons(server_port);
	
	
}

/**
 *  @brief Brief
 *  
 *  @param [in] server_port Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int startServer(short server_port){
	
}

/**
 *  @brief Brief
 *  
 *  @param [in] p Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int getPacket (packet_type *pack){
	return recvfrom(m_socket,(char *)pack,(sizeof(packet_type) + pack->buff_size),
					0, (struct sockaddr *) &si_other, &slen);
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

}