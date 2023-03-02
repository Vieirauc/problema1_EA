// Static size array saved on the stack
#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

struct qr_comp{
    int n;
    vector<int> lb;
    vector<int> cb;
    vector<int> lt;
    vector<int> ct;
    vector<int> qb;
    vector<int> db;
};

//Check if the qr code is valid
int isValid(qr_comp qr_comp, vector<vector<int>> qr){
    int n = qr_comp.n - 1;
    for (int i = 0; i <= n; i++){
        for (int j = 0; j <= n; j++){
            if (j != n  && qr[i][j+1] != qr[i][j]){
                qr_comp.lt[i]--;
            }
            if (i != n   && qr[i+1][j] != qr[i][j]){
                qr_comp.ct[j]--;
            }
            if (qr[i][j] == 1){
                qr_comp.lb[i]--;
                qr_comp.cb[j]--;
                if (i == j){
                    qr_comp.db[0]--;
                }
                if (n - i  == j ){
                    qr_comp.db[1]--;
                }
                if (i <= floor(n/2) && j > floor(n/2)){
                    qr_comp.qb[0]--;
                }
                if (i <= floor(n/2) && j <= floor(n/2)){
                    qr_comp.qb[1]--;
                }
                if (i > floor(n/2) && j <= floor(n/2)){
                    qr_comp.qb[2]--;
                }
                if (i > floor(n/2) && j > floor(n/2)){
                    qr_comp.qb[3]--;
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        if (qr_comp.lb[i] != 0 || qr_comp.cb[i] != 0){
            //cout << "lb/cb!" << endl;
            return 0;
        }
    }
    for (int i = 0; i < n; i++){
        if (qr_comp.lt[i] != 0 || qr_comp.ct[i] != 0){
            //cout << "lt/ct!" << endl;
            return 0;
        }
    }
    for (int i = 0; i < 2; i++){
        if (qr_comp.db[i] != 0){
            //cout << "db!" << endl;
            return 0;
        }
    }
    for (int i = 0; i < 4; i++){
        if (qr_comp.qb[i] != 0){
            //cout << "qb!" << endl;
            return 0;
        }
    }
    return 1;
}

//Function that checks all possible qr codes and counts how many are valid
int generate_check(qr_comp qr_comp, vector<vector<int>> & qr, vector<vector<vector<int>>> & valid_qrs, int x, int y , int paint){
    int count_valid = 0;
    while(qr[x][y] == 1){
        if (x == qr_comp.n){
            x = 0;
            y += 1;
        }else{
            x += 1;
        }
    }
    if(paint == 1)(qr)[x][y] = 1;
    if (x == qr_comp.n - 1 && y == qr_comp.n - 1){
        if (isValid(qr_comp,qr)){ 
            if(valid_qrs.size() == 0){
                valid_qrs.push_back(qr);
            }
            count_valid++;
        }
    }
    else if (x == qr_comp.n - 1 && y <= qr_comp.n - 1){
        count_valid += generate_check(qr_comp,qr,valid_qrs,0,y+1,1);
        count_valid += generate_check(qr_comp,qr,valid_qrs,0,y+1,0);
    }
    else if ( x < qr_comp.n - 1){
        count_valid += generate_check(qr_comp,qr,valid_qrs,x+1,y,1);
        count_valid += generate_check(qr_comp,qr,valid_qrs,x+1,y,0);   
    }
    return count_valid;   
}

void pre_proccess(qr_comp qr_comp,  vector<vector<int>> & qr){
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.lb[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                qr[i][j] = 1;
            }    
        }
    }
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.cb[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                qr[j][i] = 1;
            }    
        }
    }
    for(int i = 0; i < 2; i++){
        if(qr_comp.db[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                if (i == 0){
                    qr[j][j] = 1;
                }
                else{
                    qr[j][qr_comp.n - 1 - j] = 1;
                }
            }    
        }
    }
}

int main(){
    int n_qr;
    cin >> n_qr;
    

    //Read compressed qr code characteristics
    while(n_qr != 0){
        n_qr--;
        qr_comp qr_comp;
        int value;
        //Read size of the qr code
        cin >> qr_comp.n;
        //Read the amount of black squares in each row
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.lb.push_back(value);
        }

        //Read the amount of black squares in each column
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.cb.push_back(value);
        }

        //Read the amount of transitions in each row
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.lt.push_back(value);
        }

        //Read the amount of transitions in each column
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.ct.push_back(value);
        }

        //Read the amount of black squares per quadrant
        for(int i = 0; i < 4; i++){
            cin >> value;
            qr_comp.qb.push_back(value);
        }

        //Read the amount of transitions in each diagonal - main  and antidiagonal
        for(int i = 0; i < 2; i++){
            cin >> value;
            qr_comp.db.push_back(value);
        }

        vector<vector<int>> qr(qr_comp.n, vector<int>(qr_comp.n, 0));
        vector<vector<vector<int>>> valid_qrs;

        pre_proccess(qr_comp,qr);

        int counter = generate_check(qr_comp,qr,valid_qrs,0,0,1) + generate_check(qr_comp,qr,valid_qrs,0,0,0);

        if (counter == 1)
        {
            cout << "VALID: 1 QR Code generated!" << endl;
            //print qr code
            cout << "+";
            for (int i = 0; i < qr_comp.n; i++){
                cout << "-";
            }
            cout << "+" << endl;
            
            for (int i = 0; i < qr_comp.n; i++){
                cout << "|";
                for (int j = 0; j < qr_comp.n; j++){
                    if(valid_qrs[0][i][j] == 1){
                        cout << "#";
                    }else if(valid_qrs[0][i][j] == 0){
                        cout << " ";
                    }
                }
                cout << "|" << endl;
            }

            cout << "+";
            for (int i = 0; i < qr_comp.n; i++){
                cout << "-";
            }
            cout << "+" << endl;
        }else if(counter > 1){
            cout << "INVALID: "<< counter << " QR Codes generated!" << endl;
        }else{
            cout << "DEFECT: No QR Code generated!" << endl;
        }

        qr_comp.lb.clear();
        qr_comp.cb.clear();
        qr_comp.lt.clear();
        qr_comp.ct.clear();
        qr_comp.qb.clear();
        qr_comp.db.clear();
    }

}