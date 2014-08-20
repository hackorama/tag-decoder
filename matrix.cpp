#include "matrix.h"

Matrix::Matrix()
{
	RB_TR[0] = 8;
	RB_TR[1] = 9;
	RB_TR[2] = 4;
	RB_TR[3] = 5;
	RB_TR[4] = 3;
	RB_TR[5] = 2;
	RB_TR[6] = 7;
	RB_TR[7] = 6;
	RB_TR[8] = 1;
	RB_TR[9] = 0;

	RB_BL[0] = 9;
	RB_BL[1] = 8;
	RB_BL[2] = 5;
	RB_BL[3] = 4;
	RB_BL[4] = 2;
	RB_BL[5] = 3;
	RB_BL[6] = 7;
	RB_BL[7] = 6;
	RB_BL[8] = 0;
	RB_BL[9] = 1;

	RB_BR[0] = 1;
	RB_BR[1] = 0;
	RB_BR[2] = 3;
	RB_BR[3] = 2;
	RB_BR[4] = 5;
	RB_BR[5] = 4;
	RB_BR[6] = 6;
	RB_BR[7] = 7;
	RB_BR[8] = 9;
	RB_BR[9] = 8;

	TL[0] = 0;
	TL[1] = 4;
	TL[2] = 8;
	TL[3] = 1;
	TL[4] = 5;
	TL[5] = 9;
	TL[6] = 2;
	TL[7] = 6;
	TL[8] = 10;
	TL[9] = 3;
	TL[10] = 7; 
	TL[11] = 11;

	TR[0] = 1;
	TR[1] = 5;
	TR[2] = 9;
	TR[3] = 3;
	TR[4] = 7;
	TR[5] = 11;
	TR[6] = 0;
	TR[7] = 4;
	TR[8] = 8;
	TR[9] = 2;
	TR[10] = 6; 
	TR[11] = 10;

	BL[0] = 2;
	BL[1] = 6;
	BL[2] = 10;
	BL[3] = 0;
	BL[4] = 4;
	BL[5] = 8;
	BL[6] = 3;
	BL[7] = 7;
	BL[8] = 11;
	BL[9] = 1;
	BL[10] = 5; 
	BL[11] = 9;

	BR[0] = 3;
	BR[1] = 7;
	BR[2] = 11;
	BR[3] = 2;
	BR[4] = 6;
	BR[5] = 10;
	BR[6] = 1;
	BR[7] = 5;
	BR[8] = 9;
	BR[9] = 0;
	BR[10] = 4; 
	BR[11] = 8;

}

Matrix::~Matrix()
{
}

void
Matrix::transCode(int code[])
{
	for(int i=0; i < 12; i++){
		cout << code[transCode(i)] << endl;
	}

}

int
Matrix::transCode(int anchor, int index)
{
	if( index < 0 || index > 11 ) return -3;

	switch( anchor ){
		case TOP_L:
			return TL[index];
		case TOP_R:
			return TR[index];
			break;
		case BOT_L:
			return BL[index];
			break;
		case BOT_R:
			return BR[index];
			break;
		default:
			break;
	}
	return index;
}

int
Matrix::transCode(int index)
{
	if( index < 0 || index > 11 ) return -3;
	return TL[index];
}

int
Matrix::rotateBlock(int anchor, int value)
{
	if( value < 0 || value > 9 ) return -2;
	switch( anchor ){
		case TOP_L:
			return value;
			break;
		case TOP_R:
			return RB_TR[value];
			break;
		case BOT_L:
			return RB_BL[value];
			break;
		case BOT_R:
			return RB_BR[value];
			break;
		default:
			break;
	}
	return value;
}
