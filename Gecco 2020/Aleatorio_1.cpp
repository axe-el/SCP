#include <bits/stdc++.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

double timeval_diff(struct timeval *a, struct timeval *b);
void Read(char *nombre,vector < vector<int> > &filas,vector < vector<int> > &columnas);
void Revolver(vector<int> &vecinos);
void guardar(char *nombre,vector<int> &solucion1, int costo, double secs);		
void iniciacion_random1(vector<int> &s1,vector<int> &cobertura_s1,vector < vector<int> > &filas,vector < vector<int> > &columnas);
int Costo_1(vector<int> s1);
int Factible(vector<int> cobertura_s1);
void Cobertura(vector<int> &s1,vector<int> &cobertura_s,vector < vector<int> > &columnas);	
void Indices(vector<int> &s1,vector<int> &indices);
				
int main(int argc, char *argv[]){
	//Se requieren 4 argumentos
	// ./ejecutable instancia semilla_aleat archivo_salida
	// ./ejecutable prueba.txt 5 prueba.txt
	if(argc==4){
	
		//Variables tiempo(segundos),iniciar tiempo 
		struct timeval t_ini, t_fin;
		double secs;
		gettimeofday(&t_ini, NULL);
			
    		//Semilla para numeros aleatorios 	
		srand(atoi(argv[2]));
		
		//Matriz (Listas de adyacencia)
		vector < vector<int> > filas;
		vector < vector<int> > columnas;				
		Read( argv[1],filas,columnas);

		//Costo solucion
		int costo_s;
		//Vector solucion
		vector<int> s1(columnas.size(),0);
		//Vector cobertura filas
		vector<int> cobertura_s1(filas.size(),0);
		//Solucion random y calculamos su costo
		iniciacion_random1(s1,cobertura_s1,filas,columnas);		
		costo_s = Costo_1(s1);	
			
		//Tiempo transcurrido		
		gettimeofday(&t_fin, NULL);
		secs = timeval_diff(&t_fin, &t_ini);

  		//Guardar Solucion,Costo,Tiempo 
  		guardar(argv[3],s1,costo_s,secs); 
  				
	}
	else{
		cout<<"Error ejecutar parametros invalidos\n";
	}	
				
}

//indices vector vacio, regresamos las posiciones de los 1's del vector solucion
void Indices(vector<int> &s1,vector<int> &indices){
	for(int i = 0 ; i< s1.size() ; i++) {
		if(s1[i]==1){
			indices.push_back(i);
		}
	}	
}

//A partir de una solucion calculamos el vector de cobertura de esta
void Cobertura(vector<int> &s1,vector<int> &cobertura_s,vector < vector<int> > &columnas){
	//Para todas las entradas del vector solucion
	for(int i = 0 ; i< s1.size() ; i++) {
		//Si esa columna pertenece a la solucion
		if(s1[i]==1){
			//+1 al vector cobertura, para todas filas cubiertas por esta columna
			for(int j = 0 ; j< columnas[i].size() ; j++) {	
				cobertura_s [ columnas[i][j] ] = cobertura_s [ columnas[i][j] ] + 1; 
			}				
		}
	 }
}

//Nuestra solucion es factible o infactible, si factible regresa -1, si infactible
//regresa el indice de la fila que no esta cubierta(regresara el primer indice encontrado, se debe meter a un loop)
int Factible(vector<int> cobertura_s1){
	//Para todas las entradas del vector cobertura
	for(int i = 0 ; i< cobertura_s1.size() ; i++) {
		//Si uno entrada es 0
		if(cobertura_s1[i]==0){
			return i;
		}
	}
	return -1;
}

//Costo =? #numero de 1's en nuestro vector sulucion
int Costo_1(vector<int> s1){
	int c_1=0;
	for(int i = 0 ; i< s1.size() ; i++) {
		if(s1[i]==1){
			c_1 = c_1 + 1;
		}
	}
	return c_1;
}

//Hacemos swaps entre entradas del vector para aleatorizar el orden de sus elementos
void Revolver(vector<int> &vecinos){

	int j;
	//Intercambiar entradas del vector	
	for (int i = 0; i < vecinos.size(); i++){
		j= i +rand() %(vecinos.size()-i);
		swap(vecinos[i], vecinos[j]);

	}
}

//Solucion aleatoria vamos añadiendo columnas
void iniciacion_random1(vector<int> &s1,vector<int> &cobertura_s1,vector < vector<int> > &filas,vector < vector<int> > &columnas){

	//Orden en el que visitar el #filas
	vector<int> orden(cobertura_s1.size(),0);
	for(int i = 0 ; i< orden.size() ; i++) {	
		orden[i] = i;
	}
	//Aleatorizamos el orden
	Revolver(orden);

	
	// culumna = columna correspondiente, num = aleatorio entre (0,#columnas)
	int fila,columna,num;
	//Visitamos todas las filas
	for(int i = 0 ; i< orden.size() ; i++) {
		fila = orden[i];
		
		//Si esa fila no esta cubierta
		if( cobertura_s1[ fila ] == 0){
			//Obtenemos una columna aleatoria de las que cubren esa fila
		 	num=rand()%( filas[ fila ].size()  );					 	
			columna = filas[ fila ][num];
			//Asignamos 1 a dicha entrada del vector solucion(Esa columna pertenece a la solucion)
			s1[columna] = 1;
			//Sumamos +1 al vector cobertura de la solucion , para todas filas cubiertas por columna recien asignada
			for(int j = 0 ; j< columnas[columna].size() ; j++) {	

				cobertura_s1[ columnas[columna][j] ] = cobertura_s1[ columnas[columna][j] ] + 1; 
			}
					
		}
	
	}
	
}

//Leer intancia
void Read(char *nombre,vector < vector<int> > &filas,vector < vector<int> > &columnas){
  	FILE *fichero;
  	//Si el nombre es incorrecto, imprimir error y salir
	if((fichero = fopen(nombre,"rt")) == NULL){  
		printf("\n INVALID FILE");
		exit(1);
	}
	//Variable int para leer el archivo
	int dato;
	//Saltamos dos primeros datos
	//Numero filas y columnas	
	for(int i=0;i<2;i++){
		fscanf(fichero,"%d",&dato);
	}
	//Numero de columnas que cubren fila actual
	int contador=0;
	//Fila actual
	int I;	
	while(fscanf(fichero,"%d",&dato) == 1){
		//dato = numero de fila
		if(contador ==0){
			//Metemos renglones vacios a la matriz Filas
			while(dato>=filas.size()){
	 			vector<int> Renglon;	
	 			filas.push_back(Renglon);	
	 		}
	 		//I = fila actual
	 		I = dato;
			//Hacemos contador = -1
	 		contador =-1;
		}
		//dato = Numero de columnas que cubren fila actual
		else if(contador == -1){
			// contador = Numero de columnas que cubren fila actual
			contador = dato;
		}
		//dato = columna que cubre a la actual fila
		else if(contador>0){
			//Metemos esa columna a lista adyacencia Fila[I] = { d1,d2,...,d_contador}
			filas[I].push_back(dato);
			//Metemos renglones vacios a la matriz columnas
			while(dato>=columnas.size()){
	 			vector<int> Renglon;	
	 			columnas.push_back(Renglon);	
	 		}
	 		//Metemos esa fila a lista adyacencia Columnas[d1] = { I1,I2,...,I*}
	 		columnas[dato].push_back(I);
	 		//Reducimos contador
			contador = contador-1;
		}
	}
	//Cerramos archivo
	fclose(fichero);
}


//Funcion para ver cuanto tiempo paso en segundos
double timeval_diff(struct timeval *a, struct timeval *b){
	return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}



//Guardar Solucion con costo,tiempo asociados
void guardar(char *nombre,vector<int> &solucion1, int costo, double secs){

	//Obtenemos indices de columnas pertenecientes a la solucion
	vector<int> indices;
	Indices(solucion1,indices);

	//Costo
	//Tiempo
	ofstream file;			
	file.open(nombre);
	file <<costo<<"\n";
	file <<secs<<"\n";
	
	//Solucion(columnas que si pertenecen)	
	for(int i = 0; i < indices.size(); i++) {
		file << indices[i] <<"\n";		
	}	
	file.close();
}








			
				

