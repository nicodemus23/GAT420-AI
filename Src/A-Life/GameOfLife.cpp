#include "GameOfLife.h"
#include "Texture.h"
#include "Random.h"
#include <iostream>

bool GameOfLife::Initialize()
{	// resize buffers to number of cells
	bufferA.resize(size.x * size.y);
	bufferB.resize(size.x * size.y);

	return true;
}

bool isBufferEmpty(const std::vector<uint8_t>& buffer) {
	for (auto cell : buffer) {
		if (cell != 0) {
			return false; // Found an 'alive' cell
		}
	}
	return true; // All cells are dead
}

void GameOfLife::Step()
{
	frame++;
	// when the frame # is even, read from bufferA and write to bufferB (and vice versa)
	std::vector<uint8_t>& readBuffer = (frame % 2) ? bufferA : bufferB;
	std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;

	// update buffer
	// clear write buffer
	std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

	// DEBUG:: test on random cells
	//TestOnRandom();

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{   // count number of living neighbors
			int weight = 0;

			// read surrounding 8 cells, accumulate weight (3 x 3 kernel)

			// dy = 0 is current cell, dy = -1 is cell above, dy = 1 is cell below
			for (int dy = -1; dy <= 1; dy++) // d stands for delta (change) 
			{   // dx = 0 is current cell, dx = -1 is cell to the left, dx = 1 is cell to the right
				for (int dx = -1; dx <= 1; dx++)
				{
					if (dx == 0 && dy == 0) continue; // skip current cell
					weight += Read<uint8_t>(readBuffer, x + dx, y + dy);
				}
			}

			// GAME OF LIFE RULES
		   // if cell is alive, update
			if (Read<uint8_t>(readBuffer, x, y))
			{   // any live cell with fewer than two live neighbours dies, as if by underpopulation 
				// and any live cell with greater than 3 living neighbors dies, as if my overpopulation
				if (weight < 2 || weight > 3)
				{
					Write<uint8_t>(writeBuffer, x, y, 0);
				}
				else
				{   // any live cell with 2 or 3 live neighbors lives on to the next generation
					(Write<uint8_t>(writeBuffer, x, y, 1));
				}
			}
			else
			{
				// if cell is dead, update
				if (weight == 3)
				{
					Write<uint8_t>(writeBuffer, x, y, 1);
				}
			}

			std::cout << "Cell [" << x << ", " << y << "]: Current state: " << (int)Read<uint8_t>(readBuffer, x, y)
				<< ", Weight: " << weight << ", New state: ";

			if (Read<uint8_t>(readBuffer, x, y)) {
				if (weight < 2 || weight > 3) {
					std::cout << "0 (dead)";
				}
				else {
					std::cout << "1 (alive)";
				}
			}
			else {
				if (weight == 3) {
					std::cout << "1 (alive)";
				}
				else {
					std::cout << "0 (dead)";
				}
			}
			std::cout << std::endl;



			// print out the new state of cell values
			//if (x < 20 && y < 20) {
			//	//std::cout << "Cell " << x << ", " << y << ": " << (int)Read<uint8_t>(readBuffer, x, y) << std::endl;
			//}
		}


	}
	if (isBufferEmpty(writeBuffer)) {
		std::cout << "Write buffer is empty (all cells are dead)." << std::endl;
	}
	else {
		std::cout << "Write buffer has living cells." << std::endl;
	}

	// convert buffer data format to color buffer
	std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
		{	// if v is true, return 0xffffffff, else return 0
			{ return  v ? 0xffffffff : 0; }
		});

	// copy color buffer to texture
	texture->Copy(color_buffer);
	//std::cout << "Texture updated with new color buffer\n" << std::endl;

	std::cout << "First 10 color buffer values: ";
	for (int i = 0; i < 10; ++i) {
		std::cout << color_buffer[i] << " ";
	}
	std::cout << std::endl;
}
// when the r key is pressed, write random 'alive' cells to buffer
void GameOfLife::KeyDown(SDL_Keycode keycode)
{

	if (keycode == SDLK_r)
	{
		std::cout << "R key pressed - Randomizing cells\n" << std::endl;;
		// write random 'alive' cells to buffer
		// when the frame # is even, read from bufferA and write to bufferB (and vice versa)
		std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
		for (int i = 0; i < writeBuffer.size(); i++)
		{
			int randomNumber = random(0, 10);
			// if random number is 0, set cell to 1, else set cell to 0
			writeBuffer[i] = (randomNumber == 0) ? 1 : 0;

			// print out the random number
			//std::cout << "Random number: " << randomNumber << std::endl;
			// print out the new state of cell values
			//std::cout << "Cell " << i << ": " << (int)writeBuffer[i] << std::endl;
		}
		int aliveCellsCount = std::count(writeBuffer.begin(), writeBuffer.end(), 1);
		std::cout << "Number of alive cells: " << aliveCellsCount << std::endl;

		std::cout << "First 10 write buffer values after randomization: ";
		for (int i = 0; i < 10; ++i) {
			std::cout << (int)writeBuffer[i] << " ";
		}
		std::cout << std::endl;
	}

}
// DEBUG function to test random cell values
void GameOfLife::TestOnRandom()
{
	std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
	for (int i = 0; i < writeBuffer.size(); i++)
	{
		int randomNumber = random(1, 10);
		writeBuffer[i] = (randomNumber == 1) ? 1 : 0;

		// print out the random number
		//std::cout << "Random number: " << randomNumber << std::endl;
		// print out the new state of cell values
		//std::cout << "Cell " << i << ": " << (int)writeBuffer[i] << std::endl;
	}

}


