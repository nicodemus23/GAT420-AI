#pragma once
#include "Environment.h"

class GameOfLife : public Environment
{
public:
	GameOfLife(int width, int height, std::shared_ptr<class Texture> texture) :
		Environment{ width, height, texture } {}

	bool Initialize() override;
	void Step() override;
	void KeyDown(SDL_Keycode keycode) override;

protected:
	int frame{ 0 };

	// double buffer
	std::vector<uint8_t> bufferA;
	std::vector<uint8_t> bufferB;

};
