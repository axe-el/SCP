#include <bits/stdc++.h>
#include <time.h>
#include <sys/time.h>
#include <map>
#include <queue>
#include <vector>

using namespace std;


class fit{ 	
	public:  
		double largo;
		double especial;						
};

double timeval_diff(struct timeval *a, struct timeval *b);
void Inicializar_poblacion(vector < vector<int> > &soluciones,vector < vector<int> > &filas,vector < vector<int> > &columnas);
void Constructivo_1(vector<int> &s1,vector < vector<int> > &filas,vector < vector<int> > &columnas);
void Read(char *nombre,vector < vector<int> > &filas,vector < vector<int> > &columnas);
void Revolver(vector<int> &vecinos);
int Intersecta_con( int columna, vector < vector<int> > & columnas ,vector<int> & cobertura_s1 );
int Cuantos_faltan(vector<int> &cobertura_s1);
int Evolutivo( vector < vector<int> > &soluciones,vector<int> &best_solucion ,vector < vector<int> > &filas,vector < vector<int> > &columnas);
int Costo_1(vector<int> s1);
void muta1(vector<int> &hijo);
void cruza1(vector<int> &padre1,vector<int> &padre2,vector<int> &hijo1,int costo_1,int costo_2);
void torneo (int &i1, int &i2,vector<int> &padre1,vector<int> &padre2,vector <vector<int>> &soluciones,vector<int> costos);
void Cobertura(vector<int> &s1,vector<int> &cobertura_s,vector < vector<int> > &columnas);
int Factible(vector<int> cobertura_s1);
void Arreglar(vector<int> &hijo1,vector<int> &cobertura1,vector < vector<int> > &columnas,vector < vector<int> > &filas);
int next_generacion( vector < vector<int> > &soluciones,vector<int> &best_solucion,vector < vector<int> > &filas,vector < vector<int> > &columnas,vector<int> &costos);
void guardar(char *nombre,vector<int> &solucion1, int costo);
void Indices(vector<int> &s1,vector<int> &indices);
void Quitar_columnas(vector<int> &s1,vector<int> &cobertura_s1,vector < vector<int> > &columnas,vector < vector<int> > &filas);
		
int main(int argc, char *argv[]){
	//Se requieren 4 argumentos
	// ./ejecutable instancia semilla_aleat archivo_salida
	// ./ejecutable prueba.txt 5 prueba.txt
	if(argc==4){

	
    		//Semilla para numeros aleatorios y numero de canales para resolver la instancia	
		srand(atoi(argv[2]));
		
		//Matriz (Lista de adyacencia), contador para cuantas antenas	
		vector < vector<int> > filas;
		vector < vector<int> > columnas;				
		Read( argv[1],filas,columnas);


		//Vector Solucion (0's), inicializar solucion Aleatorios	
		int poblacion=50;	
		
		
		vector<int> s1(columnas.size(),0);
		//Vector cobertura filas

		
		vector <vector<int>> soluciones(poblacion,vector<int>(columnas.size(),0));		

						
		Inicializar_poblacion( soluciones,filas,columnas);

		vector<int> best_solucion(columnas.size(),0);	
		int e =Evolutivo(soluciones,best_solucion ,filas,columnas);	

		
														
  		//Guardar Solucion,Costo,Tiempo 
  		guardar(argv[3],best_solucion,e); 
 
	}
	else{
		cout<<"Error ejecutar parametros invalidos\n";
	}	
				
}

//Funcion para ver cuanto tiempo paso en segundos
double timeval_diff(struct timeval *a, struct timeval *b){
	return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int Costo_1(vector<int> s1){
	int c_1=0;
	for(int i = 0 ; i< s1.size() ; i++) {
		if(s1[i]==1){
			c_1 = c_1 + 1;
		}
	}
	return c_1;
}

//Bueno
//Muta, cambiar el valor
void muta1(vector<int> &hijo){
	int i = rand()%(hijo.size());
	if(hijo[i]==1){
		hijo[i]=0;
	}
	else{
		hijo[i]=1;
	}
}



//bueno
void cruza1(vector<int> &padre1,vector<int> &padre2,vector<int> &hijo1,vector<int> &hijo2,int costo_1,int costo_2){
	double aleat,prob;
	for(int j=0; j<padre1.size();j++){
		if(padre1[j]==padre2[j]){
			hijo1[j]=padre1[j];
			hijo2[j]=padre2[j];	
		}
		else{
			prob=(double) costo_2 / (costo_1 + costo_2);
			aleat = (0 + rand() % (100 - 0))/100.0;
			if(aleat<=prob){
				hijo1[j]=padre1[j];
				hijo2[j]=padre2[j];
			}
			else{
				hijo2[j]=padre1[j];
				hijo1[j]=padre2[j];
			}
		}
	}	
}

//bueno
//Torneo binario (Guardamos el indice de los padres en la matriz);
void torneo (int &i1, int &i2,vector<int> &padre1,vector<int> &padre2,vector <vector<int>> &soluciones,vector<int> costos){
	int ind1,ind2;
	//Dos veces una para cada padre
	for(int j=0; j<2;j++){
		//Seleccionamos 2 individuos al azar
		ind1 = rand()%(soluciones.size());
		ind2 = rand()%(soluciones.size());

		//Padre 1 = el mejor de los dos individuos
		if(j==0){
			i1=ind2;
			if(costos[ind1] <= costos[ind2]){
				i1=ind1;
			}
			padre1=soluciones[i1];	
		}
		
		// Con otros 2 individuos al azar Padre 2 = mejor de estos
		else if(j==1){
			i2=ind2;
			if(costos[ind1] <= costos[ind2]){
				i2=ind1;
			}
			padre2=soluciones[i2];					
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

void Quitar_columnas(vector<int> &s1,vector<int> &cobertura_s1,vector < vector<int> > &columnas,vector < vector<int> > &filas){

	vector<int> orden;
	Indices(s1,orden);
		
	//Aleatorizamos el orden
	Revolver(orden);

	// culumna = columna correspondiente
	int columna,fila,error=0;
	//Visitamos todas las filas
	for(int i = 0 ; i< orden.size() ; i++) {
		columna = orden[i];
		
		//Si vector solucion == 1 en esa posicio
		if( s1[ columna ] == 1){
			i = orden.size()+1;
			//Volvemos 0 esa posicion(Quitamos columna de la solucion)
			s1[ columna ] = 0;
			//Recorremos todas las filas que cubria esa columna
			for(int j = 0 ; j< columnas[columna].size() ; j++) {	
				fila = columnas[columna][j];
				//Restamos 1 a cada entrada del vector cobertura que cubria esa columna(Vector_cobertura ya calculado)
				cobertura_s1[ fila ] = cobertura_s1[ fila ] -1; 
				//Si esa fila ya no esta cubierta(no podemos quitar esa columna)
				if( cobertura_s1[ fila ] <= 0 ){
					s1[ columna ] = 1;
					for(int k = 0 ; k<= j ; k++) {
						cobertura_s1[ columnas[columna][k] ] = cobertura_s1[ columnas[columna][k] ] +1;	
					}	
					//break;
					//Ya no seguimos restando 1 a cada entrada del vector cobertura que cubria esa columna
					j = columnas[columna].size()+1;	
				}
			}				
		}		
	}
	
}

void Arreglar(vector<int> &hijo1,vector < vector<int> > &columnas,vector < vector<int> > &filas){
	vector<int> cobertura1(filas.size(),0);
	Cobertura(hijo1,cobertura1,columnas);
	int F=Factible(cobertura1),columna,este;
	double intersecciones,max,maximo = 0;

	while(F!=-1){
		for(int j = 0; j < filas[ F ].size(); j++) {
			max = columnas[ filas[ F ][j] ].size();
			intersecciones = Intersecta_con( filas[ F ][j], columnas ,cobertura1 )  ; 
			max = max * intersecciones;
			if(max > maximo){
				maximo = max;
				este = j;
			}			
			maximo = 0;	
			columna = filas[ F ][este] ; 
			hijo1[ columna ] = 1;
			for(int k = 0 ; k< columnas[ columna ].size() ; k++) {	

				cobertura1[ columnas[ columna ][k] ] = cobertura1[ columnas[ columna ][k] ] + 1; 
	
			}

		}			
		F=Factible(cobertura1);
	}
	Quitar_columnas(hijo1,cobertura1,columnas,filas);	
	
}


//bueno
//Seleccion, cruza, muta, remplazo (Siempre hay muta)
int next_generacion( vector < vector<int> > &soluciones,vector<int> &best_solucion,vector < vector<int> > &filas,vector < vector<int> > &columnas,vector<int> &costos){
		
	vector<int> padre1;
	vector<int> padre2;
	vector<int> hijo1(best_solucion.size(),0);
	vector<int> hijo2(best_solucion.size(),0);

	int i1,i2;	
	int c1,c2;

	vector<int> new_costos(soluciones.size(),0);
	vector <vector<int>> new_soluciones(soluciones.size(),vector<int>(soluciones[0].size(),0));

	int ii=0;	
	//Poblacion es numero par				
	for(int i=0 ; i<soluciones.size()/2 ; i++ ){
				
		//Seleccion padres
		torneo(i1,i2,padre1,padre2,soluciones,costos);
		
		//Cruza padres para obtener 2 hijos
		cruza1(padre1,padre2,hijo1,hijo2,costos[i1],costos[i2]);
		
		//Muta de ambos hijos y costo de los hijos
		muta1(hijo1);
		muta1(hijo2);
		Arreglar(hijo1,columnas,filas);
		Arreglar(hijo2,columnas,filas);

		c1 =Costo_1(hijo1);	
		c2 =Costo_1(hijo2);	

		//Vamos creando proxima generacion
		new_soluciones[ii]=hijo1;
		new_costos[ii]=c1;

		ii=ii+1;
		new_soluciones[ii]=hijo2;
		new_costos[ii]=c2;

						
		ii=ii+1;
					
	}
	
	//Encontramos la mejor solucion de la generacion 
	int c_b = costos[0];
	best_solucion = soluciones[0];

	for(int i = 0; i < soluciones.size(); i++) {
		if(costos[i] < c_b){
			c_b = costos[i];
			best_solucion = soluciones[i];

		}
	}

	//SI la mejor solucion de la generacion antigua es mejor que cualquiera de los hijos sobrevive
	for(int i = 0; i < new_soluciones.size(); i++) {	
		if(c_b < new_costos[i]){
			new_costos[i]=c_b;
			new_soluciones[i]=best_solucion;
			break;
		}
	}
	
	//Mejor solucion en nueva poblacion
	for(int i = 0; i < new_soluciones.size(); i++) {
		if(new_costos[i] < c_b){
			c_b = new_costos[i];
			best_solucion = new_soluciones[i];

		}
	}
	
	//Actualizamos poblacion y costos asociados
	costos=new_costos;
	soluciones=new_soluciones;
	//Regresamos costo mejor solucion
	return c_b;
}

//Bueno
int Evolutivo( vector < vector<int> > &soluciones,vector<int> &best_solucion ,vector < vector<int> > &filas,vector < vector<int> > &columnas){
	//Variables tiempo(segundos) 
	struct timeval t_ini, t_fin;
	double secs=0;
	
	//Costos poblacion
	vector<int> costos(soluciones.size(),0);

	//Calculamos costos poblacion
	for(int i = 0; i < soluciones.size(); i++) {
		int c=Costo_1(soluciones[i]);
		costos[i]=c;
	}
	
	long long int generacion=0;
	gettimeofday(&t_ini, NULL);
	int costo_f=0;
			
	while(secs<86400){

		//Proxima generacion
		costo_f=next_generacion(soluciones,best_solucion,filas,columnas,costos);

		//Medir tiempo 	
		gettimeofday(&t_fin, NULL);
		secs = timeval_diff(&t_fin, &t_ini);
		generacion=generacion+1;
	
	}
		
	return costo_f;	
		
}



void Inicializar_poblacion(vector < vector<int> > &soluciones,vector < vector<int> > &filas,vector < vector<int> > &columnas){

	for(int i = 0; i < soluciones.size(); i++) {
		Constructivo_1(soluciones[i],filas,columnas);
	}

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

int Cuantos_faltan(vector<int> &cobertura_s1){
	int c_1=0;
	for(int i = 0 ; i< cobertura_s1.size() ; i++) {
		if(cobertura_s1[i]==0){
			c_1 = c_1 + 1;
		}
	}
	return c_1;
}

int Intersecta_con( int columna, vector < vector<int> > & columnas ,vector<int> & cobertura_s1 ) {
	int cantidad = 0;

	for(int j = 0 ; j< columnas[ columna ].size() ; j++) {	

		if( cobertura_s1[ columnas[ columna ][j] ] ==0 ){
			cantidad = cantidad+1;
		} 
		
	}

	return cantidad;
}

void Constructivo_1(vector<int> &s1,vector < vector<int> > &filas,vector < vector<int> > &columnas){

	vector<int> cobertura_s1(filas.size(),0);
	//Orden en el que visitar el #filas
	vector<int> orden(cobertura_s1.size(),0);
	for(int i = 0 ; i< orden.size() ; i++) {	
		orden[i] = i;
	}
	//Aleatorizamos el orden
	Revolver(orden);


	vector<fit> fitnes_columnas;
	double suma =0;
	for(int i = 0; i < columnas.size(); i++) {	
	
			fit uno;
			uno.largo=columnas[i].size();
			for(int j = 0; j < columnas[i].size(); j++) {	
				suma = suma + filas[columnas[i][j]].size() ;
			}
			uno.especial=suma/columnas[i].size();
			suma = 0;
			fitnes_columnas.push_back(uno);	

	}	

	double limite = cobertura_s1.size() * 0;
	double cobertura_total = cobertura_s1.size();
	double intersecciones,max, maximo =0,min,minimo;
	int este;
	// culumna = columna correspondiente, num = aleatorio entre (0,#columnas)
	int fila,columna,num;
	//Visitamos todas las filas
	for(int i = 0 ; i< orden.size() ; i++) {
		fila = orden[i];

		//Si esa fila no esta cubierta
		if( cobertura_s1[ fila ] == 0){
		
			if(cobertura_total > limite){
				for(int j = 0; j < filas[ fila ].size(); j++) {	
					max = fitnes_columnas [ filas[ fila ][j] ].largo;
					intersecciones = Intersecta_con( filas[ fila ][j], columnas ,cobertura_s1 )  ; 
					max = max * intersecciones;
					if(max > maximo){
						maximo = max;
						este = j;
					}
				}
				maximo = 0;
				columna = filas[ fila ][este] ; 
				s1[ columna ] = 1;

				for(int j = 0 ; j< columnas[ columna ].size() ; j++) {	

					cobertura_s1[ columnas[ columna ][j] ] = cobertura_s1[ columnas[ columna ][j] ] + 1; 
					
				}	
				cobertura_total = Cuantos_faltan(cobertura_s1);
					
			}
			else{

				for(int j = 0; j < filas[ fila ].size(); j++) {	

					min = fitnes_columnas [ filas[ fila ][j] ].especial;
					intersecciones = Intersecta_con( filas[ fila ][j], columnas ,cobertura_s1 ); 
					min = min * intersecciones;
					
					if(j==0){
						minimo = min;
						este = j;	
					}
					if(min < minimo){
						minimo = min;
						este = j;
					}
				}
				columna = filas[ fila ][este] ; 
				s1[ columna ] = 1;

				for(int j = 0 ; j< columnas[ columna ].size() ; j++) {	

					cobertura_s1[ columnas[ columna ][j] ] = cobertura_s1[ columnas[ columna ][j] ] + 1; 
					
				}	
			
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

//indices vector vacio, regresamos las posiciones de los 1's del vector solucion
void Indices(vector<int> &s1,vector<int> &indices){
	for(int i = 0 ; i< s1.size() ; i++) {
		if(s1[i]==1){
			indices.push_back(i);
		}
	}	
}

//Guardar Solucion con costo,tiempo asociados
void guardar(char *nombre,vector<int> &solucion1, int costo){

	//Obtenemos indices de columnas pertenecientes a la solucion
	vector<int> indices;
	Indices(solucion1,indices);

	//Costo
	//Tiempo
	ofstream file;			
	file.open(nombre);
	file <<costo<<"\n";
	//Solucion(columnas que si pertenecen)
		
	for(int i = 0; i < indices.size(); i++) {
		file << indices[i] <<"\n";		
	}	
	file.close();
}

