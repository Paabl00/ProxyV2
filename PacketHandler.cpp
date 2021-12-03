#include "includes.h"

PacketHandler::PacketHandler(Client* c) {
	this->p2s = c;
	this->index_buffer = 0;
	this->index_ids = 0;
	this->TweakLoopBack = 0;
}

void PacketHandler::fLoopBack(char* buffer, int length, std::string uBuffer) {

	if (this->TweakLoopBack == 0) {
		return;
	}

	for (int i = 0; i < 100; i++) {
		if (size_t pos = uBuffer.find(std::string(ids[i])) != std::string::npos) {
			
			if (pos != 1) {
				return;
			}

			for (int ii = 0; ii < (int)idAmounts[i]; ii++) {
				std::cout << "[" << idAmounts[i] << "] -> " << uBuffer << std::endl;
				this->p2s->ClientSend(buffer, length);
			}

		}
	}
}

void PacketHandler::fStop(char* buffer, int length, std::string uBuffer) { //FINISH
	if (this->TweakStop == 0) {
		return;
	}
}

void PacketHandler::HandlePackets(char* buffer, int length, std::string uBuffer) {
	if (this->index_buffer == 99) {
		this->index_buffer = 0;
	}

	ZeroMemory(oBuffers[this->index_buffer].memory, 4096);
	oBufferSize[this->index_buffer] = 0;
	uBuffers[this->index_buffer].clear();

	memcpy_s(oBuffers[this->index_buffer].memory, 4096, buffer, length);
	oBufferSize[this->index_buffer] = length;
	uBuffers[this->index_buffer] = uBuffer;

	fStop(buffer, length, uBuffer);
	fLoopBack(buffer, length, uBuffer);

	this->index_buffer++;
}

int PacketHandler::FindPackage(std::string ubuffer, char* buffer) {
	for (int i = 0; i < 100; i++) {
		if (uBuffers[i] == ubuffer) {
			memcpy_s(buffer, 4096, oBuffers[i].memory, 4096);
			return oBufferSize[i];
		}
		else {
			continue;
		}
	}
	return 0;
}

void PacketHandler::addLoopBackId(std::string id, int amount) {
	if (this->TweakLoopBack == 0) {
		this->TweakLoopBack = 1;
	}
	
	if (this->index_ids == 99) {
		this->index_ids = 0;
	}

	this->ids[this->index_ids] = id;
	this->idAmounts[this->index_ids] = amount;

	this->index_ids++;
}

void PacketHandler::addStopId(std::string id, int amount) { //FINISH
	if (this->TweakStop == 0) {
		this->TweakStop = 1;
	}
}