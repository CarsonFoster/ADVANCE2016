#include "stdafx.h"
#include <iostream>
#include "point.h"


using namespace std;

// ** constructors **
point::point() { // default
	xCoord = 0.0;
	yCoord = 0.0;
}
point::point(float a, float b) { //parameter list
	xCoord = a;
	yCoord = b;
}
point::point(const point &c) { // copy constructor
	xCoord = c.getXcoord();
	yCoord = c.getYcoord();
}

// ** Inspectors **
float point::getXcoord() const {
	return xCoord;
}
float point::getYcoord() const {
	return yCoord;
}

// ** mutators **
void point::setXcoord(float d) {
	xCoord = d;
	return;
}
void point::setYcoord(float e) {
	yCoord = e;
	return;
}

// ** Facilitators **
//right hand side point
point &point::operator= (const point &rhsp) {
	xCoord = rhsp.getXcoord();
	yCoord = rhsp.getYcoord();
	return *this;
}
void point::insert(ostream &sout) const {
	sout << "(" << xCoord << "," << yCoord << ")";
	return;
}
void point::extract(istream &sin) {
	char leftParen, comma, rightParen;
	float x, y;
	sin >> leftParen >> x >> comma >> y >> rightParen;
	xCoord = x;
	yCoord = y;
	return;
}
int point::getQuad() {
	if (xCoord > 0.0 && yCoord > 0.0) {
		return 1;
	} else if (xCoord < 0.0 && yCoord > 0.0) {
		return 2;
	} else if (xCoord < 0.0 && yCoord < 0.0)  {
		return 3;
	} else if (xCoord > 0.0 && yCoord < 0.0) {
		return 4;
	} else {
		return 0;
	}
}

// ** auxiliary functions ** outside class
ostream &operator << (ostream &sout, const point &p) {
	p.insert(sout);
	return sout;
}
istream &operator >> (istream &sin, point &p) {
	p.extract(sin);
	return sin;
}