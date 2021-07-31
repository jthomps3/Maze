#include <iostream>
#include <Windows.h>
#include <stack>
#include <cstdlib>
#include <ctime>

struct Coordinate
{
    int x, y;
};

const int mazeRows = 15;
const int mazeColumns = 15;
const char WALL = (char)219;
const char PLAYER = 'P';
const char PATH = ' ';
const char DESTINATION = 'D';

using std::cout;
using std::endl;
using std::stack;

// This function fills the maze with walls
void initializeMaze(char theMaze[][mazeColumns])
{
    for (int row = 0; row < mazeRows; row++)
    {
        for (int column = 0; column < mazeColumns; column++)
        {
            theMaze[row][column] = WALL;
        }
    }
}

// This function randomly chooses a start location for the player
void startLocation(char theMaze[][mazeColumns], Coordinate& playerStart)
{
    do
    {
        playerStart.x = (rand() % mazeRows + 1);
        playerStart.y = (rand() % mazeColumns + 1);
    } while (playerStart.x % 2 == 0 || playerStart.y % 2 == 0 || playerStart.x == 0 || playerStart.x == mazeRows ||
        playerStart.y == 0 || playerStart.y == mazeColumns);
}

// This function works in conjuction with function digPath to determine if the random path passed is valid
bool isMoveValid(Coordinate testedLocation, char theMaze[][mazeColumns])
{
    if (testedLocation.x <= 0 || testedLocation.y <= 0 || testedLocation.x >= mazeColumns || testedLocation.y >= mazeRows ||
        theMaze[testedLocation.x][testedLocation.y] == PATH || theMaze[testedLocation.x][testedLocation.y] == PLAYER)
    {
        return false;
    }
    return true;
}

// This function, beginning at the starting point, digs paths to create the maze
void digPath(char theMaze[][mazeColumns], Coordinate playerStart)
{
    Coordinate currentLocation = playerStart;
    Coordinate START = currentLocation;
    Coordinate UP, DOWN, LEFT, RIGHT;
    stack <Coordinate> cellLocation;
    int travelDirection;

    do
    {
        // Check the 4 possible directions from the currentLocation.
        // For each possible direction see if it has been visited and if
        //    it is within the grid boundaries.

        UP.x = currentLocation.x - 2;
        UP.y = currentLocation.y;
        DOWN.x = currentLocation.x + 2;
        DOWN.y = currentLocation.y;
        LEFT.x = currentLocation.x;
        LEFT.y = currentLocation.y - 2;
        RIGHT.x = currentLocation.x;
        RIGHT.y = currentLocation.y + 2;

        bool isMoveAvailable = false;
        bool upValid = false;
        bool downValid = false;
        bool rightValid = false;
        bool leftValid = false;

        if (isMoveValid(UP, theMaze))
        {
            isMoveAvailable = true;
            upValid = true;
        }
        if (isMoveValid(DOWN, theMaze))
        {
            isMoveAvailable = true;
            downValid = true;
        }
        if (isMoveValid(LEFT, theMaze))
        {
            isMoveAvailable = true;
            leftValid = true;
        }
        if (isMoveValid(RIGHT, theMaze))
        {
            isMoveAvailable = true;
            rightValid = true;
        }

        if (isMoveAvailable)
        {
            // Push the currentLocation onto the stack
            cellLocation.push(currentLocation);

            // Move randomly in one of the valid directions
            bool moveSuccessfull = false;
            while (!moveSuccessfull)
            {
                travelDirection = (rand() % 4 + 1);
                if (travelDirection == 1 && upValid)
                {
                    theMaze[currentLocation.x - 1][currentLocation.y] = PATH;
                    currentLocation.x = currentLocation.x - 2;
                    theMaze[currentLocation.x][currentLocation.y] = PATH;
                    moveSuccessfull = true;
                }
                else if (travelDirection == 2 && downValid)
                {
                    theMaze[currentLocation.x + 1][currentLocation.y] = PATH;
                    currentLocation.x = currentLocation.x + 2;
                    theMaze[currentLocation.x][currentLocation.y] = PATH;
                    moveSuccessfull = true;
                }
                else if (travelDirection == 3 && leftValid)
                {
                    theMaze[currentLocation.x][currentLocation.y - 1] = PATH;
                    currentLocation.y = currentLocation.y - 2;
                    theMaze[currentLocation.x][currentLocation.y] = PATH;
                    moveSuccessfull = true;
                }
                else if (travelDirection == 4 && rightValid)
                {
                    theMaze[currentLocation.x][currentLocation.y + 1] = PATH;
                    currentLocation.y = currentLocation.y + 2;
                    theMaze[currentLocation.x][currentLocation.y] = PATH;
                    moveSuccessfull = true;
                }
            }
        }
        else
        {
            if (!cellLocation.empty())
            {
                currentLocation = cellLocation.top();
                cellLocation.pop();
            }
        }

    } while (!cellLocation.empty());
}

// This function randomly designates a coordinate in the maze to be used as the destination (end)
void setDestination(char theMaze[][mazeColumns])
{
    Coordinate destination;

    do
    {
        destination.x = (rand() % mazeRows + 1);
        destination.y = (rand() % mazeColumns + 1);
    } while (destination.x % 2 == 0 || destination.y % 2 == 0 || destination.x == 0 || destination.x == mazeRows ||
        destination.y == 0 || destination.y == mazeColumns || theMaze[destination.x][destination.y] == WALL ||
        theMaze[destination.x][destination.y] == PLAYER);

    theMaze[destination.x][destination.y] = DESTINATION;
}

// This function determines what key the user pressed
int getKey() {
    int result = 0;
    while (result == 0)
    {
        short MAX_SHORT = 0x7FFF;
        if (GetAsyncKeyState(VK_LEFT) & MAX_SHORT)
        {
            result = VK_LEFT;
        }
        else if (GetAsyncKeyState(VK_UP) & MAX_SHORT)
        {
            result = VK_UP;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & MAX_SHORT)
        {
            result = VK_RIGHT;
        }
        else if (GetAsyncKeyState(VK_DOWN) & MAX_SHORT)
        {
            result = VK_DOWN;
        }
    }
    return result;
}

// This function is used in conjunction with getKey to move the player through the maze and determine whether
// or not the maze has been completed
void playGame(Coordinate playerStart, char theMaze[][mazeColumns])
{
    int key = getKey();
    COORD oldPosition = { playerStart.y, playerStart.x };
    COORD newPosition = { playerStart.y, playerStart.x };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPosition);
    while (key != VK_ESCAPE)
    {
        oldPosition = newPosition;
        if (key == VK_LEFT && theMaze[newPosition.Y][newPosition.X - 1] != WALL)
        {
            newPosition.X--;
        }
        else if (key == VK_RIGHT && theMaze[newPosition.Y][newPosition.X + 1] != WALL)
        {
            newPosition.X++;
        }
        else if (key == VK_UP && theMaze[newPosition.Y - 1][newPosition.X] != WALL)
        {
            newPosition.Y--;
        }
        else if (key == VK_DOWN && theMaze[newPosition.Y + 1][newPosition.X] != WALL)
        {
            newPosition.Y++;
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldPosition);
        cout << PATH;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPosition);
        if (theMaze[newPosition.Y][newPosition.X] == DESTINATION)
        {
            cout << "Maze Complete!";
            exit(0);
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
            cout << PLAYER;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPosition);
            key = getKey();
        }
    }
}


void main()
{
    // Create 2D array to hold maze
    char theMaze[mazeRows][mazeColumns];

    // Call initializeMaze to fill maze with walls
    initializeMaze(theMaze);

    // Seed random number generator
    srand((unsigned int)time(0));

    // Randomly select start location
    Coordinate playerStart;
    startLocation(theMaze, playerStart);

    // Dig out paths
    digPath(theMaze, playerStart);

    // Set the player marker within the maze array
    theMaze[playerStart.x][playerStart.y] = PLAYER;

    // Call the function to randomly designate an end to the maze
    setDestination(theMaze);

    // Write out maze to the console
    for (int row = 0; row < mazeRows; row++)
    {
        for (int column = 0; column < mazeColumns; column++)
        {
            // Colors changed for the destination, player and walls
            if (theMaze[row][column] == DESTINATION)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            }
            else if (theMaze[row][column] == PLAYER)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            }
            cout << theMaze[row][column];
        }
        cout << endl;
    }

    // Initialize playing of the game
    playGame(playerStart, theMaze);
}