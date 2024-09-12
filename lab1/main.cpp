
#include "FM.h"

vector<Net> netAll;
vector<node*> nodeAll;
vector<node*> partGain0; 
vector<node*> partGain1;
int balNum1, balNum2;
int maxgain;


int main(int argc, char** argv){
    //const clock_t start_time = clock();
    //double run_time;

    //int nodeNum, netNum;
    float balFactor;
    ifstream FILEIN;
    FILEIN.open(argv[1]);
    FILEIN >> balFactor;


    vector<vector<int>> nodeNet;
    string ss;
    map<string, int> nodeMap;
    int netAmount =0;
    int nodeAmount = 0;
    //input
    while(FILEIN >> ss){
        if(ss == "NET"){
            FILEIN >> ss;
            FILEIN >> ss;
            Net net;
            while(ss != ";"){
                map<string, int>::iterator iter;//save node name
                iter = nodeMap.find(ss);
                //if node exist in map 
                if(iter != nodeMap.end()){
                    int nodeNum = nodeMap[ss];//find node index
                    vector<int> newNodeNet = (nodeAll[nodeNum]->getNet());
                    bool same = 0;
                    for(int i=0; i<newNodeNet.size(); i++){
                        if( newNodeNet[i] ==  netAmount)
                            same = 1;
                    }
                    if(same == 1) goto readData;
                    net.NodeList.push_back(nodeNum);//save node in net
                    //node in which net
                    nodeNet[nodeNum].push_back(netAmount);
                    nodeAll[nodeNum]->renewNet(nodeNet[nodeNum]);
                }
                //if node not exist
                else{
                    nodeMap.insert(pair<string, int>(ss, nodeAmount));//add node into node map
                    net.NodeList.push_back(nodeAmount);//save node in net
                    nodeNet.resize(nodeAmount + 1);
                    nodeNet[nodeAmount].push_back(netAmount);//node in which net
                    //create new node
                    node* nodetemp = new node(nodeAmount, ss, nodeNet[nodeAmount]);
                    nodeAll.push_back(nodetemp);
                    nodeAmount++;
                }
                readData:
                FILEIN >> ss;
                net.amount[0] = 0;
            }
            net.amount[0] = 0;
            net.amount[1] = 0;
            netAll.push_back(net);
            netAmount++;
        }
    }
    //compute balance number
    balNum1 = ceil(float((float)(nodeAmount * 0.5) - (int)(balFactor * nodeAmount * 0.5)));
    balNum2 = floor( nodeAmount * (1 + balFactor) / 2);/*改*/
    //calculate max gain
    maxgain = 0;
    for(int i = 0; i < nodeAmount; i++){
        if(nodeNet[i].size() > maxgain)
            maxgain = nodeNet[i].size();
    }
    for(int i = 0; i < nodeAmount; i++){
        int* gain = new int;
        *gain =  maxgain;
        nodeAll[i]->initialGain(gain);
    }
    //let node on same net together, seperate to 2 part
    int nodeCnt = 0;
    vector<bool> initialPartition(nodeAmount);
    for(int net_i=0; net_i < netAll.size(); net_i++){
        for(int node_j=0; node_j < netAll[net_i].NodeList.size(); node_j++){
            if(nodeAll[netAll[net_i].NodeList[node_j]]->getPart() == -1){
                if(nodeCnt < balNum1){
                    nodeAll[netAll[net_i].NodeList[node_j]]->initialPart(0);
                    nodeCnt++;
                    initialPartition[netAll[net_i].NodeList[node_j]] = 0;
                    netAll[net_i].amount[0]++;
                }else {
                    nodeAll[netAll[net_i].NodeList[node_j]]->initialPart(1);
                    nodeCnt++;
                    initialPartition[netAll[net_i].NodeList[node_j]] = 1;
                    netAll[net_i].amount[1]++;
                }
                //if(nodeCnt == nodeAmount) break;
            }else{
                netAll[net_i].amount[nodeAll[netAll[net_i].NodeList[node_j]]->getPart()]++;
            }

        }
        
        //if(nodeCnt == nodeAmount) break;
    }



    int part0Amount = balNum1;
    int part1Amount = nodeAmount - balNum1;
    int ansPart0Amount = part0Amount;
    int ansPart1Amount = part1Amount;
    
    /*revise*/
    partGain0.resize(2*maxgain + 1);
    partGain1.resize(2*maxgain + 1);
    int cutSizeAns = netAmount;
    for(int i = 0; i < 14; i++){
        FM(part0Amount, part1Amount, ansPart0Amount, ansPart1Amount, cutSizeAns, initialPartition);
        part0Amount = ansPart0Amount;
        part1Amount = ansPart1Amount;
    }



    //output
    ofstream OUTFILE(argv[2]);
    
    
    OUTFILE << "Cutsize = " << cutSizeAns << endl;
    OUTFILE << "G1 " << ansPart0Amount << endl;
    for(int node_i=0; node_i < nodeAmount; node_i++){
        if(initialPartition[node_i] == 0){
            OUTFILE << nodeAll[node_i]->getName() << " ";
        }
    }
    OUTFILE << ";" << endl;
    OUTFILE << "G2 " << ansPart1Amount << endl;
    for(int node_i=0; node_i < nodeAmount; node_i++){
        if(initialPartition[node_i] == 1){
            OUTFILE << nodeAll[node_i]->getName() << " ";
        }
    }
    OUTFILE << ";" << endl;

    //run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    //cout << "CPU Run time: " << run_time << " seconds" << endl;
    return 0;
}
