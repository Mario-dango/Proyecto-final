//Dirección del proyecto cd Documents/informatica2020/proyecto/proto

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct  
{
	int ciclo;
	float temp;
	float hum;
	char *fecha;
}REGISTRO;

REGISTRO *datos;


int lectura(FILE *flujo);
void copiar (char cadena[], int i);
void vaciar (char cadena[]);
void valores_LCD();
int finde(char *cadena);

int main(void){

	char temp[100];
	int cont;
	FILE *flujo;
	lectura(flujo);


	short flag = 1;

	while (flag == 1){							//Condición ciclica del programa
		for (int j = 0; j < 28; j++){			//for para guardar datos en la matriz

	/************************** Evaluo el estado de mi Temperatura*/

		valores_LCD(&flag, j);
			
		}
	}
	//system("pause");
	return 0;

}

//strtof() transforma cadenas en float
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
	
	while (finde(&temp) == 0)
	{
		fgets(temp, 100, flujo);
		printf("%s", temp);
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


int finde(char *cadena){
	for (int i = 0; i < 150; i++)
	{
		if (cadena[i] == '#'){
			if (cadena[i+1] == ')')
			{
				return 0;
			}			
		}
	}
	return 1;
}

void copiar (char cadena[], int i){
	int N = strlen(cadena) + 1;
	datos[i].fecha = (REGISTRO*)malloc(N*sizeof(char));
	if (datos[i].fecha == NULL)
	{
		printf("No se ha podido reservar memoria. \n");
		exit(1);
	}
	strcpy(datos[i].fecha, cadena);     //función para copiar strings (char destino, char fuente)
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