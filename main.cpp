//
//  main.cpp
//  TunnelProblem
//
//  Created by Vo Hung Son on 12/31/18.
//  Copyright © 2018 Vo Hung Son. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
# define INF 0x3f3f3f3f
using namespace std;
typedef pair<int, int> iPair;

class Graph{
private:
    int row, col, start, end, V, cost;//row & coloumn. 'v' is vertices
    int **input, *parent, *position;//#->0;'.'->1; vertex is from 2
    queue<int> vertices;//save the position of vertices.
    stack<int> shortest;
public:
    void inputFile(){
        ifstream file_in ("maze_input.txt");
        string line;
        if(file_in.is_open()){
            file_in >> line;
            row = getNumber(line);
            file_in >> line;
            col = getNumber(line);
            input = new int*[row];//initialize the 2D-matrix for from input
            for(int i = 0; i < row; i++) input[i] = new int[col];
            for(int i = 0; i < row; i++){
                file_in >> line;
                for(int j = 0; j < col; j++){
                    if(line.at(j)=='#') input[i][j] = 0;
                    else input[i][j] = 1;
                }
            }
            file_in >> line;
            start = getNumber(line);
            file_in >> line;
            end = getNumber(line);
            file_in.close();
        }
        else cout << "\nUnable to open the file! Please check it again!";
    }
    
    int getNumber(string x){
        int number = 0;
        for(int i = 0; i < x.length(); i++)
            number = number*10 + (int)x.at(i) - 48;
        return number;
    }
    
    void findVertices(){//find vertices in the input matrix
        V = 0;//initialize the number of vertices is 0
        for(int i = 0; i < row; i++){cout<<"\n";
            for(int j = 0; j < col; j++){
                int dot = 0;//count the '.'(1) around the Node
                if(i>0)     if(input[i-1][j]!=0) dot++; //check up
                if(i<row-1) if(input[i+1][j]!=0) dot++; //check down
                if(j>0)     if(input[i][j-1]!=0) dot++; //check left
                if(j<col-1) if(input[i][j+1]!=0) dot++; //check right
                if(dot!=2&&input[i][j]!=0) {//find out the vertex.
                    input[i][j] = 2 + V;//vertices are from number 2.
                    vertices.push(i*col + j);//save position of vertices
                    V++;//vertices++
                }
                if(input[i][j]<10) cout<<"  "<<input[i][j];
                else cout<<" "<<input[i][j];
            }
        }
        cout <<"\nThe number of vertices: "<<V;
    }
    
    void findWeight(){
        int s = 0;//get vertex s from vertices and go to other adjacency
        position = new int[V];
        vector<iPair > adj[V];
        while(!vertices.empty()){
            position[s] = vertices.front();
            int i = position[s]/col;
            int j = position[s] - i*col;
            int w = 0; //weight of the edge
            vertex(i,j,4,w,s,adj);
            s++;
            vertices.pop();
        }
        int begin = input[start/col][start-(start/col)*col]-2;//ex: 17 -> 1
        int stop = input[end/col][end-(end/col)*col]-2;//ex: 55 -> 8
        shortestPath(adj, begin, stop);
    }
    
    void vertex(int i,int j,int t,int w,int s, vector<pair<int,int>>adj[]){
        if(input[i][j]>1&&w!=0)addEdge(adj, s, input[i][j]-2, w);
        else{w++; //t-> turn: 0 -> left, 1 -> up, 2 -> right, 3 -> down
            if(j>0)    if(input[i][j-1]!=0&&t!=2) vertex(i,j-1,0,w,s,adj);
            if(i>0)    if(input[i-1][j]!=0&&t!=3) vertex(i-1,j,1,w,s,adj);
            if(j<col-1)if(input[i][j+1]!=0&&t!=0) vertex(i,j+1,2,w,s,adj);
            if(i<row-1)if(input[i+1][j]!=0&&t!=1) vertex(i+1,j,3,w,s,adj);
        }
    }
    
    void addEdge(vector <pair<int, int> > adj[], int u, int v, int wt){
        adj[u].push_back(make_pair(v, wt));
    }
    
    void shortestPath(vector<pair<int,int> > adj[], int begin, int stop){
        priority_queue< iPair, vector <iPair> , greater<iPair> > pq;
        vector<int> dist(V, INF);//vector with distances as infinite
        pq.push(make_pair(0, begin));
        dist[begin] = 0;
        parent = new int[V];
        parent[begin] = begin;
        while (!pq.empty()){//Looping till priority queue becomes empty
            int u = pq.top().second;//get vertex u from priority queue
            pq.pop();
            // Get all adjacent of u.
            for (auto x : adj[u]){
                int v = x.first;//vertex
                int weight = x.second;
                // If there is shorted path to v through u.
                if (dist[v] > dist[u] + weight){
                    dist[v] = dist[u] + weight;// Updating distance of v
                    parent[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
        cost = dist[stop];//cost of the path from start to end.
        while(parent[stop]!=begin){//save shortest path in the Queue
            stop = parent[stop];
            shortest.push(stop);
        }
        outputFile(adj);
    }
    
    void outputFile(vector<pair<int,int> > adj[]){
        cout <<"\n\nFile: diagram.txt";
        ofstream file_out1 ("diagram.txt");
        if(file_out1.is_open()){
            for(int u = 0; u < V; u++){
                cout<<"\n "<<position[u];
                file_out1 <<"\n "<<position[u];
                for(auto x: adj[u]){
                    cout<<" ("<<position[x.first]<<","<<x.second<<")";
                    file_out1<<" ("<<position[x.first]<<","<<x.second<<")";
                }
            }
            file_out1.close();
        }
        cout <<"\n\nFile: shortest_path.txt";
        ofstream file_out2 ("shortest_path.txt");
        if(file_out2.is_open()){
            file_out2 <<start;
            cout<<"\n"<<start;
            while(!shortest.empty()){
                file_out2 <<" "<<position[shortest.top()];
                cout <<" "<<position[shortest.top()];
                shortest.pop();
            }
            file_out2 <<" "<<end<<"\n"<<cost;
            cout <<" "<<end<<"\n"<<cost;
            file_out2.close();
        }
    }
};

int main(int argc, const char * argv[]) {
    Graph graph;
    graph.inputFile();
    graph.findVertices();
    graph.findWeight();
    cout << "\n";
    return 0;
}
