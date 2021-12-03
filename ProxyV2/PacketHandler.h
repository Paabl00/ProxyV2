#pragma once
class Buffer {
public:
	char* memory[4096]{0};
};

class PacketHandler{
private:
	Client* p2s;
	int oBufferSize[100]{ 0 };
	Buffer oBuffers[100];
	std::string uBuffers[100];

	std::string ids[100]; //loop back identificators
	int idAmounts[100]{ 0 }; //loop back amounts

	int index_buffer;
	int index_ids;
	
	void fLoopBack(char* buffer, int length, std::string uBuffer);
	void fStop(char* buffer, int length, std::string uBuffer);
	int TweakLoopBack;
	int TweakStop;
public:
	PacketHandler(Client* c);
	void HandlePackets(char* buffer, int length, std::string uBuffer);
	int FindPackage(std::string ubuffer, char* buffer);
	void addLoopBackId(std::string id, int amount);
	void addStopId(std::string id, int amount);
};