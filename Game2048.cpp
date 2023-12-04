#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <iomanip>

using namespace std;

enum Dir { No, Up, Down, Right, Left };

const int Max = 4;
const int Min = 0;

bool GameOver;
int a[Max][Max] = {0};

void DrawBoard();
void RandomSpawn();
void Slide();
void Merge();
void Reverse();
void Rotate(int n);
void Logic(int Direction);
void Move();
void SetUp();
bool IsWin();
bool IsLose();

int main()
{
	srand(time(NULL));
	
	SetUp();
	
	while (!GameOver)
	{
		Move();
		DrawBoard();
		
		if(IsWin()){
			cout << "You Win";
			Sleep(500);
			GameOver = true;
			break;
		} else if (IsLose()) {
			cout << "You Lose";
			Sleep(500);
			GameOver = true;
			break;
		}
		Sleep(500);
	}
	
	return 0;
}

void DrawBoard()
{
	system("cls");
//	cout << "\x1B[103m";
//	cout << "\x1B[31m";
	cout << endl << endl;
	for (int i = 0; i < Max; i++)
	{
		for (int j = 0; j < Max; j++)
		{
			if (a[i][j] == 0) cout << '.' << setw(8);
			else cout << a[i][j] << setw(8);
		}
		cout << endl << endl << endl;
	}
}

void RandomSpawn()
{
	int randomX = rand()%4;
	int randomY = rand()%4;
	int randomNum = rand()%100;
	while (a[randomX][randomY] != 0) 
	{
		randomX = rand()%4;
		randomY = rand()%4;
	}
	a[randomX][randomY] = randomNum > 9 ? 2 : 4;
}

void Slide()
{
	for (int i = 0; i < 4; i++) {
		int b[4] = {0}; int count = 0;
		for (int j = 0; j < 4; j++){
			if (a[i][j] != 0) b[count++] = a[i][j];
		}
			
		for (int j = 0; j < 4; j++){
			if (b[j] != 0) a[i][j] = b[j];
			else a[i][j] = 0;
		}
	}
}

void Merge()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++){
			if (a[i][j] == a[i][j+1]) {
				a[i][j]*= 2; 
				a[i][j+1] = 0;
			}
		}
	}
}

void Reverse() {
	int b[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			b[i][j] = a[i][j];
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			a[i][j] = b[i][3-j];
		}
	}
}

void Rotate(int n) {
	int b[16]; int count = 0;
	if (n == 1){
		for (int j = 3; j >= 0; j--){
			for (int i = 0; i < 4; i++){
				b[count++] = a[i][j];
			}
		}
	} else if (n == -1) {
		for (int j = 0; j < 4; j++){
			for (int i = 3; i >= 0; i--){
				b[count++] = a[i][j];
			}
		}		
	}
	
	count = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			a[i][j] = b[count++];
		}
	}
}

void Logic(int Direction) {
	if (Direction == Left) {
		Slide();
		Merge();
		Slide();
	}
	
	if (Direction == Right) {
		Reverse();
		Slide();
		Merge();
		Slide();
		Reverse();
	}
	
	if (Direction == Up)  {
		Rotate(1);
		Slide();
		Merge();
		Slide();
		Rotate(-1);
	}
	
	if (Direction == Down) {
		Rotate(-1);
		Slide();
		Merge();
		Slide();
		Rotate(1);
	}
	
	if (Direction != No) RandomSpawn();
}

void Move() {
	if (_kbhit()) {
		switch(_getch()) {
			case 'q' : GameOver = true;
				break;
			case 'a' : Logic(Left);
				break;
			case 'd' : Logic(Right);
				break;
			case 's' : Logic(Down);
				break;
			case 'w' : Logic(Up);
				break;
		}
	}
}

void SetUp() {
	RandomSpawn();
	RandomSpawn();
	GameOver = false;
}

bool IsWin(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (a[i][j] >= 2048) return true;
		}
	}
	return false;
}

bool IsLose(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (a[i][j] == 0) return false;
			if (a[i][j] == a[i+1][j] && i != 3) return false;
			if (a[i][j] == a[i][j+1] && j != 3) return false;
		}
	}
	return true;
}