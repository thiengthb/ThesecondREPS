#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

bool GameOver;
int SnakeX[100], SnakeY[100], FoodX, FoodY;
int Xspeed, Yspeed;

int num;

int  random(int num);
void gotoxy(int x, int y);
void Setup();
void Move();
void Logic();
void Update();
void Draw();

void FoodSpawn();

int main() {
	srand(time(NULL));
	
	Setup();
	
	while (!GameOver) {
		Move();
		Update();
		Logic();	
		Draw();
		Sleep(25);
	}
	
	return 0;
}

int random(int num) {
	return rand()%num;
}

void gotoxy(int x, int y) {
	cout<< "\x1B[" << y << ";" << x << "H";
}

void Setup() {
	SnakeX[0] = 3; SnakeY[0] = 1;
	SnakeX[1] = 2; SnakeY[1] = 1;
	SnakeX[2] = 1; SnakeY[2] = 1; 
	Xspeed = 1; Yspeed = 0;
	
	FoodX = random(WIDTH-1)+1;
	FoodY = random(HEIGHT-1)+1;
	
	GameOver = false;
	num = 3;
}

void Move() {
	if (kbhit()) {
		switch(getch()) {
			case 'a' : 
				if (Yspeed != 0) { Xspeed = -1; Yspeed = 0;}
				break;
			case 'd' : 
				if (Yspeed != 0) { Xspeed = +1; Yspeed = 0;}
				break;
			case 'w' : 
				if (Xspeed != 0) { Xspeed = 0; Yspeed = -1;}
				break;
			case 's' : 
				if (Xspeed != 0) { Xspeed = 0; Yspeed = +1;}
				break;
			case 'q' : GameOver = true;
				break;
		}
	}
}

void Logic() {
	// Eat Food
	if (SnakeX[0] == FoodX && SnakeY[0] == FoodY) {
		num++;
		FoodX = random(WIDTH-1)+1;
    	FoodY = random(HEIGHT-1)+1;
	}
	
	// When the head touch the body then game over
	for (int i = 1; i < num; i++) {
		if (SnakeX[0] == SnakeX[i] && SnakeY[0] == SnakeY[i]) 
			GameOver = true;
	}
	
	// When to the border goto the opposite
	if (SnakeX[0] <= 1) SnakeX[0] = WIDTH-1;
	if (SnakeY[0] <= 1) SnakeY[0] = HEIGHT-1;
	if (SnakeX[0] >= WIDTH)  SnakeX[0] = 1;
	if (SnakeY[0] >= HEIGHT) SnakeY[0] = 1;
	
}

void Update() {
	for (int i = num-1; i >= 1; i--) {
		SnakeX[i] = SnakeX[i-1];
		SnakeY[i] = SnakeY[i-1];
	}
	SnakeX[0]+= Xspeed; 
	SnakeY[0]+= Yspeed;
}

void Draw() {
	// Hide the cursor
	cout << "\033[?25l";
	//clear screen 
	system("cls");  
	
	// Draw the Boarder
	for (int i = 1; i <= WIDTH; i++) {
		gotoxy(i,HEIGHT); cout << '.';
	}
	
	for (int i = 1; i <= HEIGHT; i++) {
		gotoxy(WIDTH,i); cout << '.';
	}
	
	for (int i = 1; i <= WIDTH; i++) {
		gotoxy(i,1); cout << '.';
	}
	
	for (int i = 1; i <= HEIGHT; i++) {
		gotoxy(1,i); cout << '.';
	}
	
	// Draw the Snake
	
		// Draw the snake Head 
		gotoxy(SnakeX[0], SnakeY[0]); cout << 'O';
		
		// Draw the tail
		for (int i = 1; i < num; i++) {
			gotoxy(SnakeX[i], SnakeY[i]);
			cout << 'o';
		}
	
	// Draw the Food
	gotoxy(FoodX, FoodY); cout << '*';
	
	// Draw the Score
	gotoxy(1,HEIGHT+1); cout << "Score: " << (num-3)*10;
	
}