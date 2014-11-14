/**
*  \file socket.cpp
*  \brief Funções de gerenciamento das conexões UDP
*/

socket_type my_sock;
socket_type other_sock;
  
int initSocket (socket_type sock){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != NO_ERROR) return -1;
	
	return 0;
}

int startClient (char *server_ip,short server_port){

	if(!my_sock.socket = socket(AF_INET,SOCK_DGRAM,0)) return -1;
	
	my_sock.address.cliAddr.sin_family = AF_INET;
	my_sock.address.cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_sock.address.cliAddr.sin_port = htons(0);
	
	other_sock.address.sin_addr.s_addr = inet_addr(server_ip);
	other_sock.address.cliAddr.sin_port = htons(server_port);
}

int startServer(short server_port){

}

int getPacket (packet_type &p){

}

int sendPacket(packet_type &p){

}