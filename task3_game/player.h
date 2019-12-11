#pragma once
#include "field.h"

class Player {
protected:
	void FreeField();
	int aliveShips;
	Field field;
	char type = 0;
public:
	virtual void GenerateField() = 0;
	virtual Point MakeTurn(Field field) = 0;
	Player() = default;
	Field Transform();
	bool ProcessTurn(Point shoot);
	int GetAlive() { return aliveShips; }
	char Type() {
		return type;
	}
};

class Gamer : public Player {
public:
	void GenerateField() override;
	Gamer();
	Point MakeTurn(Field field) override;
};

class RandomBot : public Player {
public:
	void GenerateField() override;
	RandomBot();
	Point MakeTurn(Field field) override;
};

class OptimalBot : public Player {
private:
	Point KillCheck(Field enemys);
public:
	void GenerateField() override;
	OptimalBot();
	Point MakeTurn(Field field) override;
};
