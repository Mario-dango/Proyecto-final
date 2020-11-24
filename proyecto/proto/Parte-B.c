//Dirección del proyecto cd Documents/informatica2020/proyecto/proto

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

/////////////// LCD i2c  ////////////////
// Define some device parameters
#define I2C_ADDR   0x27 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0b00000100 // Enable bit

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);
int fd;  // seen by all subroutines
///////////////////////////////////////


typedef struct  
{
	int ciclo;
	float temp;
	float hum;
	char *fecha;
}REGISTRO;

REGISTRO *datos;

/*///////////////////////////////////////
float prom (registro datos, int index);
float dev (registro datos , int index);
float var (registro datos , int index);
float moda (registro datos , int index);
*/


int lectura(FILE *flujo);
void copiar (char cadena[], int i);
void vaciar (char cadena[]);
void valores_LCD();


int main(void){

	char temp[100];
	int cont;
	FILE *flujo;
	lectura(flujo);


	if (wiringPiSetup () == -1) exit (1);

	short flag = 1;
	pinMode(7, OUTPUT);		//Pin Led Verde (GPIO 7) [BCM-GPIO 04]
	pinMode(2, OUTPUT);     //Pin Led Amarillo (GPIO 0) [BCM-GPIO 17]
	pinMode(0, OUTPUT);     //Pin Led Rojo (GPIO 2) [BCM-GPIO 27]
	pinMode(1, INPUT);      //Pin del Pulsador (GPIO 1) [BCM-GPIO 18]

	fd = wiringPiI2CSetup(I2C_ADDR);

	lcd_init();		// Función de inicialización del LCD
	printf( "Inicio del programa proyecto final Parte B" );

	ClrLcd();
	lcdLoc(LINE2);
	typeln("Iniciando ...");
	lcdLoc(LINE1);
	typeln("Informatic 2020");
	delay(3000);

	while (flag == 1){							//Condición ciclica del programa
		for (int j = 0; j < 28; j++){			//for para guardar datos en la matriz

	/************************** Evaluo el estado de mi Temperatura*/

		valores_LCD(&flag, j);
			
		}
	}

	return 0;

}


/*************** Funciones de fichero******/

int lectura(FILE *flujo){

	char temp[100], auxiliar;
	int cont = 0;
	flujo = fopen("archivo.txt", "r");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		return 1;
	}

	flujo = fopen("archivo.txt", "r");
	
	while (!feof(flujo))
	{
		fgets(temp, 100, flujo);
	}
	rewind(flujo);             //coloca el cursor del archivo que se encuentra en una posición x, al inicio del archivo
	
	datos = (REGISTRO*)malloc(cont*sizeof(REGISTRO));

	for (int i = 0; !feof(flujo); i++)
	{
		vaciar(temp);
		auxiliar = '0';
		for (int j = 0; (auxiliar != '|'); j++)
		{
			auxiliar = fgetc(flujo);
			if (auxiliar != '|')
			{
				temp[j] = auxiliar;
			} /*else if ((auxiliar == '=') && (auxiliar == ')') && (auxiliar == '('))
			{
				printf("%c", auxiliar);
			}*/
						
		}
		copiar(temp,i);		

		fgets(temp, 100, flujo);
		datos[i].ciclo = atoi(temp);

	}
	fclose(flujo);
}



void copiar (char cadena[], int i){
	int N = strlen(cadena) + 1;
	datos[i].fecha = (REGISTRO*)malloc(N*sizeof(char));
	if (datos[i].fecha == NULL)
	{
		printf("No se ha podido reservar memoria. \n");
		exit(1);
	}
	strcpy(datos[i].temp, cadena);     //función para copiar strings (char destino, char fuente)
	return;	
}

void vaciar (char cadena[]){
	for (int i = 0; i < 100; i++)
	{
		cadena[i] = '\0';
	}
	return;	
}

void valores_LCD(int *flag, int j){
	//ClrLcd();
	lcdLoc(LINE1);
	typeln("Temp:    ");
	typeFloat(datos[j].temp);
	typeln("*c");
	lcdLoc(LINE2);
	typeln("Humd:    ");
	typeFloat(datos[j].hum);
	typeln("%r");
	delay(200);

	if (!digitalRead(1)){
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Se presiono B1?");
		lcdLoc(LINE2);
		typeln("Me finalizas?");
		delay(3000);

		if (!digitalRead(1)) {
			ClrLcd();
			lcdLoc(LINE1);
			typeln("Guardando datos...");
			lcdLoc(LINE2);
			typeln("Close program.");
			delay(1500);
			flag = 0;				//condición para salir del while
			j = 28;					//condición para salir del for
			ClrLcd();
			digitalWrite(7, LOW);
			digitalWrite(2, LOW);
			digitalWrite(0, LOW);
		}
	}	
}

/*************** Funciones Estadisticas******/
/*
float prom (registro datos, int index){
	float promedio;
	float sum;
	for (int i = 0; i < index; i++){
		sum += datos[i].temp;
	}
	promedio = sum / index;
	return (promedio);
}

float dev (registro datos , int index){
	//asdasdasd
}
float moda (registro datos , int index){	
	float moda;
	int aux0, aux1;
	for (int j = 0; j < index; j++)
	{
		aux1 = 0;
		for (int i = 0; i < index; i++){
			if ((datos[i].temp == datos[j].temp) && (i != j)){
				aux1++;
			}
		}
		if ((aux1 > 0) && (aux1 > aux0))
		{
			aux0 = aux1;
			moda = datos[j].temp;
		} /*else if ((aux1 == aux0) && (aux0 > 0))
		{
			//hay dos valores modales

		}
	}	
	return moda;
}
*/


/*************** Funciones del LCD 16x02******/

// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void lcd_init()   {
  // Inicialización del LCD
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}
