#include "includes.h"
//Server

Server::Server(int port) {
	this->Init(port);
}

void Server::Init(int port) {

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		MessageBoxA(NULL, "Failed starting up WSAData!", "Error", MB_OK);
		this->Exit();
	}

	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock == INVALID_SOCKET) {
		MessageBoxA(NULL, "Cant create a socket!", "Error", MB_OK);
		this->Exit();
	}

	sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(this->sock, (sockaddr*)&info, sizeof(info));

	listen(this->sock, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	this->clientSocket = accept(this->sock, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << " connected on port: " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port: " << ntohs(client.sin_port) << std::endl;
	}

	closesocket(this->sock);

}

int Server::ServerRecieve(char* buffer) {
	ZeroMemory(this->tBuffer, 4096);
	int bytesRecieved = recv(this->clientSocket, this->tBuffer, 4096, 0);
	if (bytesRecieved == SOCKET_ERROR) {
		MessageBoxA(NULL, "Error in recv, client connection closed.", "Error", MB_OK);
		this->Exit();
	}
	if (bytesRecieved == 0) {
		MessageBoxA(NULL, "Client disconnected.", "Error", MB_OK);
		this->Exit();
	}

	memcpy(buffer, this->tBuffer, 4096);
	return bytesRecieved;
}

void Server::ServerSend(char* buffer, int length) {
	send(this->clientSocket, buffer, length, 0);
}

void Server::Exit() {
	closesocket(this->clientSocket);
	WSACleanup();
	exit(-1);
}

//Client

Client::Client(std::string ip, int port) {
	this->Init(ip, port);
}

void Client::Init(std::string ip, int port) {
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0) {
		MessageBoxA(NULL, "Failed starting up WSAData!", "Error", MB_OK);
		this->Exit();
	}

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == INVALID_SOCKET) {
		MessageBoxA(NULL, "Cant create a socket!", "Error", MB_OK);
		this->Exit();
	}

	sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &info.sin_addr);
	 
	int connResult = connect(this->serverSocket, (sockaddr*)&info, sizeof(info));
	if (connResult == SOCKET_ERROR) {
		MessageBoxA(NULL, "Cant connect to server!", "Error", MB_OK);
		this->Exit();
	}

}

int Client::ClientRecieve(char* buffer) {
	ZeroMemory(this->tBuffer, 4096);
	int bytesRecieved = recv(this->serverSocket, this->tBuffer, 4096, 0);
	if (bytesRecieved == SOCKET_ERROR) {
		MessageBoxA(NULL, "Error in recv, Server connection closed.", "Error", MB_OK);
		this->Exit();
	}
	if (bytesRecieved == 0) {
		MessageBoxA(NULL, "Server disconnected.", "Error", MB_OK);
		this->Exit();
	}

	memcpy(buffer, this->tBuffer, 4096);
	return bytesRecieved;
}

void Client::ClientSend(char* buffer, int length) {
	int sendResult = send(this->serverSocket, buffer, length, 0);
	if (sendResult == SOCKET_ERROR) {
		MessageBoxA(NULL, "send() fialed. Server disconnected", "Error", MB_OK);
		this->Exit();
	}
}

void Client::Exit() {
	closesocket(this->serverSocket);
	WSACleanup();
	exit(-1);
}