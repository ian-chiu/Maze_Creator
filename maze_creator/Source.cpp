#include <SDL.h>
#include <iostream>
#include <stack>
#include <vector>

enum
{
	CELL_PATH_N = 0x01,
	CELL_PATH_E = 0x02,
	CELL_PATH_S = 0x04,
	CELL_PATH_W = 0x08,
	CELL_PATH_VISITED = 0x10
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
	std::vector<int> maze(nMazeHeight * nMazeWidth, 0);
	int nPathCell = 3; // the side lengeth of a path (cell unit)

	myStack.push(std::make_pair(0, 0));
	maze[0] = CELL_PATH_VISITED;
	nVistedCells = 1;

	maze[25] = CELL_PATH_E;
	maze[10] = CELL_PATH_S;

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
		for (int x = 0; x < nMazeWidth; x++)
		{
			for (int y = 0; y < nMazeHeight; y++)
			{
				int mazeBlock_cor_x = x * (nPathCell + 1) * nCellUnit;
				int mazeBlock_cor_y = y * (nPathCell + 1) * nCellUnit;
				mazeBlock = { 
					mazeBlock_cor_x,
					mazeBlock_cor_y,
					nCellUnit * nPathCell,
					nCellUnit * nPathCell
				};

				if (maze[y * nMazeWidth + x] & CELL_PATH_VISITED)
				{
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &mazeBlock);
				}
				else
				{
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &mazeBlock);
				}

				SDL_Rect pathEastLinkRec = {
					mazeBlock_cor_x,
					mazeBlock_cor_y,
					nCellUnit * (nPathCell + 1), // plus 1 to elimate the wall
					nCellUnit * nPathCell
				};

				SDL_Rect pathSouthLinkRec = {
					mazeBlock_cor_x,
					mazeBlock_cor_y,
					nCellUnit * nPathCell, // plus 1 to elimate the wall
					nCellUnit * (nPathCell + 1)
				};

				if (maze[y * nMazeWidth + x] & CELL_PATH_E)
				{
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &pathEastLinkRec);
				}

				if (maze[y * nMazeWidth + x] & CELL_PATH_S)
				{
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &pathSouthLinkRec);
				}
				
			}
		}

		SDL_RenderPresent(gRenderer);
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	
	return 0;
}