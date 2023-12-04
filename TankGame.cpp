#include <iostream>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <ctime>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 25;

int GameTime = 0;

enum Dir { 
	None, Up, Down, Right, Left
};

int random(int num) {
	return rand()%num;
}

void gotoxy(int x, int y) {
	cout<< "\x1B[" << y << ";" << x << "H";
}

class Bullets
{
	public:
		Bullets() : x(-1), y(-1), direction(None) {}
		
		float GetPositionX() {
			return x;
		}
		
		float GetPositionY() {
			return y;
		}
		
		Dir GetDirection() {
			return direction;
		}
		
		void SetPositionX (float x) {
			this->x = x;
		}
		
		void SetPositionY (float y) {
			this->y = y;
		}
		
		void SetDirection(Dir direction){
			this->direction = direction;
		}
		
		void Reset(){
			x = -1.0f;
			y = -1.0f;
			direction = None;
		}
		
	private:
		float x, y;
		Dir direction;
};

class Tank
{
	public:
		Tank() : x(0), y(0), direction(Right), health(100){}
		
		Tank(int x, int y) : x(x), y(y) {}
		
		int GetPositionX(){ return x; }
		int GetPositionY(){ return y; }
		Dir GetDirection(){ return direction; }
		int GetHealth(){ return health;}
		
		void SetPositionX(int x) {
			this->x = x;
		}
		
		void SetPositionY(int y) {
			this->y = y;		
		}
		
		void SetDirection(Dir direction) {
			this->direction = direction;
		}
		
		void SetHealth(int health){
			this->health = health;
		}
		
	private:
		int x, y;
		int health;
		Dir direction;
};

class Object {
	public: 
		int x;
		int y;
		
		Object() {
			x = random(WIDTH);
			y = random(HEIGHT);
		}
		
		void Spawn() {
			x = random(WIDTH);
			y = random(HEIGHT);
		}
		
		void Explode() {
			for (int i = 0; i < 3; i++) {
			 	gotoxy(x,y+i); cout << "***";
			}
			Sleep(5);
			for (int i = -1; i < 4; i++) {
			 	if (i == -1 || i == 3) {
			 		gotoxy(x-1,y+i); cout << " *** ";
				} else {
					gotoxy(x-1,y+i); cout << "*   *";
				}
			}
			Sleep(15);
		}
};

class Game
{
	public:
		Game()
		{
			Score = 0;
			Player.SetPositionX(1);
			Player.SetPositionY(1);
			GameOver = false;
		}
		
		void Draw();
		void Move();
		void Run();
		void Logic();
		
	private:
		Tank Player;	
		Bullets bullet[10];
		bool GameOver;
		Object object;
		int Score;
};

void Game::Draw()
{
	std::cout << "\033[?25l";
	
	gotoxy(1,1);
	
	int GunBarrel[2];
	if      (Player.GetDirection() == Left ) {
		GunBarrel[0] = Player.GetPositionX() - 1;
		GunBarrel[1] = Player.GetPositionY();
	} else if (Player.GetDirection() == Right) {
		GunBarrel[0] = Player.GetPositionX() + 1;
		GunBarrel[1] = Player.GetPositionY();
	} else if (Player.GetDirection() == Up   ) {
		GunBarrel[0] = Player.GetPositionX();
		GunBarrel[1] = Player.GetPositionY() - 1;
	} else if (Player.GetDirection() == Down ) {
		GunBarrel[0] = Player.GetPositionX();
		GunBarrel[1] = Player.GetPositionY() + 1;
	}
	
	for (int j = 0; j < HEIGHT; j++)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			if (i == Player.GetPositionX() && j == Player.GetPositionY()) {
				cout << 'O';
			} else if (i == GunBarrel[0] && j == GunBarrel[1]) {
				cout << 'o';
			} else if (i == object.x && j == object.y) {
				cout << 'x';
			}
			else cout << ' ';
		}	
		cout << endl;
	}
	
	cout << endl << endl << "Score: " << Score << endl;
	cout << "Health: " << Player.GetHealth() << endl;
	
	for (int i = 0; i < 10; i++) {
		if (bullet[i].GetDirection() != None){
			gotoxy((int)bullet[i].GetPositionX()+1, (int)bullet[i].GetPositionY()+1);
			cout << 'o';
	    } 
	}
	
	for (int i = 1; i <= HEIGHT+1; i++) {
		gotoxy(WIDTH+1,i);
		cout << 'o';
	}
	for (int i = 1; i <= WIDTH+1; i++) {
		gotoxy(i,HEIGHT+1);
		cout << 'o';
	}
	
	Sleep(10);
//	system("cls");
}

void Game::Logic() {
//	float difer = random(100) >= 50? 1.0 : -1.0;
//	difer *= random(1000)/20000.0;
	
	float difer = 0.0f;
	for (int i = 0; i < 10; i++) {
		if (bullet[i].GetDirection() == Up) {
			bullet[i].SetPositionY(bullet[i].GetPositionY() - 1);
			bullet[i].SetPositionX(bullet[i].GetPositionX() + difer);
		} else if (bullet[i].GetDirection() == Down) {
			bullet[i].SetPositionY(bullet[i].GetPositionY() + 1);
			bullet[i].SetPositionX(bullet[i].GetPositionX() + difer);
		} else if (bullet[i].GetDirection() == Right) {
			bullet[i].SetPositionX(bullet[i].GetPositionX() + 1);
			bullet[i].SetPositionY(bullet[i].GetPositionY() + difer);
		} else if (bullet[i].GetDirection() == Left) {
			bullet[i].SetPositionX(bullet[i].GetPositionX() - 1);
			bullet[i].SetPositionY(bullet[i].GetPositionY() + difer);
		}	
	}
	
	for (int i = 0; i < 10; i++){
		if ((int)bullet[i].GetPositionX() == object.x && (int)bullet[i].GetPositionY() == object.y) {
			object.Explode();
			object.Spawn();
			bullet[i].Reset();
	  		Score++;
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (bullet[i].GetDirection() == None) continue;
		if (bullet[i].GetPositionX() < 0 || bullet[i].GetPositionX() > WIDTH ||
			bullet[i].GetPositionY() < 0 || bullet[i].GetPositionY() > HEIGHT) 
			bullet[i].Reset();
	}
	
//	for (int i = 0; i < 10; i++) {
//		if (bullet[i].GetDirection() == None) continue;
//		if (bullet[i].GetPositionX() < 0)  		bullet[i].SetPositionX(WIDTH);
//		if (bullet[i].GetPositionX() > WIDTH) 	bullet[i].SetPositionX(0);
//		if (bullet[i].GetPositionY() < 0) 		bullet[i].SetPositionY(HEIGHT);
//		if (bullet[i].GetPositionY() > HEIGHT) 	bullet[i].SetPositionY(0);
//	}
	
	for (int i = 0; i < 10; i++) {
		for (int j = i+1; j < 10; j++) {
			if ((int)bullet[i].GetPositionX() == (int)bullet[j].GetPositionX() &&
				(int)bullet[i].GetPositionY() == (int)bullet[j].GetPositionY()){
					bullet[i].Reset();
					bullet[j].Reset();
				}
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (bullet[i].GetPositionX() == Player.GetPositionX() &&
			bullet[i].GetPositionY() == Player.GetPositionY()) {
				bullet[i].Reset();
				Player.SetHealth(Player.GetHealth()-10);
		}
	}
	
	if (Player.GetHealth() <= 0) GameOver = true;
	
	if (GameTime % 50 == 0) {
		object.x += rand()%100 > 50 ? 1 : -1;
		object.y += rand()%100 > 50 ? 1 : -1;
	}
	
	if (object.x >= WIDTH ) object.x = WIDTH-1;
	if (object.x <= 0     ) object.x = 1;
	if (object.y >= HEIGHT) object.y = HEIGHT-1;
	if (object.y <= 0     ) object.y = 1;
}

int count = 0;

void Game::Move()
{
	Dir direction;
	if(kbhit()){
		switch(getch()){
			case 'a' :
				direction = Left;
				if (direction == Player.GetDirection())
					Player.SetPositionX(Player.GetPositionX() - 1);
				else Player.SetDirection(direction);
				break;
			case 'd' :
				direction = Right;
				if (direction == Player.GetDirection())
					Player.SetPositionX(Player.GetPositionX() + 1);
				else Player.SetDirection(direction);
				break;
			case 'w' :
				direction = Up;
				if (direction == Player.GetDirection())
					Player.SetPositionY(Player.GetPositionY() - 1);
				else Player.SetDirection(direction);
				break;
			case 's' :
				direction = Down;
				if (direction == Player.GetDirection())
					Player.SetPositionY(Player.GetPositionY() + 1);
				else Player.SetDirection(direction);
				break;
			case 'j' :
				bullet[count].SetPositionX(Player.GetPositionX());
				bullet[count].SetPositionY(Player.GetPositionY());
				bullet[count].SetDirection(Player.GetDirection());
				if (count == 10) count = 0;
				else count++;
				break;
			case 'q' :
				GameOver = true;
				break;
		}
	}
}

void Game::Run()
{
	while(!GameOver)
	{
		if (GameTime % 5 == 0)
			Game::Move();
			
		Game::Logic();
		Game::Draw();
		
		if (GameTime >= 1000) 
			GameTime = 0; 
		else GameTime++;
	}
}

int main()
{
	srand(time(NULL));
	Game game;
	game.Run();
	return 0;
}