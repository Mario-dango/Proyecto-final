//Dirección del proyecto cd Documents/informatica2020/proyecto/proto

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#define linea 53

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

/////////////// DHT11  ////////////////

#define MAX_TIMINGS	85
#define DHT_PIN		3	/* GPIO-22 */
 
int data[5] = { 0, 0, 0, 0, 0 };
float h;
float c;
float *p_hum = &h;
float *p_temp = &c;

float read_dht_data(void);

///////////////////////////////////////
typedef struct  
{
	int ciclo;
	float temp;
	float hum;
	char *fecha;
}REGISTRO;

REGISTRO *datos;

///////////////////////////////////////
float prom (REGISTRO datos, int index);
float dev (REGISTRO datos , int index);
float var (REGISTRO datos , int index);
float moda (REGISTRO datos , int index);

int lectura(FILE *flujo);
int escritura(FILE *flujo, int indice);
void evaluar ();
void vaciar (char cadena[], int tam);
void cargar (char cadena[], int index);
void cargarFecha (char cadena[], int i);
void tomarfecha (char *cadena[]);
void valores_LCD();


int main(void){

	if (wiringPiSetup () == -1) exit (1);

	pinMode(7, OUTPUT);	//Pin Led Verde (GPIO 7) [BCM-GPIO 04]
	pinMode(2, OUTPUT);     //Pin Led Amarillo (GPIO 0) [BCM-GPIO 17]
	pinMode(0, OUTPUT);     //Pin Led Rojo (GPIO 2) [BCM-GPIO 27]
	pinMode(1, INPUT);      //Pin del Pulsador (GPIO 1) [BCM-GPIO 18]


	fd = wiringPiI2CSetup(I2C_ADDR);

	//printf("fd = %d ", fd);

	lcd_init();		// Función de inicialización del LCD
	printf( "Inicio del programa proyecto final Parte A" );

	ClrLcd();
	lcdLoc(LINE2);
	typeln("projt integrador");
	lcdLoc(LINE1);
	typeln("Informatic 2020");
	delay(3000);

	printf("\nPor favor presionar el boton de la estacion para comenzar a registrar datos.");
	ClrLcd();
	lcdLoc(LINE2);
	typeln("Presionar para");
	lcdLoc(LINE1);
	typeln("inicializar");
	while (digitalRead(1))
	ClrLcd();

	int cont, indice;
	float temp_prom, humd_prom;
	char tiempo;
  	clock_t t_ini, t_fin;  
	double secs, t_ciclo;
	FILE *p_flujo1, *p_flujo2;
	datos = (REGISTRO*)malloc(cont*sizeof(REGISTRO));
	int flag = lectura(p_flujo1);
	int F_cont = escritura(p_flujo2, flag);	

	t_ini = clock();	
	while (flag == 1){							//Condición ciclica del programa
		t_fin = clock();
	/************************** Evaluo el estado de mi Temperatura*/
		evaluar();
		read_dht_data(); 	 // Función de lectura del DHT22

		if (!digitalRead(1)){
			t_ini = clock();
			ClrLcd();
			lcdLoc(LINE1);
			typeln("Se presiono B1?");
			lcdLoc(LINE2);
			typeln("Me finalizas?");
			while (digitalRead(1))
			t_fin = clock();				
			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
			if (secs <= 2.5)
			{
				ClrLcd();
				lcdLoc(LINE1);
				typeln("Guardando datos...");
				lcdLoc(LINE2);
				typeln("Close program.");
				delay(200);
				
				datos[indice].temp = *p_temp;
				datos[indice].hum = *p_hum;
				tomarfecha(tiempo);
				cargarFecha(tiempo, indice);

				flag = 0;				//condición para salir del while
				ClrLcd();
				digitalWrite(7, LOW);
				digitalWrite(2, LOW);
				digitalWrite(0, LOW);
			} else if (secs > 10)
			{
				t_ciclo = 10;
			} else
			{
				t_ciclo = secs;
				secs = 0;
			}
		}	

		/**************************Asignación de valores en matriz datos*/
		if ((t_ini - t_fin) >= t_ciclo)
		{	
			while ((*p_temp == 0) && (*p_hum == 0))
			{
				read_dht_data(); 	 // Función de lectura del DHT22
			}
			
			datos[indice].temp = *p_temp;
			datos[indice].hum = *p_hum;
			tomarfecha(tiempo);
			cargarFecha(tiempo, indice);
			indice ++;
			escritura(p_flujo2, indice);
			//temp_prom = promedio(&p_temp);	
			/*****LCD****/		
			ClrLcd();
			lcdLoc(LINE1);
			typeln("Temp:    ");
			typeFloat(*p_temp);
			typeln("*c");
			lcdLoc(LINE2);
			typeln("Humd:    ");
			typeFloat(*p_hum);
			typeln("%r");
			/*****PRINTF****/	
			printf("\nLa temperatura actual es de %4.2f y en promedio hasta el momento es de %4.f", datos[indice].temp, temp_prom);
			printf("\nLa humedad relativa actual es de %4.2f y en promedio hasta el momento es de %4.f", datos[indice].hum, humd_prom);	
			
			if (indice == 28){
				printf("\n\nSe han registrado 28 lecturas correctamente.");
				printf("\nSi desea registrar otras 28 lecturas mantenga presionado el botón por 5 segundos");
				printf("\nCaso contrario para finalizar sueltelo antes.");
				while (digitalRead(1))
				t_ini = clock();
				while (!digitalRead(1))
				t_fin = clock();				
				secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
				if (secs >= 5)
				{
					printf("\nHa de proceder a relizar un nuevo bloque de registro.");
					temp_prom = 0;
					humd_prom = 0;
					indice = 0;
				} else if (secs < 5)
				{
					printf("\nSe ha de finalizar el programa, gracias por usarlo.");
					flag = 0;
				} else
				{
					printf("\n\nHa ocurrido un problema en el conteo, se procede a cerrar el programa.");
					flag = 0;
				}
			}
		}			
	}
	return 0;
}

/*************** Funciones de Ficheros******/
int lectura(FILE *flujo){

	char auxiliar;
	flujo = fopen("archivo.txt", "r");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		exit(1);
	}

	while (auxiliar != '\n')
	{
		auxiliar = fgetc(flujo);
		if ((auxiliar == '=') && (auxiliar == '*'))				//reviso si está el titulo escrito
		{
			fclose(flujo);	
			return 1;
		}		
	}
	fclose(flujo);	
	return 0;
}

int escritura(FILE *flujo, int indice){

	char temp[linea], auxiliar;
	flujo = fopen("archivo.txt", "a");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		return 1;
	}
	if (indice == 0)
	{
		fputc('(', flujo);
		for (int i = 0; i < (linea-3); i++)
		{
			fputc('=', flujo);
		}
		fputc(')\n', flujo);
		fprintf(flujo, "TRABAJO INTEGRADOR INFORMATICA 2020, INTEGRANTES:\n");
		fprintf(flujo, "*Mario Papetti 11807\n");
		fprintf(flujo, "*Augusto Caceres 11820\n");
		fprintf(flujo, "*Miled Felicito 11901\n");
		fprintf(flujo, "*Franco Siccatto 12225\n");
		fputc('(', flujo);
		for (int i = 0; i < (linea-3); i++)
		{
			fputc('=', flujo);
		}
		fputc(')\n', flujo);
	} else if (indice == 1)
	{	
		if (indice == 0)
		{
			fputc('\n(', flujo);
			for (int i = 0; i < (linea-3); i++)
			{
				fputc('#', flujo);
			}
			fputc(')\n', flujo);
			fprintf(flujo, "Registro iniciado en la fecha: ");
			tomarfecha(&auxiliar);
			fprintf(flujo, auxiliar);
			fputc('\n', flujo);
			vaciar(auxiliar, linea);
			fprintf(flujo, "(ciclo)|(temperatura)|(humedad)|      (fecha)\n");
		}
		fprintf(flujo, "   %i.  |     %3.1f    |  %3.1f   | %s", datos[indice].ciclo, datos[indice].temp, datos[indice].hum, datos[indice].fecha);
	} else 
	{
		fprintf(flujo, "   %i.  |     %3.1f    |  %3.1f   | %s", datos[indice].ciclo, datos[indice].temp, datos[indice].hum, datos[indice].fecha);
	}
	fclose(flujo);	
	return;
}

void vaciar (char cadena[], int tam){
	for (int i = 0; i < tam; i++)
	{
		cadena[i] = '\0';
	}
	return;	
}

void tomarfecha (char *cadena[]){	
	time_t tiempo;
	struct tm *tlocal;

	tiempo = time(NULL);
	tlocal = localtime(&tiempo);
	strftime(cadena, linea, "%d/%m/%Y %H:%M:%S", tlocal);
	return;
}

/*************** Funciones Estadisticas******/



void cargarFecha (char cadena[], int i){
	int N = strlen(cadena) + 1;						//tamaño de caracteres + el '\n'
	datos[i].fecha = (REGISTRO*)malloc(N*sizeof(char));
	if (datos[i].fecha == NULL)
	{
		printf("No se ha podido reservar memoria. \n");
		exit(1);
	}
	strcpy(datos[i].fecha, cadena);     //función para copiar strings (char destino, char fuente)
	return;	
}

void evaluar (){	
	if ((*p_temp >= 25.5)&&(*p_hum >= 50.5)){
		printf("La temperatura ha superado el valor de 25.5°c.\n");
		digitalWrite(7, LOW);
		digitalWrite(2, LOW);
		digitalWrite(0, HIGH);
	}else if ((*p_temp >= 25.5)||(*p_hum >= 50.5)){
		digitalWrite(7, LOW);
		digitalWrite(2, HIGH);
		digitalWrite(0, LOW);
	} else {
		digitalWrite(7, HIGH);
		digitalWrite(2, LOW);
		digitalWrite(0, LOW);
	}
}


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

float read_dht_data(void){
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode( DHT_PIN, OUTPUT );
	digitalWrite( DHT_PIN, LOW );
	delay( 18 );

	/* prepare to read the pin */
	pinMode( DHT_PIN, INPUT );

	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHT_PIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHT_PIN );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 50 )
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];	// for DHT11
		}
		c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];	// for DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}
		float f = c * 1.8f + 32;
		//printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f);
	}else  {
		c = 0; h = 0;
		//printf( "Data not good, skip\n" );
	}
}