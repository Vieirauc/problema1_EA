// Static size array saved on the stack
#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#define DEBUG 0

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

//Function that returns the next cell to be filled. TODO: change to a more efficient way and jump to the next cell if it is already filled
vector<int> nextCell(int n, int x, int y) {
    int nextX = x; //(x + 1) % n;
    int nextY = y;
    if (y == n - 1) {
        nextX = x + 1;
        nextY = 0;
    } else {
        nextX = x;
        nextY = y + 1;
    }
    return {nextX, nextY};
}

//Check if the qr code is valid. If it is valid return 2, if it is not valid return 0 (can´t proceed), if it is valid but not complete return 1
//TODO: corrigir as falhas de validação. nao deteta todos os invalidos
int isValid(qr_comp qr_comp, vector<vector<int>> qr){
    int n = qr_comp.n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (j != n-1  && qr[i][j+1] != qr[i][j]){
                qr_comp.lt[i]--;
            }
            if (i != n-1   && qr[i+1][j] != qr[i][j]){
                qr_comp.ct[j]--;
            }
            if (qr[i][j] == 1){
                qr_comp.lb[i]--;
                qr_comp.cb[j]--;
                if (i == j){
                    qr_comp.db[0]--;
                }
                if (n - i - 1 == j ){
                    qr_comp.db[1]--;
                }
                if ((i+1) <= floor(n/2) && (j+1) > floor(n/2)){
                    qr_comp.qb[0]--;
                }
                if ((i+1) <= floor(n/2) && (j+1) <= floor(n/2)){
                    qr_comp.qb[1]--;
                }
                if ((i+1) > floor(n/2) && (j+1) <= floor(n/2)){
                    qr_comp.qb[2]--;
                }
                if ((i+1) > floor(n/2) && (j+1) > floor(n/2)){
                    qr_comp.qb[3]--;
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        if (qr_comp.lb[i] <= -1 || qr_comp.cb[i] <= -1){
            //cout << "lb/cb!" << endl;
            return 0;
        } else if (qr_comp.lb[i] != 0 || qr_comp.cb[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < n; i++){
        if (qr_comp.lt[i] <= -1 || qr_comp.ct[i] <= -1){
            //cout << "lt/ct!" << endl;
            return 0;
        } else if (qr_comp.lt[i] != 0 || qr_comp.ct[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < 2; i++){
        if (qr_comp.db[i] <= -1){
            //cout << "db!" << endl;
            return 0;
        } else if (qr_comp.db[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < 4; i++){
        if (qr_comp.qb[i] <= -1){
            //cout << "qb!" << i << " " << qr_comp.qb[i] <<  endl;
            return 0;
        } else if (qr_comp.qb[i] != 0){
            return 1;
        }
    }
    return 2;
}


void print_qr(vector<vector<int>> qr, int n){
    cout << "+";
    for (int i = 0; i < n; i++){
        cout << "-";
    }
    cout << "+" << endl;
    
    for (int i = 0; i < n; i++){
        cout << "|";
        for (int j = 0; j < n; j++){
            if(qr[i][j] == 1){
                cout << "#";
            }else if(qr[i][j] == 0){
                cout << " ";
            }
        }
        cout << "|" << endl;
    }

    cout << "+";
    for (int i = 0; i < n; i++){
        cout << "-";
    }
    cout << "+" << endl;
}

//TODO fazer backtracking ( validar se pode pintar a preto ou a branco , caso se puder pinta senão volta atrás)
//Fuction that fills the empty spaces of the qr code verifying if it can paint it black or white using backtracking
int generate_check(qr_comp qr_comp, vector<vector<int>> qr, vector<vector<vector<int>>> & valid_qrs, int & counter, int row, int col, int level){
    #ifdef DEBUG
    cout << "row: " << row << " col: " << col << endl;
    print_qr(qr, qr_comp.n);
    #endif
    
    // check if the qr code is valid
    int valid = isValid(qr_comp, qr);
    switch(valid){
        case 0:
            #ifdef DEBUG
            cout << "Invalid" << endl;
            #endif
            return 0;
        case 1:
            #ifdef DEBUG
            cout << "Valid but not complete" << endl;
            #endif
            break;
        case 2:
            #ifdef DEBUG
            cout << "Valid and complete" << endl;
            #endif
            if(!counter) { // é p poupar memoria, mas podemos ver se e necessario
                valid_qrs.push_back(qr);
            }
            counter++;
            return 1; // @luis aqui parece-me q é return como tinhas mas quero ver isso, n tenho a certeza se estou a pensar bem. os filhos tem obrigatoriamente os pretos dos pais, pelo que acho que mais abaixo na arvore vamos infringir regras (se corresponder ao encoding entao é pq ja nao pode ter mais pretos)
    }


    // generate descendants
    vector<int> next{row, col};
    while((next = nextCell(qr_comp.n, next[0], next[1]))[0] != qr_comp.n){
        qr[next[0]][next[1]] = 1;
        generate_check(qr_comp, qr, valid_qrs, counter, next[0], next[1], level + 1);
        qr[next[0]][next[1]] = 0; // @luis tive q por isto para evitar um wrapper. assim a primeira chamada à funcao pode ser feita com row=-1 e col=n-1
        // aceitam-se ideias melhores
    }
    return 0;
}

/*void pre_proccess(qr_comp qr_comp,  vector<vector<int>> & qr){
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
                for(int k = 0; k < qr_comp.n; k++){
                    if (i == 0){
                        qr[j][k] = 1;
                    }
                    else{
                        qr[j][qr_comp.n - 1 - k] = 1;
                    }
                }
            }    
        }
    }
}*/

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
        int counter = 0;
        //pre_proccess(qr_comp,qr);
        // start_generation(qr_comp, qr);
        
        generate_check(qr_comp, qr, valid_qrs, counter, -1, qr_comp.n - 1, 0);

        if (counter == 1)
        {
            cout << "VALID: 1 QR Code generated!" << endl;
            //print qr code
            print_qr(valid_qrs[0],qr_comp.n);
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