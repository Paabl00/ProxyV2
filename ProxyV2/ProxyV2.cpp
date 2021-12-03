#include "includes.h"

TextParser* tp;
PacketHandler* ph;

DWORD WINAPI SecondThread(LPVOID lp);

Game2Proxy::Game2Proxy(int port) {
	this->server = new Server(port);
}

void Game2Proxy::run() {
	std::string text;

	while (true) {
		int bytes = this->server->ServerRecieve(buffer);
		if (bytes) {
			std::cout << "[] -> ";
			text = tp->HexConvert(buffer, 20);
			ph->HandlePackets(buffer, bytes, text);
			std::cout << tp->Split(text, 4) << std::endl;
			this->p2s->ClientSend(buffer, bytes);
		}
	}
}

Proxy2Server::Proxy2Server(std::string ip, int port) {
	this->client = new Client(ip, port);
}

void Proxy2Server::run() {
	while (true) {
		int bytes = this->client->ClientRecieve(buffer);
		if (bytes) {
			//std::cout << "[] <- ";
			//std::cout << printAsHex(buffer) << std::endl;
			this->g2p->ServerSend(buffer, bytes);
		}
	}
}

Proxy::Proxy(std::string to_host, int port) {
	this->to_host = to_host;
	this->port = port;
}

void Proxy::run() {
	while (true) {
		std::cout << "Establishing proxy on port " << this->port << std::endl;
		Game2Proxy* g2p = new Game2Proxy(port);
		Proxy2Server* p2s = new Proxy2Server(to_host, port);
		std::cout << "Connection Established " << this->port << std::endl;
		g2p->p2s = p2s->client;
		p2s->g2p = g2p->server;

		tp = new TextParser();
		ph = new PacketHandler(p2s->client);

		HANDLE game2proxy = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadHandler::Game2ProxyHandler, g2p, 0, 0);
		HANDLE proxy2server = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadHandler::Proxy2ServerHandler, p2s, 0, 0);
		HANDLE secondThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SecondThread, g2p, 0, 0);

		WaitForSingleObject(game2proxy, INFINITE);
		WaitForSingleObject(proxy2server, INFINITE);
		WaitForSingleObject(secondThread, INFINITE);
	}
}

void ThreadHandler::Game2ProxyHandler(LPVOID obj) {
	Game2Proxy* instance = (Game2Proxy*)obj;
	instance->run();
}

void ThreadHandler::Proxy2ServerHandler(LPVOID obj) {
	Proxy2Server* instance = (Proxy2Server*)obj;
	instance->run();
}

void ThreadHandler::ProxyHandler(LPVOID obj) {
	Proxy* instance = (Proxy*)obj;
	instance->run();
}

DWORD WINAPI SecondThread(LPVOID lp) {
	//AllocConsole(); //Two consoles cant run within a single program...
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	std::string data;
	Game2Proxy* g2p = (Game2Proxy*)lp;
	char buffer[4096];
	int bytes;

	while (true) {

		do {

			//std::cout << "[MANAGER] -> " << std::endl; //needs to be fixed until second console isnt fixed.
			std::getline(std::cin, data);

			if (data.size() > 0) {

				if (data.at(0) == '0' && data.at(1) == 'x') {
					data.erase(0, 2);
					ZeroMemory(buffer, 4096);
					if ((bytes = ph->FindPackage(data, buffer)) == 0) {
						std::cout << "Packet not found!" << std::endl;
					}
					else {

						g2p->p2s->ClientSend(buffer, bytes);

					}
				}

				else if (data.at(0) == 'l' && data.at(1) == 'o' && data.at(2) == 'o' && data.at(3) == 'p' && data.at(4) == 'b' && data.at(5) == 'a' && data.at(6) == 'c' && data.at(7) == 'k') {

					size_t pos = data.find("[");
					if (pos != std::string::npos) {
						char id[4];
						id[0] = data.at(pos + 1);
						id[1] = data.at(pos + 2);
						id[2] = data.at(pos + 3);
						id[3] = data.at(pos + 4);

						size_t pos2 = data.find(",");
						size_t pos3 = data.find("]");
						const int val = pos3 - pos2;
						if (pos2 != std::string::npos && pos3 != std::string::npos) {
							char* values = new char(val);

							for (int i = 0; i < val; i++) {
								values[i] = data.at(pos2 + i + 1);
							}

							std::string s(values);
							std::stringstream ss(s);
							int amount;
							ss >> amount;

							ph->addLoopBackId(tp->CharacterArrayToString(id, 4), (int)amount);
							std::cout << "Added a new id " << tp->CharacterArrayToString(id, 4) << " to the loopback system! repetation: " << (int)amount << std::endl;

						}
						else {
							std::cout << "Cant understand your arguments! Please use - loopback [id,amount]" << std::endl;
						}

					}

					else {
						std::cout << "Cant understand your arguments! Please use - loopback [id,amount]" << std::endl;
					}

				}
				
				else if (data.at(0) == 'i' && data.at(1) == 'f') {
					size_t pos = data.find("[");
					size_t pos_comma = data.find(",");
					if (pos != std::string::npos && pos_comma != std::string::npos) {

					}

					else {
						std::cout << "Cant understand your arguments! Please use - loopback [id,amount]" << std::endl;
					}
				}

				else if (data.at(0) == 's' && data.at(1) == 't' && data.at(2) == 'o' && data.at(3) == 'p') { //FINISH
					size_t pos = data.find("[");
					size_t pos_ending = data.find("]");

					if (pos != std::string::npos && pos_ending != std::string::npos) {
						size_t length = pos_ending - pos;
						char* id = new char(length);

						for (int i = 0; i < length; i++) {
							id[i] = data.at(pos + i + 1);
						}


						
					}

					else {
						std::cout << "Cannot understand your arguments, Please use stop[id]" << std::endl;
					}

				}

				else {
					std::cout << "unknown command!" << std::endl;
				}

			}

		} while (data.size() > 0);
		
	}

	//FreeConsole();
}

int main() {

	char ipstr[INET6_ADDRSTRLEN];
	struct addrinfo hints, * res, *p;
	int status;
	std::string ip; int port;

	WORD w = MAKEWORD(2, 2);
	WSAData data;
	WSAStartup(w, &data);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	std::cout << "IP: ";
	std::getline(std::cin, ip);
	std::cout << "PORT: ";
	std::cin >> port;

	if (status = getaddrinfo(ip.c_str(), NULL, &hints, &res) != 0) {
		MessageBoxA(NULL, "Problem while fetching ip. Check for correct domain.", "Error", MB_OK);
		exit(-1);
	}

	void* addr;
	for (p = res; p != NULL; p = p->ai_next) {
		if (p->ai_addr->sa_family == AF_INET) {
			struct sockaddr_in* i = (struct sockaddr_in*)p->ai_addr;
			inet_ntop(AF_INET, &i->sin_addr, ipstr, sizeof(ipstr));
		}
		else {
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
			addr = &(ipv6->sin6_addr);

			inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		}
	}

	WSACleanup();
	freeaddrinfo(res);

	Proxy* proxy = new Proxy(ipstr, port);
	HANDLE hProxy = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadHandler::ProxyHandler, proxy, 0, 0);
	WaitForSingleObject(hProxy, INFINITE);

	return 0;
}