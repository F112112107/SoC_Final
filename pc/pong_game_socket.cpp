#include <iostream>
#include <Windows.h>

#include "numbers.h"

#define WIDTH 50
#define HEIGHT 30

#define PLAYER_1_BAR_X 4
#define PLAYER_2_BAR_X WIDTH*2-6

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Player {
    class Bar {
        private:
        	int x;
            int y = HEIGHT / 2  - 2 ;
            
        public:
            void setX(int x) {
            	this->x = x;
			}
            int getX() {
            	return x;
			}
            int getY() {
            	return y;
			}
			int setY(int y) {
				this->y = y;
			}
    };
    
	private:
	    Bar bar;
	    int score = 0;
	        
    public:
    	Player(int x) {
    		bar.setX(x);
		}
        int getScore() {
        	return score;
		}
		void setScore(int score) {
			this->score = score;
		}
    	int getBarYtop() {
    		return bar.getY() - 2 + 2;
		}
		int getBarYbottom() {
			return bar.getY() + 2 + 2;
		}
        void setBarY(int y) {
        	bar.setY(y);
		}
};


Player player1(PLAYER_1_BAR_X);
Player player2(PLAYER_2_BAR_X);

class Ball{
	private:
		double x = 1, y = 1;	
	public:
		int getX() {
			return x;
		}
		int getY() {
			return y;
		}
		void setX(int x) {
			this->x = x;
		}
		void setY(int y) {
			this->y = y;
		}
};

Ball ball;

void gotoxy(int xpos, int ypos) {
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void drawBar() {
	for (int i=1; i<HEIGHT-1; i++) {
		gotoxy(PLAYER_1_BAR_X, i);
		if (i >= player1.getBarYtop() && i <= player1.getBarYbottom()) {
			cout << "¢i";
		} else {
			cout << "  ";
		}
	}
	for (int i=1; i<HEIGHT-1; i++) {
		gotoxy(PLAYER_2_BAR_X, i);
		if (i >= player2.getBarYtop() && i <= player2.getBarYbottom()) {
			cout << "¢i";
		} else {
			cout << "  ";
		}
	}
}

void drawBoundary(int width, int height) {
	gotoxy(0, 0);
	for (int i=0; i<width; i++) { // top
		cout << "¢i";
	}
	gotoxy(0, HEIGHT - 1);
	for (int i=0; i<width; i++) { // bottom
		cout << "¢i";
	}
}

void clearBall() {
	gotoxy(ball.getX(), ball.getY());
	cout << "  ";
}

void drawBall() {
	gotoxy(ball.getX(), ball.getY());
	cout << "¢i";
}

void drawScore() {
	int x = 10, y = 2;
	for (int i = 0; i < 15; i++) {
		if (i % 3 == 0) {
			gotoxy(x, ++y);
		}
		if (number[player1.getScore()][i] == 1) {
			cout << "¢i";
		} else {
			cout << "  ";
		}
	}
	
	x = WIDTH * 2 - 20, y = 2;
	for (int i = 0; i < 15; i++) {
		if (i % 3 == 0) {
			gotoxy(x, ++y);
		}
		if (number[player2.getScore()][i] == 1) {
			printf("¢i");
		} else {
			printf("  ");
		}
	}
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return -1;
    }
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error binding socket." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cerr << "Error listening for connections." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Server is listening for connections..." << endl;

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error accepting connection." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Connection established." << endl;
    char buffer[1024];
    int bytesRead;
    
    /* Game init begin */
    system("mode 100, 32");
    drawBoundary(WIDTH, HEIGHT);
    /* Game init end */ 

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }
        buffer[bytesRead] = '\0';
        
        /* Game loop begin */
        int bx, by, p1y, p2y, p1s, p2s;
        sscanf(buffer, "%d,%d,%d,%d,%d,%d", &bx, &by, &p1y, &p2y, &p1s, &p2s);
        
        clearBall();
        
        ball.setX(bx);
        ball.setY(by);
        player1.setBarY(p1y);
        player2.setBarY(p2y);
        player1.setScore(p1s);
        player2.setScore(p2s);
        
        drawBar();
        drawBall();
        drawScore();
        /* Game loop end */
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    cout << "Server closed." << endl;
}

