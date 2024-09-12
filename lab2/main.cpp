#include "BST.h"

//vector<block> allBlock;
double ratioLow, ratioHigh;
int rootBlock = -1;


int main(int argc, char** argv){
    //test time
    const clock_t start_time = clock();
    double run_time;

    srand(540);
    vector<block> allBlock;
    double totalArea;
    //read input
    ifstream FILEIN;
    FILEIN.open(argv[1]);
    readFille(FILEIN, allBlock, totalArea);
    vector<block> BlockNULL = allBlock;
    //*debug
    /*
    cout << ratioLow << " " << ratioHigh << endl;
    for(int i = 0; i < allBlock.size(); i++){
        cout << allBlock[i].name << " " << allBlock[i].width << " " << allBlock[i].height << endl; 
    }*/
    //
    int allBlockSize = allBlock.size();
    bestSol best;
    best.area = 0;
    best.ratio = 0;
    /*
    while(best.ratio < ratioLow || best.ratio > ratioHigh){
        allBlock = BlockNULL;
    }*/
    //initial tree
    initialBTree(allBlock, best);
    annealing(allBlock, best, totalArea);

    //output
    ofstream FILEOUT;
    FILEOUT.open(argv[2]);
    output(best, FILEOUT);

    
    ////debug
    //ofstream FILEOUT_DEBUG;
    //FILEOUT_DEBUG.open("debug.txt");
    //outputDebug(best, FILEOUT_DEBUG);

    //close file
    FILEIN.close();
    FILEOUT.close();

    ////test
    //run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    //cout << "CPU Run time: " << run_time << " seconds" << endl;

    return 0;

}
