#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>
#include "game.h"
#include "standard.h"

/* Address of the temperature sensor on the I2C bus */


//*************************************************************************

/* Wait for I2C bus to become idle */
void i2c_idle() {
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data) {
	i2c_idle();
	I2C1TRN = data;
	i2c_idle();
	return !(I2C1STAT & (1 << 15)); //ACKSTAT
}

/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; //RCEN = 1
	i2c_idle();
	I2C1STATCLR = 1 << 6; //I2COV = 0
	return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5; //ACKDT = 0
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5; //ACKDT = 1
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send start conditon on the bus */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; //SEN
	i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; //RSEN
	i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; //PEN
	i2c_idle();
}

#define HIGH_HIGH 0x0
#define HIGH_LOW 0x01
#define HIGH_SCORE 0x50

void writeTo(int a)
{
	do
	{
			i2c_start();
	}
	while(!i2c_send(HIGH_SCORE << 1));
	i2c_send(HIGH_HIGH);
	i2c_send(HIGH_LOW);
	i2c_send(a);
	i2c_stop();
}
int readFrom(void)
{
	do
	{
			i2c_start();
	}
	while(!i2c_send(HIGH_SCORE << 1));
	i2c_send(HIGH_HIGH);
	i2c_send(HIGH_LOW);
	do
	{
			i2c_start();
	}
	while(!(i2c_send(HIGH_SCORE << 1 | 1)));
	int a = i2c_recv();
	i2c_nack();
	i2c_stop();
	return a;
}

void delay(int a){
	int i;
	for(i = 0; i < a; i++){
	}
}

void startScreen(void){
	while(BUTTON1)
	{

	}
	r = 0;
	while(!BUTTON1){
		renderStartScreen();
		r++;

	}
	running = 1;
}

void gameOverScreen(){
	delay(1000000);

	renderEndScreen();
	while(!BUTTON1){
		//renderEndScreen();
	}
	gameOver = 0;
}


int main(void) {

	/* Code from labs */

	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
  SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

		/* Own code */

	/* Set up i2c */
	I2C1CON = 0x0;
	/* I2C Baud rate should be less than 400 kHz, is generated by dividing
	the 40 MHz peripheral bus clock down */
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 13; //SIDL = 1
	I2C1CONSET = 1 << 15; // ON = 1


	/* Initiate timer */
	TMR2 = 0;
	T2CON |= (0x6 << 4);
	int gameTime;
 	if((PORTD >> 10) & 0x1)
 	{
 		gameTime = 30000;
 	}
 	else if((PORTD >> 9) & 0x1)
 	{
 		gameTime = 20000;
 	}
 	else if((PORTD >> 8) & 0x1)
 	{
 		gameTime = 10000;
 	}

	PR2 = gameTime;
	T2CON |= 0x8000;

	TRISE &= ~0xff;

	display_init();

	while(1){

		while(!running){
			startScreen();
		}

		srand((unsigned) r);

		initGame();
		int gameTime;
	 	if((PORTD >> 10) & 0x1)
	 	{
	 		gameTime = 30000;
	 	}
	 	else if((PORTD >> 9) & 0x1)
	 	{
	 		gameTime = 20000;
	 	}
	 	else if((PORTD >> 8) & 0x1)
	 	{
	 		gameTime = 10000;
	 	}
		PR2 = gameTime;

		while(running){
			if(IFS(0)){
				IFS(0) = 0;
				tick();
				render();
				gameTime -= 4;
				PR2 = gameTime;
			}
		}

		currentHighscore = readFrom();
		if(score > currentHighscore){
			currentHighscore = score;
			writeTo(currentHighscore);
		}

		while(gameOver){
			gameOverScreen();
		}

	}

}