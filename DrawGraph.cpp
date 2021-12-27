//
//  DrawGraph.cpp
//  Maze
//
//  Created by Harrison Walls on 3/13/16.
//

#include "DrawGraph.h"
#include <iostream>

//Debug: dump graph for inspection
void DrawGraph::printGraph(){
    for(int j = 0; j < adjList.size(); j++){
        vector<int> temp = adjList[j];
        cout<< j << ": ";
        for(int i = 0; i < temp.size(); i++) {
            int val = temp[i];
            cout<< val << " ";
        }
        cout<<endl;
    }
}

//Construct an empty inital adjacency matrix
DrawGraph::DrawGraph(int n)
{
    int size;
    int **adj;
    if(n < 2)
    {
        size = 2;
    }
    else
    {
        size = n;
    }
    
    adj = new int*[size];            // Allocate memory for adjacency matrix
    
    for(int i=0; i<size; i++)
    {
        adj[i] = new int[size];    // Allocate memory for adjacency matrix
    }
    
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            adj[i][j] = 0;        // Initialize the vertices to 0
        }
    }
    
}

//Return a copy of the adjacency matrix
vector<vector<int> > DrawGraph:: get()
{
    return adjList;
}

//Add a graph edge (remove wall on a cell)
void DrawGraph::addEdge(int v1, int v2)
{
    adjList[v1].push_back(v2);
}

//Perform depth-first search of graph
stack<int> DrawGraph ::depthFirstSearch(int start, int end)
{
    int n;
    stack<int> S;
    bool* visited = new bool[n+1];
    
    for(int i=0; i<=n; ++i)
    {
        visited[i] = false;
    }
    
    S.push(start);
    visited[start] = true;
    
    if(start == end)
    {
        return S;
    }
    
    while(!S.empty())
    {
        int k = S.top();
        S.pop();
        
        if(k == end)
        {
            break;
        }
        
    }
    delete [] visited;
    
    return S;
}
