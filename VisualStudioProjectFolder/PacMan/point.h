// point.h
// the point class header file

#ifndef POINT_H
#define POINT_H 

#include "stdafx.h"
#include <iostream>


using namespace std;

class point {

	public:
		// ** constructors **
		point();		// default
		point(float a, float b); //parameter list
		point(const point &c);	// copy constructor
		// ** Inspectors **
		float getXcoord() const;
		float getYcoord() const;
		// ** mutators **
		void setXcoord(float d);
		void setYcoord(float e);
		// ** Facilitators **
		//right hand side point
		point &operator = (const point &rhsp);
		void insert(ostream &sout) const;
		void extract(istream &sin);
		int getQuad();
	private:
		// data members almost always private
		float xCoord;
		float yCoord;
};

// ** auxiliary functions **
ostream &operator << (ostream &sout, const point &p);
istream &operator >> (istream &sin, point &p);

#endif