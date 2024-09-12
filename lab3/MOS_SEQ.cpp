#include "MOS_SEQ.h"

void readFile(MOS_group &MOS_G, ifstream& FILEIN){
    string str_in;
    int erase_pos;
    getline(FILEIN, str_in);
    while(getline(FILEIN, str_in)){
        if(str_in != ".ENDS"){
            MOS MOS_in;
            string tmp;
            bool MOS_type;
            stringstream stream_in(str_in);
            stream_in >> tmp;
            erase_pos = tmp.find("M");
            tmp.erase(0, 1);
            MOS_in.name = tmp;
            stream_in >> MOS_in.D >> MOS_in.G >> MOS_in.S;
            stream_in >> tmp;
            stream_in >> tmp;
            if(tmp.find("nmos") != -1){
                MOS_type = 0;
            }else {
                MOS_type = 1;
            }
            //width process
            stream_in >> tmp;
            if(MOS_G.nWidth == -1 || MOS_G.pWidth == -1){
                erase_pos = tmp.find("=");
                tmp.erase(0, erase_pos+1);
                erase_pos = tmp.find("n");
                tmp.erase(erase_pos, 1);
                if(MOS_type == 0 && MOS_G.nWidth == -1){
                    MOS_G.nWidth = stod(tmp);
                }else if(MOS_type == 1 && MOS_G.pWidth == -1){
                    MOS_G.pWidth = stod(tmp);
                }
            }
            //length process
            stream_in >> tmp;
            if(MOS_G.length == -1){
                erase_pos = tmp.find("=");
                tmp.erase(0, erase_pos+1);
                erase_pos = tmp.find("n");
                tmp.erase(erase_pos, 1);
                MOS_G.length = stod(tmp);
            }
            //save gate in map
            map <string, pair<vector<int>, vector<int>>> :: iterator iter_G;
            iter_G = MOS_G.AllGate.find(MOS_in.G);
            if(iter_G == MOS_G.AllGate.end()){
                pair<vector<int>, vector<int>> NP_mos;
                if(MOS_type == 0){
                    NP_mos.first.push_back(MOS_G.NMOS.size());
                }else{
                    NP_mos.second.push_back(MOS_G.PMOS.size());
                }
                MOS_G.AllGate.insert(pair<string, pair<vector<int>, vector<int>>>(MOS_in.G, NP_mos));
            }else{
                if(MOS_type == 0){
                    MOS_G.AllGate[MOS_in.G].first.push_back(MOS_G.NMOS.size());
                }else{
                    MOS_G.AllGate[MOS_in.G].second.push_back(MOS_G.PMOS.size());
                }
            }
            //save SD in map
            map <string, Position> :: iterator iter_SD;
            iter_SD = MOS_G.AllSD.find(MOS_in.D);
            if(iter_SD == MOS_G.AllSD.end()){
                Position pos;
                if(MOS_type == 0){
                    pos.NMOS.push_back(MOS_G.NMOS.size());
                }else{
                    pos.PMOS.push_back(MOS_G.PMOS.size());
                }
                MOS_G.AllSD.insert(pair<string, Position>(MOS_in.D, pos));
            }else{
                if(MOS_type == 0){
                    iter_SD->second.NMOS.push_back(MOS_G.NMOS.size());
                }else{
                    iter_SD->second.PMOS.push_back(MOS_G.PMOS.size());
                }
            }
            iter_SD = MOS_G.AllSD.find(MOS_in.S);
            if(iter_SD == MOS_G.AllSD.end()){
                Position pos;
                if(MOS_type == 0){
                    pos.NMOS.push_back(MOS_G.NMOS.size());
                }else{
                    pos.PMOS.push_back(MOS_G.PMOS.size());
                }
                MOS_G.AllSD.insert(pair<string, Position>(MOS_in.S, pos));
            }else{
                if(MOS_type == 0){
                    iter_SD->second.NMOS.push_back(MOS_G.NMOS.size());
                }else{
                    iter_SD->second.PMOS.push_back(MOS_G.PMOS.size());
                }
            }

            //NPMOS & gatepair save
            if(MOS_type == 0){
                //gate pair
                int num = MOS_G.gateSeq.size();
                gatePair gateNew = {num, MOS_in.G, 0, 0, -1, -1, false};
                MOS_G.gateSeq.push_back(gateNew);
                //NMOS
                MOS_G.NMOS.push_back(MOS_in);
            }else{
                MOS_G.PMOS.push_back(MOS_in);
            }

        }


    }
    int num = MOS_G.gateSeq.size();
    MOS_G.Dummy.resize(num,false);
}

void insertSeq(MOS_group &MOS_G, int pairNum, int pairBehind){
    if(MOS_G.firstGate == pairNum){
        MOS_G.firstGate = MOS_G.gateSeq[pairNum].next;
    }
    if(pairBehind == -1){
        if(MOS_G.gateSeq[pairNum].prev != -1)
            MOS_G.gateSeq[pairNum].prev = -1;
        if(MOS_G.firstGate != -1){
            gatePair* pairNext = &MOS_G.gateSeq[MOS_G.firstGate];
            pairNext->prev = pairNum;
            MOS_G.gateSeq[pairNum].next = pairNext->num;
        }
        MOS_G.firstGate = pairNum;
    }else{
        //pairNum preprocess
        int prevNum = MOS_G.gateSeq[pairNum].prev;
        int nextNum = MOS_G.gateSeq[pairNum].next;
        gatePair* pairPrev = NULL;
        gatePair* pairNext = NULL;
        if(prevNum != -1){
            pairPrev = &MOS_G.gateSeq[prevNum];
        }
        if(nextNum != -1){
            pairNext = &MOS_G.gateSeq[nextNum];
        }
        if(pairPrev != NULL){
            if(pairNext == NULL)
                pairPrev->next = -1;
            else
                pairPrev->next = pairNext->num;
        }
        if(pairNext != NULL){
            if(pairPrev == NULL)
                pairNext->prev = -1;
            else
                pairNext->prev = pairPrev->num;
        }
        //move to pair behind
        nextNum = MOS_G.gateSeq[pairBehind].next;
        gatePair* behindNext = NULL;
        if(nextNum != -1)
            behindNext = &MOS_G.gateSeq[nextNum];
        MOS_G.gateSeq[pairBehind].next = pairNum;
        MOS_G.gateSeq[pairNum].prev = pairBehind;
        MOS_G.gateSeq[pairNum].next = nextNum;
        if(behindNext != NULL)
            behindNext->prev = pairNum;
    }
}

void rotateJudge(MOS* preMOS, MOS* curMOS){
    string preD = (preMOS->rotate == false) ? preMOS->D : preMOS->S;
    string preS = (preMOS->rotate == false) ? preMOS->S : preMOS->D;
    if(preD == curMOS->D && preMOS->prevSame == false){
        preMOS->rotate = true;
        curMOS->rotate = false;
        curMOS->prevSame = true;
    }/*else if(preD == curMOS->D && preMOS->prevSame == true){
        int random = rand() % 2;
        if(random == 1){
            preMOS->rotate = !preMOS->rotate;
            preMOS->prevSame = false;
            curMOS->rotate = false;
            curMOS->prevSame = true;
        }else{
            curMOS->rotate = false;
            curMOS->prevSame = false;
        }
    }*/
    else if(preS == curMOS->S){
        curMOS->rotate = true;
        curMOS->prevSame = true;
    }else if(preS == curMOS->D){
        curMOS->rotate = false;
        curMOS->prevSame = true;
    }else if(preD == curMOS->S && preMOS->prevSame == false){
        preMOS->rotate = true;
        curMOS->rotate = true;
        curMOS->prevSame = true;
    }/*else if(preD == curMOS->S && preMOS->prevSame == true){
        int random = rand() % 2;
        if(random == 1){
            preMOS->rotate = !preMOS->rotate;
            preMOS->prevSame = false;
            curMOS->rotate = true;
            curMOS->prevSame = true;
        }else{
            curMOS->rotate = false;
            curMOS->prevSame = false;
        }
    }*/
    else{
        curMOS->rotate = false;
        curMOS->prevSame = false;
    }
}

void rotateAll(MOS_group &MOS_G){
    gatePair* curPair = &MOS_G.gateSeq[MOS_G.firstGate];
    MOS_G.NMOS[curPair->NMOS].rotate = false;
    MOS_G.NMOS[curPair->NMOS].prevSame = false;
    MOS_G.PMOS[curPair->PMOS].rotate = false;
    MOS_G.PMOS[curPair->PMOS].prevSame = false;
    for(int i=0; i < MOS_G.gateSeq.size(); i++){
        //NMOS rotate or not
        int preNum = curPair->prev;
        gatePair* prePair = NULL;
        if(preNum != -1)
            prePair = &MOS_G.gateSeq[preNum];
        if(prePair != NULL){
            MOS* preNMOS = &MOS_G.NMOS[prePair->NMOS];
            MOS* curNMOS = &MOS_G.NMOS[curPair->NMOS];
            rotateJudge( preNMOS, curNMOS);
        
            //PMOS rotate or not
            MOS* prePMOS = &MOS_G.PMOS[prePair->PMOS];
            MOS* curPMOS = &MOS_G.PMOS[curPair->PMOS];
            rotateJudge( prePMOS, curPMOS);
        }
        curPair = &MOS_G.gateSeq[curPair->next];
    }
}

void initialSeq(MOS_group &MOS_G){
    //gete sequence
    int gateSize = MOS_G.NMOS.size();
    int randPair = rand() % gateSize;
    MOS_G.gateSeq[randPair].choosed = true;
    insertSeq(MOS_G, randPair, -1);
    int pairBehind = randPair;
    for(int i=1; i < gateSize; i++){
        randPair = rand() % gateSize;
        if(MOS_G.gateSeq[randPair].choosed == true){
            i--;
            continue;
        }else{
            MOS_G.gateSeq[randPair].choosed = true;
            insertSeq(MOS_G, randPair, pairBehind);
            pairBehind = randPair;
        }
    }
    //NPMOS sequence
    int randMOS;
    map <string, pair<vector<int>, vector<int>>> :: iterator iter_G;
    gatePair* curPair = &MOS_G.gateSeq[MOS_G.firstGate];
    for(int i=0; i < gateSize; i++){
        string gateName = curPair->gateName;
        iter_G = MOS_G.AllGate.find(gateName);
        vector<int> NMOS_v = iter_G->second.first;
        vector<int> PMOS_v = iter_G->second.second;
        if(NMOS_v.size() == 1){
            //NMOS
            curPair->NMOS = NMOS_v[0];
            MOS_G.NMOS[NMOS_v[0]].choosed = true;
            //PMOS
            curPair->PMOS = PMOS_v[0];
            MOS_G.PMOS[PMOS_v[0]].choosed = true;
        }else{
            //NMOS
            int randNum = rand() % NMOS_v.size();
            while(MOS_G.NMOS[NMOS_v[randNum]].choosed == true){
                randNum = rand() % NMOS_v.size();
            }
            MOS_G.NMOS[NMOS_v[randNum]].choosed = true;
            curPair->NMOS = NMOS_v[randNum];
            //PMOS
            randNum = rand() % PMOS_v.size();
            while(MOS_G.PMOS[PMOS_v[randNum]].choosed == true){
                randNum = rand() % PMOS_v.size();
            }
            MOS_G.PMOS[PMOS_v[randNum]].choosed = true;
            curPair->PMOS = PMOS_v[randNum];
        }
        if(curPair->next != -1)
            curPair = &MOS_G.gateSeq[curPair->next];   
    }
    rotateAll(MOS_G);

}

void insertDummy(MOS_group &MOS_G){
    int num = 0;
    gatePair* curPair = &MOS_G.gateSeq[MOS_G.firstGate];
    gatePair* nextPair = NULL;
    if(curPair->next != -1)
        nextPair = &MOS_G.gateSeq[curPair->next];
    //string curD = (curPair->NMOS->rotate == false) ? curPair->NMOS->D : curPair->NMOS->S;
    string curNS = (MOS_G.NMOS[curPair->NMOS].rotate == false) ? MOS_G.NMOS[curPair->NMOS].S : MOS_G.NMOS[curPair->NMOS].D;
    string curPS = (MOS_G.PMOS[curPair->PMOS].rotate == false) ? MOS_G.PMOS[curPair->PMOS].S : MOS_G.PMOS[curPair->PMOS].D;
    string nextND;
    string nextPD;
    //string nextS = (nextPair->NMOS->rotate == false) ? nextPair->NMOS->S : nextPair->NMOS->D;
    while(num < MOS_G.gateSeq.size()-1){
        if(curPair->next != -1)
            nextPair = &MOS_G.gateSeq[curPair->next];
        nextND = (MOS_G.NMOS[nextPair->NMOS].rotate == false) ? MOS_G.NMOS[nextPair->NMOS].D : MOS_G.NMOS[nextPair->NMOS].S;
        nextPD = (MOS_G.PMOS[nextPair->PMOS].rotate == false) ? MOS_G.PMOS[nextPair->PMOS].D : MOS_G.PMOS[nextPair->PMOS].S;
        if(curNS != nextND || curPS != nextPD)
            MOS_G.Dummy[curPair->num] = true;
        else
            MOS_G.Dummy[curPair->num] = false;
        num++;
        curPair = nextPair;
        curNS = (MOS_G.NMOS[curPair->NMOS].rotate == false) ? MOS_G.NMOS[curPair->NMOS].S : MOS_G.NMOS[curPair->NMOS].D;
        curPS = (MOS_G.PMOS[curPair->PMOS].rotate == false) ? MOS_G.PMOS[curPair->PMOS].S : MOS_G.PMOS[curPair->PMOS].D;
    }
    MOS_G.Dummy[curPair->num] = false;
}

void updateSDPos(MOS_group &MOS_G, map <string, Position> :: iterator it_SD, double curX, bool type){
    double ori_left = it_SD -> second.left;
    double ori_right = it_SD -> second.right;
    double height = MOS_G.nWidth / 2 + MOS_G.pWidth / 2 + ACTIVE_S_1;
    if(ori_left > curX || ori_left == -1)
        it_SD -> second.left = curX;
    else if(ori_right < curX || ori_right == -1)
        it_SD -> second.right = curX;
}

void XYCalInitial(MOS_group &MOS_G){
    insertDummy(MOS_G);
    int seqSize = MOS_G.gateSeq.size();
    int num = 0;
    gatePair* curPair = &MOS_G.gateSeq[MOS_G.firstGate];
    //gatePair* nextPair = curPair->next;
    double curX = GAYE_ACTIVE_EX_2 / 2;
    string findInMap;
    map <string, Position> :: iterator it_SD;
    double ori_left;
    double ori_right;
    for(auto &it : MOS_G.AllSD){
        it.second.right = -1;
        it.second.left = -1;
    }

    while(num < seqSize){
        //NMOS drain
        findInMap = (MOS_G.NMOS[curPair->NMOS].rotate == false) ? MOS_G.NMOS[curPair->NMOS].D : MOS_G.NMOS[curPair->NMOS].S;
        it_SD = MOS_G.AllSD.find(findInMap);
        updateSDPos(MOS_G, it_SD, curX, false);
        //PMOS drain
        findInMap = (MOS_G.PMOS[curPair->PMOS].rotate == false) ? MOS_G.PMOS[curPair->PMOS].D : MOS_G.PMOS[curPair->PMOS].S;
        it_SD = MOS_G.AllSD.find(findInMap);
        updateSDPos(MOS_G, it_SD, curX, true);
        //update X
        if(num == 0 || num == seqSize-1)
            curX += (GAYE_ACTIVE_EX_2 / 2 + GATE_W_1 + GATE_S_2 / 2);
        else
            curX += (GATE_W_1 + GATE_S_2);
        //NMOS source
        findInMap = (MOS_G.NMOS[curPair->NMOS].rotate == false) ? MOS_G.NMOS[curPair->NMOS].S : MOS_G.NMOS[curPair->NMOS].D;
        it_SD = MOS_G.AllSD.find(findInMap);
        updateSDPos(MOS_G, it_SD, curX, false);
        //PMOS source
        findInMap = (MOS_G.PMOS[curPair->PMOS].rotate == false) ? MOS_G.PMOS[curPair->PMOS].S : MOS_G.PMOS[curPair->PMOS].D;
        it_SD = MOS_G.AllSD.find(findInMap);
        updateSDPos(MOS_G, it_SD, curX, true);
        if(MOS_G.Dummy[curPair->num] == true)
            curX += (2*(GATE_W_1 + GATE_S_2));

        num++;
        if(curPair->next != -1)
            curPair = &MOS_G.gateSeq[curPair->next];
    }
}

void CostCal(MOS_group &MOS_G){
    XYCalInitial(MOS_G);
    double width = 0;
    double height = 0;
    double height_const = MOS_G.nWidth/2 + MOS_G.pWidth/2 + ACTIVE_S_1;
    MOS_G.totalHPWL = 0;
    for (const auto& it_SD : MOS_G.AllSD) {
        if(it_SD.second.NMOS.size() == 0 || it_SD.second.PMOS.size() == 0)
            height = 0;
        else
            height = height_const;

        if(it_SD.second.left == -1 || it_SD.second.right == -1)
            width = 0;
        else
            width = it_SD.second.right - it_SD.second.left;
        MOS_G.totalHPWL += (height + width);
    }
}

void swap(MOS_group &MOS_G){
    /*
    //test
    int pair1Num = 11;
    int pair2Num = 15;*/
    //random choose
    int pair1Num = rand() % MOS_G.gateSeq.size();
    int pair2Num = rand() % MOS_G.gateSeq.size();
    while(pair1Num == pair2Num)
        pair2Num = rand() % MOS_G.gateSeq.size();
    if(pair2Num < pair1Num){
        int tmp = pair1Num;
        pair1Num = pair2Num;
        pair2Num = tmp;
    }
    //swap
    gatePair* pair1 = &MOS_G.gateSeq[pair1Num];
    gatePair* pair2 = &MOS_G.gateSeq[pair2Num];
    //first gate
    if(pair1Num == MOS_G.firstGate)
        MOS_G.firstGate = pair2Num;
    else if(pair2Num == MOS_G.firstGate)
        MOS_G.firstGate = pair1Num;
    //change pointer
    int pair1PreNum = pair1->prev;
    int pair2PreNum = pair2->prev;
    int pair1NextNum = pair1->next;
    int pair2NextNum = pair2->next;
    gatePair* pair1Pre = NULL;
    if(pair1PreNum != -1)
        pair1Pre = &MOS_G.gateSeq[pair1PreNum];
    gatePair* pair1Next = NULL;
    if(pair1NextNum != -1)
        pair1Next = &MOS_G.gateSeq[pair1NextNum];
    gatePair* pair2Pre = NULL; 
    if(pair2PreNum != -1)
        pair2Pre = &MOS_G.gateSeq[pair2PreNum];
    gatePair* pair2Next = NULL;
    if(pair2NextNum != -1)
        pair2Next = &MOS_G.gateSeq[pair2NextNum];
    
    if(pair1Pre != NULL)
        pair1Pre->next = pair2->num;
    if(pair1Next != NULL)
        pair1Next->prev = pair2->num;
    if(pair2Pre != NULL)
        pair2Pre->next = pair1->num;
    if(pair2Next != NULL)
        pair2Next->prev = pair1->num;
    


    if(pair2Pre == pair1)
        pair1->prev = pair2Num;
    else
        pair1->prev = pair2PreNum;
    if(pair2Next == pair1)
        pair1->next = pair2Num;
    else
        pair1->next = pair2NextNum;
    if(pair1Pre == pair2)
        pair2->prev = pair1Num;
    else
        pair2->prev = pair1PreNum;
    if(pair1Next == pair2)
        pair2->next = pair1Num;
    else
        pair2->next = pair1NextNum;

    rotateAll(MOS_G);
}

void SA(MOS_group &MOS_G,bestSol &best){
    const double r = 0.9;
    const double P = 0.95;
    const int GateNum = MOS_G.gateSeq.size();
    const int cons = 2;
    const int uphillMax = GateNum * cons;
    const double epsilon = 0.001;

    
    initialSeq(MOS_G);
    CostCal(MOS_G);
    best = {MOS_G.firstGate, MOS_G.totalHPWL, MOS_G.gateSeq, MOS_G.NMOS, MOS_G.PMOS, MOS_G.Dummy};

    int uphill = 0;
    int total = 0;
    double T = (-(best.totalHPWL) * GateNum) / log(P);
    if(T > 20000)
        T = 20000;
    //double preHPWL = best.totalHPWL;
    int preFirst = best.firstGate;
    MOS_group preMOS_G = MOS_G;
    
    while(T >= epsilon){
        //cout << "uphill : "<< uphill << endl;
        uphill = 0;
        total = 0;
        while(uphill < uphillMax && total <= 20*GateNum){
            int random = rand() % 3;
            if(random == 0){
                swap(MOS_G);
            }else{
                int pairNum = rand() % MOS_G.gateSeq.size();
                int pairBehind = rand() % MOS_G.gateSeq.size();
                while(pairNum == pairBehind)
                    pairBehind = rand() % MOS_G.gateSeq.size();
                insertSeq(MOS_G, pairNum, pairBehind);
            }
            
            CostCal(MOS_G);
            double curHPWL = MOS_G.totalHPWL;
            double deltaHPWL = curHPWL - preMOS_G.totalHPWL;
            double r=((double)rand()/(RAND_MAX));
            if(deltaHPWL <= 0 || r < exp(-deltaHPWL/T)){
                if(deltaHPWL > 0)
                    uphill++;
                preMOS_G = MOS_G;
                if(curHPWL < best.totalHPWL){
                    best = {MOS_G.firstGate, MOS_G.totalHPWL, MOS_G.gateSeq, MOS_G.NMOS, MOS_G.PMOS, MOS_G.Dummy};
                    //cout << best.totalHPWL << endl;
                }
            }else{
                MOS_G = preMOS_G;
            }
            total++;
        }
        T *= r;
    }
}



void output(bestSol &best, ofstream& FILEOUT){
    bool preDummy = true;
    FILEOUT << best.totalHPWL<< endl;
    gatePair* seq = &best.gateSeq[best.firstGate];
    //PMOS instance
    for(int i=0; i < best.gateSeq.size(); i++){
        FILEOUT << best.PMOS[seq->PMOS].name << " ";
        if(best.Dummy[seq->num] == true)
            FILEOUT << "Dummy" << " ";
        if(seq ->next != -1)
            seq = &best.gateSeq[seq ->next];
    }
    FILEOUT << endl;
    seq = &best.gateSeq[best.firstGate];
    //PMOS pin
    for(int i=0; i < best.gateSeq.size(); i++){
        string seqD = (best.PMOS[seq->PMOS].rotate == false) ? best.PMOS[seq->PMOS].D : best.PMOS[seq->PMOS].S;
        string seqS = (best.PMOS[seq->PMOS].rotate == false) ? best.PMOS[seq->PMOS].S : best.PMOS[seq->PMOS].D;
        if(preDummy == true)
            FILEOUT << seqD << " ";
        FILEOUT << best.PMOS[seq->PMOS].G << " " << seqS << " ";
        if(best.Dummy[seq->num] == true)
            FILEOUT << "Dummy" << " ";
        if(seq != NULL)
            preDummy = best.Dummy[seq->num];
        if(seq ->next != -1)
            seq = &best.gateSeq[seq ->next];
    }
    FILEOUT << endl;
    seq = &best.gateSeq[best.firstGate];
    //NMOS instance
    for(int i=0; i < best.gateSeq.size(); i++){
        FILEOUT << best.NMOS[seq->NMOS].name << " ";
        if(best.Dummy[seq->num] == true)
            FILEOUT << "Dummy" << " ";
        if(seq ->next != -1)
            seq = &best.gateSeq[seq ->next];
    }
    FILEOUT << endl;
    seq = &best.gateSeq[best.firstGate];
    preDummy = true;
    //NMOS pin
    for(int i=0; i < best.gateSeq.size(); i++){
        string seqD = (best.NMOS[seq->NMOS].rotate == false) ? best.NMOS[seq->NMOS].D : best.NMOS[seq->NMOS].S;
        string seqS = (best.NMOS[seq->NMOS].rotate == false) ? best.NMOS[seq->NMOS].S : best.NMOS[seq->NMOS].D;
        if(preDummy == true)
            FILEOUT << seqD << " ";
        FILEOUT << best.NMOS[seq->NMOS].G << " " << seqS << " ";
        if(best.Dummy[seq->num] == true)
            FILEOUT << "Dummy" << " ";
        if(seq != NULL)
            preDummy = best.Dummy[seq->num];
        if(seq ->next != -1)
            seq = &best.gateSeq[seq ->next];
    }

}