#include "FM.h"

extern vector<Net> netAll;
extern vector<node*> nodeAll;
extern vector<node*> partGain0; 
extern vector<node*> partGain1;
extern int balNum1, balNum2;
extern int maxgain;

void erase_node(node* Node){
    vector<node*>* partGain;
    if(Node->getPart()){
        partGain = &partGain1;
    }else{
        partGain = &partGain0;
    }
    int gain = *Node->getGain();
    if(*Node->getPre() == NULL)
        (*partGain)[gain] = *Node -> getNext();
    else
        *(*Node->getPre())->getNext() = *Node->getNext();
    if(*Node->getNext() != NULL)
        *(*Node->getNext())->getPre() = *Node->getPre();
    *Node->getPre() = NULL;
    *Node->getNext() = NULL;
}

void add_node(node* Node){
    vector<node*>* partGain;
    vector<node*>& partGaintest = *partGain;
    int testpart = Node->getPart();
    if(Node->getPart()){
        partGain = &partGain1;
    }else{
        partGain = &partGain0;
    }
    int gain = *Node->getGain();
    node *tmp = (*partGain)[gain];
    (*partGain)[gain] = Node;
    *Node->getPre() = NULL;
    *Node->getNext() = tmp;
    if(tmp != NULL)
        *tmp->getPre() = (*partGain)[gain];
}


//calculate initial gain
void computeGainInitial(){
    //by net->node
    int netNum = netAll.size();
    int nodeNum = nodeAll.size();
    for(int i=0; i < netNum; i++){
        int mark0;
        int mark1;
        int part0 = 0;
        int part1 = 0;
        bool noGain = false;
        for(int j=0; j < netAll[i].NodeList.size(); j++){
            if(nodeAll[netAll[i].NodeList[j]]->getPart()){
                //save the node which gain may +1
                if(part1 == 0)
                    mark1 = netAll[i].NodeList[j];
                part1++;
            }
            else{
                if(part0 == 0)
                    mark0 = netAll[i].NodeList[j];
                part0++;
            }
            //if no larger then 2 in part0 and part1, gain is same
            if(part0 >= 2 && part1 >= 2){
                noGain = true;
                break;
            }
        }
        //if only one node in one part, then gain +1
        if(part0 == 1)
            (*nodeAll[mark0]->getGain())++;
        if(part1 == 1)
            (*nodeAll[mark1]->getGain())++;
        //all in same part, gain -1
        if(part0 == 0 || part1 == 0){
            for(int j=0; j < netAll[i].NodeList.size(); j++)
                (*nodeAll[netAll[i].NodeList[j]]->getGain())--;
        }
    }
    //save gain in gain vector of two part, to choose move node later
    for(int node_i = 0; node_i < nodeNum; node_i++){
        add_node(nodeAll[node_i]);
    }

}
//cutsize & ans initialize

void ansInitial(int& cutSize, int &ansPart0Amount, int &ansPart1Amount, int part0Amount, int part1Amount){
    int netNum = netAll.size();
    int nodeNum = nodeAll.size();
    int cutSizeNew = 0;
    for(int net_i=0; net_i < netNum; net_i++){
        bool previousPart = nodeAll[netAll[net_i].NodeList[0]]->getPart();
        for(int net_j=1; net_j < netAll[net_i].NodeList.size(); net_j++){
            if(nodeAll[netAll[net_i].NodeList[net_j]]->getPart() != previousPart){
                cutSizeNew++;
                break;
            }
            previousPart = nodeAll[netAll[net_i].NodeList[net_j]]->getPart();
        }
    }
    cutSize = cutSizeNew;
    ansPart0Amount = part0Amount;
    ansPart1Amount = part1Amount;
}


//check if it will unbalance after move
bool checkBalance(int part0Amount, int part1Amount, int balNum1, int &balNum2, int moveNode){
    int changePart = nodeAll[moveNode]->getPart();
    int part0AmountTemp = part0Amount;
    int part1AmountTemp = part1Amount;
    bool checkBalance = 1;
    if(changePart){
        part0AmountTemp++;
        part1AmountTemp--;
    }else{
        part1AmountTemp++;
        part0AmountTemp--;
    }
    if(part0AmountTemp > balNum2 || part0AmountTemp < balNum1
    || part1AmountTemp > balNum2 || part1AmountTemp < balNum1)
        checkBalance = 0;
    return checkBalance;
}


//move choosen node to another part
int moveNode(int &part0Amount, int &part1Amount, int balNum1, int balNum2){
    //choose node to move
    int moveGain = 0;
    int moveNodeNum = -1;
    for(int gain_i = partGain0.size()-1; gain_i >= 0; gain_i--){
        if(partGain0[gain_i] != NULL){
            if(checkBalance(part0Amount, part1Amount, balNum1, balNum2, partGain0[gain_i]->getNum())){
                moveGain = gain_i;
                moveNodeNum = partGain0[gain_i]->getNum();
            }
            break;
        }
    }
    for(int gain_i = partGain1.size()-1; gain_i >= 0; gain_i--){
        if(partGain1[gain_i] != NULL){
            if(gain_i > moveGain){
                if(checkBalance(part0Amount, part1Amount, balNum1, balNum2, partGain1[gain_i]->getNum())){
                    moveGain = gain_i;
                    moveNodeNum = partGain1[gain_i]->getNum();
                }
                break;
            }
        }
    }
    //delete the move node in partgain
    if(moveNodeNum != -1){
        if(nodeAll[moveNodeNum]->getPart()){
            part0Amount++;
            part1Amount--;
        }
        else{
            part1Amount++;
            part0Amount--;
        }
    }
    
    return moveNodeNum;
}

//update gain of node connected to move node according to diff case
void renewGain(int moveNodeNum, int& cutSize, const int maxgain){
    int addgain = *nodeAll[moveNodeNum]->getGain();
    bool part_F = nodeAll[moveNodeNum]->getPart();
    bool part_T = !part_F;
    //delete from partGain
    cutSize = cutSize - addgain + maxgain;
    erase_node(nodeAll[moveNodeNum]);
    //moveNode lock
    nodeAll[moveNodeNum]->lock = 1;

    for(int net_i=0; net_i < nodeAll[moveNodeNum]->getNet().size(); net_i++){
        int netNum = nodeAll[moveNodeNum]->getNet()[net_i];
        if(netAll[netNum].amount[part_T] == 0){
            for(int node_j=0; node_j < netAll[netNum].NodeList.size(); node_j++){
                if(nodeAll[netAll[netNum].NodeList[node_j]]->lock != 1){
                    erase_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                    (*nodeAll[netAll[netNum].NodeList[node_j]]->getGain())++;
                    add_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                }
            }
        }else if(netAll[netNum].amount[part_T] == 1){
            for(int node_j=0; node_j < netAll[netNum].NodeList.size(); node_j++){
                if(nodeAll[netAll[netNum].NodeList[node_j]]->getPart() == part_T){
                    if(nodeAll[netAll[netNum].NodeList[node_j]]->lock != 1){
                        erase_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                        (*nodeAll[netAll[netNum].NodeList[node_j]]->getGain())--;
                        add_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                    }
                    break;
                }
            }
        }
        netAll[netNum].amount[part_T]++;
        netAll[netNum].amount[part_F]--;
        //change part to moveNode

        if(netAll[netNum].amount[part_F] == 0){
            for(int node_j=0; node_j < netAll[netNum].NodeList.size(); node_j++){
                if(nodeAll[netAll[netNum].NodeList[node_j]]->lock != 1){
                    erase_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                    (*nodeAll[netAll[netNum].NodeList[node_j]]->getGain())--;
                    add_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                }
            }
        }
        else if(netAll[netNum].amount[part_F] == 1){
            for(int node_j=0; node_j < netAll[netNum].NodeList.size(); node_j++){
                if(nodeAll[netAll[netNum].NodeList[node_j]]->getPart() == part_F && nodeAll[netAll[netNum].NodeList[node_j]]->getNum() != moveNodeNum){
                    if(nodeAll[netAll[netNum].NodeList[node_j]]->lock != 1){
                        erase_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                        (*nodeAll[netAll[netNum].NodeList[node_j]]->getGain())++;
                        add_node(nodeAll[netAll[netNum].NodeList[node_j]]);
                    }
                    break;
                }

            }
        }

    }
    nodeAll[moveNodeNum]->changePart();
    
}

void FM(int part0Amount, int part1Amount,  int& ansPart0Amount, int& ansPart1Amount, int& cutSizeAns, vector<bool>& initialPartition){
    computeGainInitial();
    int netAmount = netAll.size();
    int nodeAmount = nodeAll.size();
    int cutSize = netAmount;
    int moveNodeNum = -1;
    vector<int> moveList(nodeAmount);
    ansInitial(cutSize, ansPart0Amount,ansPart1Amount, part0Amount, part1Amount);
    int bestMove = -1;
    if(cutSize < cutSizeAns){
        cutSizeAns = cutSize;
        ansPart0Amount = part0Amount;
        ansPart1Amount = part1Amount;
    }
    for(int turn_i = 0; turn_i < nodeAmount; turn_i++){
        int cutSizeNew;
        moveNodeNum = moveNode(part0Amount, part1Amount, balNum1, balNum2);
        if(moveNodeNum == -1 )
            break;
        else
            moveList[turn_i] = moveNodeNum;
        renewGain(moveNodeNum, cutSize, maxgain);
        if(cutSize < cutSizeAns){
            bestMove = turn_i;
            cutSizeAns = cutSize;
            ansPart0Amount = part0Amount;
            ansPart1Amount = part1Amount;
        }
    }
    if(bestMove != -1)
        updateAns(initialPartition, bestMove, moveList);
    reset(initialPartition);
}

void reset(vector<bool>& initialPartition){
    for(int node_i = 0; node_i < nodeAll.size(); node_i++){
        nodeAll[node_i]->part = initialPartition[node_i];
        nodeAll[node_i]->lock = 0;
        nodeAll[node_i]->pre = NULL;
        nodeAll[node_i]->next = NULL;
        *(nodeAll[node_i]->getGain()) = maxgain;
    }
    for(int gain_i = 0; gain_i < partGain0.size(); gain_i++){
        partGain0[gain_i] = NULL;
        partGain1[gain_i] = NULL;
    }
    for(int net_i = 0; net_i < netAll.size(); net_i++){
        netAll[net_i].amount[0] = 0;
        netAll[net_i].amount[1] = 0;
        for(int node_j = 0; node_j < netAll[net_i].NodeList.size(); node_j++){
            if(nodeAll[netAll[net_i].NodeList[node_j]]->part){
                netAll[net_i].amount[1]++;
            }else {
                netAll[net_i].amount[0]++;
            }
        }
    }
}
void updateAns(vector<bool>& initialPartition, int bestMove, vector<int> moveList){
    for(int move_i=0; move_i <= bestMove; move_i++){
        initialPartition[moveList[move_i]] = !initialPartition[moveList[move_i]];
    }
}
