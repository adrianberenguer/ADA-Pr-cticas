//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>
#include <limits>

using namespace std;

const int CENTINELA = -1;

// Leer el fichero de entrada y guardar sus valores
bool leerFichero(string inputFile, int &n, int &m, vector<double> &capacidades, vector<double> &distancias) {
    //n m
    //ci (capacidad de almacenamiento)
    //di (distancia al nodo origen)
    vector<double> temporal;

    ifstream file;
    file.open(inputFile.c_str(), ios::in);

    if(file.is_open())
    {
        string linea = "";
        double number = -1;

        while(getline(file, linea)) {
            stringstream ss(linea);

            if(n == -1 && m == -1) {
                ss >> n;
                ss >> m;
            }
            else {
                while(ss >> number) {
                    temporal.push_back(number);
                }

                if(capacidades.size() == 0) {
                    capacidades = temporal;
                    temporal.clear();
                }
                else if(distancias.size() == 0) {
                    distancias = temporal;
                    temporal.clear();
                }
            }
        }

        file.close();
        return true;
    }
    else {
        cerr << "ERROR: can't open file: " << inputFile << endl;
        cout << "Usage:" << endl << "$met [-t] [--ignore-naive] -f file" << endl;
        return false;
    }
}

// Comprobar los argumentos
bool compruebaArgs(int argc, char *argv[], string &inputFile, bool &ignoraSinAlmIng, bool &tablas) {
    //met [ -t] [ --ignore-naive] -f fichero entrada
    if(argc == 1) {
        return false;
    }

    for(int i = 1; i < argc; i++)
    {
        if((string)argv[i] == "--ignore-naive") {
            ignoraSinAlmIng = false;
        }
        else if((string)argv[i] == "-t") {
            tablas = true;
        }
        else if ((string)argv[i] == "-f") {
            if(i != argc - 1) {
                inputFile = argv[i+1];
                i++;                   
            }
            else {
                cerr << "ERROR: missing filename." << endl;
                return false;
            }   
        }
        else {
            cerr << "ERROR: unknown option " << argv[i] << "." << endl;
            return false;
        }
    }

    return true;
}

vector<int> puertas;

double ogw(int k, int n, vector<double> &capacidades, vector<double> &distancias) {
    double result = 0.0;//numeric_limits<int>::max();
    double resultGood = numeric_limits<int>::max();
    int puerta;

    for (int i = k; i < n; i++)
    {
        result = 0.0;
        for(int j = k; j < n; j++) 
        {
            if(j != i) {
                result += capacidades[j] * abs(distancias[j] - distancias[i]);
            } 
        }
        if(result < resultGood) { 
            resultGood = result;
            puerta = i;
        }
        
    }
    puertas.push_back(puerta);
    return resultGood;    
}

// Recursivo sin almacén(versión ingenua)
double met_naive(int m, int n, vector<double> &capacidades, vector<double> &distancias) {
    double result = numeric_limits<int>::max();
    
    if(m == n) {
        result = 0.0;
    }
    else if(m == 1) {
        result =  ogw(0, n, capacidades, distancias);
    }
    else {
        for(int k = m-1; k <= n-1; k++) {
            result = min(result, ogw(k, n, capacidades, distancias) + met_naive(m-1, k, capacidades, distancias));
        }
    }

    return result;
}

// Recursivo con almacén
double met_memo(int m, int n, vector<double> &capacidades, vector<double> &distancias, vector<vector<double>> &matriz) {
    double result = numeric_limits<int>::max();

    if(matriz[m][n] != CENTINELA) {
        return matriz[m][n];
    }
    if(m == 0) {
        return matriz[0][0] = 0.0;
    }
    else {
        for(int k = m-1; k <= n-1; k++) {
            result = min(result, ogw(k, n, capacidades, distancias) + met_memo(m-1, k, capacidades, distancias, matriz));
        }
    }

    return matriz[m][n] = result;
}

// Iterativo con almacén que hace uso de una tabla para 
// almacenar resultados intermedios
double met_it_matrix(int m, int n, vector<double> &capacidades, vector<double> &distancias) {

    return 0;
}

// Iterativo con almacén con complejidad espacial mejorada
double met_it_vector(int m, int n, vector<double> &capacidades, vector<double> &distancias) {

    return 0;
}

// Tabla de encaminamiento
void met_parser() {

}

// Main
int main(int argc, char *argv[]) {
    string inputFile = "";
    bool ignoraSinAlmIng = true, tablas = false;

    if(!compruebaArgs(argc, argv, inputFile, ignoraSinAlmIng, tablas))
    {
        cout << "Usage:" << endl << "met [-t] [--ignore-naive] -f file" << endl;
    }
    else
    {
        int n = -1, m = -1;
        vector<double> capacidades, distancias;

        if(!leerFichero(inputFile, n, m, capacidades, distancias)) {
            return 0;
        }

        if(ignoraSinAlmIng) {
            double result = met_naive(m, n, capacidades, distancias);
            cout <<result <<" ";
        }
        else {
            cout <<"- ";
        }

        vector<vector<double>> matrizMemo(m+1, vector<double>(n+1, -1));

        double result2 = met_memo(m, n, capacidades, distancias, matrizMemo);
        cout <<result2 <<" ";

        cout << "? ?"<<endl;
        cout << "?" <<endl;
        cout <<"?" <<endl;

        if(tablas) {
            cout <<"Memoizaton matrix: "<<endl;
            /*for(int i=0; i<m; i++) {
                for(int j=0; j<n; j++) {
                    if(matrizMemo[i][j] == -1) {
                        cout <<"- "
                    }
                    else {
                        cout <<matrizMemo[i][j]<<" ";
                    }
                }
                cout <<endl;
            }*/

            cout <<"Iterative matrix: "<<endl;
            cout <<"?"<<endl;
        }

    }
    return 0;
}

/*for(int i=0; i<capacidades.size(); i++) {
        cout <<capacidades[i]<<" ";
    }
    cout <<endl;
    
    0,2,3,6
    */
       /*for (int i = k; i < n-1; i++)
    {
        result = 0.0;
        for(int j = i+1; j < n; j++) 
        {
            //cout <<"entra"<<endl;
            result += capacidades[j] * abs(distancias[j] - distancias[i]);  
        }   
        cout <<result <<endl;
        if(result < resultGood) { 
            resultGood = result;
            puerta = i;
        }
        
    }*/