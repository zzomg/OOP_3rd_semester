#include "stdafx.h"
#include "player.h"

void Gamer::GenerateField(Field &field) {
	int sizes[10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	field.aliveShips() = 20;
	for (int i = 0; i < 10; i++) {
		bool vert = false;
		bool allowed = true;
		int biasX = 0;
		int biasY = 0;
		char key = 0;
		for (int j = 0; j < sizes[i]; j++) {
			if (vert) {
				if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
					field.field[biasY + j][biasX] = AS;
				}
				else {
					if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
					if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
				}
			}
			else {
				if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
					field.field[biasY][biasX + j] = AS;
				}
				else {
					if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
					if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
				}
			}
		}
		while (!(key == 13 && allowed)) { //13 = enter
			system("cls");
			field.Draw();
			allowed = true;
			for (int o = 0; o < 10; o++) {
				for (int u = 0; u < 10; u++) {
					if (field.field[o][u] == TWUA) field.field[o][u] = UA;
				}
			}
			key = _getch();
			if (key == 'r') {
				field.Free();
				i = -1;
				break;
			}
			if (key == 75 && biasX > 0) { //left key
				biasX--;
				if (vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + j][biasX + 1] != A && field.field[biasY + j][biasX + 1] != UA) field.field[biasY + j][biasX + 1] = E;
					}
				}
				else {
					if (field.field[biasY][biasX + sizes[i]] != A && field.field[biasY][biasX + sizes[i]] != UA) field.field[biasY][biasX + sizes[i]] = E;
				}
			}
			if (key == 77 && biasX < 10 - ((vert == true) ? 1 : sizes[i])) { //right key
				biasX++;
				if (vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + j][biasX - 1] != A && field.field[biasY + j][biasX - 1] != UA) field.field[biasY + j][biasX - 1] = E;
					}
				}
				else {
					if (field.field[biasY][biasX - 1] != A && field.field[biasY][biasX - 1] != UA) field.field[biasY][biasX - 1] = E;
				}
			}
			if (key == 72 && biasY > 0) { //up key
				biasY--;
				if (!vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + 1][biasX + j] != A && field.field[biasY + 1][biasX + j] != UA) field.field[biasY + 1][biasX + j] = E;
					}
				}
				else {
					if (field.field[biasY + sizes[i]][biasX] != A && field.field[biasY + sizes[i]][biasX] != UA) field.field[biasY + sizes[i]][biasX] = E;
				}
			}
			if (key == 80 && biasY < 10 - ((!vert) ? 1 : sizes[i])) { //down key
				biasY++;
				if (!vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY - 1][biasX + j] != A && field.field[biasY - 1][biasX + j] != UA) field.field[biasY - 1][biasX + j] = E;
					}
				}
				else {
					if (field.field[biasY - 1][biasX] != A && field.field[biasY - 1][biasX] != UA) field.field[biasY - 1][biasX] = E;
				}
			}
			if (key == 32) { //space
				if (!vert) {
					int oldbiasY = biasY;
					if (biasY + sizes[i] > 10) biasY = 10 - sizes[i];
					for (int j = 1; j < sizes[i]; j++) {
						if (field.field[oldbiasY][biasX + j] != A && field.field[oldbiasY][biasX + j] != UA) field.field[oldbiasY][biasX + j] = E;
					}
				}
				else {
					int oldbiasX = biasX;
					if (biasX + sizes[i] > 10) biasX = 10 - sizes[i];
					for (int j = 1; j < sizes[i]; j++) {
						if (field.field[biasY + j][oldbiasX] != A && field.field[biasY + j][oldbiasX] != UA) field.field[biasY + j][oldbiasX] = E;
					}
				}
				vert ^= 1;
			}
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && field.field[biasY][biasX + j] != A &&field.field[biasY][biasX + j] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
			}
			if (key == 13 && allowed) {
				if (vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + j][biasX] == AS) {
							field.field[biasY + j][biasX] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + k >= 0 && biasY + j >= 0 && field.field[biasY + j][biasX + k] != A && biasY + j < 10 && biasX + k < 10) {
								field.field[biasY + j][biasX + k] = UA;
							}
						}
					}
				}
				else {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY][biasX + j] == AS) {
							field.field[biasY][biasX + j] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + j >= 0 && biasY + k >= 0 && field.field[biasY + k][biasX + j] != A && biasY + k < 10 && biasX + j < 10) {
								field.field[biasY + k][biasX + j] = UA;
							}
						}
					}
				}
			}
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						field.field[biasY + j][biasX] = AS;
					}
					else {
						if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
						if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
						field.field[biasY][biasX + j] = AS;
					}
					else {
						if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
						if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
					}
				}
			}
		}
	}
}

Gamer::Gamer() {
}

Point Gamer::MakeTurn(Field enemys) {
	char key = 0;
	bool allowed = true;
	int biasX = 0;
	int biasY = 0;
	Field temp = enemys;

	while (!(key == 13 && allowed)) {
		key = _getch();
		temp = enemys;
		if (key == 75 && biasX > 0) {
			biasX--;
		}
		if (key == 77 && biasX < 9) {
			biasX++;
		}
		if (key == 72 && biasY > 0) {
			biasY--;
		}
		if (key == 80 && biasY < 9) {
			biasY++;
		}
		if (enemys.field[biasY][biasX] == U || enemys.field[biasY][biasX] == E) {
			temp.field[biasY][biasX] = AS;
			allowed = true;
		}
		else {
			temp.field[biasY][biasX] = US;
			allowed = false;
		}
		if (allowed && key == 13) {
			Point res(biasX, biasY);
			return res;
		}
	}
}

Point RandomBot::MakeTurn(Field enemys) {
	char key = 0;
	bool allowed = false;
	int biasX = 0;
	int biasY = 0;
	Field temp = enemys;
	srand(__rdtsc());
	while (!allowed) {
		temp = enemys;
		biasX = rand() % 10;
		biasY = rand() % 10;
		if (enemys.field[biasY][biasX] == U || enemys.field[biasY][biasX] == E) {
			temp.field[biasY][biasX] = AS;
			allowed = true;
		}
		else {
			temp.field[biasY][biasX] = US;
			allowed = false;
		}
	}
	Sleep(500);
	Point res(biasX, biasY);
	return res;
}

RandomBot::RandomBot() {
}




void RandomBot::GenerateField(Field &field) {
	int sizes[10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	srand(__rdtsc());
	int i = 0;
	field.aliveShips() = 20;
	while (i < 10) {
		bool vert = rand() % 2;
		bool allowed = true;
		int biasX = rand() % 10;
		int biasY = rand() % 10;
		char key = 0;
		for (int j = 0; j < sizes[i]; j++) {
			if (vert) {
				if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
					field.field[biasY + j][biasX] = AS;
				}
				else {
					if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
					if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
				}
			}
			else {
				if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
					field.field[biasY][biasX + j] = AS;
				}
				else {
					if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
					if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
				}
			}
		}
		while (!(key == 13 && allowed)) {
			/*system("cls");
			field.Draw();*/
			allowed = true;
			for (int o = 0; o < 10; o++) {
				for (int u = 0; u < 10; u++) {
					if (field.field[o][u] == TWUA) field.field[o][u] = UA;
				}
			}
			key = 13;
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && field.field[biasY][biasX + j] != A &&field.field[biasY][biasX + j] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
			}
			if (key == 13 && allowed) {
				if (vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + j][biasX] == AS) {
							field.field[biasY + j][biasX] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + k >= 0 && biasY + j >= 0 && field.field[biasY + j][biasX + k] != A && biasY + j < 10 && biasX + k < 10) {
								field.field[biasY + j][biasX + k] = UA;
							}
						}
					}
				}
				else {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY][biasX + j] == AS) {
							field.field[biasY][biasX + j] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + j >= 0 && biasY + k >= 0 && field.field[biasY + k][biasX + j] != A && biasY + k < 10 && biasX + j < 10) {
								field.field[biasY + k][biasX + j] = UA;
							}
						}
					}
				}
				i++;
			}
			else {
				for (int o = 0; o < 10; o++) {
					for (int u = 0; u < 10; u++) {
						if (field.field[o][u] == AS || field.field[o][u] == US) field.field[o][u] = E;
					}
				}
				break;
			}
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						field.field[biasY + j][biasX] = AS;
					}
					else {
						if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
						if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
						field.field[biasY][biasX + j] = AS;
					}
					else {
						if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
						if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
					}
				}
			}
		}
	}
}

OptimalBot::OptimalBot() {
}

void OptimalBot::GenerateField(Field &field) {
	int sizes[10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	srand(__rdtsc());
	int i = 0;
	field.aliveShips() = 20;
	while (i < 10) {
		int wall = rand() % 4;
		bool vert = wall % 2;
		int random = rand() % 3;
		bool allowed = true;
		int biasX = rand() % 10;
		int biasY = rand() % 10;
		if (random) {
			if (vert) {
				biasX = (wall == 1) ? 0 : 9;
				biasY = rand() % (11 - sizes[i]);
			}
			else {
				biasY = (wall == 1) ? 0 : 9;
				biasX = rand() % (11 - sizes[i]);
			}
		}
		char key = 0;
		for (int j = 0; j < sizes[i]; j++) {
			if (vert) {
				if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
					field.field[biasY + j][biasX] = AS;
				}
				else {
					if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
					if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
				}
			}
			else {
				if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
					field.field[biasY][biasX + j] = AS;
				}
				else {
					if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
					if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
				}
			}
		}
		while (!(key == 13 && allowed)) {
			/*system("cls");
			field.Draw();*/
			allowed = true;
			for (int o = 0; o < 10; o++) {
				for (int u = 0; u < 10; u++) {
					if (field.field[o][u] == TWUA) field.field[o][u] = UA;
				}
			}
			key = 13;
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && field.field[biasY][biasX + j] != A &&field.field[biasY][biasX + j] != UA) {
						allowed &= true;
					}
					else {
						allowed = false;
					}
				}
			}
			if (key == 13 && allowed) {
				if (vert) {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY + j][biasX] == AS) {
							field.field[biasY + j][biasX] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + k >= 0 && biasY + j >= 0 && field.field[biasY + j][biasX + k] != A && biasY + j < 10 && biasX + k < 10) {
								field.field[biasY + j][biasX + k] = UA;
							}
						}
					}
				}
				else {
					for (int j = 0; j < sizes[i]; j++) {
						if (field.field[biasY][biasX + j] == AS) {
							field.field[biasY][biasX + j] = A;
						}
					}
					for (int j = -1; j < sizes[i] + 1; j++) {
						for (int k = -1; k < 2; k++) {
							if (biasX + j >= 0 && biasY + k >= 0 && field.field[biasY + k][biasX + j] != A && biasY + k < 10 && biasX + j < 10) {
								field.field[biasY + k][biasX + j] = UA;
							}
						}
					}
				}
				i++;
			}
			else {
				for (int o = 0; o < 10; o++) {
					for (int u = 0; u < 10; u++) {
						if (field.field[o][u] == AS || field.field[o][u] == US) field.field[o][u] = E;
					}
				}
				break;
			}
			for (int j = 0; j < sizes[i]; j++) {
				if (vert) {
					if (biasX >= 0 && biasX < 10 && biasY >= 0 && biasY < 11 - sizes[i] && allowed && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) {
						field.field[biasY + j][biasX] = AS;
					}
					else {
						if (biasY + j < 10 && field.field[biasY + j][biasX] != A && field.field[biasY + j][biasX] != UA) field.field[biasY + j][biasX] = US;
						if (biasY + j < 10 && field.field[biasY + j][biasX] == UA) field.field[biasY + j][biasX] = TWUA;
					}
				}
				else {
					if (biasX >= 0 && biasX < 11 - sizes[i] && biasY >= 0 && biasY < 10 && allowed && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) {
						field.field[biasY][biasX + j] = AS;
					}
					else {
						if (biasX + j < 10 && field.field[biasY][biasX + j] != A && field.field[biasY][biasX + j] != UA) field.field[biasY][biasX + j] = US;
						if (biasX + j < 10 && field.field[biasY][biasX + j] == UA) field.field[biasY][biasX + j] = TWUA;
					}
				}
			}
		}
	}
}

Point OptimalBot::MakeTurn(Field enemys) {
	char key = 0;
	bool allowed = false;
	int biasX = 0;
	int biasY = 0;
	Point res(-2, -2);
	Field temp = enemys;
	srand(__rdtsc());
	int offset = rand() % 2;
	while (!allowed) {
		temp = enemys;
		if (res.x == -2) res = KillCheck(enemys);
		biasY = rand() % 10;
		biasX = rand() % 10;
		if (res.x != -1 && temp.field[res.y][res.x] == U) {
			biasY = res.y;
			biasX = res.x;
		}
		if (enemys.field[biasY][biasX] == U) {
			temp.field[biasY][biasX] = AS;
			allowed = true;
		}
		else {
			temp.field[biasY][biasX] = US;
			allowed = false;
		}
	}
	Sleep(500);
	res = Point(biasX, biasY);
	return res;
}

Point OptimalBot::KillCheck(Field enemys) {
	for (int i = 0; i < 10; i++)
	for (int j = 0; j < 10; j++) {
		if (enemys.field[i][j] == D && !enemys.KillCheck(Point(i, j))) {
			Point p(j, i);
			Point sh[4] = { Point(-1, -1), Point(-1, -1), Point(-1, -1), Point(-1, -1) };
			bool dd[4];
			int dir = -1;
			int ran = 0;
			int exec = -1;
			//left
			for (int k = 1; k < 4; k++) {
				if (j - k >= 0 && enemys.field[i][j - k] != E && enemys.field[i][j - k] != S) {
					dd[0] &= true;
					if (enemys.field[i][j - k] == D) exec = 0;
				}
				else dd[0] = false;
				if (j - k >= 0 && dd[0] && enemys.field[i][j - k] == D) dir = 0;
			}
			for (int k = 0; k < 4; k++) {
				if (j - k >= 0 && enemys.field[i][j - k] == S) break;
				if (j - k >= 0 && enemys.field[i][j - k] == U) { sh[0] = Point(j - k, i); break; }
			}
			//right
			for (int k = 1; k < 4; k++) {
				if (j + k < 10 && enemys.field[i][j + k] != E && enemys.field[i][j + k] != S) {
					dd[1] &= true;
					if (enemys.field[i][j + k] == D) exec = 1;
				}
				else dd[1] = false;
				if (j + k < 10 && dd[1] && enemys.field[i][j + k] == D) dir = 1;
			}
			for (int k = 0; k < 4; k++) {
				if (j + k < 10 && enemys.field[i][j + k] == S) break;
				if (j + k < 10 && enemys.field[i][j + k] == U) { sh[1] = Point(j + k, i); break; }
			}
			//up
			for (int k = 1; k < 4; k++) {
				if (i - k >= 0 && enemys.field[i - k][j] != E && enemys.field[i - k][j] != S) {
					dd[2] &= true;
					if (enemys.field[i - k][j] == D) exec = 2;
				}
				else dd[2] = false;
				if (i - k >= 0 && dd[2] && enemys.field[i - k][j] == D) dir = 2;
			}
			for (int k = 0; k < 4; k++) {
				if (i - k >= 0 && enemys.field[i - k][j] == S) break;
				if (i - k >= 0 && enemys.field[i - k][j] == U) { sh[2] = Point(j, i - k); break; }
			}
			//down
			for (int k = 1; k < 4; k++) {
				if (i + k < 10 && enemys.field[i + k][j] != E && enemys.field[i + k][j] != S) {
					dd[3] &= true;
					if (enemys.field[i + k][j] == D) exec = 3;
				}
				else dd[3] = false;
				if (i + k < 10 && dd[3] && enemys.field[i + k][j] == D) dir = 3;
			}
			for (int k = 0; k < 4; k++) {
				if (i + k < 10 && enemys.field[i + k][j] == S) break;
				if (i + k < 10 && enemys.field[i + k][j] == U) { sh[3] = Point(j, i + k); break; }
			}
			if (exec > -1) return sh[exec];
			else {
				ran = rand() % 4;
				while (sh[ran].x == -1) {
					ran = rand() % 4;
				}
				return sh[ran];
			}
		}
	}
	return Point(-1, -1);
}
