#include <iostream>
#include <ctime>
#include <stack>
#include <conio.h>

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

using namespace std;

const int TabSize = 10;

struct room {
	bool Exist;
	bool Hidden;
	bool Locked;
	bool Last;

	bool Shop;
	bool Boss;

	bool Left;
	bool Right;
	bool Up;
	bool Down;

	bool HiddenLeft;
	bool HiddenRight;
	bool HiddenUp;
	bool HiddenDown;
};

room Tab[TabSize][TabSize];

int CountDoors(room Room) {
	int Doors = 0;

	if (Room.Left)
		Doors++;
	if (Room.Right)
		Doors++;
	if (Room.Up)
		Doors++;
	if (Room.Down)
		Doors++;

	return Doors;
}

void ResetTab(int Tsize, room Tab[TabSize][TabSize]) {
	for (int i = 0;i < Tsize;i++) {
		for (int j = 0;j < Tsize;j++) {
			Tab[i][j].Exist = false;
			Tab[i][j].Hidden = false;
			Tab[i][j].Locked = false;
			Tab[i][j].Last = false;

			Tab[i][j].Shop = false;
			Tab[i][j].Boss = false;

			Tab[i][j].Left = false;
			Tab[i][j].Right = false;
			Tab[i][j].Up = false;
			Tab[i][j].Down = false;

			Tab[i][j].HiddenLeft = false;
			Tab[i][j].HiddenRight = false;
			Tab[i][j].HiddenUp = false;
			Tab[i][j].HiddenDown = false;
		}
	}
}

void AddTunnel(string Direction, float ChanceToConnection, int &CurrentPosX, int &CurrentPosY, room Tab[TabSize][TabSize], int Tsize) {
	int Chances = 1 / ChanceToConnection;
	bool Drawned = false;

	if ((rand() % 100) % Chances == 0)
		Drawned = true;

	if (Drawned) {
		if (Direction == "LEFT" && CurrentPosX > 1 && Tab[CurrentPosY][CurrentPosX - 1].Exist && !Tab[CurrentPosY][CurrentPosX - 1].Hidden) {
			Tab[CurrentPosY][CurrentPosX].Left = true;
			Tab[CurrentPosY][CurrentPosX - 1].Right = true;
		}
		else if (Direction == "RIGHT" && CurrentPosX < Tsize - 1 && Tab[CurrentPosY][CurrentPosX + 1].Exist && !Tab[CurrentPosY][CurrentPosX + 1].Hidden) {
			Tab[CurrentPosY][CurrentPosX].Right = true;
			Tab[CurrentPosY][CurrentPosX + 1].Left = true;
		}
		else if (Direction == "UP" && CurrentPosY > 1 && Tab[CurrentPosY - 1][CurrentPosX].Exist && !Tab[CurrentPosY - 1][CurrentPosX].Hidden) {
			Tab[CurrentPosY][CurrentPosX].Up = true;
			Tab[CurrentPosY - 1][CurrentPosX].Down = true;
		}
		else if (Direction == "DOWN" && CurrentPosY < Tsize - 1 && Tab[CurrentPosY + 1][CurrentPosX].Exist && !Tab[CurrentPosY + 1][CurrentPosX].Hidden) {
			Tab[CurrentPosY][CurrentPosX].Down = true;
			Tab[CurrentPosY + 1][CurrentPosX].Up = true;
		}
	}
}

void DrawTab(int Tsize, room Tab[TabSize][TabSize]) {
	system("cls");

	for (int i = 0;i < Tsize;i++) {
		for (int j = 0;j < Tsize;j++) {
			if (Tab[i][j].Hidden)
				cout << "[" << "H" << "]";
			else if (Tab[i][j].Locked)
				cout << "[" << "L" << "]";
			else if (Tab[i][j].Shop)
				cout << "[" << "S" << "]";
			else if (Tab[i][j].Boss)
				cout << "[" << "B" << "]";
			else if (Tab[i][j].Last)
				cout << "[" << "E" << "]";
			else if (Tab[i][j].Exist)
				cout << "[" << " " << "]";
			else
				cout << " " << " " << " ";

			if (Tab[i][j].Right && Tab[i][j + 1].Left)
				cout << "-";
			else
				cout << " ";
		}
		cout << "\n";
		for (int j = 0;j < Tsize;j++) {
			if (Tab[i][j].Down && Tab[i+1][j].Up)
				cout << " |  ";
			else
				cout << "    ";
		}
		cout << "\n";
	}

	cout << "\n";
	cout << "[ ] - Normal\n";
	cout << "[H] - Hidden\n";
	cout << "[L] - Locked\n";
	cout << "[S] - Shop\n";
	cout << "[B] - Boss\n";
	cout << "[E] - End\n";
}

bool AddSpecialRoom(string Type, float Chance, int &CurrNumber, int MaxNumber, int &PosX, int &PosY) {
	int Temp = 1 / Chance;
	bool Added = false;

	if (CurrNumber < MaxNumber && (rand() % 100) % Temp == 0) {
		if (Type == "SHOP") {
			Added = true;
			Tab[PosY][PosX].Shop = true;
			CurrNumber++;
		}
		else if (Type == "BOSS") {
			Added = true;
			Tab[PosY][PosX].Boss = true;
			CurrNumber++;
		}
		else if (Type == "HIDDEN") {
			Added = true;
			Tab[PosY][PosX].Hidden = true;
			CurrNumber++;
		}
		else if (Type == "LOCKED") {
			Added = true;
			Tab[PosY][PosX].Locked = true;
			CurrNumber++;
		}
	}
	return Added;
}

void UndoRoute(int &PosX, int &PosY, stack<int> &PositionsX, stack<int> &PositionsY) {
	PosX = PositionsX.top();
	PosY = PositionsY.top();

	PositionsX.pop();
	PositionsY.pop();
}

void AddSpecialRooms(float ChanceToBoss, float ChanceToShop, float ChanceToHidden, float ChanceToLocked, 
					 int &CurrNumberOfBosses, int &CurrNumberOfShops, int &CurrNumberOfHidden, int &CurrNumberOfLocked, 
					 int MaxNumberOfBosses, int MaxNumberOfShops, int MaxNumberOfHidden, int MaxNumberOfLocked,
					 int &CurrentPosX, int &CurrentPosY, stack<int>PositionsX, stack<int>PositionsY) {
	bool IsBoss = false;
	bool IsShop = false;
	bool IsHidden = false;
	bool IsLocked = false;

	IsBoss = AddSpecialRoom("BOSS", ChanceToBoss, CurrNumberOfBosses, MaxNumberOfBosses, CurrentPosX, CurrentPosY);

	if (!IsBoss)
		IsShop = AddSpecialRoom("SHOP", ChanceToShop, CurrNumberOfShops, MaxNumberOfShops, CurrentPosX, CurrentPosY);
	else
		IsShop = false;

	if (!IsBoss && !IsShop)
		IsHidden = AddSpecialRoom("HIDDEN", ChanceToHidden, CurrNumberOfHidden, MaxNumberOfHidden, CurrentPosX, CurrentPosY);

	if (!IsBoss && !IsHidden && !IsShop)
		IsLocked = AddSpecialRoom("LOCKED", ChanceToLocked, CurrNumberOfLocked, MaxNumberOfLocked, CurrentPosX, CurrentPosY);

	if (IsHidden || IsLocked)
		UndoRoute(CurrentPosX, CurrentPosY, PositionsX, PositionsY);
}

int main() {
	srand(time(NULL));

	int StartPosX = TabSize / 2;
	int StartPosY = TabSize / 2;

	int CurrentPosX = StartPosX;
	int CurrentPosY = StartPosY;

	int MaxNumberOfRooms = 1;
	int MaxNumberOfHidden = 1;
	int MaxNumberOfShops = 1;
	int MaxNumberOfBosses = 1;
	int MaxNumberOfLocked = 1;

	int CurrNumberOfRooms = 0;
	int CurrNumberOfHidden = 0;
	int CurrNumberOfShops = 0;
	int CurrNumberOfBosses = 0;
	int CurrNumberOfLocked = 0;

	float ChanceToConnection = 0.3f;
	float ChanceToHiddenRoom = 0.1f;
	float ChanceToShop = 0.1f;
	float ChanceToHidden = 0.1f;
	float ChanceToBoss = 0.1f;
	float ChanceToLocked = 0.1f;

	int LoopCounter = 0;

	int Direction = 0;

	stack<int>PositionsX;
	stack<int>PositionsY;

	do{
		cout << "Number of Rooms: ";
		cin >> MaxNumberOfRooms;
		cout << "\nMax number of shops: ";
		cin >> MaxNumberOfShops;
		cout << "\nMax number of bosses: ";
		cin >> MaxNumberOfBosses;
		cout << "\nMax number of hidden: ";
		cin >> MaxNumberOfHidden;
		cout << "\nMax number of locked: ";
		cin >> MaxNumberOfLocked;

		if (MaxNumberOfRooms > (int)TabSize*TabSize*0.9)
			MaxNumberOfRooms = (int)TabSize*TabSize*0.9;
		else if (MaxNumberOfRooms < 2)
			MaxNumberOfRooms = 2;

		ResetTab(TabSize, Tab);

		CurrNumberOfRooms = 1;
		CurrNumberOfBosses = 0;
		CurrNumberOfHidden = 0;
		CurrNumberOfShops = 0;

		CurrentPosX = StartPosX;
		CurrentPosY = StartPosY;

		LoopCounter = 0;

		Tab[CurrentPosY][CurrentPosX].Exist = true;

		PositionsX.push(CurrentPosX);
		PositionsY.push(CurrentPosY);

		do {
			Direction = rand() % 4; //0=Left, 1=Up, 2=Right, 3=Down

			if (Direction == 0 && CurrentPosX > 0 && !Tab[CurrentPosY][CurrentPosX - 1].Exist) { //Left
				Tab[CurrentPosY][CurrentPosX].Left = true;
				Tab[CurrentPosY][CurrentPosX - 1].Right = true;
				Tab[CurrentPosY][CurrentPosX - 1].Exist = true;

				CurrentPosX--;
				CurrNumberOfRooms++;

				AddSpecialRooms(ChanceToBoss, ChanceToShop, ChanceToHidden, ChanceToLocked,
								CurrNumberOfBosses, CurrNumberOfShops, CurrNumberOfHidden, CurrNumberOfLocked,
								MaxNumberOfBosses, MaxNumberOfShops, MaxNumberOfHidden, MaxNumberOfLocked,
								CurrentPosX, CurrentPosY, PositionsX, PositionsY);

				PositionsX.push(CurrentPosX);
				PositionsY.push(CurrentPosY);

				AddTunnel("LEFT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("UP", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("DOWN", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
			}
			else if (Direction == 1 && CurrentPosY > 0 && !Tab[CurrentPosY - 1][CurrentPosX].Exist) { //Up
				Tab[CurrentPosY][CurrentPosX].Up = true;
				Tab[CurrentPosY - 1][CurrentPosX].Down = true;
				Tab[CurrentPosY - 1][CurrentPosX].Exist = true;

				CurrentPosY--;
				CurrNumberOfRooms++;

				AddSpecialRooms(ChanceToBoss, ChanceToShop, ChanceToHidden, ChanceToLocked,
								CurrNumberOfBosses, CurrNumberOfShops, CurrNumberOfHidden, CurrNumberOfLocked,
								MaxNumberOfBosses, MaxNumberOfShops, MaxNumberOfHidden, MaxNumberOfLocked,
								CurrentPosX, CurrentPosY, PositionsX, PositionsY);

				PositionsX.push(CurrentPosX);
				PositionsY.push(CurrentPosY);

				AddTunnel("LEFT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("RIGHT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("UP", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
			}
			else if (Direction == 2 && CurrentPosX < TabSize-1 && !Tab[CurrentPosY][CurrentPosX + 1].Exist) { //Right
				Tab[CurrentPosY][CurrentPosX].Right = true;
				Tab[CurrentPosY][CurrentPosX + 1].Left = true;
				Tab[CurrentPosY][CurrentPosX + 1].Exist = true;

				CurrentPosX++;
				CurrNumberOfRooms++;

				AddSpecialRooms(ChanceToBoss, ChanceToShop, ChanceToHidden, ChanceToLocked,
								CurrNumberOfBosses, CurrNumberOfShops, CurrNumberOfHidden, CurrNumberOfLocked,
								MaxNumberOfBosses, MaxNumberOfShops, MaxNumberOfHidden, MaxNumberOfLocked,
								CurrentPosX, CurrentPosY, PositionsX, PositionsY);

				PositionsX.push(CurrentPosX);
				PositionsY.push(CurrentPosY);

				AddTunnel("RIGHT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("UP", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("DOWN", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
			}
			else if (Direction == 3 && CurrentPosY < TabSize-1 && !Tab[CurrentPosY + 1][CurrentPosX].Exist) { //Down
				Tab[CurrentPosY][CurrentPosX].Down = true;
				Tab[CurrentPosY + 1][CurrentPosX].Up = true;
				Tab[CurrentPosY + 1][CurrentPosX].Exist = true;

				CurrentPosY++;
				CurrNumberOfRooms++;

				AddSpecialRooms(ChanceToBoss, ChanceToShop, ChanceToHidden, ChanceToLocked,
								CurrNumberOfBosses, CurrNumberOfShops, CurrNumberOfHidden, CurrNumberOfLocked,
								MaxNumberOfBosses, MaxNumberOfShops, MaxNumberOfHidden, MaxNumberOfLocked,
								CurrentPosX, CurrentPosY, PositionsX, PositionsY);

				PositionsX.push(CurrentPosX);
				PositionsY.push(CurrentPosY);

				AddTunnel("LEFT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("RIGHT", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
				AddTunnel("DOWN", ChanceToConnection, CurrentPosX, CurrentPosY, Tab, TabSize);
			}
			else if ((Tab[CurrentPosY + 1][CurrentPosX].Exist && Tab[CurrentPosY - 1][CurrentPosX].Exist && Tab[CurrentPosY][CurrentPosX + 1].Exist && Tab[CurrentPosY][CurrentPosX - 1].Exist) || LoopCounter > 5) {
				if (LoopCounter > 5)
					LoopCounter = 0;

				CurrentPosX = PositionsX.top();
				CurrentPosY = PositionsY.top();

				PositionsX.pop();
				PositionsY.pop();
			}
			LoopCounter++;

			if (CurrNumberOfRooms == MaxNumberOfRooms)
				Tab[CurrentPosY][CurrentPosX].Last = true;
		} while (CurrNumberOfRooms < MaxNumberOfRooms);

		DrawTab(TabSize, Tab);

		while (!PositionsX.empty())
			PositionsX.pop();
		while (!PositionsY.empty())
			PositionsY.pop();

		system("PAUSE");
		system("cls");

	} while (1);
}