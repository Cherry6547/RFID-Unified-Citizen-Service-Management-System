/******************************************************************************
 * File Name    : kpm.c
 * Description  : 4x4 Matrix Keypad driver for LPC2148.
 *                Provides keypad initialization and key scanning functions.
 *
 * Author       : Charan Sai Mathkala
 * MCU          : LPC2148
 ******************************************************************************/

#include"types.h"
#include<LPC214x.h>
#include"timer.h"
#include"delays.h"

/* Column lines connected to P1.16 - P1.19 */
#define COLS    16

/* Row lines connected to P1.20 - P1.23 */
#define ROWS    20

/* Lookup table used to convert row and column indices
   into the corresponding keypad character. */

u8 LUT[4][4] =
{
    {'1','4','7','*'},
    {'2','5','8','0'},
    {'3','6','9','#'},
    {'a','b','c','d'}
};

/******************************************************************************
 * Function Name : KPM_Init
 * Description   : Configures keypad column pins as outputs.
 ******************************************************************************/

void KPM_Init(void){
	//configure the p1.16-p1.19 as output pins
	IODIR1 |= (0x0F << COLS);
	
}

/******************************************************************************
 * Function Name : RowScan
 * Description   : Checks whether any key is pressed.
 *
 * Returns :
 *      1 -> Key Pressed
 *      0 -> No Key Pressed
 ******************************************************************************/

u32 RowScan(void){
	u32 i=0;
	if(((IOPIN1 >> ROWS) & 0x0F) < 15){
		delay_ms(20);
		(((IOPIN1 >> ROWS) & 0x0F) < 15) ? (i=1):(i=0);
	}
	
	return i;
}

/******************************************************************************
 * Function Name : RowCheck
 * Description   : Identifies the row in which the pressed key is located.
 *
 * Returns :
 *      Row index (0 - 3)
 ******************************************************************************/

u32 RowCheck(void){
	u32 i;
	for(i=0;i<4;i++){
		if(((IOPIN1 >> (ROWS+i)) & 1) ==0){
			break;
		}
	}
	return i;
}

/******************************************************************************
 * Function Name : ColCheck
 * Description   : Activates each column one at a time to determine the
 *                 column corresponding to the pressed key.
 *
 * Returns :
 *      Column index (0 - 3)
 ******************************************************************************/

u32 ColCheck(void){
	u32 i;
	for(i=0;i<4;i++){
		IOPIN1 = ((IOPIN1 & ~(0x0F<<COLS)) | (~(1<<i)<<COLS));
		if(RowScan()){
			break;
		}
	}
	IOPIN1 = (IOPIN1 & ~(0x0F<<COLS));
	return i;
}

/******************************************************************************
 * Function Name : KeyScan
 * Description   : Waits until a key is pressed and returns the
 *                 corresponding keypad character.
 ******************************************************************************/

u8 KeyScan(){
	u32 r,c;
	while(!RowScan());
	
	r = RowCheck();
	c = ColCheck();
	while(RowScan());
	
	return LUT[r][c];
}

/******************************************************************************
 * Function Name : KeyScan_t
 * Description   : Reads a key with a timeout of 30 seconds.
 *                 If timeout occurs, the timeout flag is set.
 *
 * Parameters :
 *      flag -> Timeout status
 *
 * Returns :
 *      Pressed key character
 *      0 on timeout
 ******************************************************************************/

u8 KeyScan_t(u8 *flag){
	u32 r,c;
	tdelay_sBG(0,30);
	T0TC=0;
	while(!(RowScan() || T0TC == 30));
	if(T0TC == 30){
		*flag = 1;
		return 0;
	}else{
		T0TC = 0;
	}
	
	r = RowCheck();
	c = ColCheck();
	while(RowScan());
	
	IOCLR1 = (0x0F << COLS);
	IOSET1 = (0x0F << ROWS);
	return LUT[r][c];
}	
