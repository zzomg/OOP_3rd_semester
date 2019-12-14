#pragma once
#include "stdafx.h"
#include "field.h"

enum class PlayerType {
	Human,
	Bot
};

class Player {
public:
	virtual ~Player() = default;
	virtual void GenerateField(Field &field) = 0;
	virtual Point MakeTurn(Field field) = 0;
	virtual PlayerType Type() = 0;


};

class Gamer : public Player {
public:
	void GenerateField(Field &field) override;
	Gamer();
	Point MakeTurn(Field field) override;
	PlayerType Type() override { return PlayerType::Human; }
};

class RandomBot : public Player {
public:
	void GenerateField(Field &field) override;
	RandomBot();
	Point MakeTurn(Field field) override;
	PlayerType Type() override { return PlayerType::Bot; }

};

class OptimalBot : public Player {
private:
	Point KillCheck(Field enemys);
public:
	void GenerateField(Field &field) override;
	OptimalBot();
	Point MakeTurn(Field field) override;
	PlayerType Type() override { return PlayerType::Bot; }
};
