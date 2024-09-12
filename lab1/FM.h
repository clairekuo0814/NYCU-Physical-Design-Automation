#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
using namespace std;

void computeGainInitial();
void ansInitial(int& cutSize, int &ansPart0Amount, int &ansPart1Amount, int part0Amount, int part1Amount);
bool checkBalance(int part0Amount, int part1Amount, int balNum1, int &balNum2, int moveNode);
int moveNode(int &part0Amount, int &part1Amount, int balNum1, int balNum2);
void renewGain(int moveNodeNum, int& cutSize, const int maxgain);
void updateAns(vector<bool>& initialPartition, int bestMove, vector<int> moveList);
void FM(int part0Amount, int part1Amount,  int& ansPart0Amount, int& ansPart1Amount, int& cutSizeAns, vector<bool>& initialPartition);
void reset(vector<bool>& initialPartition);


class node{
    private:
        int node_num;
        string node_name;
        vector<int> net;
        int* gain;
    public:
        node();
        node(int n, string name, vector<int>& s):node_num(n), node_name(name), part(-1), net(s), gain(NULL), pre(NULL), next(NULL), lock(0){};
        int getNum(){return node_num;}
        string getName(){return node_name;}
        int getPart(){return part;}
        vector<int>& getNet(){return net;}
        void renewNet(vector<int> n){net = n;}
        int* getGain(){return gain;}
        void initialGain(int* maxGain){gain = maxGain ;}
        void initialPart(int p){part = p;}
        void changePart(){part = (part == 1) ? 0 : 1 ;}
        node** getPre(){return &pre;}
        node** getNext(){return &next;}
        bool lock;
        int part;
        node* pre, *next;
};

struct Net{
    vector<int> NodeList;
    int amount[2];
};