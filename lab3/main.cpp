#include "MOS_SEQ.h"

int main(int argc, char**argv){
    
    //test time
    //const clock_t start_time = clock();
    //double run_time;

    srand(40);
    //srand( time(NULL) );
    ifstream FILEIN;
    FILEIN.open(argv[1]);
    MOS_group MOS_G;
    bestSol best;
    best.totalHPWL = -1;
    readFile(MOS_G, FILEIN);
    SA(MOS_G, best);

    
    ofstream FILEOUT;
    FILEOUT.open(argv[2]);
    output(best, FILEOUT);

    
    //test
    //run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    //cout << "CPU Run time: " << run_time << " seconds" << endl;
    //cout << best.totalHPWL << endl;

    return 0;
}