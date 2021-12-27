//
//  Maze.cpp
//  Maze
//
//  Created by Harrison Walls on 3/13/16.
//


#include "Maze.h"
#include <iostream>

using namespace std;

//Constructs inital maze with all walls intact
Maze::Maze() {
    myGraph = new DrawGraph(NUM_CELLS_PER_SIDE);
    
    srand (time(NULL));
    buildEquivClasses();							//initial set of classes
   // unionFind();								//condense classes - used to remove maze walls
}

//Maze destructor
Maze::~Maze(){
    delete myGraph;
}

//randomly select an east or south wall
int Maze::selectWall(int row,int col)
{
    //since walls are bidirectional we need only consider
    //east and south walls where east = 0 and south = 1.
    return -1;
}

//performs the union find algorithm - cells with adjacent walls are placed in the
//same equivalence class. This continues until a single class remains containing start
//and finish cells => maze has a path from start to finish.
void Maze::unionFind(int n)
{
    int connect = n;
    int *id = new int[n];
    int *size = new int[n];
    for(int i=0; i<n; i++)
    {
        id[i] = 1;
        size[i] = 1;
    }
    
}

//Constructs initial equivalence classes, each vertex in its own class
void Maze::buildEquivClasses()
{
    // have each class respresentative of true and false for the rest. Change the values
    // to true when walls have an edge removed. I am not sure how to implement this.
}

//Draws initial maze with no walls removed
void Maze::drawInitWalls()
{
    // horizontal lines
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (int i=0; i <= NUM_CELLS_PER_SIDE; i++){
        glVertex2d(OFFSET, i*CELL_WIDTH + OFFSET);
        glVertex2d(CELL_WIDTH*NUM_CELLS_PER_SIDE + OFFSET,i*CELL_WIDTH + OFFSET);
    }
    // vertical lines
    for (int i=0; i <= NUM_CELLS_PER_SIDE; i++){
        glVertex2d(i*CELL_WIDTH + OFFSET, OFFSET);
        glVertex2d(i*CELL_WIDTH + OFFSET,CELL_WIDTH*NUM_CELLS_PER_SIDE + OFFSET);
    
    }
    glEnd();
    
    //character start and finish
    glColor3f (0.0, 0.0, 0.0);
    glRasterPos2d(OFFSET - 10, NUM_CELLS_PER_SIDE*CELL_WIDTH-10 + OFFSET);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'S');
    glRasterPos2d(NUM_CELLS_PER_SIDE*CELL_WIDTH + OFFSET + 5,
                  5 + OFFSET);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'F');
    
    glutSwapBuffers();
    
}


//Draws  maze with  walls removed
void Maze::drawMaze()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 0.0, 0.0);
    
    //draw outer walls
    glBegin(GL_LINES);
    glVertex2d(OFFSET, OFFSET);
    glVertex2d(OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE, OFFSET);
    glVertex2d(OFFSET, OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE);
    glVertex2d(OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE, OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE);
    glVertex2d(OFFSET , OFFSET);
    glVertex2d(OFFSET, OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE);
    glVertex2d(OFFSET+ CELL_WIDTH * NUM_CELLS_PER_SIDE , OFFSET);
    glVertex2d(OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE, OFFSET + CELL_WIDTH * NUM_CELLS_PER_SIDE);
    
    //draw inner walls based on adjacency list
    vector<vector<int> > adjMatrix = myGraph->get();
    for (int i=0; i < adjMatrix.size(); i++) {
        
        vector<int> list = adjMatrix[i];
        for(int j = 0; j < list.size(); j++) {
            int vertex = list[j];
            int row = vertex / NUM_CELLS_PER_SIDE;
            int col = vertex % NUM_CELLS_PER_SIDE;
            
            if(vertex == (i + 1)) {//east wall removed, draw south wall
                if((row+1) % NUM_CELLS_PER_SIDE == 0) continue;
                
                int x = OFFSET + (col-1) * CELL_WIDTH;
                int y = W_HEIGHT - (OFFSET + (row+1) * CELL_WIDTH);
                int x2 = OFFSET + (col) * CELL_WIDTH;
                glVertex2d(x, y);
                glVertex2d(x2, y);
                
            }
            else {//south wall removed, draw east wall
                if((col+1) % NUM_CELLS_PER_SIDE == 0) continue;
                
                int x = OFFSET + (col+1) * CELL_WIDTH;
                int y = W_HEIGHT - (OFFSET + (row-1) * CELL_WIDTH);
                int y2 = W_HEIGHT - (OFFSET + (row) * CELL_WIDTH);
                glVertex2d(x, y);
                glVertex2d(x, y2);
                
            }
        }
    }
    
    glEnd();
    
    //character start and finish
    glColor3f (0.0, 0.0, 0.0);
    glRasterPos2d(OFFSET - 10, NUM_CELLS_PER_SIDE*CELL_WIDTH-10 + OFFSET);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'S');
    glRasterPos2d(NUM_CELLS_PER_SIDE*CELL_WIDTH + OFFSET + 5,
                  5 + OFFSET);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'F');
    
    glutSwapBuffers();

}

//Draws path thru maze from stack entries
void Maze::drawPath()
{
    //Clear drawing buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    //redraw maze
    drawMaze();
    
    //find path and place in depth first stack
    dfS = myGraph->depthFirstSearch(START_CELL, STOP_CELL);

    //blue boxes for path
    glColor3f (0.0, 0.0, 1.0);
    int row, col, cell;
    
    //since path in reverse order, easier to place stack
    // into a vector
    vector<int> temp;
    while(!dfS.empty()){
        cell = dfS.top();
        dfS.pop();
        temp.push_back(cell);
    }
    
    //for each cell in path
    for(int i = temp.size() - 1; i >= 0; i--){
        cell = temp[i];
        
        //find row/col for cell
        row = cell / NUM_CELLS_PER_SIDE;
        col = cell % NUM_CELLS_PER_SIDE;
        
        //draw a blue rectangle for cell in path
        glRectd(OFFSET + col * CELL_WIDTH + 2,
                W_HEIGHT - (OFFSET + row * CELL_WIDTH + 2),
                OFFSET + col * CELL_WIDTH + CELL_WIDTH - 2,
                W_HEIGHT - (OFFSET + row * CELL_WIDTH + CELL_WIDTH - 2));
        
    }
    
    //draw the new buffer contents
    glutSwapBuffers();
}



