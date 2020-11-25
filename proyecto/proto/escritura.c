//Dirección del proyecto cd Documents/informatica2020/proyecto/proto


#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

typedef struct  
{
	float *temp;
	float *hum;
	char *date[9]
}REGISTRO;

REGISTRO *datos;


typedef struct  
{
	char *nombre;
	int edad;
}trabajadores;

trabajadores *trab;

/*///////////////////////////////////////
float prom (registro datos, int index);
float dev (registro datos , int index);
float var (registro datos , int index);
float moda (registro datos , int index);
*/


void copiar (char cadena[], int i);
void vaciar (char cadena[]);


int main(void){

	char temp[292], auxiliar;
	int cont = 0;
	FILE *flujo;
	flujo = fopen("trabajadores.txt", "r");
	if (flujo == NULL){
		printf("No se ha podido abrir el fichero");
		return 1;
	}
	
	flujo = fopen("trabajadores.txt", "r");
	while (!feof(flujo))
	{
		fgets(temp, 292, flujo);
		cont++;
	}
	rewind(flujo);             //coloca el cursor del archivo que se encuentra en una posición x, al inicio del archivo

	datos = (REGISTRO*)malloc(cont*sizeof(REGISTRO));
	trab = (trabajadores*)malloc(cont*sizeof(trabajadores));

	for (int i = 0; !feof(flujo); i++)
	{
		vaciar(temp);
		auxiliar = '0';
		for (int j = 0; auxiliar != '/'; j++)
		{
			auxiliar = fgetc(flujo);
			if (auxiliar != '/')
			{
				temp[j] = auxiliar;
			}			
		}
		copiar(temp,i);		

		fgets(temp, 292, flujo);
		trab[i].edad = atoi(temp);

		printf("\nEl trabajador %s tiene %d anios.", trab[i].nombre, trab[i].edad);
	}
	fclose(flujo);

	system("pause");
	return 0;
}

void copiar (char cadena[], int i){
	int N = strlen(cadena) + 1;
	datos[i].temp = (REGISTRO*)malloc(N*sizeof(char));
	trab[i].nombre = (trabajadores*)malloc(N*sizeof(char));
	if (trab[i].nombre == NULL)
	{
		printf("No se ha podido reservar memoria. \n");
		exit(1);
	}
	strcpy(trab[i].nombre, cadena);     //función para copiar strings (char destino, char fuente)
	return;	
}

void vaciar (char cadena[]){	
	for (int i = 0; i < 292; i++)
	{
		cadena[i] = '\0';
	}
	return;	
}

/*************** Funciones Estadisticas******/