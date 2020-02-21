#include <SDL.h>
#include <iostream>
#include <stack>

enum
{
	CELL_PATH_N = 0x01,
	CELL_PATH_E = 0x02,
	CELL_PATH_S = 0x04,
	CELL_PATH_W = 0x08,
	CELL_PATH_VISTED = 0x10
};

int main (int argc, char* argv[])
{
	int nCellUnit = 8; // a cell is a 8*8 px square
	int nScreenWidth = 160 * nCellUnit;
	int nScreenHeight = 100 * nCellUnit;

	int nMazeWidth = 40 * nCellUnit;
	int nMazeHeight = 25 * nCellUnit;
	int nVistedCells;
	std::stack<std::pair<int, int>> myStack;
	int* maze = new int[nMazeHeight * nMazeWidth];
	memset(maze, 0, nMazeHeight * nMazeWidth);
	int nPathCell = 4;

	myStack.push(std::make_pair(0, 0));
	maze[0] = CELL_PATH_VISTED;
	nVistedCells = 1;

	SDL_Window* gWindow = nullptr;
	SDL_Renderer* gRenderer = nullptr;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(nScreenWidth, nScreenHeight, SDL_WINDOW_SHOWN, &gWindow, &gRenderer);

	bool bQuit = false;
	SDL_Event event;
	SDL_Rect mazeBlock;
	while (!bQuit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				bQuit = true;
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		// Draw Maze
		for (int x = 0; x < nMazeWidth; x += nCellUnit)
		{
			for (int y = 0; y < nMazeHeight; y += nCellUnit)
			{
				mazeBlock = { 
					x * nPathCell,
					y * nPathCell,
					nCellUnit * (nPathCell - 1),
					nCellUnit * (nPathCell - 1)
				};

				if (maze[y * nMazeWidth + x] & CELL_PATH_VISTED)
				{
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &mazeBlock);
				}
				else
				{
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &mazeBlock);
				}
			}
		}

		SDL_RenderPresent(gRenderer);
	}

	std::cout << mazeBlock.x << " " << mazeBlock.y << std::endl;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	
	return 0;
}