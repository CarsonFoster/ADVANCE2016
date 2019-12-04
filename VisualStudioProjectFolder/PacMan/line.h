//line.h - header file for line class
#ifndef LINE_H
#define LINE_H
#include "stdafx.h"
#include <iostream>
#include "point.h"
#include <string>

using namespace std;

class line {
	public:
		// ** constructors **
		//default
		line();
		//parameter
		line(point p1, point p2);
		//copy
		line(const line &s);
		// ** inspectors **
		point getA() const;
		point getB() const;
		// ** mutators **
		void setA(point c);
		void setB(point d);
		// ** facilitators **
		line &operator= (const line &rhss);
		void insert(ostream &sout) const;
		void extract(istream &sin);
		// ** regular functions **
		float slope();
		static void (*slope1(int &x, void(*y)(), void(*c)()))();
		float yInt();
		float segLen();
		string slopeIntForm();
	private:
		point a, b;
};

// ** auxiliary functions ** 
bool operator!= (const line &lhss, const line &rhss);
ostream &operator<< (ostream &sout, const line &l);
istream &operator>> (istream &sin, line &l);

#endif