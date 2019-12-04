// stdcwf : my standard library

#include <iostream>
#include <windows.h>
#include <string>
#include <mmsystem.h>
#include "line.h"
#pragma comment(lib, "Winmm.lib")

using namespace std;

const enum COLOR {DARK_BLUE = 1, DARK_GREEN, DARK_TEAL, DARK_RED, DARK_PINK, DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE};
HANDLE h_con;
COLOR grey = GRAY;

void set_color(COLOR c) {
	if (h_con == NULL) {
		h_con = GetStdHandle(STD_OUTPUT_HANDLE);
	} 
	SetConsoleTextAttribute(h_con, c);
}

void beep() {
	cout << "\a";
}

void print(string output) {
	cout << output;
}
void println(string output) {
	cout << output << endl;
}
void(*dfa(int &x, void(*update)(), void(*teleport)()))() {
	void (*(*foobar)(int &, void(*)(), void(*)()))();
	foobar = &line::slope1;
	void (*t)() = teleport;
	update = foobar(x, update, t);
	return update;
}
void input(string prompt, string var) {
	cout << prompt;
	getline(cin, var);
}
void center(string s) {
	int length = s.size();
	int position = (int) ((80 - length) / 2);
	for (int i = 0; i < position; i++) {
		cout << " ";
	}
	cout << s;
}

void playWav(char* path) {
	PlaySoundA((LPCSTR) path, NULL, SND_FILENAME);
}
void playWavAsync(char* path) {
	PlaySoundA((LPCSTR) path, NULL, SND_FILENAME | SND_ASYNC);
}

//[||||||||||||||||||||||||] 100%
void loading() {
	COLOR blue = BLUE;
	COLOR green = GREEN; 
	string spaces2(29, ' ');
	set_color(green);
	cout << spaces2 << "Foster Productions 2016";
	set_color(blue);
	string nls(10, '\n');
	string spaces(25, ' ');
	cout << nls;
	center("LOADING");
	cout << "\n\n" << spaces;
	//cout << nls << spaces1 << "LOADING" << "\n\n" << spaces;
	cout << "[                         ] 0%\r" << spaces;
	int prog = 0;
	for (int num = 1; num < 26; num++){
		cout << "[";
		for (int i = 0; i < num; i++) {
			cout << "|";
		}
		for (int j = 0; j < 25 - num; j++) {
			cout << " ";
		}
		cout << "] " << num * 4 << "%\r" << spaces;
		Sleep(500);
	}
	Sleep(1000);
	playWavAsync("C:\\Windows\\winsxs\\x86_microsoft-windows-s..undthemes-landscape_31bf3856ad364e35_6.1.7600.16385_none_1e650d9135666d13\\Windows Logon Sound.wav");
	system("cls");
	set_color(grey);
}