#include "channel_routing.h"

bool myCompare(NET* a, NET* b){
    if(a->min < b->min){
        return 1;
    }else if(a->min == b->min){
        if(a->max <= b->max)
            return 1;
        else
            return 0;
    }else
        return 0;
}

void readInputFile(ifstream& FILEIN, CH_route &chanRoute){
    string get_str;
    string in;
    int left;
    int right;
    int chanNum = 0;
    int chanNumPre = 0;
    bool BStart = false;
    vector<int> topline;
    while(getline(FILEIN, get_str)){
        stringstream stream_in(get_str);
        stream_in >> in;
        if(in[0] == 'T'){
            string str_num(begin(in)+1, end(in));
            int bound_num = stoi(str_num);
            stream_in >> left >> right;
            chanNumPre = chanNum;
            chanNum = right;
            chanRoute.top.resize(chanNum+1);
            for(int i = chanNumPre; i < chanNum+1; i++){
                chanRoute.top[i] = bound_num;
            }
            
        }else if(in[0] == 'B'){
            if(!BStart){
                chanNum = 0;
                chanNumPre = 0;
                BStart = true;
            }
            string str_num(begin(in)+1, end(in));
            int bound_num = stoi(str_num);
            stream_in >> left >> right;
            chanNumPre = chanNum;
            chanNum = right;
            chanRoute.bottom.resize(chanNum+1);
            for(int i = chanNumPre; i < chanNum+1; i++){
                chanRoute.bottom[i] = bound_num;
            }
        }else{
            int get_num = stoi(in);
            if(topline.size()==0){
                chanNum = 0;
                do{
                    topline.push_back(get_num);
                    while(chanRoute.netAll.size() <= get_num){
                        NET* net = new NET;
                        net->num = chanRoute.netAll.size();
                        chanRoute.netAll.push_back(net);
                        
                    }
                    if(chanRoute.netAll[get_num]->min > chanNum || chanRoute.netAll[get_num]->min == -1)
                        chanRoute.netAll[get_num]->min = chanNum;
                    if(chanRoute.netAll[get_num]->max < chanNum || chanRoute.netAll[get_num]->max == -1)
                        chanRoute.netAll[get_num]->max = chanNum;
                    (chanRoute.netAll[get_num]->topBound).push_back(chanRoute.top[chanNum]);
                    chanNum++;
                }while(stream_in >> get_num);
            }else{
                chanNum = 0;
                do{
                    while(chanRoute.netAll.size() <= get_num){
                        NET* net = new NET;
                        chanRoute.netAll.push_back(net);
                    }
                    if(chanRoute.netAll[get_num]->min > chanNum || chanRoute.netAll[get_num]->min == -1)
                        chanRoute.netAll[get_num]->min = chanNum;
                    if(chanRoute.netAll[get_num]->max < chanNum || chanRoute.netAll[get_num]->max == -1)
                        chanRoute.netAll[get_num]->max = chanNum;
                    (chanRoute.netAll[get_num]->bottomBound).push_back(chanRoute.bottom[chanNum]);
                    int upNum = topline[chanNum];
                    //VCG
                    if(get_num != 0 && upNum != 0 && get_num != upNum){
                        bool exist = false;
                        if(chanRoute.netAll[upNum]->VC.size() != 0)
                            for(int i = 0; i < chanRoute.netAll[upNum]->VC.size(); i++){
                                if(chanRoute.netAll[upNum]->VC[i] == get_num){
                                    exist = true;
                                    break;
                                }  
                            }
                        if(!exist)
                            (chanRoute.netAll[upNum]->VC).push_back(get_num);
                    }
                    //VCG_R
                    if(get_num != 0 && upNum != 0 && get_num != upNum){
                        bool exist = false;
                        if(chanRoute.netAll[get_num]->VC_R.size() != 0)
                            for(int i = 0; i < chanRoute.netAll[get_num]->VC_R.size(); i++){
                                if(chanRoute.netAll[get_num]->VC_R[i] == upNum){
                                    exist = true;
                                    break;
                                }  
                            }
                        if(!exist)
                            (chanRoute.netAll[get_num]->VC_R).push_back(upNum);
                    }
                        
                    chanNum++;
                }while(stream_in >> get_num);
            }
            
            
        }
    }
    BuildTopBotTrack(chanRoute);
}

void BuildTopBotTrack(CH_route &chanRoute){
    int chanNum = chanRoute.top.size();
    int preTrack = -1;
    //top grid
    for(int i = 0; i < chanNum; i++){
        while(chanRoute.topGrid.size() <= chanRoute.top[i]){
            vector<bool> v(chanNum, false);
            string name = "T" + to_string(chanRoute.topGrid.size());
            TrackGrid grid = {name,v};
            chanRoute.topGrid.push_back(grid);
        }
        chanRoute.topGrid[chanRoute.top[i]].grid[i] = true;
        if(preTrack != chanRoute.top[i] && preTrack != -1){
            chanRoute.topGrid[preTrack].grid[i] = true;
        }
        preTrack = chanRoute.top[i];
    }
    int topSize = chanRoute.topGrid.size();
    for(int i = 0; i < topSize-2; i++){
        for(int j = 0; j < chanNum; j++){
            if(chanRoute.topGrid[i].grid[j] == true)
                chanRoute.topGrid[i+1].grid[j] = true;
        }
        
    }
    //bottom grid
    preTrack = -1;
    for(int i = 0; i < chanNum; i++){
        while(chanRoute.botGrid.size() <= chanRoute.bottom[i]){
            vector<bool> v(chanNum, false);
            string name = "B" + to_string(chanRoute.botGrid.size());
            TrackGrid grid = {name,v};
            chanRoute.botGrid.push_back(grid);
        }
        chanRoute.botGrid[chanRoute.bottom[i]].grid[i] = true;
        if(preTrack != chanRoute.bottom[i] && preTrack != -1){
            chanRoute.botGrid[preTrack].grid[i] = true;
        }
        preTrack = chanRoute.bottom[i];
    }
    int botSize = chanRoute.botGrid.size();
    for(int i = 0; i < botSize-2; i++){
        for(int j = 0; j < chanNum; j++){
            if(chanRoute.botGrid[i].grid[j] == true)
                chanRoute.botGrid[i+1].grid[j] = true;
        }
        
    }

}

int LeftEdgeAlgo(CH_route &chanRoute){
    vector<NET*> netAllSort = chanRoute.netAll;
    sort(netAllSort.begin()+1, netAllSort.end(), myCompare);
    int routeNum = 0;
    //top track
    int topSize = chanRoute.topGrid.size();
    for(int i = 1; i < netAllSort.size(); i++){
        for(int j = topSize-2; j >= 0 ; j--){
            if( netAllSort[i]->track == "none"){
                bool ready = true;
                if(netAllSort[i]->VC_R.size() != 0){
                    for(int j = 0; j < netAllSort[i]->VC_R.size(); j++){
                        if(chanRoute.netAll[netAllSort[i]->VC_R[j]]->track == "none")
                            ready = false;
                    }
                }
                int left = netAllSort[i]->min;
                int right = netAllSort[i]->max;
                if(ready){
                    if(!chanRoute.topGrid[j].grid[left] && !chanRoute.topGrid[j].grid[right]){
                        for(int k = left+1; k < right; k++){
                            if(chanRoute.topGrid[j].grid[k]){
                                ready = false;
                                break;
                            }
                        }
                    }else{
                        ready = false;
                    }
                }
                if(ready){
                    routeNum++;
                    netAllSort[i]->track = "T" + to_string(j);
                    for(int k = left; k <= right; k++){
                        chanRoute.topGrid[j].grid[k] = true;
                    }
                    if(j < topSize-2){
                        for(int m = j+1 ; m < topSize-1; m++){
                            chanRoute.topGrid[m].grid[left] = true;
                            chanRoute.topGrid[m].grid[right] = true;
                        }
                    }
                    
                    
                    //cout << netAllSort[i]->num << endl;
                }

            }
        }
        
    }
    //bottom track
    int bottomSize = chanRoute.botGrid.size();
    for(int i = 1; i < netAllSort.size(); i++){
        for(int j = bottomSize-2; j >= 0 ; j--){
            if( netAllSort[i]->track == "none"){
                bool ready = true;
                if(netAllSort[i]->VC.size() != 0){
                    for(int j = 0; j < netAllSort[i]->VC.size(); j++){
                        if(chanRoute.netAll[netAllSort[i]->VC[j]]->track == "none")
                            ready = false;
                    }
                }
                int left = netAllSort[i]->min;
                int right = netAllSort[i]->max;
                if(ready){
                    if(!chanRoute.botGrid[j].grid[left] && !chanRoute.botGrid[j].grid[right]){
                        for(int k = left+1; k < right; k++){
                            if(chanRoute.botGrid[j].grid[k]){
                                ready = false;
                                break;
                            }
                        }
                    }else{
                        ready = false;
                    }
                }
                if(ready){
                    routeNum++;
                    netAllSort[i]->track = "B" + to_string(j);
                    for(int k = left; k <= right; k++){
                        chanRoute.botGrid[j].grid[k] = true;
                    }
                    if(j < bottomSize-2){
                        for(int m = j+1 ; m < bottomSize-1; m++){
                            chanRoute.botGrid[m].grid[left] = true;
                            chanRoute.botGrid[m].grid[right] = true;
                        }
                    }
                }

            }
        }
        
    }

    //mid track
    int trackSum = 0;
    int waterMark = -1;
    while(routeNum < netAllSort.size()-1){
        for(int i = 1; i < netAllSort.size(); i++){
            if( netAllSort[i]->track == "none" && waterMark < netAllSort[i]->min){
                bool ready = true;
                if(netAllSort[i]->VC.size() != 0){
                    for(int j = 0; j < netAllSort[i]->VC.size(); j++){
                        if(chanRoute.netAll[netAllSort[i]->VC[j]]->track == "none")
                            ready = false;
                    }
                }  
                if(ready){
                    routeNum++;
                    netAllSort[i]->track = "C" + to_string(trackSum+1);
                    waterMark = netAllSort[i]->max;
                    //cout << netAllSort[i]->num << endl;
                }
                
            }
        }
        waterMark = -1;
        trackSum++;
    }
    return trackSum;
}

void output(const CH_route &chanRoute, ofstream& FILEOUT, int trackSum){
    FILEOUT << "Channel density: " << trackSum << endl;
    for(int i = 1; i < chanRoute.netAll.size(); i++){
        FILEOUT << "Net " << i << endl;
        FILEOUT << chanRoute.netAll[i]->track << " " << chanRoute.netAll[i]->min << " " << chanRoute.netAll[i]->max << endl;
    }
}