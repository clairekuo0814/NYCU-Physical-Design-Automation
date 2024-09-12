#include "BST.h"

extern double ratioLow, ratioHigh;
extern int rootBlock;

void readFille(ifstream &FILEIN, vector<block>& allBlock, double& totalArea){
    FILEIN >> ratioLow >> ratioHigh;
    bool noEmpty;
    string name;
    int width;
    int height;
    int num = 0;
    while(FILEIN >> name >> width >> height){
        block newblock = {name, width, height, 0, NULL, NULL, NULL, 0, 0, num};
        allBlock.push_back(newblock);
        num++;
        totalArea += width*height;
    }
}

void initialBTree(vector<block>& allBlock, bestSol& best){
    int allBlockSize = allBlock.size();
    vector<bool> choose(allBlockSize, 0);
    vector<int> chooseVector(allBlockSize);
    rootBlock = rand() % allBlockSize;
    int chooseBlock = rootBlock;
    int compParent = 0; //complete choose left and right child
    int compNode = 0;    //complete be choosed
    chooseVector[compNode] = chooseBlock;
    choose[chooseBlock] = 1;
    compNode++;
    while(compNode != allBlockSize){
        int parentNum = chooseVector[compParent];
        //random left child
        while(choose[chooseBlock] == 1){
            chooseBlock = rand() % allBlockSize;
        }
        chooseVector[compNode] = chooseBlock;
        choose[chooseBlock] = 1;
        allBlock[chooseBlock].parent = &allBlock[parentNum];
        allBlock[parentNum].left = &allBlock[chooseBlock];
        compNode++;

        if(compNode == allBlockSize)
            break;
        else{
            //random left child
            while(choose[chooseBlock] == 1){
                chooseBlock = rand() % allBlockSize;
            }
            chooseVector[compNode] = chooseBlock;
            choose[chooseBlock] = 1;
            allBlock[chooseBlock].parent = &allBlock[parentNum];
            allBlock[parentNum].right = &allBlock[chooseBlock];
            compNode++;
        }
        compParent++;

    }
    BTreeFloorPlan(allBlock);
    best.allBlock = allBlock;
    int bestRoot = rootBlock;
    costCal(best.allBlock, best.area, best.ratio);
    double preDeltaRatio = min(abs(best.ratio-ratioLow),abs(best.ratio-ratioLow));
    //conform to ratioLow && ratioHigh
    while(best.ratio < ratioLow || best.ratio > ratioHigh){
        //choose one of three change ways
        int chooseMode = rand() % 3;
        if(chooseMode == 0){
            rotate(allBlock);
        }
        else if(chooseMode == 1){
            int fromNum = rand() % allBlock.size();
            int toNum = fromNum;
            while(toNum == fromNum){
                toNum = rand() % allBlock.size();
            }
            swap(allBlock, fromNum, toNum);
        }else {
            move(allBlock);
        }
        int curArea;
        double curRatio;
        BTreeFloorPlan(allBlock);
        costCal(allBlock, curArea, curRatio);
        double curDeltaRatio = min(abs(curRatio-ratioLow), abs(curRatio-ratioLow));
        if(curDeltaRatio < preDeltaRatio || (curRatio >= ratioLow && curRatio <= ratioHigh)){
            best = {allBlock, curArea, curRatio};
            preDeltaRatio = curDeltaRatio;
            bestRoot = rootBlock;
        }else{
            allBlock = best.allBlock;
            rootBlock = bestRoot;
        }
    }


}

void xyCal(vector<block>& allBlock, block &block, vector<int> & contour, bool isLeft){
    if(isLeft)
        block.x = (*block.parent).x + (*block.parent).width;
    else
        block.x = (*block.parent).x;

    int x_end = block.x + block.width;
    if(contour.size() < x_end)
        contour.resize(x_end, 0);
    int contour_y = 0;
    for(int i = block.x; i < x_end; i++){
        if(contour[i] > contour_y)
            contour_y = contour[i];
    }

    block.y = contour_y;
    for(int i = block.x; i < x_end; i++){
        contour[i] = contour_y + block.height;
    }

    if(block.left != NULL)
        xyCal(allBlock, *block.left, contour, true);
    if(block.right != NULL)
        xyCal(allBlock, *block.right, contour, false);
}

void BTreeFloorPlan(vector<block>& allBlock){
    allBlock[rootBlock].x = 0;
    allBlock[rootBlock].y = 0;
    
    vector<int> contour(allBlock[rootBlock].width, 0);
    for(int i = 0; i < allBlock[rootBlock].width; i++)
        contour[i] = allBlock[rootBlock].height;
    if(allBlock[rootBlock].left != NULL)
        xyCal(allBlock, *allBlock[rootBlock].left, contour, true);
    if(allBlock[rootBlock].right != NULL)
        xyCal(allBlock, *allBlock[rootBlock].right, contour, false);
}

void rotate(vector<block>& allBlock){
    int randNum = rand() % allBlock.size();
    int width_temp = allBlock[randNum].width;
    allBlock[randNum].width = allBlock[randNum].height;
    allBlock[randNum].height = width_temp;
    allBlock[randNum].rotate = 1 - allBlock[randNum].rotate;
}

void swap(vector<block>& allBlock, int fromNode, int toNode){
    //check root change
    if(fromNode == rootBlock)
        rootBlock = toNode;
    else if(toNode == rootBlock)
        rootBlock = fromNode;
    int roottest = rootBlock;
    //start swap
    block & fromBlock = allBlock[fromNode];
    block & toBlock = allBlock[toNode];
    //swap parent
    block* fromParent = fromBlock.parent;
    block* toParent = toBlock.parent;
    
    if(fromParent != NULL){
        if(fromParent->left == &fromBlock)
            fromParent->left = &toBlock;
        else
            fromParent->right = &toBlock;
    }
    if(toParent != NULL){
        if(toParent->left == &toBlock)
            toParent->left = &fromBlock;
        else
            toParent->right = &fromBlock;
    }
    
    toBlock.parent = fromParent;
    fromBlock.parent = toParent;

    //swap child
    block* fromLeftChild = fromBlock.left;
    block* fromRightChild = fromBlock.right;
    block* toLeftChild = toBlock.left;
    block* toRightChild = toBlock.right;

    fromBlock.left = toLeftChild;
    fromBlock.right = toRightChild;
    toBlock.left = fromLeftChild;
    toBlock.right = fromRightChild;

    if(fromLeftChild != NULL)
        fromLeftChild->parent = &toBlock;
    if(fromRightChild != NULL)
        fromRightChild->parent = &toBlock;
    if(toLeftChild != NULL)
        toLeftChild->parent = &fromBlock;
    if(toRightChild != NULL)
        toRightChild->parent = &fromBlock;

    //fromNode and toNode are parent and child
    if(fromBlock.parent == &fromBlock)
        fromBlock.parent = &toBlock;
    else if(fromBlock.left == &fromBlock)
        fromBlock.left = &toBlock;
    else if(fromBlock.right == &fromBlock)
        fromBlock.right = &toBlock;

    if(toBlock.parent == &toBlock)
        toBlock.parent = &fromBlock;
    else if(toBlock.left == &toBlock)
        toBlock.left = &fromBlock;
    else if(toBlock.right == &toBlock)
        toBlock.right = &fromBlock;


}

void move(vector<block>& allBlock){
    /*test
    int fromNum = 0;
    int toNum = 3;
    bool isLeft = 0;*/
    
    //random choose fromNode, toNode, left/right
    int fromNum = rand() % allBlock.size();
    int toNum = fromNum;
    while(toNum == fromNum){
        toNum = rand() % allBlock.size();
    }
    bool isLeft = rand() % 2;
    block& fromNode = allBlock[fromNum];
    block& toNode = allBlock[toNum];

    //process fromNode (delete)
    if(fromNode.left == NULL && fromNode.right == NULL){        //fromNode no child
        if(fromNode.parent != NULL){
            if(fromNode.parent->left == &fromNode)
                fromNode.parent->left = NULL;
            else
                fromNode.parent->right = NULL;
        }
    }else if(fromNode.left != NULL && fromNode.right != NULL){  //fromNode two child
        //first swap
        bool toLeft;
        toLeft = rand() % 2;
        //root may change
        if(toLeft)
            swap(allBlock, fromNode.num, fromNode.left->num);
        else
            swap(allBlock, fromNode.num, fromNode.right->num);
        
        //swap until fromNode become leaf
        while(fromNode.left != NULL || fromNode.right != NULL){
            if(fromNode.left != NULL && fromNode.right != NULL)
                toLeft = rand() % 2;
            else if(fromNode.left != NULL)
                toLeft = true;
            else
                toLeft = false;
            if(toLeft)
                swap(allBlock, fromNode.num, fromNode.left->num);
            else
                swap(allBlock, fromNode.num, fromNode.right->num);
        }

        if(fromNode.parent->left == &fromNode)
            fromNode.parent->left = NULL;
        else
            fromNode.parent->right = NULL;

    }else{          //one child
        if(fromNode.left != NULL){  //left child
            if(rootBlock == fromNum){
                rootBlock = fromNode.left->num;
            }
            else{
                if(fromNode.parent->left == &fromNode)
                    fromNode.parent->left = fromNode.left;
                else
                    fromNode.parent->right = fromNode.left;
            }
            fromNode.left->parent = fromNode.parent;

        }else{  //right child
            if(rootBlock == fromNum)
                rootBlock = fromNode.right->num;
            else{
                if(fromNode.parent->left == &fromNode)
                    fromNode.parent->left = fromNode.right;
                else
                    fromNode.parent->right = fromNode.right;
            }
            fromNode.right->parent = fromNode.parent;

        }
    }
    fromNode.left = NULL;
    fromNode.right = NULL;
    //process toNode (insert)
    if(isLeft){
        if(toNode.left != NULL){            //toNode left child move
            bool toNodeLeft = rand() % 2;
            if(toNodeLeft){                 //to fromNode left
                fromNode.left = toNode.left;
            }else{                          //to fromNode right
                fromNode.left = NULL;
                fromNode.right = toNode.left;
            }
            toNode.left->parent = &fromNode;
        }
        toNode.left = &fromNode;
    }else{
        if(toNode.right != NULL){            //toNode right child move
            bool toNodeLeft = rand() % 2;
            if(toNodeLeft){                 //to fromNode left
                fromNode.left = toNode.right;
            }else{                          //to fromNode right
                fromNode.right = toNode.right;
            }
            toNode.right->parent = &fromNode;
        }
        toNode.right = &fromNode;
    }
    fromNode.parent = &toNode;
}

void costCal(const vector<block> allBlock, int& area, double& ratio){
    int x_max = 0;
    int y_max = 0;
    for(int i = 0; i < allBlock.size(); i++){
        if(allBlock[i].x + allBlock[i].width > x_max)
            x_max = allBlock[i].x + allBlock[i].width;
        if(allBlock[i].y + allBlock[i].height > y_max)
            y_max = allBlock[i].y + allBlock[i].height;
    }
    area = x_max * y_max;
    ratio = ((double)x_max) / y_max;
}

void annealing(vector<block>& allBlock, bestSol & best, double& totalArea){
    /*test
    //rotate(allBlock);
    //swap(allBlock, 0, 3);
    move(allBlock);
    BTreeFloorPlan(allBlock);
    cout << "root: "<< rootBlock << endl;*/
    //parameter
    const double r = 0.9;
    const double P = 0.95;
    const int blockNum = allBlock.size();
    const int cons = 5;
    const int uphillMax = blockNum * cons;
    const double epsilon = 0.001;

    int uphill = 0;
    int total = 0;
    double T = (-(best.area/totalArea) * blockNum) / log(P);
    int preArea = best.area;
    int preRoot = rootBlock;
    vector<block> preBlock = allBlock;
    while(T >= epsilon){
        uphill = 0;
        total = 0;
        while(uphill < uphillMax && total <= 5*blockNum){
            //choose one of three change ways
            int chooseMode = rand() % 3;
            if(chooseMode == 0){
                rotate(allBlock);
            }
            else if(chooseMode == 1){
                int fromNum = rand() % allBlock.size();
                int toNum = fromNum;
                while(toNum == fromNum){
                    toNum = rand() % allBlock.size();
                }
                swap(allBlock, fromNum, toNum);
            }else {
                move(allBlock);
            }
            int curArea;
            double curRatio;
            BTreeFloorPlan(allBlock);
            costCal(allBlock, curArea, curRatio);
            if(curRatio >= ratioLow && curRatio <= ratioHigh){
                int deltaArea = curArea - preArea;
                double r=((double)rand()/(RAND_MAX));
                if(deltaArea <= 0 || r < exp(-deltaArea/T)){
                    if(deltaArea > 0)
                        uphill++;
                    preBlock = allBlock;
                    preArea = curArea;
                    preRoot = rootBlock;
                    if(curArea < best.area){
                        best = {allBlock, curArea, curRatio};
                    }
                }else{
                    allBlock = preBlock;
                    rootBlock = preRoot;
                }

            }else{
                allBlock = preBlock;
                rootBlock = preRoot;
            }
            total++;
        }
        T *= r;
    }
}

void output(bestSol best, ofstream & FILEOUT){
    int x_max = 0;
    int y_max = 0;
    FILEOUT << "A = " << best.area << endl;
    FILEOUT << "R = " << best.ratio << endl;
    for(int i = 0; i < best.allBlock.size(); i++){
        if(!best.allBlock[i].rotate)
            FILEOUT << best.allBlock[i].name << " " << best.allBlock[i].x << " "<< best.allBlock[i].y << endl;
        else
            FILEOUT << best.allBlock[i].name << " " << best.allBlock[i].x << " "<< best.allBlock[i].y << " R" << endl;
    }

}

void outputDebug(bestSol best, ofstream & FILEOUT_DEBUG){
    int x_max = 0;
    int y_max = 0;
    FILEOUT_DEBUG << "A = " << best.area << endl;
    FILEOUT_DEBUG << "R = " << best.ratio << endl;
    for(int i = 0; i < best.allBlock.size(); i++){
        //if(!allBlock[i].rotate)
            FILEOUT_DEBUG << best.allBlock[i].name << " " << best.allBlock[i].x << " "<< best.allBlock[i].y << " ";
            FILEOUT_DEBUG << best.allBlock[i].x + best.allBlock[i].width << " " << best.allBlock[i].y + best.allBlock[i].height << endl;
        //else
        //    FILEOUT_DEBUG << allBlock[i].name << " " << allBlock[i].x << " "<< allBlock[i].y << " R" << endl;
    }

    
}