#pragma once

class Game2Proxy {
	int port;
	char buffer[4096] = { 0 };
public:
	Server* server;
	Client* p2s;

	Game2Proxy(int port = 3333);
	void run();
};

class Proxy2Server {
	std::string ip;
	int port;
	char buffer[4096] = { 0 };
public:
	Client* client;
	Server* g2p;

	Proxy2Server(std::string ip = "8.8.8.8", int port = 3333);
	void run();
};

class Proxy {
	std::string to_host;
	int port;
public:
	Proxy(std::string to_host, int port);
	void run();
};

namespace ThreadHandler {
	void Game2ProxyHandler(LPVOID obj);
	void Proxy2ServerHandler(LPVOID obj);
	void ProxyHandler(LPVOID obj);
}