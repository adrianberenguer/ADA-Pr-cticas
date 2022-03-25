
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>    //Función max()
#include <stdlib.h>     //Funcion new y delete
using namespace std;

const int CENTINELA = -1;

bool gestionArgumentos(int argc, char *argv[], string &inputFile, bool &recSinAlmacen, bool &table){

     /* Debemos recorrer el array de argumentos
       para comprobar que condiciones ha de seguir
       la ejecución del programa, ya  que los
       argumentos no han de seguir un orden determinado.      
    */ 
    for(int i = 1; i < argc; i++){
        //EXCLUSION RECURSION SIN ALMACEN
        if((string)argv[i] == "--ignore_naive")
            recSinAlmacen = false;
        //MOSTRAR MATRIZ V.ITERATIVA
        else if((string)argv[i] == "-t")
            table = true;
        
        else if ((string)argv[i] == "-f"){
            //Comprobamos que no sea el último argumento
            if(i != argc - 1){
                    inputFile = argv[i+1];
                    i++;                   
            }
            else{
                cout << "ERROR: file name needed." << endl;
                return false;
            }   
        }
           
        else{
            cout << "ERROR: unknown option " << argv[i] << "." << endl;
            return false;
        }
    }

    return true;
}

bool lecturaFichero(string inputFile, int &n, int &T, vector<int> &productos){
    //Creamos un archivo de lectura
    ifstream file;
    file.open(inputFile.c_str(), ios::in);

    if(file.is_open()){
        //  FORMATO DEL FICHERO
        //  n T

        string linea = "";
        int number = -1;

        //Recorremos el fichero linea a linea
        while(getline(file, linea)){
            //Buffeamos la linea para facilitar el volcado
            stringstream ss(linea);

            //1ªLINEA: Valores de n y T
            if(n == -1 && T == -1){
                ss >> n;
                ss >> T;
            }

            //2ªLINEA: Vector de productos
            else{
                while(ss >> number){
                    productos.push_back(number);
                }
            }
        }

        file.close();
        return true;
    }
    else{
        cout << "ERROR: can't open file: " << inputFile << endl;
        cout << "Usage:" << endl << "$maxsum [-t] [--ignore_naive] -f file" << endl;
        return false;
    }
}

int recursivoNaive(int n, int T, vector<int> productos){
    //Pequeno()
    if(n == 0)
        return 0; //Trivial()

    //Guardamos el valor del producto
    int value = productos[n-1];

    //Solución 1: No coger el objeto
    int s1 = recursivoNaive(n-1 , T, productos);

    int s2 = 0;
    //Solución 2: Coger el objeto
    if(value <= T)
        s2 = value + recursivoNaive(n-1, T-value, productos);

    //Hallamos la slución más óptima
    return max(s1, s2);
}

int recursivoMemoization(int **matriz, int n, int T, vector<int> productos){
    if(*(*(matriz + n) + T) != CENTINELA)
        return *(*(matriz + n) + T);
    if(n == 0)
        return *(*(matriz + n) + T) = 0;
    
    //Guardamos el valor del producto
    int value = productos[n-1];

    int s1 = recursivoMemoization(matriz, n-1, T, productos);
    int s2 = 0;
    if(value <= T)
        s2 = value + recursivoMemoization(matriz, n-1, T-value, productos);

    return *(*(matriz + n) + T) = max(s1, s2); 
}


int iterativoTable(int **matriz, int last_n, int last_T, vector<int> productos){

    for(int n = 1; n <= last_n; n++){
        for(int T = 1; T <= last_T; T++){
            int s1 = matriz[n-1][T];

            int s2 = 0;
            if(productos[n-1] <= T)
                s2 = productos[n-1] + matriz[n-1][T-productos[n-1]];
            matriz[n][T] = max(s1, s2);
        }
    }
    return matriz[last_n][last_T];
}    


void traza(int **matriz, int n, int T, vector<double> &v, vector<double> &m){
    if(n == 0) return;

    int s1 = matriz[n-1][T];
    int s2 = 0;
    if(productos[n-1] <= T)
        s2 = productos[n-1] + matriz[n-1][T-productos[n-1]];
    if(s1 >= s2){
        catched[n-1] = false;
        traza(matriz, n-1, T, productos, catched);
    }else{
        catched[n-1] = true;
        traza(matriz, n-1, T-productos[n-1], productos, catched);
    }
}


int iterativoVector(int last_n, int last_T, vector<int> &productos){
    /*  Como se puede obtener una linea respecto
        a la anterior no es necesario emplear una
        matriz como almacén, con 2 vector de 
        complejidad espacial lineal es suficiente
    */
    vector<int> line0(last_T+1);
    vector<int> line1(last_T+1);

    //Como en la matriz, la primera linea es todo 0
    for(int T = 0; T <= last_T; T++) line0[T] = 0;

    for(int n = 1; n <= last_n; n++){
        for(int T = 1; T <= last_T; T++ ){
            int s1 = line0[T];
            int s2 = 0;
            if(productos[n-1] <= T)
                s2 = productos[n-1] + line0[T - productos[n-1]];
            line1[T] = max(s1, s2);
        }

    swap(line0, line1);
    }

    return line0[last_T];
}


void mostrarResultados(int n, int T, bool recSinAlmacen, bool table, int primero, int segundo, int tercero, int **matriz, const vector<bool> catched, const vector<int> productos, int cuarto){

    if(recSinAlmacen)
        cout << "Naive: " << primero << endl;

    cout << "Memoization: " << segundo << endl
         << "Iterative (table): " << tercero << endl
         << "Iterative (vector): "<< cuarto << endl
         << "Selection: ";

    int sumatory = 0;

    //Recorremos el vector de productos...
    for(unsigned int i = 0; i < productos.size(); i++){
        //... en busca de los escogidos
        if(catched[i] == true){
            cout << productos[i] << " ";
            sumatory += productos[i];
        }
       
    }
    cout << endl;

    cout << "Selection value: " << sumatory << endl;

    if(table){
        cout << "Iterative table:" << endl;
        for(int i = 0; i <= n; i++){
            for(int j = 0; j <= T; j++){
                cout << matriz[i][j];
                if(j!=T)
                    cout << " ";
            }
            cout << endl;
        }
    }
        
}

int main(int argc, char *argv[]){
    string inputFile = "";
    bool recSinAlmacen = true;
    bool table = false;

    if(!gestionArgumentos(argc, argv, inputFile, recSinAlmacen, table)){
        cout << "Usage:" << endl << "$maxsum [-t] [--ignore_naive] -f file" << endl;
    }

    else{
        //OBTENER CAPACIDAD MÁXIMA
        int n = -1;
        int T = -1;

        //OBTENER EL VECTOR DE OBJETOS A PARTIR DEL FICHERO
        vector<int> productos;
        //Si no se encuentra el fichero de entrada abortamos programa
        if(!lecturaFichero(inputFile, n, T, productos)){
            return 0;
        }

        //VECTOR PARA EL SELECTION
        vector<int> selection;

        /*  COMPROBACIÓN DEL VOLCADO
        cout << n << endl;
        cout << T << endl;
        for(unsigned int i = 0; i < productos.size(); i++){
            cout << productos[i] << " ";
        }
        */

        int primero = -1;
        if(recSinAlmacen)
            primero = recursivoNaive(n, T, productos);

        /*  El almacén consistirá en una matriz indexada por:
            - n, número de objetos que queden por analizar
            - T, cantidad limite de dinero acumulado
            Además, añadiremos una fila y columna extras para los
            casos base (0 0), (1 0), (0 1)...
        */
        int **matriz;
        matriz = new int*[n+1];         //Nº de filas
    
        for(int i = 0; i<=n; i++)
            matriz[i] = new int[T+1];   //Nº de columnas por fila
    
        /*  La rellenamos de la siguiente forma:
            -1 -1 -1 -1 -1 
            -1 -1 -1 -1 -1
            -1 -1 -1 -1 -1
        */

        for(int i = 0; i<=n; i++)
            for(int j = 0; j<=T; j++)
                *(*(matriz + i) + j) = CENTINELA;   //M[i][j] = -1; 

        int segundo = recursivoMemoization(matriz, n, T, productos);      

        // METODOS DE LA PRÁCTICA 6.2
        /*  Para este método reutilizaremos la matriz
            de dimensiones [n+1][T+1], pero para ello
            debemos volver a inicializar el borde izquierdo
            y superior a 0, ya que el resto se sobreescribirán
            puesto que la implementación iterativa rellena 
            la tabla por completo  
        */
        for(int i = 0; i <= n; i++) matriz[i][0] = 0;
        for(int j = 0; j <= T; j++) matriz[0][j] = 0;

        int tercero = iterativoTable(matriz, n, T, productos);
        
        /*  Para extraer la selección de objetos del algoritmo
            declaramos un vector<bool> que nos indicara cuales 
            se han escogido.
        */
        vector<bool> catched(n);

        traza(matriz, n, T, productos, catched);

        int cuarto = iterativoVector(n, T, productos);

        mostrarResultados(n, T, recSinAlmacen, table, primero, segundo, tercero, matriz, catched, productos, cuarto);

        //Liberar memoria dinámica
        for(int i = 0; i <= n; i++)
            delete[] matriz[i];     //Columnas
        delete[] matriz;            //Filas              
    }
    return 0;
}