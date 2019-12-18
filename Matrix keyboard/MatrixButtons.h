/*
 * MatrixKeyboard.h
 *
 * Created: 07.10.2019 16:08:17
 *  Author: User
 */ 


#ifndef MATRIXBUTTONS_H_
#define MATRIXBUTTONS_H_

#include <avr/io.h>

#define ROL_PORT_INDEX	C
#define ROL_PIN_1	3
#define ROL_PIN_2	4
#define ROL_PIN_3	5

#define COL_PORT_INDEX	B
#define COL_PIN_A	0
#define COL_PIN_B	1
#define COL_PIN_C	2
#define COL_PIN_D	3

#define BT_PORT_INDEX	D
#define BT_PIN_1	6
#define BT_PIN_3	7

/* ---------------------------- Macro Magic ---------------------------- */
#define		CONCAT(a,b)			a ## b

#define		CONCAT_DDR(a)			CONCAT(DDR, a)
#define		CONCAT_PORT(a)			CONCAT(PORT, a)
#define		CONCAT_PIN(a)			CONCAT(PIN, a)

#define		ROL_DDR		CONCAT_DDR(ROL_PORT_INDEX)
#define		ROL_PIN		CONCAT_PIN(ROL_PORT_INDEX)
#define		ROL_PORT	CONCAT_PORT(ROL_PORT_INDEX)

#define		COL_DDR		CONCAT_DDR(COL_PORT_INDEX)
#define		COL_PIN		CONCAT_PIN(COL_PORT_INDEX)
#define		COL_PORT	CONCAT_PORT(COL_PORT_INDEX)

#define		BT_DDR		CONCAT_DDR(BT_PORT_INDEX)
#define		BT_PIN		CONCAT_PIN(BT_PORT_INDEX)
#define		BT_PORT		CONCAT_PORT(BT_PORT_INDEX)

void MatrixButtons_Init();

void MatrixButtons_Scan(unsigned char out[]);

#endif /* MATRIXBUTTONS_H_ */