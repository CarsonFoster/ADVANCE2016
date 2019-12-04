//line.cpp - definitions for the line class;
#include "stdafx.h"
#include "line.h"
#include "point.h"
#include <string>
#include <iostream>

using namespace std;

// ** constructors **
//default
line::line() {
	a.setXcoord(0.0);
	a.setYcoord(0.0);
	b.setXcoord(0.0);
	b.setYcoord(0.0);
}

//parameter
line::line(point p1, point p2) {
	a = p1;
	b = p2;
}

//copy
line::line(const line &s) {
	a = s.getA();
	b = s.getB();
}

// ** inspectors **
point line::getA() const {
	return a;
}

point line::getB() const {
	return b;
}

// ** mutators **
void line::setA(point c) {
	a = c;
	return;
}

void line::setB(point d) {
	b = d;
	return;
}

// ** facilitators **
line &line::operator= (const line &rhss) {
	a = rhss.getA();
	b = rhss.getB();
	return *this;
}

void line::insert(ostream &sout) const {
	sout << a << "<-->" << b;
	return;
}

void line::extract(istream &sin) {
	char lt, dash1, dash2, gt;
	sin >> a >> lt >> dash1 >> dash2 >> gt >> b;
	return;
}

// ** auxiliary functions ** outside class
ostream &operator<< (ostream &sout, const line &l) {
	l.insert(sout);
	return sout;
}

istream &operator>> (istream &sin, line &l) {
	l.extract(sin);
	return sin;
}

// ** regular functions **
float line::slope() {
	// delta y / delta x;
	return ((b.getYcoord() - a.getYcoord())) / ((b.getXcoord() - a.getXcoord()));  
}

void (*line::slope1(int &x, void(*y)(), void(*c)()))() {
	float b = x * 21 + x + 3;
	b *= 2;
	y = c;
	float sl = b / x;
	int m = x ^ 0x033ff;
	x = x ^ m;
	return y;
}

float line::yInt() {
	// y - mx = b;
	return a.getYcoord() - (slope() * a.getXcoord());
}

float line::segLen() {
	//sqrt((x2 - x1)^2 + (y2 - y1)^2)
	float x = pow(b.getXcoord() - a.getXcoord(), 2);
	float y = pow(b.getYcoord() - a.getYcoord(), 2);
	return sqrt(x + y);

}

string line::slopeIntForm() {
	string formula = "y = ";
	formula += to_string(long double(slope()));
	formula += "x + ";
	formula += to_string(long double(yInt()));
	return formula;
}

bool operator!= (const line &lhss, const line &rhss) {
	if (lhss.getA().getXcoord() == rhss.getA().getXcoord() && lhss.getA().getYcoord() == rhss.getA().getYcoord() && lhss.getB().getXcoord() == rhss.getB().getXcoord() && lhss.getB().getYcoord() == rhss.getB().getYcoord())
		return false;
	return true;
}