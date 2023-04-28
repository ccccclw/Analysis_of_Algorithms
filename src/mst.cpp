//--------------------------------------------------------------------
// Prim's algorithm for minimum spanning tree
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


class Edge {
  public:
  int src, dest, weight;
};

class Graph
{
  vector<int>* adj; 
  bool cyclefinder(int v, bool visited[],int parent, stack<int>*);
public:
  int V, E;
  vector<int> *weights;
  Graph(int V, int E);
  Edge* edge;
  void addEdge(int v, int w, int weight);
  void rmEdge(int v, int w);
  void PrintCycle(int v, stack<int> s, vector<int>*, vector<int>*); 
  bool cyclic();
};

Graph::Graph(int V,int E)
{
  this->V = V;    
  this->E = E;
  adj = new vector<int>[V];
  weights = new vector<int>[V];
  for (int i=0; i<V;i++){
    for (int j=0; j<V;j++){
      weights[i].push_back(0);
    }
  }
}

void Graph::addEdge(int v, int w, int weight)
{
  if (!(std::find(adj[w].begin(), adj[w].end(), v) != adj[w].end())) {
    // Add w to v’s adj.
    adj[v].push_back(w);
    weights[v][w]=weight; 
    // Add v to w’s adj.
    adj[w].push_back(v);
    weights[w][v]=weight; 
    ++E;
  }
}

void Graph::rmEdge(int v, int w)
{
  adj[v].erase(std::remove(adj[v].begin(), adj[v].end(), w), adj[v].end());
  adj[w].erase(std::remove(adj[w].begin(), adj[w].end(), v), adj[w].end());
}

bool Graph::cyclefinder(int v, bool visited[], 
                        int parent, stack<int>* cycle_stack)
{
  visited[v] = true;
  for (int p=0; p < adj[v].size(); ++p)
  {
    int i = adj[v][p];
    if (!visited[i])
    {
      (*cycle_stack).push(i);
      if (cyclefinder(i, visited, v,cycle_stack))
      { 
        return true;
      }
    }

    else if (i != parent)
    {
      (*cycle_stack).push(i);
      return true;
    }
  }
  return false;
}


void Graph::PrintCycle(int V, stack<int> s, vector<int>* result, 
                       vector<int>* remove_edge)
{                          
    int cycle_start = s.top();
    Edge result_edge[V];   
    s.pop();               
    vector<int> cycle_list;
    int cycle_end = s.top();     
    stack<int> temp; 
    vector<int> tmp_result;    
    while (s.empty() == false){
      cycle_list.push_back(s.top());
      s.pop();
    }
    
    (*result).push_back(cycle_end);

    for (int i =0; i< cycle_list.size()-1; ++i){
      if (cycle_list[i+1]==cycle_start){
        break;
      }
      else if (weights[cycle_end][cycle_list[i+1]]!=0){ 
        (*result).push_back(cycle_list[i+1]);
        cycle_end = cycle_list[i+1];
      }
    }
    (*result).push_back(cycle_start);  

    int costly_edge_index=0;
    int first_weight=weights[(*result)[0]][(*result)[(*result).size()-1]];
    for (int i =0; i< (*result).size()-1; ++i){
      if ((weights[(*result)[i]][(*result)[i+1]]-first_weight)>1){
        costly_edge_index=i+1;
        first_weight=weights[(*result)[i]][(*result)[i+1]];
      }
    }
    if (costly_edge_index==0){
      (*remove_edge)[0]=(*result)[0];
      (*remove_edge)[1]=(*result)[(*result).size()-1];
    }
    else{
      (*remove_edge)[0]=(*result)[costly_edge_index-1];
      (*remove_edge)[1]=(*result)[costly_edge_index];
    }
    cout << "remove edge: " << (*remove_edge)[0] << " " << (*remove_edge)[1] << " with weights: " << weights[(*remove_edge)[0]][(*remove_edge)[1]] << endl; 
    cout << "dest, src " << (*result)[0] << " " << (*result)[(*result).size()-1] << endl;
}                          


bool Graph::cyclic()     
{ 
  cout << "Adjacency list" << endl;
  list<int>::const_iterator  it;   
  for (int t = 0; t < V; ++t){
    cout  << t << " ";
    for (int it = 0; it < adj[t].size(); ++it){
      cout << adj[t][it] << " ";  
    }                      
    cout << "\n";          
  }                        
  
  int all_e=E;
  int num_cycle=0;
  while (all_e > V-1){
    vector<int> *remove_edge = new vector<int>(2);
    vector<int> *result = new vector<int>;
    bool *visited = new bool[V];   
    for (int i = 0; i < V; i++)    
        visited[i] = false;          
    for (int u = 0; u < V; u++)
    {                        
      stack<int> *cycle_stack = new stack<int>;
      (*cycle_stack).push(u);
      if (!visited[u])       
      {                      
        if (cyclefinder(u, visited, -1,cycle_stack))
        {                    
          PrintCycle(V, *cycle_stack,result,remove_edge);
          rmEdge((*remove_edge)[0],(*remove_edge)[1]);
          --all_e;
          cout << "\n"  << endl;
          for (int k =0 ; k < (*result).size(); ++k){
            cout << (*result)[k] << " " ;
          }     
          cout << "\n"  << endl;
          ++num_cycle;
          cout << "--num_cycles: " << num_cycle << endl; 
          break;
        }       
      }         
    }    
  }    
  cout << "-------------------num_cycles: " << num_cycle << endl; 
  int check_weight=0;
  for (int i = 0; i < V; i++){
    if (adj[i].size() == 0){
      check_weight+=1;
    }
  }
  if (check_weight==0){   
    return true;
  }
  else {
    return false;
  }
}

int main(int argc, char* argv[])
{
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_real_distribution<> dist(0.0, 1.0);
  std::random_device dev;   
  std::mt19937 rng(dev());  
  int v = atoi(argv[1]);
  Graph g1(v,0);
  vector<int> all_v;
  vector<int> all_v_cp;
  vector<int> init_visited;
  bool visited_cp[v];
  for (int i=0; i<v; i++){
    all_v.push_back(i);
    all_v_cp.push_back(i);
    visited_cp[i]=false;
  }
  std::uniform_int_distribution<std::mt19937::result_type> dist2(1,30);
  std::uniform_int_distribution<std::mt19937::result_type> dist3(1,9);
  int extra_edge = dist3(rng);                                         
  std::uniform_int_distribution<std::mt19937::result_type> dist1(0,all_v.size()-1);
  int unvisited_index_0 = dist1(rng);
  int unvisited_0 = all_v[unvisited_index_0];
  all_v.erase(all_v.begin()+unvisited_index_0);
  init_visited.push_back(unvisited_0);
  std::uniform_int_distribution<std::mt19937::result_type> dist11(0,all_v.size()-1);
  int unvisited_index_1 = dist11(rng); 
  int unvisited_1 = all_v[unvisited_index_1];
  all_v.erase(all_v.begin()+unvisited_index_1); 
  int weight_0 = dist2(rng);
  g1.addEdge(unvisited_0, unvisited_1, weight_0);
  init_visited.push_back(unvisited_1);
  while (all_v.size()>0){
    std::uniform_int_distribution<std::mt19937::result_type> dist11(0,all_v.size()-1);
    int unvisited_index_2 = dist11(rng);
    int unvisited_2 = all_v[unvisited_index_2];
    std::uniform_int_distribution<std::mt19937::result_type> dist111(0,init_visited.size()-1);
    all_v.erase(all_v.begin()+unvisited_index_2);
    int unvisited_3 = init_visited[dist111(rng)];
    int weight_1 = dist2(rng);
    g1.addEdge(unvisited_2, unvisited_3, weight_1);
    init_visited.push_back(unvisited_2);   
  }
  int extra_count=0;
  while (extra_count<extra_edge){
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,init_visited.size()-1);
    int unvisited_index_4 = dist1(rng);
    int unvisited_4 = init_visited[unvisited_index_4];
    std::uniform_int_distribution<std::mt19937::result_type> dist11(0,init_visited.size()-1);
    int unvisited_index_5 = dist11(rng);  
    int unvisited_5 = init_visited[unvisited_index_5];
    if (unvisited_4!=unvisited_5 && g1.weights[unvisited_4][unvisited_5]==0 ){
      int weight_2 = dist2(rng);
      g1.addEdge(unvisited_4, unvisited_5, weight_2);
      ++extra_count;
    }
  }   
  cout << "vertices, Edges: " << g1.V << " " << g1.E << endl;
  
  
  auto start = high_resolution_clock::now();
  bool connected = g1.cyclic();
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "#nodes" << " " << "time" << endl;
  cout << v << " " << duration.count() << endl;
  
  return 0;
}
