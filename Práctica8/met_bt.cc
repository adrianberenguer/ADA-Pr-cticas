//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>
#include <limits>

using namespace std;

// Leer el fichero de entrada y guardar sus valores
bool leerFichero(string inputFile, int &n, int &m, vector<double> &capacidades, vector<vector<double>> &distancias) {
    //n m
    //ci (capacidad de almacenamiento)
    //di (distancia entre nodos)
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

double calculo_trafico(int m, int n, vector<double> capacidades, vector<vector <double>> caminos, vector<double> tabla) {
    double result = 0;
    for(int j = 0; j < n; j++) {
        if(tabla[j] != j) {
            result += caminos[j][tabla[j]] * capacidades[j];
        }
    }
    return result;
}

double tabla_encaminamiento(int m, int n, vector<double> capacidades, vector<vector <double>> caminos, vector<double> tabla) {
    double result = numeric_limits<double>::max();
    vector<double> puertas = tabla;

    for(int i = 0; i < n; i++) {
        if(puertas[i] == 0) {
            result = numeric_limits<double>::max();
            for(int k = 0; k < n; k++) {
                if(caminos[i][k] < result && (puertas[k] == k || puertas[k] == 1) && k != i) {
                    result = caminos[i][k];
                    puertas[i] = k;
                }
            }
        }
        else {
            puertas[i] = i;
        }
    }

    double trafico = calculo_trafico(m, n, capacidades, caminos, puertas);
    return trafico;
}

double cota_optimista(int m, int n, vector<double> capacidades, vector<vector<double>> caminos, vector<double> puertas, int indice) {

    for(unsigned i = indice+1; i < puertas.size(); i++) {
        puertas[i] = 1;
    }
    double result = tabla_encaminamiento(m, n, capacidades, caminos, puertas);
    return result;
}

int visitados = 0, explorados = 0, hoja = 0, no_factibles = 0, no_prometedores = 0;

void backtracking
(int m, int n, vector<double> capacidades, vector<vector<double>> caminos,
 vector<double> puertas, double indice, double &best_val, double new_val) {
    
    if(indice == puertas.size()) {
        hoja++;
        //best_val = new_val;
        return ;
    }
    //cout <<"hola"<<best_val<<endl;
    for(int i = 1; i >= 0; i--) {
        puertas[indice] = i;
        //for(unsigned j = 0; j < puertas.size(); j++) {
        //    cout <<puertas[j]<<" ";
        //}
        //cout <<endl;
        double new_val = tabla_encaminamiento(m, n, capacidades, caminos, puertas);
        cout << new_val<<endl;
        if(new_val < best_val && cota_optimista(m, n, capacidades, caminos, puertas, indice) < best_val) {
            best_val = new_val;
            backtracking(m,n,capacidades,caminos,puertas, indice+1, best_val, new_val);
        }
    }
    
    //return best_val;
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

        vector<double> puertas(n, 0);
        double time = 0;
        double best_val = numeric_limits<double>::max();
        double indice = 0;

        auto start = clock();
            backtracking(m, n, capacidades, caminos, puertas, indice, best_val, 0);
        auto end = clock();

        //Mostrar
        cout <<best_val<<endl;
        time = 1000.0 * (end-start) / CLOCKS_PER_SEC;
        cout <<visitados<<" "<<explorados<<" "<<hoja<<" ";
        cout<<no_factibles<<" "<<no_prometedores<<endl;
        cout <<time<<endl;
    }
    return 0;
}