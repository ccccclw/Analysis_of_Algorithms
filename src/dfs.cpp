//--------------------------------------------------------------------
//Find the cycle in a graph using Depth First Search
//--------------------------------------------------------------------

#include <list>                
#include <limits.h>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <random>

using namespace std;
using namespace std::chrono;

class Graph
{
  int V;
  vector<int>* adj;

  bool cyclefinder(int v, bool visited[],
              int parent, stack<int>*);
public:

  Graph(int V);
  void addEdge(int v, int w);
  void PrintCycle(int v, stack<int> s, vector<int>*); 
  bool cyclic();
};

Graph::Graph(int V)
{
  this->V = V;
  adj = new vector<int>[V];  
}

void Graph::addEdge(int v, int w)
{
  // Add w to v’s adj.
  adj[v].push_back(w);
  // Add v to w’s adj.
  adj[w].push_back(v);
}

bool Graph::cyclefinder(int v, bool visited[], int parent, stack<int>* cycle_stack)
{
  visited[v] = true;
  for (int p = 0; p<adj[v].size(); ++p)
  {
    int i = adj[v][p];
    if (!visited[i])
    {
      (*cycle_stack).push(i) ;
      if (cyclefinder(i, visited, v,cycle_stack))
      { 
        return true;
      }
    }

    else if (i != parent)
    {
      (*cycle_stack).push(i) ;
      return true;
    }
  }
  return false;
}
            


void Graph::PrintCycle(int V, stack<int> s, vector<int>* result)
{
    int cycle_start = s.top();
    s.pop();
    vector<int> cycle_list;
    int cycle_end = s.top();
    stack<int> temp;
    vector<int> tmp_result;
    while (s.empty() == false){
      cycle_list.push_back(s.top());
      s.pop();
    }
    for (int i =0; i< cycle_list.size(); ++i){
      cout  << cycle_list[i] << " ";
    } 
    cout  << "\n";
    (*result).push_back(cycle_end);
    for (int i =0; i< cycle_list.size()-1; ++i){
      if (cycle_list[i+1]==cycle_start){
        break;
      }
      else if (std::find(adj[cycle_end].begin(), adj[cycle_end].end(),cycle_list[i+1] ) != adj[cycle_end].end()) {
        (*result).push_back(cycle_list[i+1]);
        cycle_end = cycle_list[i+1];                                                                                             
      }
    }
    (*result).push_back(cycle_start);
}        

bool Graph::cyclic()
{
  vector<int> *result = new vector<int>; 
  bool *visited = new bool[V];
  list<int>::iterator it;  
  for (int i = 0; i < V; i++)
    visited[i] = false;
  cout << "Adjacency lists" << endl;
  for (int t = 0; t < V; t++){
    cout << t << " ";
    for (int it = 0; it < adj[t].size(); it++){
      cout << adj[t][it] << " ";
    }
    cout << "\n";
  }
  for (int u = 0; u < V; u++)
  {
    stack<int> *cycle_stack = new stack<int>;
    if (!visited[u])
    { 
      if (cyclefinder(u, visited, -1,cycle_stack))
      {
        cout << "Cycle finding route: " << endl;
        PrintCycle(V, *cycle_stack,result);
        cout << "The cycle: " << endl;
        for (int i =0 ; i < (*result).size(); ++i){
          cout << (*result)[i] << " " ;
        }
        cout << "\n";
        return true;
      }
    }
  }
  return false;
}

int main(int argc, char* argv[])
{
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    int v = atoi(argv[1]); 
    Graph g1(v);
    int count=0;
    double p = 1/(double)v;
    for (int i=0; i < v; ++i){
        for (int j=i+1; j < v; ++j) {
            if (dist(engine) <= p) {
                g1.addEdge(i, j);
                ++count;
            }
        }
    }
    auto start = high_resolution_clock::now();
    bool connected = g1.cyclic();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    if (connected) { cout << "-----Graph contains cycle\n";}
    else {cout << "-----Graph doesn't contains cycle\n";}
    cout << "connected" <<  " " << "#edges" << " " << "#nodes" << " " << "time" << endl;
    cout << connected << " " << count << " " << v << " " << duration.count() << endl;
  return 0;
}
