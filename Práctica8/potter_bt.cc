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

using namespace std;

int glob = -1;

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
                //strcpy(fichIn, (string)argv[i+1]);
                fichIn = (string)argv[i+1];
                i++;
                if(!readFich(fichIn, n, T, t, v, m))
                {
                   //cerr << "ERROR: can't open file: " << fichIn << endl; 
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

void matriz(vector<double> v, vector<double> &v2, vector<double> m, vector<double> &p, vector<double> t)
{
    for(unsigned i=0;i<m.size();i++)
    {         
        for(unsigned j=0;j<m[i];j++)
        {
            v2.push_back(v[i]);
            p.push_back(t[i]);          
        }     
    }
    
}

double add_rest( const vector<double> &v, size_t k )
{
    double v2 = 0.0;
    for( size_t i = k; i < v.size(); i++) v2 += v[i];
    return v2;
}

double value( const vector<double> &v, const vector<unsigned> &x ) 
{
    double r = 0.0;
    for( size_t i = 0; i < v.size(); i++ ) r += v[i] * x[i];
    return r;
}

double weight( const vector<double> &w, size_t k, const vector<unsigned> &x ) 
{
    double r = 0.0;
    for( size_t i = 0; i < k; i++ ) r = w[i] * x[i];
    return r;
}

double knapsack_d(const vector<double> &v, const vector<double> &w, double W, size_t k) 
{
    vector<size_t> idx( w.size() );
    for( size_t i = 0; i < idx.size(); i++) idx[i] = i;

    sort( idx.begin(), idx.end(), [&w,&v]( size_t x, size_t y )
    {
        return v[x]/w[x] > v[y]/w[y];
    } );

    double resul = 0.0;

    for( auto i : idx ) 
    {
        if( w[i] < W ) 
        {
            resul += v[i];
            W -= w[i]; 
        }
    }

    return resul;
}

//usar el vector m
double knapsack_c(const vector<double> &v, const vector<double> &w, double W, size_t k)
{
    /*vector<unsigned> idx(w.size() - k);
    
    for( unsigned i = 0; i < idx.size(); i++) idx[i] = i+k;

    sort( idx.begin(), idx.end(),[v,w]( unsigned x, unsigned y )
    {
        return v[x]/w[x] > v[y]/w[y];
    });*/

    double acc_v = 0.0;
    double acc_w = 0.0;

    for( unsigned i = k; i < w.size(); i++ ) 
    {
        if( w[i] > W ) 
        {
            acc_v += W/w[i] * v[i];
            acc_w += W/w[i] * w[i];
            break;
        }
        acc_v += v[i];
        W -= w[i];
    }

    return acc_v;
}

int nodos_hoja_visitados=0;
int nodos_visitados=0;
int nodos_explorados=0;
int nodos_prometedores=0;
int nodos_factibles =0;

void knapsack( const vector<double> &v, const vector<double> &w,double W, size_t k, vector<unsigned> &x, double weight, double value, double &best_val )
{
    if( k == x.size() ) 
    {
        nodos_hoja_visitados++;
        best_val = max(best_val, value);
        return;
    }

    for (int j = 1; j >= 0; j--) 
    {
        x[k]=j;
        double new_weight = weight + x[k] * w[k]; 
        double new_value = value + x[k] * v[k]; 

        nodos_visitados++;

        if(new_weight <= W)
        {
            if(new_value + knapsack_c( v, w, W -new_weight, k+1) > best_val)
            {
                nodos_explorados++;
                knapsack( v, w, W, k+1, x, new_weight, new_value, best_val);
            }
            else
            {
                nodos_prometedores++;
            }
        }
        else
        {
            nodos_factibles++;
        }
    }
}

double knapsack( const vector<double> &v, const vector<double> &w, double W ) 
{
    vector<size_t> idx( v.size() ); 
    iota( begin(idx), end(idx), 0 );
    sort( begin(idx), end(idx),
        [&v,&w]( size_t i, size_t j) 
    {
        return v[i]/w[i] > v[j]/w[j];
    }
    );

    vector<double> s_v( v.size() ), s_w( w.size() );

    for( size_t i = 0; i < v.size(); i++ ) 
    {
        s_v[i] = v[ idx[i] ];
        s_w[i] = w[ idx[i] ]; 
    }

    vector<unsigned> x(v.size());
    double best_val = knapsack_d( s_v, s_w, W, 0 ); 

    knapsack( s_v, s_w, W, 0, x, 0, 0, best_val);

    return best_val;
}

int main(int argc, char *argv[])
{
    string fichIn = "";
    double T = -1;
    int n = -1;
    vector<double> v2,v,t,m,p;

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

        glob = n;

        matriz(v, v2, m, p, t);

        double s1 = 0;
        auto start = clock();
        s1 = knapsack(v2, p, T);
        auto end = clock();
        
        //sol
        cout <<s1<<endl;

        cout<<nodos_visitados<<" "<<nodos_explorados<<" "<<nodos_hoja_visitados<<" "<<nodos_factibles<<" "<<nodos_prometedores<<endl;
        //nº nodos visited, nº nodos explored ,nodos hoja visitados
        //nodos descartados por no factibles y nodos desc por no prometedores

        long double time = (((double)end - start) / CLOCKS_PER_SEC) * 1000;
        //CPU time
        cout<<time<<endl;
    }

    return 0;
}

/*void backtracking(vector<double> &v, vector<double> &w, double W, size_t k, vector<unsigned> &x, double actual_t, double actual_v, double best_v)
{
    if(k == x.size())
    {
        nodos_hoja_visitados++;
        best_v = max(best_v, acc_v);
        return ;
    }

    for(unsigned j = 0; j < 2; j++)
    {
        nodos_visitados++;
        x[k] = j;
        double actual_t = acc_t + x[k] * t[k];
        double actual_v = acc_v + x[k] * v[k];

        if(actual_t <= W)
        {
            if((actual_v + knapsack_c(v, w, W - actual_t, k+1)) > best_v)
            {
                nodos_explorados++;
                backtracking(v, w, W, k+1, x, actual_t, actual_v, best_v);
            }
            else
            {
                nodos_no_prometedores++;
            }
        }
        else
        {
            nodos_no_factibles;
        }
    }
}*/