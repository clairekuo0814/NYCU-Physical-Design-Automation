#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
//#include<cmath>

using namespace std;

class NET{
    public:
        int num;
        int min;
        int max;
        vector<int> topBound;
        vector<int> bottomBound;
        vector<int> VC;
        vector<int> VC_R;
        string track;
        NET():min(-1), max(-1), track("none"){};
        ~NET(){};
};

struct TrackGrid{
    string name;
    vector<bool> grid;
};

struct CH_route{
    vector<NET*> netAll;
    vector<int> top;
    vector<TrackGrid> topGrid;
    vector<int> bottom;
    vector<TrackGrid> botGrid;
};

bool myCompare(NET* a, NET* b);
void readInputFile(ifstream& FILEIN, CH_route &chanRoute);
void BuildTopBotTrack(CH_route &chanRoute);
int LeftEdgeAlgo(CH_route &chanRoute);
void output(const CH_route &chanRoute, ofstream& FILEOUT, int trackSum);