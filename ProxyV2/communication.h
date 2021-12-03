#pragma once

class Server {
public:
	char tBuffer[4096] = { 0 };
	SOCKET sock;
	SOCKET clientSocket;

	Server(int port = 3333);
	void Init(int port);
	int ServerRecieve(char* buffer);
	void ServerSend(char* buffer, int length);
	void Exit();
};

class Client {
public:
	char tBuffer[4096] = { 0 };
	//SOCKET sock;
	SOCKET serverSocket;

	Client(std::string ip = "8.8.8.8", int port = 3333);
	void Init(std::string ip, int port);
	int ClientRecieve(char* buffer);
	void ClientSend(char* buffer, int length);
	void Exit();
};