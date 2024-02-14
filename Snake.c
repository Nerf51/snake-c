#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define ANCHO 20
#define LARGO 20
#define TIMEOUT 250

int endGame = 1;

void generarComida(int * vector, int * longitud, int comida[]){
	srand(time(NULL));
    int numeroAleatorio1, numeroAleatorio2;
    do {
    	numeroAleatorio1 = rand() % (ANCHO - 2) + 1;
    	numeroAleatorio2 = rand() % (LARGO - 2) + 1;
	} while (existePar(vector, numeroAleatorio1, numeroAleatorio2, longitud));
	comida[0] = numeroAleatorio1;
	comida[1] = numeroAleatorio2;
}

void aumentarTamano(int * vector, int * longitud){
	*longitud += 2;
	vector = (int *)realloc(vector, (*longitud)*sizeof(int));
	if(vector == NULL){
		printf("No se pudo reservar la memoria para el vector");
		endGame = 0;
	}
}

void keyboardListener(int * direccion){ // RECORDAR QUITAR LONGITUD Y VECTOR TRAS TEST
	/*
	
	w = 119
	a = 97
	s = 115
	d = 100
	
	*/
	clock_t tstart = clock();
    int v1 = 'y';                   // default key press
    while((clock() - tstart) < TIMEOUT) {
        if(kbhit()) {
        	int code = -(*direccion);
            v1 = getch();
            if(v1 == 119){
        		code = 2;
			} else if(v1 == 97){
				code = -1;
			} else if(v1 == 115){
				code = -2;
			} else if(v1 == 100) {
				code = 1;
			}
			if(code + *direccion != 0){
				*direccion = code;
			}
        }
    }
}

void actualizarPosicion(int * vector, int * longitud, int * direccion){
	int i;
	switch(*direccion){
		case 1: {
			for(i = 0; i <= *longitud - 2; i+=2){
				if(i == *longitud - 2){
					vector[0] += 1;
				} else {
					vector[*longitud - i - 2] = vector[*longitud - i - 4];
					vector[*longitud - i - 1] = vector[*longitud - i - 3];
				}
			}
			break;
		}
		case -1: {
			for(i = 0; i <= *longitud - 2; i+=2){
				if(i == *longitud - 2){
					vector[0] -= 1;
				} else {
					vector[*longitud - i - 2] = vector[*longitud - i - 4];
					vector[*longitud - i - 1] = vector[*longitud - i - 3];
				}
			}
			break;
		}
		case 2: {
			for(i = 0; i <= *longitud - 2; i+=2){
				if(i == *longitud - 2){
					vector[1] -= 1;
				} else {
					vector[*longitud - i - 2] = vector[*longitud - i - 4];
					vector[*longitud - i - 1] = vector[*longitud - i - 3];
				}
			}
			break;
		}
		case -2: {
			for(i = 0; i <= *longitud - 2; i+=2){
				if(i == *longitud - 2){
					vector[1] += 1;
				} else {
					vector[*longitud - i - 2] = vector[*longitud - i - 4];
					vector[*longitud - i - 1] = vector[*longitud - i - 3];
				}
			}
			break;
		}
	}
}

int existePar(int * vector, int x, int y, int * longitud){
	int existe = 0, i;
	for(i = 0; i < *longitud && existe == 0; i+=2){
		if(vector[i] == x && vector[i+1] == y){
			existe = 1;
		}
	}
	return existe;
}

void dibujarZona(int anchura, int altura, int * vector, int * longitud, int * comida, int * puntuacion){
	int i, j, dos = 2;
	if(vector[0] == 0 || vector[0] == ANCHO - 1 || vector[1] == 0 || vector[1] == LARGO - 1){
		system("cls");
		printf("HAS PERDIDO EL JUEGO");
		endGame = 0;
	} else {
		for(i = 0; i < altura; i++){
			for(j = 0; j < anchura; j++){
				if(j == 0 || j == anchura - 1){
					printf("* ");
				} else {
					if(i == 0 || i == altura - 1){
						printf("* ");
					} else {
						if(existePar(vector, j, i, longitud)){
							printf("+ ");
						} else {
							if(existePar(comida, j, i, &dos)){
								printf("w ");
							} else {
								printf("  ");
							}
						}
					}
				}
			}
			printf("\n");
		}	
	}
	printf("\n");
	printf("Puntuacion: %i", *puntuacion);
	if(vector[0] == comida[0] && vector[1] == comida[1]){
		*puntuacion += 1;
		generarComida(vector, longitud, comida);
		aumentarTamano(vector, longitud);
	}
}

int main(){
	int * vector, longitud = 4, direccion = 1, puntuacion = 0;
	int comida[2];
	vector = (int *)malloc(longitud*sizeof(int));
	if(vector == NULL){
		printf("No se pudo reservar la memoria para el vector");
		endGame = 0;
	}
	vector[0] = (int)(ANCHO / 2);
	vector[1] = (int)(LARGO / 2);
	vector[2] = vector[0] - 1;
	vector[3] = vector[1];
	dibujarZona(ANCHO, LARGO, vector, &longitud, comida, &puntuacion);
	generarComida(vector, &longitud, comida);
	while(endGame){
		keyboardListener(&direccion);
		system("cls");
		actualizarPosicion(vector, &longitud, &direccion);
		dibujarZona(ANCHO, LARGO, vector, &longitud, comida, &puntuacion);
	}
	free(vector);
	return 0;
}
