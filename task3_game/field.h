#pragma once
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <intrin.h>
#include <vector>
#include <string>

using namespace std;

enum CellType {
	U = 0, //unknown
	E, //empty
	AE, //another empty
	S, //shooted 
	A, //alive ship
	D, //dead ship
	AS, //allowed ship
	US, //unallowed ship
	UA,  //UnAllowed
	TWUA //there was UA
};

class Point {
public:
	int x;
	int y;
	Point(int x1, int y1) {
		x = x1;
		y = y1;
	}
	Point() {
		x = 0;
		y = 0;
	}
};

class Field {
private:
	CellType field[10][10];
public:
	friend class Player;
	friend class Gamer;
	friend class RandomBot;
	friend class OptimalBot;
	void Draw();
	Field Transform();
	bool KillCheck(Point p);
	void MakeFrame(Point p);
};

