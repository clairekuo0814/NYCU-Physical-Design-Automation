#include "channel_routing.h"

int main(int argc, char **argv){
    //test time
    //const clock_t start_time = clock();
    //double run_time;

    ifstream FILEIN;
    CH_route chanRoute;
    FILEIN.open(argv[1]);
    readInputFile(FILEIN, chanRoute);

    int trackSum;
    trackSum = LeftEdgeAlgo(chanRoute);
    
    ofstream FILEOUT;
    FILEOUT.open(argv[2]);
    output(chanRoute, FILEOUT, trackSum);
    //test
    //run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    //cout << "CPU Run time: " << run_time << " seconds" << endl;

    return 0;
}