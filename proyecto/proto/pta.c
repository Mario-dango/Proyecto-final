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
int escritura(FILE *flujo, int flag);
void copiar (char cadena[], int i);
void vaciar (char cadena[], int tam);
void cargar (char cadena[], int index);
void tomarfecha (char *cadena[]);
void valores_LCD();

void main(){

	int cont;
	FILE *p_flujo1, *p_flujo2;
	
	datos = (REGISTRO*)malloc(cont*sizeof(REGISTRO));
	int flag = lectura(p_flujo1);
	flag = escritura(p_flujo2, flag);
/*
	while (flag == 1){							//Condición ciclica del programa
		lectura del dht()
		datos[].temp = 
		for (int j = 0; j < 28; j++){			//for para guardar datos en la matriz

	/************************** Evaluo el estado de mi Temperatura*/
/*
		valores_LCD(&flag, j);
			
		}
	}*/
		printf("\nRegreso al main");
}

//strtof() transforma cadenas en float
/*************** Funciones de fichero******/

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
	return 0;
}

int escritura(FILE *flujo, int flag){

	char temp[linea], auxiliar;
	int indice;
	flujo = fopen("archivo.txt", "a");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		return 1;
	}
	if (flag == 0)
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
		return flag = 1;
	} else
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

	}
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