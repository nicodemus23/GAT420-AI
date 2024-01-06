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

void GameOfLife::Step()
{
	frame++;
	// when the frame # is even, read from bufferA and write to bufferB (and vice versa)
	std::vector<uint8_t>& readBuffer = (frame % 2 == 0) ? bufferA : bufferB;
	std::vector<uint8_t>& writeBuffer = (frame % 2 == 0) ? bufferB : bufferA;

	// update buffer
	// clear write buffer
	std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{   // count number of living neighbors
			int weight = 0;

			// read surrounding 8 cells, accumulate weight (3 x 3 kernel)

			// dy = 0 is current cell, dy = -1 is cell above, dy = 1 is cell below
			for (int dy = -1; dy <= 1; dy++) // d stands for delta (change) 
			{   //  = 0 is left cell, x = 1 is current cell, x = 2 is right cell
				for (int dx = -1; dx <= 1; dx++)
				{
					if (dx == 0 && dy == 0) continue; // skip current cell
					weight += Read<uint8_t>(readBuffer, x + dx, y + dy);
				}
			}

			// game of life rules
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
			// print out the new state of cell values
			if (x < 50 && y < 50) {
				std::cout << "Cell " << x << ", " << y << ": " << (int)Read<uint8_t>(readBuffer, x, y) << std::endl;
			}
		}
	}

	// convert buffer data format to color buffer
	std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
		{	// if v is true, return 0xffffffff, else return 0
			{ return  v ? 0xffffffff : 0; }
		});

	// copy color buffer to texture
	texture->Copy(color_buffer);
	std::cout << "Texture updated with new color buffer\n" << std::endl; 
	
	for (int i = 0; i < 5; i++)
	{
		//std::cout << "Color buffer [" << i << "]: " << color_buffer[i] << std::endl;
	}
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
			writeBuffer[i] = (randomNumber == 0) ? 1 : 0;

			// print out the random number
			std::cout << "Random number: " << randomNumber << std::endl;
			// print out the new state of cell values
			//std::cout << "Cell " << i << ": " << (int)writeBuffer[i] << std::endl;
		}
	}
}


