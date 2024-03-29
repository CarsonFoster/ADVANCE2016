// PacMan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "line.h"
#include "point.h"
#include <iostream>
#include <conio.h>
#include <string>
#include "stdcwf.h"
#include <Windows.h>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
using namespace std;

typedef vector<vector<char>> vchar;

const enum DIR {n, e, s, w, null};
const char wall[] = {char(177), '-'};
int x, y, c, sx, sy;
int lives = 5;
int score = 0;
bool hunting = false;
bool dash[5] = {false, false, false, false, false};
vchar board;

char gwall[] = {char(177), '^', '-'};
int gCoord[8] = {0, 0, 0, 0, 0, 0, 0 ,0};
int sCoord[8] = {0, 0, 0, 0, 0, 0, 0 ,0};
DIR gdir[4] = {null, null, null, null};
DIR pdir[4] = {null, null, null, null};

bool won = false;
bool lost = false;
DIR dir = null;
void (*update)();

vchar initBoard() {
	int count = 0;
	int pcount = 0;
	vchar board;
	vector<char> tmp;
	string line;
	ifstream fin("board.txt");
	if (fin) {
		while (!fin.eof()) {
			getline(fin, line);
			for (int i = 0; i < line.size(); i++) {
				if (line[i] == ' ') {
					count++;
					pcount++;
					if (count >= 5) {
						line[i] = '+';
						count = 0;
					} else if (pcount >= 67) {
						line[i] = '*';
						pcount = 0;
					}
				} else if (line[i] == '@') {
					line[i] = char(177);
				} else if (line[i] == '`') {
					line[i] = char(177);
				} else if (line[i] == '^' || line[i] == '-' || line[i] == '.' || line[i] == 'o') {
					;
				} else {
					line[i] = ' ';
				}

				tmp.push_back(line[i]);
			}
			board.push_back(tmp);
			tmp.clear();
		}
	} else {
		cout << "File doesn't exist: board.txt";
		tmp.push_back('\xfe');
		board.push_back(tmp);
		return board;
	}
	fin.close();
	int g = 0;

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == 'o') {
				x = j;
				sx = j;
				y = i;
				sy = i;
			} else if (board[i][j] == '^') {
				if (g == 0) {
					sCoord[0] = j;
					sCoord[1] = i;
					gCoord[0] = j;
					gCoord[1] = i;
				} else if (g == 1) {
					sCoord[2] = j;
					sCoord[3] = i;
					gCoord[2] = j;
					gCoord[3] = i;
				} else if (g == 2) {
					sCoord[4] = j;
					sCoord[5] = i;
					gCoord[4] = j;
					gCoord[5] = i;
				} else if (g == 3) {
					sCoord[6] = j;
					sCoord[7] = i;
					gCoord[6] = j;
					gCoord[7] = i;
				}
				g++;
			}
		}
	}

	return board;

}

vector<int> xyDir() {
	int ny, nx;
	switch (dir) {
	case n:
		ny = y - 1;
		nx = x;
		break;
	case s:
		ny = y + 1;
		nx = x;
		break;
	case e:
		nx = x + 1;
		ny = y;
		break;
	case w:
		nx = x - 1;
		ny = y;
		break;
	default:
		nx = x;
		ny = y; 
	}
	vector<int> c;
	c.push_back(nx);
	c.push_back(ny);
	return c;
}
vector<int> gxyDir(int g) {
	int nx, ny;
	int gy, gx;
	gx = gCoord[g * 2 - 2];
	gy = gCoord[g * 2 - 1];
	switch (gdir[g - 1]) {
	case n:
		ny = gy - 1;
		nx = gx;
		break;
	case s:
		ny = gy + 1;
		nx = gx;
		break;
	case e:
		nx = gx + 1;
		ny = gy;
		break;
	case w:
		nx = gx - 1;
		ny = gy;
		break;
	default:
		nx = gx;
		ny = gy; 
	}
	vector<int> c;
	c.push_back(nx);
	c.push_back(ny);
	return c;
}
string hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

bool legal() {
	if (dir == null)
		return false;
	vector<int> c = xyDir();
	int nx = c[0];
	int ny = c[1];
	
	for (int i = 0; i < sizeof(wall); i++) {
		if (board[ny][nx] == wall[i])
			return false;
	}
	return true;
}
bool glegal(int g) {
	if (gdir[g - 1] == null)
		return false;
	vector<int> c = gxyDir(g);
	int nx = c[0];
	int ny = c[1];
	
	for (int i = 0; i < sizeof(gwall); i++) {
		if (board[ny][nx] == gwall[i])
			return false;
	}
	return true;
}

void scoring() {
	vector<int> c = xyDir();
	if (board[c[1]][c[0]] == '+') {
		score++;
	} else if (board[c[1]][c[0]] == '*') {
		score += 10;
		hunting = true;
	}
}

void teleport(int g) {
	board[gCoord[g * 2 - 1]][gCoord[g * 2 - 2]] = ' ';
	board[sCoord[g * 2 - 1]][sCoord[g * 2 - 2]] = '^';
	gCoord[g * 2 - 2] = sCoord[g * 2 - 2];
	gCoord[g * 2 - 1] = sCoord[g * 2 - 1];
}
void teleport() {
	for (int i = 1; i < 5; i++) {
		if (board[gCoord[i * 2 - 1] - 1][gCoord[i * 2 - 2]] == 'o' || board[gCoord[i * 2 - 1] + 1][gCoord[i * 2 - 2]] == 'o' || board[gCoord[i * 2 - 1]][gCoord[i * 2 - 2] + 1] == 'o' || board[gCoord[i * 2 - 1] - 1][gCoord[i * 2 - 2] - 1] == 'o') {
			teleport(i);
		}
	}
}
void telepac() {
	board[y][x] = ' ';
	y = sy;
	x = sx;
	board[y][x] = 'o';
}

bool checkBounds() {
	if ((x >= 40 && x <= 44 && (y <= 0 || y >= board.size() - 1))) {
		if (dir == s) {
			if (y >= board.size() - 1) {
				board[1][x] = 'o';
				board[y][x] = ' ';
				y = 1;
			}
		}
		if (dir == n) {
			if (y <= 0) {
				board[board.size() - 2][x] = 'o';
				board[y][x] = ' ';
				y = board.size() - 2;
			}
		}
		return true;
	}
	return false;
}
bool gcheckBounds(int g) {
	int gx, gy;
	gx = gCoord[g * 2 - 2];
	gy = gCoord[g * 2 - 1];
	if ((gx >= 40 && gx <= 44 && (gy <= 0 || gy >= board.size() - 1))) {
		if (gdir[g - 1] == s) {
			if (gy >= board.size() - 1) {
				board[1][gx] = '^';
				board[gy][gx] = '+';
				gCoord[g * 2 - 1] = 1;
			}
		}
		if (gdir[g - 1] == n) {
			if (gy <= 0) {
				board[board.size() - 2][gx] = '^';
				board[gy][gx] = '+';
				gCoord[g * 2 - 1] = board.size() - 2;
			}
		}
	}
	return false;
}

void gai() {
	for (int i = 1; i < 5; i++) {
		if (i == 1) {
			// random one - green - bob
			do {
				pdir[i - 1] = gdir[i - 1];
				gdir[i - 1] = static_cast<DIR>(rand() % null);
			} while (!glegal(i) && pdir[i - 1] != gdir[i - 1]);
		} else if (i == 2) {
			// pursue pacman - red - blinky
			gdir[1] = n;
			line nl(point(0, 0), point(0, 0));
			line sl(point(0, 0), point(0, 0));
			line wl(point(0, 0), point(0, 0));
			line el(point(0, 0), point(0, 0));
			int num = 0;
			int gx = gCoord[2];
			int gy = gCoord[3];

			if (glegal(i)) {
				if (board[gy - 1][gx] == 'o') {
					return;
				}
				nl = line(point(gx, gy - 1), point(x, y));
				num++;
			}
			gdir[1] = w;
			if (glegal(i)) {
				if (board[gy][gx - 1] == 'o') {
					return;
				}
				wl = line(point(gx - 1, gy), point(x, y));
				num++;
			}
			gdir[1] = s;
			if (glegal(2)) {
				if (board[gy + 1][gx] == 'o') {
					return;
				}
				sl = line(point(gx, gy + 1), point(x, y));
				num++;
			}
			gdir[1] = e;
			if (glegal(2)) {
				if (board[gy][gx + 1] == 'o') {
					return;
				}
				el = line(point(gx + 1, gy), point(x, y));
				num++;
			}

			if (num == 1) {
				if (nl != line())
					gdir[1] = n;
				else if (wl != line())
					gdir[1] = w;
				else if (sl != line())
					gdir[1] = s;
				else 
					gdir[1] = e;
			}

			if (num == 2) {
				if (nl != line()) {
					if (wl != line()) {
						if (nl.segLen() <= wl.segLen()) {
							gdir[1] = n;
						} else {
							gdir[1] = w;
						}
					} else if (sl != line()) {
						if (nl.segLen() <= sl.segLen()) {
							gdir[1] = n;
						} else {
							gdir[1] = s;
						}
					} else {
						if (nl.segLen() <= el.segLen()) {
							gdir[1] = n;
						} else {
							gdir[1] = e;
						}
					}
				} else if (sl != line()) {
					if (wl != line()) {
						if (sl.segLen() < wl.segLen()) {
							gdir[1] = s;
						} else {
							gdir[1] = w;
						}
					} else {
						if (sl.segLen() <= el.segLen()) {
							gdir[1] = s;
						} else {
							gdir[1] = e;
						}
					}
				} else {
					if (wl.segLen() <= el.segLen()) {
						gdir[1] = w;
					} else {
						gdir[1] = e;
					}
				}
			} else if (num == 3){
				if (nl != line() && wl != line() && sl != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[1] = n;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[1] = w;
					} else if (nl.segLen() >= sl.segLen()){
						gdir[1] = s;
					}
				} else if (nl != line() && el != line() && sl != line()) {
					if (nl.segLen() <= el.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[1] = n;
					} else if (el.segLen() < sl.segLen()) {
						gdir[1] = e;
					} else if (nl.segLen() > sl.segLen()){
						gdir[1] = s;
					}
				} else if (nl != line() && wl != line() && el != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= el.segLen()) {
						gdir[1] = n;
					} else if (wl.segLen() <= el.segLen()) {
						gdir[1] = w;
					} else if (nl.segLen() > el.segLen()){
						gdir[1] = e;
					}
				} else if (el != line() && wl != line() && sl != line()) {
					if (el.segLen() < wl.segLen() && el.segLen() < sl.segLen()) {
						gdir[1] = e;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[1] = w;
					} else if (el.segLen() >= sl.segLen()){
						gdir[1] = s;
					}
				}
			} else {
				if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen() && nl.segLen() <= el.segLen())
					gdir[1] = n;
				else if (wl.segLen() <= sl.segLen() && wl.segLen() <= el.segLen() && wl.segLen() < nl.segLen())
					gdir[1] = w;
				else if (sl.segLen() <= el.segLen() && sl.segLen() < nl.segLen() && sl.segLen() < wl.segLen())
					gdir[1] = s;
				else
					gdir[1] = e;
			}
		} else if (i == 3) {
			// ambush pacman  - pink - pinky
			gdir[2] = n;
			line nl(point(0, 0), point(0, 0));
			line sl(point(0, 0), point(0, 0));
			line wl(point(0, 0), point(0, 0));
			line el(point(0, 0), point(0, 0));
			int num = 0;
			int gx = gCoord[4];
			int gy = gCoord[5];
			int tx, ty;

			switch (dir) {
				case n:
					tx = x;
					ty = y - 4;
					break;
				case s:
					tx = x;
					ty = y + 4;
					break;
				case e:
					tx = x + 4;
					ty = y;
					break;
				case w:
					tx = x - 4;
					ty = y;
					break;
				default:
					tx = x;
					ty = y - 4;
			}

			if (glegal(3)) {
				if (board[gy - 1][gx] == 'o') {
					return;
				}
				nl = line(point(gx, gy - 1), point(tx, ty));
				num++;
			}
			gdir[2] = w;
			if (glegal(3)) {
				if (board[gy][gx - 1] == 'o') {
					return;
				}
				wl = line(point(gx - 1, gy), point(tx, ty));
				num++;
			}
			gdir[2] = s;
			if (glegal(3)) {
				if (board[gy + 1][gx] == 'o') {
					return;
				}
				sl = line(point(gx, gy + 1), point(tx, y));
				num++;
			}
			gdir[2] = e;
			if (glegal(3)) {
				if (board[gy][gx + 1] == 'o') {
					return;
				}
				el = line(point(gx + 1, gy), point(tx, ty));
				num++;
			}

			if (num == 1) {
				if (nl != line())
					gdir[2] = n;
				else if (wl != line())
					gdir[2] = w;
				else if (sl != line())
					gdir[2] = s;
				else 
					gdir[2] = e;
			}

			if (num == 2) {
				if (nl != line()) {
					if (wl != line()) {
						if (nl.segLen() <= wl.segLen()) {
							gdir[2] = n;
						} else {
							gdir[2] = w;
						}
					} else if (sl != line()) {
						if (nl.segLen() <= sl.segLen()) {
							gdir[2] = n;
						} else {
							gdir[2] = s;
						}
					} else {
						if (nl.segLen() <= el.segLen()) {
							gdir[2] = n;
						} else {
							gdir[2] = e;
						}
					}
				} else if (sl != line()) {
					if (wl != line()) {
						if (sl.segLen() < wl.segLen()) {
							gdir[2] = s;
						} else {
							gdir[2] = w;
						}
					} else {
						if (sl.segLen() <= el.segLen()) {
							gdir[2] = s;
						} else {
							gdir[2] = e;
						}
					}
				} else {
					if (wl.segLen() <= el.segLen()) {
						gdir[2] = w;
					} else {
						gdir[2] = e;
					}
				}
			} else if (num == 3){
				if (nl != line() && wl != line() && sl != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[2] = n;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[2] = w;
					} else if (nl.segLen() >= sl.segLen()){
						gdir[2] = s;
					}
				} else if (nl != line() && el != line() && sl != line()) {
					if (nl.segLen() <= el.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[2] = n;
					} else if (el.segLen() < sl.segLen()) {
						gdir[2] = e;
					} else if (nl.segLen() > sl.segLen()){
						gdir[2] = s;
					}
				} else if (nl != line() && wl != line() && el != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= el.segLen()) {
						gdir[2] = n;
					} else if (wl.segLen() <= el.segLen()) {
						gdir[2] = w;
					} else if (nl.segLen() > el.segLen()){
						gdir[2] = e;
					}
				} else if (el != line() && wl != line() && sl != line()) {
					if (el.segLen() < wl.segLen() && el.segLen() < sl.segLen()) {
						gdir[2] = e;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[2] = w;
					} else if (el.segLen() >= sl.segLen()){
						gdir[2] = s;
					}
				}
			} else {
				if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen() && nl.segLen() <= el.segLen())
					gdir[2] = n;
				else if (wl.segLen() <= sl.segLen() && wl.segLen() <= el.segLen() && wl.segLen() < nl.segLen())
					gdir[2] = w;
				else if (sl.segLen() <= el.segLen() && sl.segLen() < nl.segLen() && sl.segLen() < wl.segLen())
					gdir[2] = s;
				else
					gdir[2] = e;
			}
		} else if (i == 4) {
			// blinky based ghost - blue - inky
			gdir[3] = n;
			line nl(point(0, 0), point(0, 0));
			line sl(point(0, 0), point(0, 0));
			line wl(point(0, 0), point(0, 0));
			line el(point(0, 0), point(0, 0));
			int num = 0;
			int gx = gCoord[4];
			int gy = gCoord[5];
			int tx, ty;

			switch (dir) {
				case n:
					tx = x;
					ty = y - 2;
					break;
				case s:
					tx = x;
					ty = y + 2;
					break;
				case e:
					tx = x + 2;
					ty = y;
					break;
				case w:
					tx = x - 2;
					ty = y;
					break;
				default:
					tx = x;
					ty = y - 2;
			}
			double angle = atan2(double(ty - gCoord[7]), double(tx - gCoord[6]));
			int length = line(point(0, 0), point(tx - gCoord[6], ty - gCoord[7])).segLen();
			tx = 2 * length * cos(angle);
			ty = 2 * length * sin(angle);

			if (glegal(4)) {
				if (board[gy - 1][gx] == 'o') {
					return;
				}
				nl = line(point(gx, gy - 1), point(tx, ty));
				num++;
			}
			gdir[3] = w;
			if (glegal(4)) {
				if (board[gy][gx - 1] == 'o') {
					return;
				}
				wl = line(point(gx - 1, gy), point(tx, ty));
				num++;
			}
			gdir[3] = s;
			if (glegal(4)) {
				if (board[gy + 1][gx] == 'o') {
					return;
				}
				sl = line(point(gx, gy + 1), point(tx, y));
				num++;
			}
			gdir[3] = e;
			if (glegal(4)) {
				if (board[gy][gx + 1] == 'o') {
					return;
				}
				el = line(point(gx + 1, gy), point(tx, ty));
				num++;
			}

			if (num == 1) {
				if (nl != line())
					gdir[3] = n;
				else if (wl != line())
					gdir[3] = w;
				else if (sl != line())
					gdir[3] = s;
				else 
					gdir[3] = e;
			}

			if (num == 2) {
				if (nl != line()) {
					if (wl != line()) {
						if (nl.segLen() <= wl.segLen()) {
							gdir[3] = n;
						} else {
							gdir[3] = w;
						}
					} else if (sl != line()) {
						if (nl.segLen() <= sl.segLen()) {
							gdir[3] = n;
						} else {
							gdir[3] = s;
						}
					} else {
						if (nl.segLen() <= el.segLen()) {
							gdir[3] = n;
						} else {
							gdir[3] = e;
						}
					}
				} else if (sl != line()) {
					if (wl != line()) {
						if (sl.segLen() < wl.segLen()) {
							gdir[3] = s;
						} else {
							gdir[3] = w;
						}
					} else {
						if (sl.segLen() <= el.segLen()) {
							gdir[3] = s;
						} else {
							gdir[3] = e;
						}
					}
				} else {
					if (wl.segLen() <= el.segLen()) {
						gdir[3] = w;
					} else {
						gdir[3] = e;
					}
				}
			} else if (num == 3){
				if (nl != line() && wl != line() && sl != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[3] = n;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[3] = w;
					} else if (nl.segLen() >= sl.segLen()){
						gdir[3] = s;
					}
				} else if (nl != line() && el != line() && sl != line()) {
					if (nl.segLen() <= el.segLen() && nl.segLen() <= sl.segLen()) {
						gdir[3] = n;
					} else if (el.segLen() < sl.segLen()) {
						gdir[3] = e;
					} else if (nl.segLen() > sl.segLen()){
						gdir[3] = s;
					}
				} else if (nl != line() && wl != line() && el != line()) {
					if (nl.segLen() <= wl.segLen() && nl.segLen() <= el.segLen()) {
						gdir[3] = n;
					} else if (wl.segLen() <= el.segLen()) {
						gdir[3] = w;
					} else if (nl.segLen() > el.segLen()){
						gdir[3] = e;
					}
				} else if (el != line() && wl != line() && sl != line()) {
					if (el.segLen() < wl.segLen() && el.segLen() < sl.segLen()) {
						gdir[3] = e;
					} else if (wl.segLen() <= sl.segLen()) {
						gdir[3] = w;
					} else if (el.segLen() >= sl.segLen()){
						gdir[3] = s;
					}
				}
			} else {
				if (nl.segLen() <= wl.segLen() && nl.segLen() <= sl.segLen() && nl.segLen() <= el.segLen())
					gdir[3] = n;
				else if (wl.segLen() <= sl.segLen() && wl.segLen() <= el.segLen() && wl.segLen() < nl.segLen())
					gdir[3] = w;
				else if (sl.segLen() <= el.segLen() && sl.segLen() < nl.segLen() && sl.segLen() < wl.segLen())
					gdir[3] = s;
				else
					gdir[3] = e;
			}
		}
	}
}
void frightened() {
	gwall[2] = '~';
	if (rand() % 100 > 95) {
		hunting = false;
		for (int i = 1; i < 5; i++) {
			teleport(i);
		}
	}
	for (int i = 1; i < 5; i++) {
		gdir[i - 1] = n;
		line nl(point(0, 0), point(0, 0));
		line sl(point(0, 0), point(0, 0));
		line wl(point(0, 0), point(0, 0));
		line el(point(0, 0), point(0, 0));
		int num = 0;
		int gx = gCoord[2];
		int gy = gCoord[3];

		if (glegal(i)) {
			nl = line(point(gx, gy - 1), point(x, y));
			num++;
		}
		gdir[i - 1] = w;
		if (glegal(i)) {
			wl = line(point(gx - 1, gy), point(x, y));
			num++;
		}
		gdir[i - 1] = s;
		if (glegal(i)) {
			sl = line(point(gx, gy + 1), point(x, y));
			num++;
		}
		gdir[i - 1] = e;
		if (glegal(i)) {
			el = line(point(gx + 1, gy), point(x, y));
			num++;
		}

		if (num == 1) {
			if (nl != line())
				gdir[i - 1] = n;
			else if (wl != line())
				gdir[i - 1] = w;
			else if (sl != line())
				gdir[i - 1] = s;
			else 
				gdir[i - 1] = e;
		}

		if (num == 2) {
			if (nl != line()) {
				if (wl != line()) {
					if (nl.segLen() >= wl.segLen()) {
						gdir[i - 1] = n;
					} else {
						gdir[i - 1] = w;
					}
				} else if (sl != line()) {
					if (nl.segLen() >= sl.segLen()) {
						gdir[i - 1] = n;
					} else {
						gdir[i - 1] = s;
					}
				} else {
					if (nl.segLen() >= el.segLen()) {
						gdir[i - 1] = n;
					} else {
						gdir[i - 1] = e;
					}
				}
			} else if (sl != line()) {
				if (wl != line()) {
					if (sl.segLen() > wl.segLen()) {
						gdir[i - 1] = s;
					} else {
						gdir[i - 1] = w;
					}
				} else {
					if (sl.segLen() >= el.segLen()) {
						gdir[i - 1] = s;
					} else {
						gdir[i - 1] = e;
					}
				}
			} else {
				if (wl.segLen() >= el.segLen()) {
					gdir[i - 1] = w;
				} else {
					gdir[i - 1] = e;
				}
			}
		} else if (num == 3){
			if (nl != line() && wl != line() && sl != line()) {
				if (nl.segLen() >= wl.segLen() && nl.segLen() >= sl.segLen()) {
					gdir[i - 1] = n;
				} else if (wl.segLen() >= sl.segLen()) {
					gdir[i - 1] = w;
				} else if (nl.segLen() <= sl.segLen()){
					gdir[i - 1] = s;
				}
			} else if (nl != line() && el != line() && sl != line()) {
				if (nl.segLen() >= el.segLen() && nl.segLen() >= sl.segLen()) {
					gdir[i - 1] = n;
				} else if (el.segLen() > sl.segLen()) {
					gdir[i - 1] = e;
				} else if (nl.segLen() < sl.segLen()){
					gdir[i - 1] = s;
				}
			} else if (nl != line() && wl != line() && el != line()) {
				if (nl.segLen() >= wl.segLen() && nl.segLen() >= el.segLen()) {
					gdir[i - 1] = n;
				} else if (wl.segLen() >= el.segLen()) {
					gdir[i - 1] = w;
				} else if (nl.segLen() < el.segLen()){
					gdir[i - 1] = e;
				}
			} else if (el != line() && wl != line() && sl != line()) {
				if (el.segLen() > wl.segLen() && el.segLen() > sl.segLen()) {
					gdir[i - 1] = e;
				} else if (wl.segLen() >= sl.segLen()) {
					gdir[i - 1] = w;
				} else if (el.segLen() <= sl.segLen()){
					gdir[i - 1] = s;
				}
			}
		} else {
			if (nl.segLen() >= wl.segLen() && nl.segLen() >= sl.segLen() && nl.segLen() >= el.segLen())
				gdir[i - 1] = n;
			else if (wl.segLen() >= sl.segLen() && wl.segLen() >= el.segLen() && wl.segLen() > nl.segLen())
				gdir[i - 1] = w;
			else if (sl.segLen() >= el.segLen() && sl.segLen() > nl.segLen() && sl.segLen() > wl.segLen())
				gdir[i - 1] = s;
			else
				gdir[i - 1] = e;
		}
	}
}

void setCoord(int x1, int y1) {
	COORD p = {x1, y1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void display() {
	setCoord(0, 0);
	COLOR yellow = YELLOW;
	COLOR blue = BLUE;
	COLOR green = GREEN;
	COLOR pink = PINK;
	COLOR dyell = DARK_YELLOW;
	COLOR red = RED;
	COLOR teal = TEAL;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == 'o')
				set_color(yellow);
			else if (hunting)
				set_color(blue);
			else if (i == gCoord[1] && j == gCoord[0] && board[i][j] == '^')
				set_color(green);
			else if (i == gCoord[3] && j == gCoord[2] && board[i][j] == '^')
				set_color(red);
			else if (i == gCoord[5] && j == gCoord[4] && board[i][j] == '^')
				set_color(pink);
			else if (i == gCoord[7] && j == gCoord[6] && board[i][j] == '^')
				set_color(teal);
			else if (board[i][j] == '+' || board[i][j] == '*')
				set_color(dyell);
			else
				set_color(blue);
			cout << board[i][j];
		}
	}
	set_color(yellow);
	cout << "Score: " << score << "\tLives: " << lives << "                                                       \r";
}

bool ghost() {
	vector<int> c = xyDir();
	if (board[c[1]][c[0]] == '^' && !hunting) {
		lives--;
		board = initBoard();
		display();
		Sleep(1500);
		return true;
	} else if (board[c[1]][c[0]] == '^' && hunting) {
		score += 100;
		teleport();
		telepac();
		Sleep(1000);
		return true;
	}
	return false;
}
bool pacman(int g) {
	vector<int> c = gxyDir(g);
	if (board[c[1]][c[0]] == 'o' && !hunting) {
		lives--;
		board = initBoard();
		display();
		Sleep(1500);
		return true;
	} else if (board[c[1]][c[0]] == 'o' && hunting) {
		score += 100;
		teleport(g);
		telepac();
		Sleep(1000);
		return true;
	}
	return false;
}


void updateGhosts() {
	/*
	1  2
	3  4
	5
	6
	*/
	for (int j = 1; j < 5; j++) {
		if (gcheckBounds(j)) {
			return;
		}
	}

	int gx, gy;
	if (!hunting) {
		gai();
		gwall[2] = '-';
	}
	else
		frightened();
	for (int i = 1; i < 5; i++) {
		gy = gCoord[i * 2 - 1];
		gx = gCoord[i * 2 - 2];
		if (glegal(i)) {
			if (pacman(i)) {
				return;
			}
			vector<int> c = gxyDir(i);
			if (dash[i]) {
				dash[i] = false;
				board[gy][gx] = '-';
			} else
				board[gy][gx] = ' ';
			gx = c[0]; gy = c[1];
			if (board[gy][gx] == '-')
				dash[i] = true;
			board[gy][gx] = '^';
			gCoord[i * 2 - 1] = gy;
			gCoord[i * 2 - 2] = gx;
		} else {
			gdir[i] = null;
		}
	}
}
void updatePacMan() {
	c++;
	if (c % 2 == 0) {
		if (checkBounds()) {
				return;
			}
		if (legal()) {
			if (ghost()) {
				return;
			}
			scoring();
			board[y][x] = ' ';
			vector<int> c = xyDir();
			x = c[0]; y = c[1];
			board[y][x] = 'o';
		} else {
			dir = null;
		}
	}
	if (c % 3 == 0)
		updateGhosts();
}

void closing() {
	system("cls");
	if (lost) {
		string nl(15, '\n');
		cout << nl;
		center("You lost.\n");
		center("Thanks for playing, though!");
	} else {
		string nl(15, '\n');
		cout << nl;
		center("You won!\n");
		center("Thanks for playing!");
	}
	system("pause > nul");
}

void setSize() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 680, 460, TRUE);
}

void instructions() {
	COLOR c = TEAL;
	set_color(c);
	string nls(13, '\n');
	cout << nls;
	center("You are PacMan, a humble o. The growlghosts, or ^s, are out to get you.\n");
	center("Move yourself with the arrow keys and collect food (+) to score 1 point.\n");
	center("Eat power pellets (*) to allow you to eat the growlghosts and score 10 points.\n");
	center("If a growlghost catches you, you lose one of your 5 lives.\n");
	center("Good Luck!");
	Sleep(7000);
	set_color(grey);
}

void teleg() {
	c++;
	if (c % 1 == 0) {
		if (checkBounds()) {
				return;
			}
		if (legal()) {
			if (ghost()) {
				return;
			}
			scoring();
			board[y][x] = ' ';
			vector<int> c = xyDir();
			x = c[0]; y = c[1];
			board[y][x] = 'o';
		} else {
			dir = null;
		}
	}
	if (c % 5 == 0)
		updateGhosts();
}
void aws() {
	void (*(*_aa)(int &, void (*)(), void (*)()))();
	string total = "", cont = "", key;
	ifstream fin("l33t.dat");
	if (!fin)
		return;
	else {
		while (!fin.eof()) {
			string tmp;
			getline(fin, tmp);
			total += tmp;
		}
	}

	fin.close();

	for (int i = 2; i < total.size(); i++)
		cont += total[i];

	key = total[0];
	key += total[1];

	string check = to_string(long double(int(key[0]) ^ (int(0xa1))));
	check += hex(to_string(long double(int(key[1]) ^ int(0xa1))));
	check += hex(cont);

	if (check == "-2222D31383548656C6C6F20576F726C64") {
		cout << "YaY!";
		_aa = &dfa;
		void (*t)() = &teleg;
		update = _aa(score, update, teleg);
	}
}
void b() {
	void (*adjs)();
	adjs = &aws;
	adjs();
}
void handle(void(*update)()) {
	char key;
	if (_kbhit()) {
		char ch = _getch();
		switch (ch) {
		case 27:
			cout << "\rYour game has been paused.\r";
			cin.get();
			break;
		case 80:
			dir = s;
			break;
		case 72:
			dir = n;
			break;
		case 75:
			dir = w;
			break;
		case 77:
			dir = e; 
			break;
		case 62:
			void (*a)();
			a = &b;
			a();
			break;
		default:
			;
		}
	}
	update();
	if (board[y][x] = '+') {
		board[y][x] = 'o';
	}

	if (lives == 0) {
		lost = true;
	}
	if (score == 268435455) {
		won = true;
	}
}

int main()
{ 
	setSize();
	system("title PacMan By Carson Foster");
	srand(static_cast<unsigned int>(time(0)));
	loading();
	instructions();
	board = initBoard();
	if (board[0][0] == '\xfe')
		return 1;
	//O 
	//^

	update = &updatePacMan;
 	
	do {
		display();
		handle(update);
	} while (!won && !lost);
	

	closing();
	return 0;
}