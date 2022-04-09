//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include <algorithm>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>
#include <limits>

using namespace std;

// Leer el fichero de entrada y guardar sus valores
bool leerFichero(string inputFile, int &n, int &m, vector<double> &capacidades, vector<vector<double>> &distancias) {
    //n m
    //ci (capacidad de almacenamiento)
    //di (distancia al nodo origen)
    vector<double> temporal;
    vector<vector<double>> temporal2;

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
                    distancias.push_back(temporal);
                    temporal.clear();
                }
                else {
                    distancias.push_back(temporal);
                    temporal.clear();
                }
            }
        }

        file.close();
        return true;
    }
    else {
        cerr << "ERROR: can't open file: " << inputFile << endl;
        cout << "Usage:" << endl << "$met -f file" << endl;
        return false;
    }
}

// Comprobar los argumentos
bool compruebaArgs(int argc, char *argv[], string &inputFile) {
    //met -f fichero entrada
    if(argc == 1) {
        return false;
    }

    for(int i = 1; i < argc; i++)
    {
        if ((string)argv[i] == "-f") {
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

double ogw(int k, int n, vector<double> &capacidades, vector<vector<double>> &distancias) {
    double result = 0.0;
    double resultGood = numeric_limits<int>::max();

    if(k > n) {
        return 0.0;
    }
    if(k == n) {
        return 0.0;
    }
    for (int i = k; i < n; i++)
    {
        result = 0.0;
        for(int j = k; j < n; j++) 
        {
            if(j != i) {
                result += capacidades[j] * distancias[j][i];
            } 
        }
        if(result < resultGood) { 
            resultGood = result;
        }
        
    }

    return resultGood;    
}

//Sink
void sink(vector<double> &v, int n, int i)
{
    int largest;
    int l, r;

    do{
        largest = i;
        l = 2*i + 1;
        r = 2*i + 2;
        if (l < n && v[l] > v[largest]) {
            largest = l; 
        }
    
        if (r < n && v[r] > v[largest]) {
            largest = r; 
        }
    
        if (largest == i) break;
        
        swap(v[i], v[largest]);
        i=largest;  
    } while (true);
    
}
 
//HeapSort
void heapSort(vector<double> &v, size_t n)
{
    for (size_t i = n / 2 - 1; i >= 0; i--){
        sink(v, n, i);
        if (i==0) break;
	}

    for (size_t i=n-1; i>0; i--)
    {
        swap(v[0], v[i]);
        sink(v, i, 0);
    }
}

//Tabla de encaminamiento heurística 1
void tabla_encaminamiento1(int m, int n, vector<int> puertas, vector<vector<double>> &caminos, vector<int> &tabla) {
    
    for(int i = 0; i < n; i++) {
        double distancia = numeric_limits<double>::max();
        for(int j = 0; j < m; j++) {
            if(caminos[i][puertas[j]] < distancia) {
                distancia = caminos[i][puertas[j]];
                tabla[i] = puertas[j];
            }
        }
    }
}

//m puertas en n nodos con mayor capacidad de almacenamiento
double met_greedy_1(int m, int n, vector<double> &capacidades, vector<vector<double>> &caminos, vector<int> &tabla) {
    if(m == n) {
        return 0;
    }
    vector<double> cap2 = capacidades;
    heapSort(cap2, n);
    vector<int> puertasPosi;

    for(int i = n-m; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(cap2[i] == capacidades[j]) {
                puertasPosi.push_back(j);
            }
        }
    }
    
    tabla_encaminamiento1(m, n, puertasPosi, caminos, tabla);

    double result = 0.0;
    for(int j = 0; j < n; j++) {
        if(j != tabla[j]) {
            result += caminos[j][tabla[j]] * capacidades[j];
        }
    }

    return result;
}

//Tráfico estimado heurísitca 2
double ogw2(int m, int n, vector<double> &capacidades, vector<vector<double>> &caminos, int nodo, vector<int> puertas, vector<int> tabla) {
    double result = 0.0;
    for(int j = 0; j < n; j++) {
        if(j != tabla[j]) {
            result += caminos[j][tabla[j]] * capacidades[j];
        }
    }

    return result;
}

//Tabla de encaminamiento heurística 2
vector<int> tabla_encaminamiento2(int m, int n, vector<int> puertas, vector<vector<double>> &caminos, vector<int> tabla) {
    
    for(int i = 0; i < n; i++) {
        double distancia = numeric_limits<double>::max();
        for(unsigned j = 0; j < puertas.size(); j++) {
            if(caminos[i][puertas[j]] < distancia) {
                distancia = caminos[i][puertas[j]];
                tabla[i] = puertas[j];
            }
        }
    }

    return tabla;
}

//Tabla de encaminamiento final
vector<int> tablaBuena;
//colocar una puerta en cada nodo e ir quitando las que hagan que aumente menos el tráfico
double met_greedy_2(int m, int n, vector<double> &capacidades, vector<vector<double>> &caminos) {
    vector<int> tabla2(n);
    for(int i = 0; i < n; i++) {
        tabla2[i] = i;
    }
    vector<int> puertasAux;
    int puerta = 0;
    double result1 = numeric_limits<double>::max();
    double resultFinal = 0;
    
    for(int i = 1; i < n-m+1; i++) {
        double result2 = numeric_limits<double>::max();
        vector<int> tabla(n, -1);
        for(int j = 0; j < n; j++) {
            if(tabla2[j] != -1) {
                puertasAux = tabla2;
                vector<int> puertasPosi;
                for(int x = 0; x < int(puertasAux.size()); x++) {    
                    if(x != j && puertasAux[x] != -1) {
                        puertasPosi.push_back(puertasAux[x]);
                    }
                }
                tabla = tabla_encaminamiento2(m, n, puertasPosi, caminos, tabla);
                
                result1 = ogw2(m, n, capacidades, caminos, j, tabla2, tabla);            
                if(result1 < result2) {
                    result2 = result1;
                    resultFinal = result2;
                    puerta =  j;
                    tablaBuena = tabla;
                }
            }
        }
        tabla2[puerta] = -1;
    }

    return resultFinal;
}

// Tabla de encaminamiento
double met_parser(int m, int n, vector<double> &capacidades, vector<vector<double>> &caminos, vector<int> &tabla) {
    double result = 0;
    for(int j = 0; j < n; j++) {
        if(j != tabla[j]) {
            result += caminos[j][tabla[j]] * capacidades[j];
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    string inputFile = "";

    if(!compruebaArgs(argc, argv, inputFile))
    {
        cout << "Usage:" << endl << "met [-t] [--ignore-naive] -f file" << endl;
    }
    else
    {
        int n = -1, m = -1;
        vector<double> capacidades;
        vector<vector<double>> caminos;

        if(!leerFichero(inputFile, n, m, capacidades, caminos)) {
            return 0;
        }
        
        vector<int> tabla(n, -1);
        
        double result1 = met_greedy_1(m, n, capacidades, caminos, tabla);
        cout <<result1 <<" ";

        double result2 = met_greedy_2(m, n, capacidades, caminos);
        cout <<result2 <<endl;

        for(unsigned i = 0; i < tablaBuena.size(); i++) {
            cout <<tablaBuena[i]<<" ";
        }
        cout <<endl;

        double result3 = met_parser(m, n, capacidades, caminos, tablaBuena);
        cout <<result3 <<endl;
    }
}