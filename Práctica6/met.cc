//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>

using namespace std;

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

// Recursivo sin almacén(versión ingenua)
double met_naive(int n, int m, vector<double> &capacidades, vector<double> &distancias) {

    return 0;
}

// Recursivo con almacén
double met_memo(int n, int m, vector<double> &capacidades, vector<double> &distancias) {

    return 0;
}

// Iterativo con almacén que hace uso de una tabla para 
// almacenar resultados intermedios
double met_it_matrix(int n, int m, vector<double> &capacidades, vector<double> &distancias) {

    return 0;
}

// Iterativo con almacén con complejidad espacial mejorada
double met_it_vector(int n, int m, vector<double> &capacidades, vector<double> &distancias) {

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

        

    }


    return 0;
}