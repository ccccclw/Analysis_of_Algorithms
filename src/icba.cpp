//----------------------------------------------------
// Interval based constant best approximation.
//----------------------------------------------------

#include <list>                                       
#include <limits.h>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <stack>
#include <random>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <sys/time.h>
#include <sys/resource.h>

   
using namespace std;
using namespace std::chrono;

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}    

int getValue(){ //The value is in Kb
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
     
    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmPeak:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;                                   
}    

int Icba(int* x, float* y, int m, float p)
{
//    int less_x[m] ;
    vector<int> less_x;
    vector<int> less_x_count;
    vector<float> less_y;
    vector<float> less_y2;
    less_x_count.push_back(0);
    //for (int i=0; i<m; i++)
    int tmp_count=1;
    int tmp_i=0;
    float tmp_y=0;
    float tmp_y2=0;
    while (tmp_i<m){
      tmp_y+=y[tmp_i];
      tmp_y2+=y[tmp_i]*y[tmp_i];
      if (x[tmp_i+1]==x[tmp_i]){
        tmp_count++;
        tmp_i++;
      }
      else{
        if (tmp_count==1){
          less_x.push_back(x[tmp_i]);
          less_x_count.push_back(tmp_count+less_x_count[less_x_count.size()-1]);
          tmp_count=1;
          less_y.push_back(y[tmp_i]);
          less_y2.push_back(y[tmp_i]*y[tmp_i]);
          tmp_i++;          
          tmp_y=0;
          tmp_y2=0;
        }
        else{
          less_x.push_back(x[tmp_i]);
          less_x_count.push_back(tmp_count+less_x_count[less_x_count.size()-1]);
          less_y.push_back(tmp_y);
          less_y2.push_back(tmp_y2);
          tmp_count=1;
          tmp_i++;
          tmp_y=0;
          tmp_y2=0;
        }
      }
    }
    less_x_count.erase(less_x_count.begin());
    m=less_y.size();
    int x_max = x[m];
    float err[m];
    float err2[m];
    float tmp_sum = 0;
    float tmp_sum2 = 0; 
    for (int i=0; i<m;i++ ){
        tmp_sum+=less_y[i];
        tmp_sum2+=less_y2[i];
        err[i] = tmp_sum;
        err2[i] = tmp_sum2;
    }
    float errs[m][m];
    for (int i=0; i<m;i++ ){ 
        for (int j=0; j<m;j++ ){
            errs[i][j]=0;
        }
    }
    for (int i=0; i<m;i++ ){ 
      for (int j=0; j<=i;j++ ){ 
        if (j==0){
          errs[j][i]=err2[i]-(err[i]*err[i]/(less_x_count[i]));
        }
        else{
          errs[j][i]=err2[i]-err2[j-1]-((err[i]-err[j-1])*(err[i]-err[j-1])/(less_x_count[i]-less_x_count[j-1]));
        }
      }
    }
    for (int i=0; i<m;i++ ){ 
        for (int j=0; j<m;j++ ){
          cout << errs[i][j] << " " ;
        }
        cout << "\n" ;     
    }

    float M[m+1];
    M[0]=0;
    int seg[m];
    for (int i=0; i<m;i++){
      float min_ij = errs[0][i]+p;
      int min_index = 0;
      for (int j=1; j<=i; j++){
        float tmp_err = errs[j][i]+p+M[j];
        if (min_ij>tmp_err){
          min_ij=tmp_err;
          //seg[i][j] = 1;
          min_index = j;
        }
      }
      seg[i]=min_index;
      M[i+1]=min_ij;
    }
    for (int i=0; i<m+1;i++){
      cout << "M " << M[i] << endl;
    }
    for (int i=0; i<m;i++){
        cout << " " << seg[i] ;
    }

  return 0;
}
int main(int argc, char* argv[])
{
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    std::random_device dev;
    std::mt19937 rng(dev());                                 
    int m=atoi(argv[1]);
    float penalty=atof(argv[2]);
    float X[m][2];
    int x[m];
    float y[m];
    m=9;
   
    X[0][0]=2; X[1][0]=1; X[2][0]=3; X[3][0]=5;
    X[4][0]=5; X[5][0]=5; X[6][0]=3; X[7][0]=1; X[8][0]=3;
    X[0][1]=0.2;X[1][1]=0.3;X[2][1]=0.5;X[3][1]=0.7;X[4][1]=0.9;X[5][1]=0.5;X[6][1]=0.7;X[7][1]=0.9;X[8][1]=0.8;
    std::qsort(X, m, sizeof(*X),
    [](const void *arg1, const void *arg2)->int
    {
        int const *lhs = static_cast<int const*>(arg1);
        int const *rhs = static_cast<int const*>(arg2);
        return (lhs[0] < rhs[0]) ? -1
            :  ((rhs[0] < lhs[0]) ? 1
            :  (lhs[1] < rhs[1] ? -1
            :  ((rhs[1] < lhs[1] ? 1 : 0))));
    });   
    for (int i=0; i<m;i++){
      x[i]=int(X[i][0]);
      y[i]= X[i][1];
//      cout <<"X: " << int(X[i][0]) << ", Y: " << X[i][1] << endl;
    } 
    auto start = high_resolution_clock::now();
//    int value=getValue();
//    cout << value << endl;
    Icba(x,y,m,penalty);
//    int values=getValue();
//    cout << values << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "time: " << duration.count() << endl;

 
    return 0;
}
