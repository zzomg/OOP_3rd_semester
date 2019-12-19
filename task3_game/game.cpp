#include "game.h"

bool Game::Start() {
	field1.Free();
	field2.Free();
	player1->GenerateField(field1);
	player2->GenerateField(field2);
	Turn();
	cout << endl << "Play again?" << endl << "Press 'Enter' for answer 'Yes'";
	if (_getch() == 13) return 1;
	else return 0;
}

bool Game::Turn() {
	while (ProcessTurn(player1->MakeTurn(field2.Transform()), field2))
	{
		system("cls");
		field1.Draw();
		cout << endl; 
		system("cls");
		field2.Draw();
		cout << endl;
	}

	if (field2.getAliveShips()) {
		if (player1->Type() == PlayerType::Human) {
			system("cls");
			cout << "Finish turn?";
			_getch();
		}
		system("cls");
		while (ProcessTurn(player2->MakeTurn(field1.Transform()), field1))
		{
			system("cls");
			field2.Draw();
			cout << endl;
			system("cls");
			field1.Draw();
			cout << endl;
		}
	}
	if (field1.getAliveShips() && field2.getAliveShips()) {
		if (player2->Type() == PlayerType::Human) {
			system("cls");
			cout << "Finish turn?";
			_getch();
		}
		system("cls");
		Turn();
		return 0;
	}
	else {
		cout << ((field2.getAliveShips() > 0) ? "Player #2 won!" : "Player #1 won!");
		return 1;
	}
}

bool GUI::MenuElements::GetActive() {
	return selected;
}

GUI::Button::Button(string txt) {
	text = txt;
	selected = false;
	type = 2;
}

GUI::Label::Label(string txt) {
	text = txt;
	selected = false;
	type = 1;
}

void GUI::MenuElements::SetActive(bool act) {
	selected = act;
}

void GUI::Button::Render() {
	if (!selected) {
		cout << text << endl;
	}
	else {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 1));
		cout << text << endl;
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	}
}

void GUI::Label::Render() {
	cout << text << endl;
}

GUI::MenuPages::MenuPages(vector<MenuElements*>& dat) {
	data = dat;
	int i = 0;
	for (auto elem : data) {
		if (elem->GetType() == 2) {
			elem->index = i;
			i++;
		}
	}
	btns = i;
}

void GUI::MenuPages::Render() {
	for (auto MenuElement : data) {
		MenuElement->Render();
	}
}

void GUI::MenuPages::CheckActive() {
	for (auto MenuElem : data) {
		if (MenuElem->index == cur_elem && MenuElem->GetType() == 2) {
			MenuElem->SetActive(true);
		}
		else {
			MenuElem->SetActive(false);
		}
	}
}

GUI::Menu::Menu(vector<MenuPages*>& pgs) {
	pages = pgs;
}

void GUI::Menu::Start() {
	Update();
}

void GUI::Menu::Update() {
	char key = 0;
	Game game;
	Player* p1 = new Gamer(), * p2 = new Gamer();
	while (1) {
		system("cls");
		if (cur_page < 5) {
			pages[cur_page]->CheckActive();
			pages[cur_page]->Render();
		}
		key = _getch();
		switch (key) {
		case 72:
			pages[cur_page]->cur_elem--;
			if (pages[cur_page]->cur_elem < 0) pages[cur_page]->cur_elem = pages[cur_page]->GetButtons() + pages[cur_page]->cur_elem;
			break;
		case 80:
			pages[cur_page]->cur_elem++;
			if (pages[cur_page]->cur_elem >= pages[cur_page]->GetButtons()) pages[cur_page]->cur_elem = -pages[cur_page]->GetButtons() + pages[cur_page]->cur_elem;
			break;
		}
		switch (cur_page) {
		case 0: //основное меню
			if (key == 13) {
				switch (pages[cur_page]->cur_elem) {
				case 0:
					cur_page = 1; //game
					break;
				case 1:
					cur_page = 3; //controls
					break;
				case 2:
					cur_page = 4; //about
					break;
				case 3:
					exit(0); //about
					break;
				}
			}
			break;
		case 1: //game
			if (key == 13) {
				switch (pages[cur_page]->cur_elem) {
				case 0:
					delete p1;
					p1 = new Gamer();
					cur_page = 2;
					break;
				case 1:
					delete p1;
					p1 = new RandomBot();
					cur_page = 2;
					break;
				case 2:
					delete p1;
					p1 = new OptimalBot();
					cur_page = 2;
					break;
				case 3:
					cur_page = 0;
					break;
				}
			}
			break;
		case 2: //game
			if (key == 13) {
				switch (pages[cur_page]->cur_elem) {
				case 0:
					delete p2;
					p2 = new Gamer();
					cur_page = 5;
					break;
				case 1:
					delete p2;
					p2 = new RandomBot();
					cur_page = 5;
					break;
				case 2:
					delete p2;
					p2 = new OptimalBot();
					cur_page = 5;
					break;
				case 3:
					cur_page = 1;
					break;
				}
			}
			break;
		case 3: //controls
			if (key == 13 && pages[cur_page]->cur_elem == 0) cur_page = 0;
			break;
		case 4: //about
			if (key == 13 && pages[cur_page]->cur_elem == 0) cur_page = 0;
			break;
		case 5:
			game = Game(p1, p2);
			if (game.Start()) cur_page = 1;
			else exit(0);
			break;
		}
	}
}

bool Game::ProcessTurn(const Point& shoot, Field& field) {
	bool res = false;
	if (field.field[shoot.y][shoot.x] == E) field.field[shoot.y][shoot.x] = S;
	if (field.field[shoot.y][shoot.x] == AE) field.field[shoot.y][shoot.x] = S;
	if (field.field[shoot.y][shoot.x] == UA) field.field[shoot.y][shoot.x] = S;
	if (field.field[shoot.y][shoot.x] == A) {
		field.field[shoot.y][shoot.x] = D;
		field.getAliveShips()--;
		if (field.getAliveShips() == 0) {
			return 0;
		}
		field.KillCheck(shoot);
		res = true;
	}
	return res;
}
