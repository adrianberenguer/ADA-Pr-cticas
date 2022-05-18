//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>
#include <limits>
#include <iomanip>
#include <queue>
#include <tuple>

using namespace std;

// Leer el fichero de entrada y guardar sus valores
bool leerFichero(string inputFile, int &n, int &m, vector<double> &capacidades, vector<vector<double> > &distancias) {
    //n m
    //ci (capacidad de almacenamiento)
    //di (distancia entre nodos)
    vector<double> temporal;
    vector<vector<double> > temporal2;

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

int visitados = 0, explorados = 0, hoja = 0, no_factibles = 0, no_prometedores = 0,
prometedores_descartados = 0, mejor_actualizada_hoja = 0, mejor_actualizada_no_hoja = 0;
vector<int> tablaFinal;
vector<int> tablaGreedy;

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
                    tablaGreedy = tabla;
                }
            }
        }
        tabla2[puerta] = -1;
    }

    return resultFinal;
}

//Calcula el tráfico en base a tabla de 0 y 1
double tablaBuena(int m, int n, vector<double> capacidades, vector<vector <double>> caminos, vector<int> tabla) {
    double result;
    double total = 0.0;

    for (int i = 0; i < n; i++) {
        result = numeric_limits<double>::max();
        for (int j = 0; j < n; j++) {
            if(tabla[j] == 1) {
                result = min(result, capacidades[i] * caminos[i][j]);
            }
        }
        total+= result;
    }

    return total;
}

//Cota optimista basada en poner 1 en los nodos restantes
double cota_optimista(int m, int n, vector<double> capacidades, vector<vector<double>> caminos, vector<int> puertas, int indice) {

    for(unsigned i = indice; i < puertas.size(); i++) {
        puertas[i] = 1;
    }
    double result = tablaBuena(m, n, capacidades, caminos, puertas);//tabla_encaminamiento(m, n, capacidades, caminos, puertas);
    return result;
}

double met_bb(int m, int n, vector<double> capacidades, vector<vector<double> > caminos, 
vector<int> puertas, double indice, double best_val, int cont) {
    using Sol = vector<short>;
    using Node = tuple<double, Sol, int>;
    Sol vec(puertas.size());
    priority_queue<Node> pq;
    Node nodo;
    double ind, value, cota;
    cota = cota_optimista(m, n, capacidades, caminos, puertas, ind);
    best_val = met_greedy_2(m, n, capacidades, caminos);
    puertas = tablaGreedy;
    
    pq.emplace(numeric_limits<double>::max(), Sol(puertas.size()), 0);

    while( !pq.empty() ) {
        nodo = pq.top();
        value = get<0>(nodo);
        vec = get<1>(nodo);
        ind = get<2>(nodo);
        pq.pop();

        if(ind == puertas.size()) { //Nodos hoja
            hoja++;
            double bestnew = tablaBuena(m, n, capacidades, caminos, puertas);
            if(bestnew < best_val) {
                best_val = bestnew;
                tablaFinal = puertas;
            }
            continue ;
        }

        if(ind >= n - m && m - cont == n - ind) { //Mejora -> Si quedan x nodos por explorar y x puertas, coloco puertas en los que faltan y termino
            for(unsigned x = ind; x < puertas.size(); x++) {
                vec[x] = 1;
            }
            double aux = tablaBuena(m, n, capacidades, caminos, puertas);
            if(aux < best_val) {
                best_val = aux;
                tablaFinal = puertas;
            }
            continue ;
        }
        if(cont == m) { //Mejora -> si ya tengo m puertas colocadas, pongo 0 en el resto y se acabó
            for(unsigned x = ind; x < puertas.size(); x++) {
                vec[x] = 0;
            }
            double aux = tablaBuena(m, n, capacidades, caminos, puertas);
            if(aux < best_val) {
                best_val = aux;
                tablaFinal = puertas;
            }
            continue ;
        }

        for(int i = 0; i < 2; i++) { //Nodos visitados
            visitados++;
            vec[ind] = i;
            
            if(i == 1) { 
                cont++; //Número de puertas colocadas
            }
            if(cont <= m) { //Nodos explorados
                explorados++;
                double newBest = cota_optimista(m, n, capacidades, caminos, puertas, ind);
                if(newBest < best_val) {
                    //met_bb(m,n,capacidades,caminos,puertas, ind+1, best_val, cont);
                    pq.emplace(newBest, vec, ind+1);
                }
                else { //Descartados por no prometedores
                    no_prometedores++;
                }
            }
            else { //Descartados por no factibles
                no_factibles++;
            }
            
        }
    }

    return best_val;
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
        vector<vector<double> > caminos;

        if(!leerFichero(inputFile, n, m, capacidades, caminos)) {
            return 0;
        }

        vector<int> puertas(n, 0);
        long double time;
        double best_val = numeric_limits<double>::max();
        double indice = 0;

        auto start = clock();
            double result = met_bb(m, n, capacidades, caminos, puertas, indice, best_val, 0);
        auto end = clock();

        //Mostrar resultados
        cout << fixed << setprecision(2) << result << endl;
        if(tablaFinal.size() == 0) { //El voraz saca el óptimo
            for(unsigned i = 0; i < tablaGreedy.size(); i++) {
                if(tablaGreedy[i] == (int)i) {
                    cout <<tablaGreedy[i]<<" ";
                }
            }
        }
        else {
            for(unsigned i = 0; i < tablaFinal.size(); i++) {
                if(tablaFinal[i] == 1) {
                    cout <<i<<" ";
                }
            }
        }
        cout<<endl;
        time = (((double)end - start) / CLOCKS_PER_SEC) * 1000;
        cout <<visitados<<" "<<explorados<<" "<<hoja<<" ";
        cout <<no_factibles<<" "<<no_prometedores<<" "<< prometedores_descartados<<" ";
        cout <<mejor_actualizada_hoja<<" "<<mejor_actualizada_no_hoja <<endl;
        cout <<time<<endl;
    }
    return 0;
}