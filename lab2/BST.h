#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<cmath>

using namespace std;
struct block{
    string name;
    int width;
    int height;
    bool rotate;
    block* parent;
    block* left;
    block* right;
    int x;
    int y;
    int num;
};

struct bestSol{
    vector<block> allBlock;
    int area;
    double ratio;
};

void readFille(ifstream &FILEIN, vector<block>& allBlock, double& totalArea);
void initialBTree(vector<block>& allBlock, bestSol& best);
void xyCal(vector<block>& allBlock, block &block, vector<int> & contour, bool isLeft);
void BTreeFloorPlan(vector<block>& allBlock);
void costCal(const vector<block> allBlock, int& area, double& ratio);
void rotate(vector<block>& allBlock);
void swap(vector<block>& allBlock, int fromNode, int toNode);
void move(vector<block>& allBlock);
void annealing(vector<block>& allBlock, bestSol & best, double& totalArea);
void output(bestSol best, ofstream & FILEOUT);
void outputDebug(bestSol best, ofstream & FILEOUT_DEBUG);