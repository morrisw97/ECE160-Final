#include <iostream>
#include <fstream>
#include <string>


using namespace std;

int hasKey = 0;
int gameOver = 0;
int playerWon = 0;
int playerLost = 0;

class Square {
	char type; // 'w' for wall, 'o' for open, 'e' for exit
	int open;
	int seen;
	char object; // 'k' for key, 'n' for none
public:
	Square()
	{
		type = 'o';
		open = 1;
		seen = 0;
		object = 'n';
	}
	char getType()
	{
		return type;
	}
	void setType(char tt)
	{
		type = tt;
	}
	int isOpen()
	{
		return open;
	}
	void setOpen(int oo)
	{
		open = oo;
	}
	int isSeen()
	{
		return seen;
	}
	char getObject()
	{
		return object;
	}
	void setObject(char obj)
	{
		object = obj;
	}
	void setSeen(int ss)
	{
		seen = ss;
	}
};

Square board[21][21];

class Character { // Parent class for characters player and enemy
	int posX;
	int posY;
public:
	virtual void move() = 0; // Virtual function that is overwritten by Player and Enemy
};

class Player : public Character { // Player class
	int posX;
	int posY;
public:
	void move()
	{
		char dir;
		int valid = 0;
		int targetX;
		int targetY;
        	//cout << "Player's position: X=" << posX << ", Y=" << posY << endl; // Debug purposes
		
		while (valid == 0)
		{
	                cout << "Input a direction to move (A for left, D for right, W for up, S for down): ";
	                cin >> dir;

			if (dir == 'A' || dir == 'a') // Left
			{
				valid = 1;
				targetX = posX - 1;
				targetY = posY;
			}
			else if (dir == 'D' || dir == 'd') // Right
			{
				valid = 1;
				targetX = posX + 1;
				targetY = posY;
			}
			else if (dir == 'W' || dir == 'w' ) // Up
			{
				valid = 1;
				targetY = posY - 1;
				targetX = posX;
			}
			else if (dir == 'S' || dir == 's') // Down
			{
				valid = 1;
				targetY = posY + 1;
				targetX = posX;
			}
			else if (dir == 'X' || dir == 'x') // Exit from game (debug purposes)
			{
				valid = 1;
				cout << "Quitting game..." << endl;
				throw exception();
			}
			else
			{
				cout << "Invalid Input" << endl;
			}
		}

		if (board[targetX][targetY].isOpen()) // If target is a valid square
		{
			posX = targetX;
			posY = targetY;

			if (board[posX][posY].getObject() == 'k')
			{
				hasKey = 1;
				board[posX][posY].setObject('n');
			}
			if (board[posX][posY].getType() == 'e' && hasKey == 1)
			{
				gameOver = 1;
				playerWon = 1;
				return;
			}
		}
		else // Invalid square
		{
			cout << "You can't go that way! You lose a turn!" << endl;
			cout << endl;
			cout << endl;
		}


		for (int i = posX - 1; i < posX + 2; i++) // Updates the player's vision
		{
			for (int j = posY - 1; j < posY + 2; j++)
			{
				board[i][j].setSeen(1);
			}
		}

		cout << endl;
		cout << endl;
	}

	void setX(int x)
	{
		posX = x;
	}

	void setY(int y)
	{
		posY = y;
	}

	int getX()
	{
		return posX;
	}
	int getY()
	{
		return posY;
	}

};

Player p;

class Enemy : public Character { // Enemy class
        int posX;
        int posY;
	int targetX;
	int targetY;
	char currentDir = 'd';
public:
	void checkDead() // Checks if the player and the enemy are on the same square
        {
                if (posX == p.getX() && posY == p.getY())
		{
                        gameOver = 1;
			playerLost = 1;
			return;
		}
		else
                        return;
        }
	int checkDir(char c)
	{
		        if (c == 'u')
                        {
                                if (board[posX][posY-1].isOpen())
					return 1;
				else
					return 0;
                        }
                        else if (c == 'd')
                        {
                                if (board[posX][posY+1].isOpen())
                                        return 1;
                                else
                                        return 0;
                        }
                        else if (c == 'l')
                        {
                                if (board[posX-1][posY].isOpen())
                                        return 1;
                                else
                                        return 0;
                        }
                        else if (c == 'r')
                        {
                                if (board[posX+1][posY].isOpen())
                                        return 1;
                                else
                                        return 0;
                        }
                        else
                        {
                                cout << "Invalid Direction Check: " << c << endl;
                                cout << endl;
                                cout << endl;
                        }

	}
	char findDir() // Finds an open direction for the enemy to move
	{
		if (checkDir(currentDir) == 1)
			return currentDir;
		else if (currentDir == 'u' || currentDir == 'd')
		{
			if (checkDir('l'))
				return 'l';
			else if (checkDir('r'))
				return 'r';
			else if (checkDir('u'))
				return 'u';
			else if (checkDir('d'))
				return 'd';
			else
				cout << "Something broke in findDir for currentDir = " << currentDir << endl;

		}
		else if (currentDir == 'l' || currentDir == 'r')
		{
			if (checkDir('u'))
                                return 'u';
                        else if (checkDir('d'))
                                return 'd';
                        else if (checkDir('l'))
                                return 'l';
                        else if (checkDir('r'))
                                return 'r';
                        else
                                cout << "Something broke in findDir for currentDir = " << currentDir << endl;
		}
		else
			cout << "Something broke in findDir for currentDir = " << currentDir << endl;

	}
        void move()
        {
		checkDead();
		char c = findDir();
		currentDir = c;
		if (c == 'u')
		{
			targetX = posX;
			targetY = posY-1;
		}
		else if (c == 'd')
		{
			targetX = posX;
			targetY = posY+1;
		}
		else if (c == 'l')
		{
			targetX = posX-1;
			targetY = posY;
		}
		else if (c == 'r')
		{
			targetX = posX+1;
			targetY = posY;
		}
		else
		{
			cout << "Enemy move broken" << endl;
			cout << endl;
			cout << endl;
		}
		posX = targetX;
		posY = targetY;
		checkDead();
	}
	int getX()
	{
		return posX;
	}
	int getY()
	{
		return posY;
	}
	void setX(int x)
	{
		posX = x;
	}
	void setY(int y)
	{
		posY = y;
	}
};

Enemy e;

void importMap(string mapName) // Loading the map from a text file with a 21x21 maze
{
	ifstream map(mapName); // Begins reading from map text file
	char c;
	
	for (int i = 0; i < 21; i++) // Loops through map text file, and assumes it's 21x21
	{
		for (int j = 0; j < 21; j++)
		{
			map.get(c);
			//cout << c; // Used for debug purposes
			//board[i][j].setSeen(1); // Used for debug purposes
			if (c == 'X') // Wall
			{
				board[i][j].setType('w');
				board[i][j].setOpen(0);	
			}
			else if (c == ' ') // Open
			{
				board[i][j].setType('o');
				board[i][j].setOpen(1);
			}
			else if (c == 'k') // Key
			{
				board[i][j].setType('o');
				board[i][j].setObject('k');
			}
			else if (c == 'E') // Exit
			{
				board[i][j].setType('e');
			}
			else if (c == 'p') // Player marker
			{
				board[i][j].setType('o');
				p.setX(i);
				p.setY(j);
		                for (int m = i - 1; m <= i + 1; m++) // Sets up initial seen squares
                		{
                     		   	for (int n = j - 1; n <= j + 1; n++)
                        		{
	      	          	                board[m][n].setSeen(1);
        		                }
	        	        }

			}
			else if (c == 'm') // Minotaur marker
			{
				board[i][j].setType('o');
				e.setX(i);
				e.setY(j);
			}
			else // Bad character in map
			{
				cout << "Invalid Character in map at x=" << i << ", y=" << j << endl;
				return;
			}
		}
		map.get(c);
		//cout << c; // Used for debug purposes
	}
	map.close();

}

void drawBoard() // Prints out entire visible maze
{
	for (int j = 0; j < 21; j++)
	{
		for (int i = 0; i < 21; i++)
		{
			if (board[i][j].isSeen() == 1) // If player has mapped this square
			{
				char type = board[i][j].getType(); // Makes calls shorter
				char obj = board[i][j].getObject();

				if (type == 'w') // Wall
					cout << 'X';
				else if (type == 'o') // Non-Wall
				{
					if(p.getX() == i && p.getY() == j) // Player position
						cout << 'p';
					else if (e.getX() == i && e.getY() == j) // Enemy position
						cout << 'm';
					else if (obj == 'k') // Key
						cout << 'k';
					else // Empty space
						cout << ' ';
				}
				else if (type == 'e') // Exit
					cout << 'E';
			}
			else // Doesn't display unseen squares
			{
				cout << ' ';
			}
		}
		cout << endl;
	}
}


int main()
{
	cout << "Welcome to the Labyrinth." << endl;
	cout << endl; 
	cout << "Explore the maze to reveal more if it." << endl;
	cout << endl;
	cout << "Find the key and reach the exit. Don't get eaten by the minotaur." << endl;
	cout << endl;
	cout << "Good Luck!" << endl;
	importMap("map1.txt");

	cout << endl;
	cout << endl;

	drawBoard();

	cout << endl;
	cout << endl;
	
	// cout << "Player's starting position: X=" << p.getX() << ", Y=" << p.getY() << endl;
	cout << endl;

	while(!gameOver) // While game still going
	{
		p.move();
		e.move();
		drawBoard();
	}
	
	if (playerWon)
	{
		cout << endl;
		cout << "You escaped the maze! Congratulations!" << endl;
		cout << endl;
	}
	if (playerLost)
	{
		cout << "You were too slow, and the Minotaur ate you. That's a damn shame." << endl;
	}

	cout << "Re-run the .exe to play again." << endl;

	return 0;
}


