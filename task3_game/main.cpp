#include <iostream>
#include "game.h"

using namespace GUI;

int main() {
	vector<MenuElements*> dt0 = { new Label("Welcome to the Seabattle Game!\n"), new Button("Play!"), new Button("Controls"), new Button("About"), new Button("Quit") };
	vector<MenuElements*> dt1 = { new Label("Choose Player #1\n"), new Label("Player #1: "), new Button("Gamer"), new Button("Easy Bot"), new Button("Hard Bot"), new Button("\nBack") };
	vector<MenuElements*> dt1_1 = { new Label("Choose Player #2\n"), new Label("Player #2: "), new Button("Gamer"), new Button("Easy Bot"), new Button("Hard Bot"), new Button("\nBack") };
	vector<MenuElements*> dt2 = { new Label("Controls\n"), new Label("Use arrow keys to move ships and 'space' to rotate. \nPress 'enter' to set the ship and to attack.\n"), new Button("Back") };
	vector<MenuElements*> dt3 = { new Label("About\n"), new Label("A classic seabattle game. Enjoy!\n"), new Button("Back") };
	
	Menu menu = Menu(vector<MenuPages*>({ new MenuPages(dt0), new MenuPages(dt1), new MenuPages(dt1_1), new MenuPages(dt2), new MenuPages(dt3) }));
	
	menu.Start();

	_getch();

	return 0;
}
