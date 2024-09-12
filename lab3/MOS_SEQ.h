#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<cmath>


using namespace std;
const double GAYE_ACTIVE_EX_2 = 25;
const double GATE_W_1 = 20;
const double GATE_S_2 = 34;
const double ACTIVE_S_1 = 27;


class MOS{
    public:
        MOS(): choosed(0), rotate(0), prevSame(0){};
        string name;
        string D;
        string G;
        string S;
        bool rotate;
        bool choosed;
        bool prevSame;
};

struct gatePair{
    int num;
    string gateName;
    int NMOS;
    int PMOS;
    //bool Dummy;
    int prev;
    int next;
    bool choosed;
};

struct bestSol{
    int firstGate;
    double totalHPWL;
    vector<gatePair> gateSeq;
    vector<MOS> NMOS;
    vector<MOS> PMOS;
    vector<bool> Dummy;
};


class Position{
    public:
        Position(): left(-1), right(-1){};
        //Position(int u, int d, int l, int r) : up(u), down(d), left(l), right(r){};
        //double up;
        //double down;
        double left;
        double right;
        vector<int> NMOS;
        vector<int> PMOS;
};


class MOS_group{
    public : 
        MOS_group(): nWidth(-1), pWidth(-1), length(-1), firstGate(-1), totalHPWL(-1){};
        vector<MOS> NMOS;
        vector<MOS> PMOS;
        //GATE_CLASSIFY GATE_classify;
        double nWidth;
        double pWidth;
        double length;
        map<string, pair<vector<int>, vector<int>>> AllGate;
        map<string, Position> AllSD;
        vector<gatePair> gateSeq;
        int firstGate;
        double totalHPWL;
        vector<bool> Dummy;

};

void readFile(MOS_group &MOS_G, ifstream& FILEIN);
void insertSeq(MOS_group &MOS_G, int pairNum, int pairBehind);
void rotateJudge(MOS* preMOS, MOS* curMOS);
void rotateAll(MOS_group &MOS_G);
void initialSeq(MOS_group &MOS_G);
void insertDummy(MOS_group &MOS_G);
void updateSDPos(MOS_group &MOS_G, map <string, Position> :: iterator it_SD, double curX, bool type);
void XYCalInitial(MOS_group &MOS_G);
void CostCal(MOS_group &MOS_G);
void swap(MOS_group &MOS_G);
void SA(MOS_group &MOS_G,bestSol &best);
void output(bestSol &best, ofstream& FILEOUT);