//---------------------------------------------------------------------------------
// Find the longest common subsequence of two strings.
//---------------------------------------------------------------------------------

#include <list>                                       
#include <map>
#include <unordered_map>
#include <limits.h>
#include <string>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <stack>
#include <random>
   
using namespace std;
using namespace std::chrono;


int stringfinder(char* X, char* Y, int m, int n, float w, float p)
{
 
    static std::map<char, float> freq{};                      
    freq['A']=10000*0.082;freq['B']=10000*0.015;freq['C']=10000*0.028;freq['D']=10000*0.043;freq['E']=10000*0.13;freq['F']=10000*0.022;freq['G']=10000*0.02;freq['H']=10000*0.061;freq['I']=10000*0.07;freq['J']=10000*0.0015;
    freq['K']=10000*0.0077;freq['L']=10000*0.04;freq['M']=10000*0.024;freq['N']=10000*0.067;freq['O']=10000*0.019;freq['P']=10000*0.019;freq['Q']=10000*0.00095;freq['R']=10000*0.06;freq['S']=10000*0.063;freq['T']=10000*0.091;
    freq['U']=10000*0.028;freq['V']=10000*0.0098;freq['W']=10000*0.024;freq['X']=10000*0.0015;freq['Y']=10000*0.02;freq['Z']=10000*0.00074;

    float cost[m + 1][n + 1];
    for (int i = 0; i < m+1; i++){
      for (int j = 0; j < n+1; j++){
        cost[i][j]=0;
      }
    }
    int result = 0; 
    int result_index[2];
    result_index[0]=result_index[1]=0;
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                cost[i][j] = 0;
 
            else if (X[i - 1] == Y[j - 1]) {
                cost[i][j] = cost[i - 1][j - 1] + 1;//freq[X[i - 1]];
                if (cost[i][j] - result>1e-10){
                  result = cost[i][j];
                  result_index[0]=i-1; 
                  result_index[1]=j-1; }

            }
            else{
                if (cost[i-1][j-1]!=0){
                  cost[i][j] =  cost[i-1][j-1] - 10;//p;
                  if (cost[i][j] > result){
                    result = cost[i][j];
                    result_index[0]=i-1; 
                    result_index[1]=j-1; }


                }
                else{
                   if (cost[i][j] > result){
                     result = cost[i][j];
                     result_index[0]=i-1; 
                     result_index[1]=j-1; }

                }
        
            }
        }
    }
    for (int i=0; i<m+1; i++){
      for (int j=0; j<n+1; j++){
        cout << cost[i][j] << " ";
      }
      cout << "\n";
    }
    cout << "result_index: " << result_index[0]+1 << " " << result_index[1]+1 << endl;
    result_index[0]=result_index[0];
    result_index[1]=result_index[1];

    for (int i=max(m,n); i>0; i--){
      if (cost[result_index[0]][result_index[1]]!=0){
          cout << X[result_index[0]] << " " << Y[result_index[1]] << endl;
          result_index[0]--;
          result_index[1]--;
      }

    }
    cout << X[result_index[0]] << " " << Y[result_index[1]] << endl;

    return 0;
}

int main(int argc, char* argv[])
{
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist2(1,26);
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    float w=1;
    float p=10; 
    std::uniform_int_distribution<std::mt19937::result_type> dist3(1,10);
    float penalty[10];
    for (int i=0; i<10;i++){
      penalty[i]=7.4+144*i;
    }


    if (argc>2){
      int  m = atoi(argv[1]); 
      int  n = atoi(argv[2]);
      char X[m];
      char Y[n];

      for (int i = 0; i < m; i++) {      
        X[i]=alpha[dist2(rng)- 1];
        cout << X[i] ;
      } 
      cout <<"\n";
      for (int i = 0; i < n; i++) {      
          Y[i]=alpha[dist2(rng)- 1];
          cout << Y[i] ;
      }                  
      cout <<"\n";
      auto start = high_resolution_clock::now();
      stringfinder(X, Y, m, n,w,p);
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      cout << m << " " << n << " " << duration.count() << endl;
    }
    else{  
      char X[]= ("CAABCAA");
      char Y[]= ("BCAACCBB");
      int  m = strlen(X);
      int  n = strlen(Y);
      for (int i = 0; i < m; i++) {    
        cout << X[i] ;   
      }
      cout <<"\n";   
      for (int i = 0; i < n; i++) {
        cout << Y[i] ;
      }
      cout <<"\n"; 
      auto start = high_resolution_clock::now();
      stringfinder(X, Y, m, n,w,p);
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      cout << m << " " << n << " " << p << " " << duration.count() << endl;

    }
    return 0;
}
