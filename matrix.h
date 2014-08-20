#ifndef _MATRIX_H_INCLUDED
#define _MATRIX_H_INCLUDED

#include "common.h"

#define TOP_L 1
#define TOP_R 2
#define BOT_L 3
#define BOT_R 4

using namespace std;

class Matrix
{

public:
	Matrix();
	~Matrix();

	void transCode(int code[]);
	int  transCode(int anchor, int index);
	int  transCode(int index);
	int  rotateBlock(int anchor, int value);

private:
	int TL[12];
	int TR[12];
	int BL[12];
	int BR[12];

	int RB_TR[10];
	int RB_BL[10];
	int RB_BR[10];
};

#endif /* _MATRIX_H_INCLUDED */
