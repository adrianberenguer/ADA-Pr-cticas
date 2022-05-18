//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h> 
#include <cstring> 
#include <string.h>
#include <numeric>
#include <queue>
#include <stdio.h>
#include <tuple>

using namespace std;

int nodos_visitados = 0;
int nodos_explorados = 0;
int nodos_hoja_visitados = 0;
int nodos_no_factibles = 0;
int nodos_no_prometedores = 0;
int nodos_prom_desc = 0;
int actual1 = 0;
int actual2 = 0;

bool readFich(string fichIn, int &n, double &T, vector<double> &t, vector<double> &v, vector<double> &m)
{
    ifstream file;
    file.open(fichIn.c_str(), ios::in);

    if(file.is_open())
    {
        //  n T
        //  t1 t2 t3 tiempos
        //  v1 v2 v3 dinero
        //  m1 m2 m3 num max de copias a realizar

        string line = "";
        double num = -1;
        vector<double> aux;
        double n2 = T;

        while(getline(file, line))
        {
            stringstream ss(line);
            
            if(n == -1 && n2 == -1)
            {
                ss >> n;
                ss >> T;
            }
            else
            {
                while(ss >> num)
                {
                    aux.push_back(num);
                }

                if(t.size() == 0)
                {
                    t = aux;
                    aux.clear();
                }
                else if(v.size() == 0)
                {
                    v = aux;
                    aux.clear();
                }
                else if(m.size() == 0)
                {
                    m = aux;
                    aux.clear();
                }
            }
        }

        file.close();

        return true;
    }
    else
    {
        cerr << "ERROR: can't open file: " << fichIn << endl;

        return false;
    }

}

bool args(int argc, char *argv[], string &fichIn)
{
    int n;
    double T;
    vector<double> t,v,m;

    for(int i = 1; i < argc; i++){
       
        if(strcmp(argv[i], fichIn.c_str()) ==0)
        {

        }
        else if (strcmp(argv[i], "-f")==0)
        {    
            if(i < argc - 1)
            {
                fichIn = (string)argv[i+1];
                i++;
                if(!readFich(fichIn, n, T, t, v, m))
                {
                   return false;
                }
            }
            else
            {
                cerr << "ERROR: missing filename." << endl;
                return false;
            }     
        }   
        else
        {
            cerr << "ERROR: unknown option " << argv[i] << "." << endl;
            return false;
        }
    }

    return true;
}

double knapsack_d(const vector<double> &v, const vector<double> &w, vector<double> &m, size_t k, double W) 
{
    double resul = 0.0;

    for( unsigned i = k; i < w.size() ; i++ ) 
    {
        for(int h = 0; h < m[i]; h++)
        {
            if(w[i] <= W)
            {
                resul += v[i];
                W -= w[i];
            }
            if(W == 0)
            {
                return resul;
            }
        }
    }

    return resul;
}

double knapsack_c(const vector<double> &v, const vector<double> &w,vector<double> &m, size_t k, double W)
{
    double acc_v = 0.0;

    for( unsigned i = k; i < w.size() ; i++ ) 
    {
        for(int h = 0; h < m[i]; h++)
        {
            if( w[i] > W ) 
            {
                acc_v += W/w[i] * v[i];
                return acc_v;
            }
            else{
                W -= w[i];
                acc_v += v[i];
            }
        }
    }

    return acc_v;
}

double knapsack( const vector<double> &v, const vector<double> &w, vector<double> &m, double W ) 
{
    vector<size_t> idx( v.size() );
    iota( begin(idx), end(idx), 0 );

    sort( begin(idx), end(idx),[&v,&w]( size_t i, size_t j) 
    {
        return v[i]/w[i] > v[j]/w[j];
    });

    vector<double> s_v( v.size() ), s_w( w.size() ), s_m(m.size());
    for( size_t i = 0; i < v.size(); i++ ) 
    {
        s_v[i] = v[ idx[i] ];
        s_w[i] = w[ idx[i] ];
        s_m[i] = m[ idx[i] ];
    }

    typedef vector<short> Sol;
    typedef tuple< double, double, double, Sol, unsigned > Node;
    priority_queue< Node > pq;
    Sol x(v.size());
    size_t k;
    Node n;
    double value, weight, ignore;

    double best_val = knapsack_d( s_v, s_w, s_m, 0, W);
    double opt_bound = knapsack_c(s_v, s_w, s_m, 0, W);
    
    pq.emplace( opt_bound, 0.0, 0.0, Sol(v.size()), 0 );

    while( !pq.empty() )
    {
        n = pq.top();
        ignore = get<0>(n);
        value = get<1>(n);
        weight = get<2>(n);
        x = get<3>(n);
        k = get<4>(n);
        pq.pop();

        if(ignore < best_val)
        {
            nodos_prom_desc++; //nodo
        }
        
        if( k == v.size() ) { 
            nodos_hoja_visitados++; //nodo

            best_val = max( best_val, value );

            if(value > best_val)
            {
                actual1++; //nodo
                best_val = value;
            }
            continue;
        }

        for (unsigned j = 0; j <= s_m[k]; j++ ) { 
            nodos_visitados++; //nodo

            x[k] = j;
            double new_weight = weight + x[k] * s_w[k]; 
            double new_value = value + x[k] * s_v[k]; 

            if( new_weight <= W ) 
            {
                double pes_bound = new_value + knapsack_d( s_v, s_w, s_m, k+1, W-new_weight);
                if(pes_bound > best_val)
                {
                    actual2++; //nodo
                    best_val = max( best_val, pes_bound);
                }
                
                double opt_bound = new_value + knapsack_c( s_v, s_w, s_m, k+1, W-new_weight);

                if( opt_bound > best_val) 
                {
                    nodos_explorados++; //nodo 
                    pq.emplace( opt_bound, new_value, new_weight, x, k+1 );
                }
                else{
                    nodos_no_prometedores++; //nodo    
                }
            }
            else{
                nodos_no_factibles++; //nodo
            }
        }
    }

    return best_val;
}

int main(int argc, char *argv[])
{
    string fichIn = "";
    double T = -1;
    int n = -1;
    vector<double> v,t,m;

    if(!args(argc, argv, fichIn))
    {
        cerr << "Usage:" << endl << "potter -f file" << endl;
    }
    else
    {
        if(!(readFich(fichIn, n, T, t, v, m)))
        {
            return 0;
        }

        double s1 = 0;
        auto start = clock();
        s1 = knapsack(v, t, m, T);
        auto end = clock();
        
        //sol
        cout <<s1<<endl;

        //nodes
        if(nodos_visitados == 0)
        {
            cout <<"- ";
        }
        else{
            cout<<nodos_visitados<<" ";
        }
        if(nodos_explorados == 0)
        {
            cout <<"- ";
        }
        else{
            cout<<nodos_explorados<<" ";
        }
        if(nodos_hoja_visitados == 0)
        {
            cout <<"- ";
        }
        else{
            cout <<nodos_hoja_visitados<<" ";
        }
        if(nodos_no_factibles == 0)
        {
            cout <<"- ";
        }
        else{
            cout <<nodos_no_factibles<<" ";
        }
        if(nodos_no_prometedores == 0)
        {
            cout <<"- ";
        }
        else{
            cout <<nodos_no_prometedores<<" ";
        }
        if(nodos_prom_desc == 0)
        {
            cout <<"- ";
        }
        else{
            cout <<nodos_prom_desc<<" ";
        }
        if(actual1 == 0){
            cout <<"- ";
        }
        else{
            cout <<actual1<<" ";
        }
        if(actual2 == 0){
            cout <<"- "<<endl;
        }
        else{
            cout <<actual2 <<endl;
        }

        //nº nodos visited, nº nodos explored ,nodos hoja visitados
        //nodos descartados por no factibles y nodos desc por no prometedores
        //nodos prometedores pero descartados
        //nº veces que la mejor solucion se ha actualizado a partir de un nodo
        //nº veces que la mejor solucion se ha actualizado a partir de la cota pesimista
        long double time = (((double)end - start) / CLOCKS_PER_SEC) * 1000;
        //CPU time
        cout<<time<<endl;
    }

    return 0;
}