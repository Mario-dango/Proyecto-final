//Dirección del proyecto cd Documents/informatica2020/proyecto/proto

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define linea 53

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
void vaciar (char cadena[], int tam);
void cargar (char cadena[], int index);
void valores_LCD();

void main(){

	int cont;
	FILE *p_flujo;
	
	datos = (REGISTRO*)malloc(cont*sizeof(REGISTRO));
	lectura(p_flujo);


	short flag = 1;
/*
	while (flag == 1){							//Condición ciclica del programa
		for (int j = 0; j < 28; j++){			//for para guardar datos en la matriz

	/************************** Evaluo el estado de mi Temperatura*/
/*
		valores_LCD(&flag, j);
			
		}
	}*/
		printf("\nRegreso al main");

	for (int i = 0; i < 8; i++)
	{
		printf("\nCiclo: %d  Temp: %.2f*c  Humd: %.2fr  Fecha: %s", datos[i].ciclo, datos[i].temp, datos[i].hum, datos[i].fecha);
	}
	
	system("pause");
}

//strtof() transforma cadenas en float
/*************** Funciones de fichero******/

int lectura(FILE *flujo){

	char temp[linea], auxiliar;
	int cont = 0, i = 0, indice = -12;
	flujo = fopen("archivo.txt", "r");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		return 1;
	}

	vaciar(temp, linea);
	while (auxiliar != ']')
	{
		while (auxiliar != '\n')
		{
			temp[i] = auxiliar;
			printf("%c", auxiliar);
			auxiliar = fgetc(flujo);
			i ++;
		}
		indice ++;
		cargar(temp, indice);
		printf("\n");
		i = 0;
		auxiliar = fgetc(flujo);
	}	
		printf("fin de lectura");
	fclose(flujo);
	return;
}


void cargar (char cadena[], int index){	
	int cont = 0;
	char auxiliar[20];
	if ((index >=0) && (cadena[linea-1] != ']'))
	{
		for (int i = 0; i < linea; i++)
		{	
			if ((cadena[i] == '.') && (cont == 0)){
				datos[index].ciclo = index;
				cont ++;			
			} else if ((cadena[i] == '.') && (cont == 1)){
				auxiliar[0] = cadena[i-2]; 
				auxiliar[1] = cadena[i-1]; 
				auxiliar[2] = cadena[i]; 
				auxiliar[3] = cadena[i+1]; 
				datos[index].temp = strtod(auxiliar, NULL);			//convierto y almaceno
				vaciar(auxiliar, 4);
				cont ++;
			} else if ((cadena[i] == '.') && (cont == 2)){
				auxiliar[0] = cadena[i-2]; 
				auxiliar[1] = cadena[i-1]; 
				auxiliar[2] = cadena[i]; 
				auxiliar[3] = cadena[i+1]; 
				datos[index].hum = strtod(auxiliar, NULL);		//convierto y almaceno
				vaciar(auxiliar, 4);
				cont ++;
			} else if ((cadena[i] == ':') && (cont == 3)){
				for (int j = 0; j < 19; j++)
				{
					auxiliar[j] = cadena[(linea - 19) + j];			// (linea - 19)   =   33
				}
				copiar(auxiliar, index);
				vaciar(auxiliar, 19);
				cont = 0;
			}
		}
	}
}

void copiar (char cadena[], int i){
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

void vaciar (char cadena[], int tam){
	for (int i = 0; i < tam; i++)
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