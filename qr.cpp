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
    int sum;
};

//Function that returns the next cell to be filled. TODO: change to a more efficient way and jump to the next cell if it is already filled
vector<int> nextCell(int n, int x, int y, vector<vector<int>> qr) {
    int nextX = x; //(x + 1) % n;
    int nextY = y;
    if (y == n - 1) {
        nextX = x + 1;
        nextY = 0;
    } else {
        nextX = x;
        nextY = y + 1;
    }
    if( nextX < n && nextY < n && qr[nextX][nextY] != -1){
        return nextCell(n, nextX, nextY, qr);
    }
    return {nextX, nextY};
}

//Check if the qr code is valid. If it is valid return 2, if it is not valid return 0 (can´t proceed), if it is valid but not complete return 1
//TODO: corrigir as falhas de validação. nao deteta todos os invalidos
int isValid(qr_comp qr_comp, vector<vector<int>> qr){
    int n = qr_comp.n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (qr_comp.lt[i] > qr_comp.lb[i]*2){
                return 0;
            }
            if (qr_comp.ct[j] > qr_comp.cb[j]*2){
                return 0;
            }
            
            /*
            if (j != n-1  && (((qr[i][j] == 0 || qr[i][j] == -1) && qr[i][j+1] == 1) || (qr[i][j] == 1 && (qr[i][j+1] == 0 || qr[i][j+1] == -1)))){
                qr_comp.lt[i]--;
            } 
            if (i != n-1   && (((qr[i+1][j] == 0 || qr[i+1][j] == -1) && qr[i][j] == 1) || (qr[i+1][j] == 1 && (qr[i][j] == 0 || qr[i][j] == -1)))){
                qr_comp.ct[j]--;
            }
            */

            if (j != n-1  && (!(qr[i][j]-1) != !(qr[i][j+1]-1))){
                qr_comp.lt[i]--;
            } 
            if (i != n-1  && (!(qr[i][j]-1) != !(qr[i+1][j]-1))){
                qr_comp.ct[j]--;
            }


            if (qr[i][j] == 1){
                qr_comp.lb[i]--;
                if (qr_comp.lb[i] < 0) return 0;
                qr_comp.cb[j]--;
                if (qr_comp.cb[j] < 0) return 0;
                if (i == j){
                    qr_comp.db[0]--;
                    if (qr_comp.db[0] < 0) return 0;
                }
                if (n - i - 1 == j ){
                    qr_comp.db[1]--;
                    if (qr_comp.db[1] < 0) return 0;
                }
                if ((i+1) <= floor(n/2) && (j+1) > floor(n/2)){
                    qr_comp.qb[0]--;
                    if (qr_comp.qb[0] < 0) return 0;
                }
                if ((i+1) <= floor(n/2) && (j+1) <= floor(n/2)){
                    qr_comp.qb[1]--;
                    if (qr_comp.qb[1] < 0) return 0;
                }
                if ((i+1) > floor(n/2) && (j+1) <= floor(n/2)){
                    qr_comp.qb[2]--;
                    if (qr_comp.qb[2] < 0) return 0;
                }
                if ((i+1) > floor(n/2) && (j+1) > floor(n/2)){
                    qr_comp.qb[3]--;
                    if (qr_comp.qb[3] < 0) return 0;
                }
            }
        }
    }

    /*
    cout << "lb:" << qr_comp.lb[0] << " " << qr_comp.lb[1] << " " << qr_comp.lb[2] << " " << qr_comp.lb[3] << endl;
    cout << "cb:" << qr_comp.cb[0] << " " << qr_comp.cb[1] << " " << qr_comp.cb[2] << " " << qr_comp.cb[3] << endl;
    cout << "lt:" << qr_comp.lt[0] << " " << qr_comp.lt[1] << " " << qr_comp.lt[2] << " " << qr_comp.lt[3] << endl;
    cout << "ct:" << qr_comp.ct[0] << " " << qr_comp.ct[1] << " " << qr_comp.ct[2] << " " << qr_comp.ct[3] << endl;
    cout << "qb:" << qr_comp.qb[0] << " " << qr_comp.qb[1] << " " << qr_comp.qb[2] << " " << qr_comp.qb[3] << endl;
    cout << "db:" << qr_comp.db[0] << " " << qr_comp.db[1] << endl;
    */
    

    for (int i = 0; i < n; i++){
        if (qr_comp.lb[i] != 0 || qr_comp.cb[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < n; i++){
        if (qr_comp.lt[i] != 0 || qr_comp.ct[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < 2; i++){
        if (qr_comp.db[i] != 0){
            return 1;
        }
    }
    for (int i = 0; i < 4; i++){
        if (qr_comp.qb[i] != 0){
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
            }else if(qr[i][j] == -1){
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
    cout << "level: " << level << endl;
    #endif
    if(level > qr_comp.sum){
        #ifdef DEBUG
        cout << "Max level reached" << endl;
        #endif
        return 0;
    }
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

    if(level >= qr_comp.sum) return 0; // aproveitando-me do facto de na nova arvore o nivel de recursao ser igual ao de celulas pintadas - 1
    // generate descendants
    vector<int> next{row, col};
    while((next = nextCell(qr_comp.n, next[0], next[1], qr))[0] != qr_comp.n){
        //if(qr[next[0]][next[1]] == 1)continue;
        qr[next[0]][next[1]] = 1;
        generate_check(qr_comp, qr, valid_qrs, counter, next[0], next[1], level + 1);
        qr[next[0]][next[1]] = 0; // @luis tive q por isto para evitar um wrapper. assim a primeira chamada à funcao pode ser feita com row=-1 e col=n-1
        // aceitam-se ideias melhores
    }
    return 0;
}

int pre_proccess(qr_comp & qr_comp,  vector<vector<int>> & qr){
    int pintadas = 0;
    //Pre process linhas
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.lb[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                if(qr[i][j] == 0) return -1;
                if(qr[i][j] == -1) {
                   pintadas++;
                    qr[i][j] = 1;
                }
                //qr_comp.lb[i]--;
            }        
        }else if(qr_comp.lb[i] == 0){
            for (int j = 0; j < qr_comp.n; j++){
                if(qr[i][j] == 1) return -1;
                qr[i][j] = 0;
                //qr_comp.lb[i]--;
            }        
        }
    }
    //Pre process colunas
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.cb[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                if(qr[j][i] == 0) return -1;
                if(qr[j][i] == -1) {
                   pintadas++;
                    qr[j][i] = 1;
                }
                //qr_comp.cb[i]--;
            }    
        } else if(qr_comp.cb[i] == 0){
            for (int j = 0; j < qr_comp.n; j++){
                if(qr[i][j] == 1) return -1;
                qr[j][i] = 0;
                //qr_comp.cb[i]--;
            }    
        }
    }
    //Pre process diagonais
    for(int i = 0; i < 2; i++){
        if(qr_comp.db[i] == qr_comp.n){
            for (int j = 0; j < qr_comp.n; j++){
                for(int k = 0; k < qr_comp.n; k++){
                    if (i == 0 && j == k){
                        if(qr[j][k] == 0) return -1;
                        if(qr[j][k] == -1)  {
                            pintadas++;
                            qr[j][k] = 1;
                        }
                    }
                    else if (i == 1 && qr_comp.n - j - 1 == k){
                        if(qr[j][qr_comp.n - 1 - k] == 0) return -1;
                        if(qr[j][qr_comp.n - 1 - k] == -1)  {
                            pintadas++;
                            qr[j][qr_comp.n - 1 - k] = 1;
                        }                        
                    }
                    //qr_comp.db[i]--;
                }
            }    
        }else if(qr_comp.db[i] == 0){
            for (int j = 0; j < qr_comp.n; j++){
                for(int k = 0; k < qr_comp.n; k++){
                    if (i == 0 && j == k){
                        if(qr[j][k] == 1) return -1;
                        qr[j][k] = 0;
                    }
                    else if (i == 1 && qr_comp.n - j - 1 == k ){
                        if(qr[j][qr_comp.n - 1 - k] == 1) return -1;
                        qr[j][qr_comp.n - 1 - k] = 1; // qr[j][k] = 0; //alterei esta linha q me pareceu q estava mal. vê o que achas. acho q nos enganamos a copiar de cima. na antidiagonal acho q é (j, n-1-k)
                    }
                    //qr_comp.db[i]--;
                }
            }    
        }
    }
    return pintadas;
}

int main(){
    int n_qr;
    cin >> n_qr;
    

    //Read compressed qr code characteristics
    while(n_qr != 0){
        n_qr--;
        qr_comp qr_comp;
        int value;
        int sum = 0;
        //Read size of the qr code
        cin >> qr_comp.n;
        //Read the amount of black squares in each row
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.lb.push_back(value);
            sum += value;
        }
        qr_comp.sum = sum;

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

        vector<vector<int>> qr(qr_comp.n, vector<int>(qr_comp.n, -1));
        //cout << "BEFORE pre process" << endl;
        //print_qr(qr, qr_comp.n);
        //cout << endl;
        vector<vector<vector<int>>> valid_qrs;
        int counter = 0;
        int pintadas = pre_proccess(qr_comp,qr);
        #ifdef DEBUG
        cout << "Pre process" << endl;
        print_qr(qr, qr_comp.n);
        cout << endl;
        cout << "Falta pintar: " << qr_comp.sum - pintadas << endl;
        #endif
        if(pintadas == -1){
            cout << "DEFECT: No QR Code generated!" << endl;
            continue;
        }
        generate_check(qr_comp, qr, valid_qrs, counter, -1, qr_comp.n - 1, pintadas);

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