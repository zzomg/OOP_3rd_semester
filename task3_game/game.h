#pragma once
#include "stdafx.h"
#include "player.h"

class Game {
private:
	Player* player1;
	Player* player2;
	Field field1;
	Field field2;

	bool ProcessTurn(const Point &shoot, Field &field);

public:
	Game() = default;
	Game(Player* p1, Player* p2) {
		player1 = p1;
		player2 = p2;
	}
	bool Turn();
	bool Start();
};

namespace GUI {
	class MenuElements {
	protected:
		bool selected = false;
		int type = 0;
		string text = "";
	public:
		int index = -1;
		virtual void Render() = 0;
		bool GetActive();
		int GetType() {
			return type;
		}
		void SetActive(bool act);
	};

	class Button : public MenuElements {
	private:
	public:
		Button() = default;
		Button(string txt);
		void Render() override;
	};

	class Label : public MenuElements {
	private:
	public:
		Label() = default;
		Label(string txt);
		void Render() override;
	};

	class MenuPages {
	private:
		vector<MenuElements*> data;
		int btns = 0;
	public:
		int cur_elem = 0;
		MenuPages(vector<MenuElements*> &dat);
		void Render();
		void CheckActive();
		int GetButtons() {
			return btns;
		}
	};

	class Menu {
	private:
		vector<MenuPages*> pages;
		int cur_page = 0;
	public:
		Menu(vector<MenuPages*> &pgs);
		void Start();
		void Update();
	};
};
