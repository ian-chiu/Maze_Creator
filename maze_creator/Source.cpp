#include <SDL.h>
#include <iostream>
#include <stack>
#include <vector>
#include <thread>
#include <chrono>

enum
{
	CELL_PATH_N = 0x01,			//00001
	CELL_PATH_E = 0x02,			//00010
	CELL_PATH_S = 0x04,			//00100
	CELL_PATH_W = 0x08,			//01000
	CELL_PATH_VISITED = 0x10    //10000
};

int main (int argc, char* argv[])
{
	int nCellUnit = 8; // a cell is a 8*8 px square
	int nScreenWidth = 160 * nCellUnit;
	int nScreenHeight = 100 * nCellUnit;
	srand(time(0));

	int nMazeWidth = 40;
	int nMazeHeight = 25;
	int nVistedCells;
	std::stack<std::pair<int, int>> myStack;
	std::vector<int> maze(nMazeHeight * nMazeWidth, 0);
	int nPathCell = 3; // the side lengeth of a path (cell unit)

	myStack.push(std::make_pair(0, 0));
	maze[0] = CELL_PATH_VISITED;
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
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				bQuit = true;
		}

		// =======DO MAZE ALGORITHM======
		if (nVistedCells < nMazeHeight * nMazeWidth)
		{
			int CurrentPos_x = myStack.top().first;
			int CurrentPos_y = myStack.top().second;
			auto offset = [&](int x, int y) {
				return (myStack.top().second + y) * nMazeWidth + myStack.top().first + x;
			};
			/// Step 1: Create a set of unvisited neighbor
			std::vector<int> neighbors{};
			// Checking north neighbor
			if (myStack.top().second > 0 && ( (maze[offset(0, -1)] & CELL_PATH_VISITED) == 0 )) 
				neighbors.push_back(0);
			// East neighbor
			if (myStack.top().first < nMazeWidth -1 && ( (maze[offset(1, 0)] & CELL_PATH_VISITED) == 0 ))
				neighbors.push_back(1);
			// South neighbor
			if (myStack.top().second < nMazeHeight -1 && ( (maze[offset(0, 1)] & CELL_PATH_VISITED) == 0 ))
				neighbors.push_back(2);
			// West neighbor
			if (myStack.top().first > 0 && ( (maze[offset(-1, 0)] & CELL_PATH_VISITED) == 0 ))
				neighbors.push_back(3);

			if (!neighbors.empty())
			{
				// choose one avaliable neighbor at random
				int next_cell_dir = neighbors[rand() % neighbors.size()];
				switch (next_cell_dir)
				{
				case 0:
					maze[offset(0, 0)] |= CELL_PATH_N;
					maze[offset(0, -1)] |= CELL_PATH_S;
					myStack.push(std::make_pair(CurrentPos_x, CurrentPos_y - 1));
					break;

				case 1: 
					maze[offset(0, 0)] |= CELL_PATH_E;
					maze[offset(1, 0)] |= CELL_PATH_W;
					myStack.push(std::make_pair(CurrentPos_x + 1, CurrentPos_y));
					break;

				case 2:
					maze[offset(0, 0)] |= CELL_PATH_S;
					maze[offset(0, 1)] |= CELL_PATH_N;
					myStack.push(std::make_pair(CurrentPos_x, CurrentPos_y + 1));
					break;

				case 3:
					maze[offset(0, 0)] |= CELL_PATH_W;
					maze[offset(-1, 0)] |= CELL_PATH_E;
					myStack.push(std::make_pair(CurrentPos_x - 1, CurrentPos_y));
					break;
				}

				maze[offset(0, 0)] |= CELL_PATH_VISITED;
				nVistedCells++;
			}
			else
			{
				myStack.pop();
			}

		}


		// ========DRAW SOME STUFF=======
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

				SDL_Rect shuttle = {
					myStack.top().first * (nPathCell + 1)* nCellUnit,
					myStack.top().second * (nPathCell + 1)* nCellUnit,
					nCellUnit * nPathCell,
					nCellUnit * nPathCell
				};

				SDL_SetRenderDrawColor(gRenderer, 241, 90, 34, 255);
				SDL_RenderFillRect(gRenderer, &shuttle);
				
			}
		}
		SDL_RenderPresent(gRenderer);
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	
	return 0;
}