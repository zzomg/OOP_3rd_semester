#include "stdafx.h"
#include "field.h"

void PrintCell(CellType c) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (c) {
	case U:
		cout << " ";
		break;
	case E:
		cout << ".";
		break;
	case S:
		cout << "X";
		break;
	case A:
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
		cout << "*";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case D:
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
		cout << "*";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case AE:
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 5));
		cout << ".";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case AS:
		SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 4));
		cout << " ";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case TWUA:
		SetConsoleTextAttribute(hConsole, (WORD)((4 << 4) | 4));
		cout << " ";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case US:
		SetConsoleTextAttribute(hConsole, (WORD)((4 << 4) | 4));
		cout << " ";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	case UA:
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 5));
		cout << ".";
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		break;
	}
}

void Field::Draw() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 1));
	cout << "   A B C D E F G H I J   " << endl;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	for (int i = 0; i < 10; i++) {
		SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 1));
		cout << " " << i;
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		for (int j = 0; j < 10; j++) {
			cout << " ";
			PrintCell(field[i][j]);
		}
		cout << " ";
		SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 7));
		cout << "  " << endl;
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	}
	SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 7));
	cout << "                         " << endl;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
}

bool Field::KillCheck(Point p) {
	if (field[p.y][p.x] != D) return false;
	else {
		int i = 0;
		bool killed = false, dd[4] = { true };
		Point ship[5] = { Point(-1, -1), Point(-1, -1), Point(-1, -1), Point(-1, -1), Point(-1, -1) };
		//up
		for (i = 0; i < 4; i++) {
			if (p.y - i >= 0 && field[p.y - i][p.x] == D) dd[0] = true;
			else if (p.y - i >= 0 && field[p.y - i][p.x] == A) {
				dd[0] = false;
				break;
			}
			else {
				break;
			}
		}
		i--;
		if (dd[0]) {
			for (; i >= 0; i--) ship[i] = Point(p.x, p.y - i);
		}
		//down
		for (i = 0; i < 4; i++) {
			if (p.y + i < 10 && field[p.y + i][p.x] == D) dd[1] = true;
			else if (p.y + i < 10 && field[p.y + i][p.x] == A) {
				dd[1] = false;
				break;
			}
			else {
				break;
			}
		}
		i--;
		if (dd[1]) {
			for (; i >= 0; i--) ship[i] = Point(p.x, p.y + i);
		}
		//left
		for (i = 0; i < 4; i++) {
			if (p.x - i >= 0 && field[p.y][p.x - i] == D) dd[2] = true;
			else if (p.x - i >= 0 && field[p.y][p.x - i] == A) {
				dd[2] = false;
				break;
			}
			else {
				break;
			}
		}
		i--;
		if (dd[2]) {
			for (; i >= 0; i--) ship[i] = Point(p.x - i, p.y);
		}
		//right
		for (i = 0; i < 4; i++) {
			if (p.x + i < 10 && field[p.y][p.x + i] == D) dd[3] = true;
			else if (p.x + i < 10 && field[p.y][p.x + i] == A) {
				dd[3] = false;
				break;
			}
			else {
				break;
			}
		}
		i--;
		if (dd[3]) {
			for (; i >= 0; i--) ship[i] = Point(p.x + i, p.y);
		}
		if (dd[0] && dd[1] && dd[2] && dd[3]) {
			for (i = 0; ship[i].x >= 0; i++) MakeFrame(ship[i]);
		}
	}
	return false;
}

Field Field::Transform() {
	Field res;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			res.field[i][j] = field[i][j];
			if (field[i][j] == A) res.field[i][j] = U;
			if (field[i][j] == E) res.field[i][j] = U;
			if (field[i][j] == AE) res.field[i][j] = E;
			if (field[i][j] == UA) res.field[i][j] = U;
		}
	}
	return res;
}

void Field::MakeFrame(Point p) {
	for (int i = p.y - 1; i < p.y + 2; i++) {
		for (int j = p.x - 1; j < p.x + 2; j++) {
			if (j >= 0 && j < 10 && i >= 0 && i < 10 && (field[i][j] == UA || field[i][j] == E)) field[i][j] = AE;
		}
	}
}

void Field::Free() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = E;
		}
	}
}
