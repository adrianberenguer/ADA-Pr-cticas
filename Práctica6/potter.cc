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

int glob = -1;
int nGlob;
vector<string> mMemo;
vector<string> mIter;
vector<double> copiasBuenas;
void copias(vector<vector<double>> &m2, vector<double> &m, const vector<double> &v, int n, unsigned T,const vector<double> &p);

bool readFich(string fichIn, int &n, unsigned &T, vector<double> &t, vector<double> &v, vector<double> &m)
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
        int num = -1;
        vector<int> aux1;
        vector<unsigned> aux2;
        vector<double> aux;
        int n2 = T;

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

bool args(int argc, char *argv[], string &fichIn, bool &comp2, bool &comp1)
{
    int n;
    unsigned T;
    vector<double> t,v,m;

    for(int i = 1; i < argc; i++){
       
        if(strcmp(argv[i], "--ignore-naive") ==0)
            comp2 = false;
        else if(strcmp(argv[i], "-t")==0)
            comp1 = true;
        else if(strcmp(argv[i], fichIn.c_str()) ==0)
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

int recurSin(int n, unsigned T, const vector<double> &t, const vector<double> &v, vector<double> &m)
{
    int s1 = 0, s2 = 0;

    if(n == 0)
        return 0; 
    
    vector<double> aux = m;

    if(aux[n-1] > 0)
    {
        --aux[n-1];
        s1 = recurSin(n, T, t, v, aux);
    }
    else
    {
        s1 = recurSin(n-1 , T, t, v, aux);
    }

    if(t[n-1] <= T)
    {
        if(m[n-1] > 1)
        {
            --m[n-1];
            s2 = v[n-1] + recurSin(n, T-t[n-1], t, v, m);
        } 
        else
        {
            s2 = v[n-1] + recurSin(n-1, T-t[n-1], t, v, m);
        }
    }
    
    return max(s1, s2);
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

int recurMemo(int n, unsigned T, const vector<double> &p, const vector<double> &v2, vector<vector<double>> &m2)
{
    double s1 = -1, s2 = -1;

    if(m2[n][T] != glob)
    {
        return m2[n][T];
    }
    if(n == 0)
    {
        return m2[n][T] = 0.0;
    }

    s1 = recurMemo(n-1, T, p, v2, m2);
    s2 = numeric_limits<double>::lowest();

    if(p[n-1] <= T)
    {
        s2 = v2[n-1]+recurMemo(n-1, T-p[n-1], p, v2, m2);
    }

    return m2[n][T] = max(s1, s2);
}

int recurMemo(const vector<double> &v2, const vector<double> &p, int n, unsigned T, vector<double> &m, vector<double> &t)
{
    vector<vector<double>> m2(n+1, vector<double>(T+2, glob));
    double s2 = recurMemo(n, T, p, v2, m2);
    int i=0;

    for(int x = 0; x < nGlob; x++)
    { //para la matriz de memoization
        i = i+m[x];
        m2[i][T+1] = x;

        for(unsigned y = 0; y < T+1 ; y++)
        {
            if(m2[i][y] == -1)
            {
                mMemo.push_back("- ");
            }
            else
            {
                mMemo.push_back(to_string((int)m2[i][y]));
                if(y != T)
                {
                    mMemo.push_back(" ");
                }
            }
        }
        mMemo.push_back("\n");
    }

    copias(m2, m, v2, n, T, t);

    return s2;
}

void copias(vector<vector<double>> &m2, vector<double> &m, const vector<double> &v, int n, unsigned T,const vector<double> &t)
{   
    for(int x=0; x<nGlob; x++)
    {
        copiasBuenas.push_back(0);
    }

    int buena = m2[n][T+1],columna = T;

    for(int fila =n; fila > 0; fila--)
    {
        if(m2[fila][T+1] != glob)
        {
            buena = m2[fila][T+1];
        }
        if(m2[fila][columna] != m2[fila-1][columna])
        {

            columna = columna - t[buena];
            copiasBuenas[buena]++;
        }
    }

}

int iterAlm(int n2, unsigned T, vector<double> &v, vector<double> &w, vector<double> &m)
{
    vector< vector< double >> M( n2+1, vector<double>(T+2));

    for( unsigned W = 0; W <= T; W++ ) M[0][W] = 0; 
 
    for( int n = 1; n <= n2; n++ )
    {
        for( unsigned W = 1; W <= T; W++ )
         {
            double S1 = M[n-1][W];
            double S2 = numeric_limits<double>::lowest();
            if( W >= w[n-1] ) 
                S2 = v[n-1] + M[n-1][W-w[n-1]]; 

            M[n][W] = max( S1, S2 ); 
        }
    }

    int i = 0;
    for(int x = 0; x < nGlob; x++)
    { //para la matriz iterativa
        i = i + m[x];
        M[i][T+1] = x;

        for(unsigned y = 0; y < T+1 ; y++)
        {
            if(M[i][y] == -1)
            {
                mIter.push_back("- ");
            }
            else
            {
                mIter.push_back(to_string((int)M[i][y]));
                if(y != T)
                {
                    mIter.push_back(" ");
                }
            }
        }
        mIter.push_back("\n");
    }     

    return M[n2][T]; 
}

int iterMejor(int n2, unsigned T, vector<double> &v, vector<double> &w)
{
    vector<double> v0(T+1);
    vector<double> v1(T+1);

    for( unsigned W = 0; W <= T; W++ ) v0[W] = 0; 

    for( int n = 1; n <= n2; n++ ) 
    {
        for( unsigned W = 1; W <= T; W++ ) 
        {
            double S1 = v0[W];
            double S2 = numeric_limits<double>::lowest();
            if( W >= w[n-1] ) 
                S2 = v[n-1] + v0[W-w[n-1]];

            v1[W] = max( S1, S2 ); 
        }
        swap(v0,v1);
    }

    return v0[T];
}

void traza(vector<vector<int>>matriz, int n, int T, vector<double> &v, vector<double> &m, vector<double> &p, vector<double>obj){
    
    if(n == 0) return;

    int s1 = matriz[n-1][T];
    int s2 = 0;
    
    if(p[n-1] <= T)
        s2 = p[n-1] + matriz[n-1][T-p[n-1]];
 
    if(s1 >= s2)
    { //no lo coge
        if(m[n-1] > 1)
        {
            m[n-1]--;
            traza(matriz, n, T, v, m, p, obj);
        }
        else
        {
            traza(matriz, n-1, T,v,m, p, obj);
        }     
    }
    else
    { //lo coge
        if(m[n-1] > 1)
        {   
            m[n-1]--;
            obj[n-1]++;
            traza(matriz, n-1, T-p[n-1],v,m, p, obj);
        }
        else
        {
            obj[n-1]++;
            traza(matriz, n, T-p[n-1],v,m, p, obj);
        }  
    }

}

int main(int argc, char *argv[])
{
    string fichIn = "";
    bool comp1 = false;
    bool comp2 = true;
    unsigned T = -1;
    int n = -1, num1 = -1, num2 = -1;
    vector<double> v2,v,t,m,p;

    if(!args(argc, argv, fichIn, comp2, comp1))
    {
        cerr << "Usage:" << endl << "potter [-t] [--ignore_naive] -f file" << endl;
    }
    else
    {
        if(!readFich(fichIn, n, T, t, v, m))
        {
           return 0;
        }
        nGlob = n;
        vector<double> auxM = m;

        if(comp2)
        {
            num1 = recurSin(n, T, t, v, auxM);
            cout <<num1<<" ";
        }

        matriz(v, v2, m, p, t);
        int tot = 0;

        for (size_t i = 0; i < m.size(); i++)
        {
            tot = tot + m[i];
        }
        //cout <<"tot: "<<tot<<endl;
        nGlob = n;
        
        //recursivo memoization
        num2 = recurMemo(v2, p, tot, T, m, t);

        cout <<num2<<" ";

        //iterativo con almacém
        int num3 = iterAlm(tot, T, v2, p, m);

        cout <<num3<<" ";

        //iterativo mejorado
        int num4 = iterMejor(tot, T, v2, p);

        cout <<num4<<endl;

        //vector de copias cogidas
        for(unsigned x = 0; x < copiasBuenas.size(); x++)
        {
            cout <<copiasBuenas[x]<<" ";
        }
        cout <<endl;

        //tiempo necesitado
        int tiempos = 0;
        for(unsigned x = 0; x < copiasBuenas.size(); x++)
        {  
            tiempos += copiasBuenas[x] * t[x];
        }
        cout <<tiempos<<endl;

        if(comp1)
        {
            cout<<"Memoization matrix: "<<endl;
            for(unsigned x = 0; x < mMemo.size(); x++)
            {
                cout << mMemo[x];
            }

            cout<<"Iterative matrix: "<<endl;
            for(unsigned x = 0; x < mIter.size(); x++)
            {
                cout << mIter[x];
            } 
        }       
        
    }

    return 0;
}