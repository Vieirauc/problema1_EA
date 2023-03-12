// Static size array saved on the stack
#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

// #define DEBUG

using namespace std;

struct qr_comp{
    int n;
    //pretos
    vector<int> lb;
    vector<int> cb;
    vector<int> qb;
    vector<int> db;
    //soma dos pretos
    int sum;
    //brancos
    vector<int> lw;
    vector<int> cw;
    vector<int> qw;
    vector<int> dw;
    //indefinidos
    vector<int> indef_l; //indefinidos em cada linha
    vector<int> indef_c; //indefinidos em cada coluna
    vector<int> indef_q; //indefinidos em cada quadrante
    vector<int> indef_d; //indefinidos em cada diagonal
    //transicoes
    vector<int> lt;
    vector<int> ct;
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

int getQuadrant(int n, int row, int col){
    if(row+1 <= n/2) { //quadrantes 1 e 2
        if(col+1 > n/2) return 0;
        else return 1;
    } else { //quadrantes 3 e 4
        if(col+1 <= n/2) return 2;
        else return 3;
    }
}

void print_qr(vector<vector<int>> qr, int n, bool inverted){
    cout << "+";
    for (int i = 0; i < n; i++){
        cout << "-";
    }
    cout << "+" << endl;
    if(!inverted) {
        for (int i = 0; i < n; i++){
            cout << "|";
            for (int j = 0; j < n; j++){
                if(qr[i][j] == 1){
                    cout << "#";
                }else if(qr[i][j] == 0){
                    cout << "0";
                }else if(qr[i][j] == -1){
                    cout << " ";
                }
            }
            cout << "|" << endl;
        } 
    } else {
        for (int i = 0; i < n; i++){
            cout << "|";
            for (int j = 0; j < n; j++){
                if(qr[i][j] == 1){
                    cout << " ";
                }else if(qr[i][j] == 0){
                    cout << "#"; //aqui tanto o branco como o indef têm que ser #
                }else if(qr[i][j] == -1){
                    cout << "#"; //aqui tanto o branco como o indef têm que ser #
                }
            }
            cout << "|" << endl;
        } 
    }
    

    cout << "+";
    for (int i = 0; i < n; i++){
        cout << "-";
    }
    cout << "+" << endl;
}

void print_qr_comp(qr_comp qr_comp, int row, int col) { //para nbao dar destaque basta por row e col a -1
    cout << "lb: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == row)
        cout << "|" << qr_comp.lb[i] << "|" << " ";
        else
            cout << qr_comp.lb[i] << " ";
    }
    cout << endl;
    cout << "cb: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == col)
            cout << "|" << qr_comp.cb[i] << "|" << " ";
        else
            cout << qr_comp.cb[i] << " ";
    }
    cout << endl;
    cout << "lt: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == row)
            cout << "|" << qr_comp.lt[i] << "|" << " ";
        else
            cout << qr_comp.lt[i] << " ";
    }
    cout << endl;
    cout << "ct: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == col)
            cout << "|" << qr_comp.ct[i] << "|" << " ";
        else
            cout << qr_comp.ct[i] << " ";
    }
    cout << endl;
    cout << "qb: ";
    for (int i = 0; i < 4; i++){
        if(i == getQuadrant(qr_comp.n, row, col))
            cout << "|" << qr_comp.qb[i] << "|" << " ";
        else
            cout << qr_comp.qb[i] << " ";
    }
    cout << endl;
    cout << "db: ";
    for (int i = 0; i < 2; i++){
        if((row == col && i == 0) || (row + col == qr_comp.n - 1 && i == 1))
            cout << "|" << qr_comp.db[i] << "|" << " ";
        else
            cout << qr_comp.db[i] << " ";
    }
    cout << endl;
    cout << "sum: " << qr_comp.sum << endl;
    cout << "lw: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == row)
            cout << "|" << qr_comp.lw[i] << "|" << " ";
        else
            cout << qr_comp.lw[i] << " ";
    }
    cout << endl;
    cout << "cw: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == col)
            cout << "|" << qr_comp.cw[i] << "|" << " ";
        else
            cout << qr_comp.cw[i] << " ";
    }
    cout << endl;
    cout << "qw: ";
    for (int i = 0; i < 4; i++){
        if(i == getQuadrant(qr_comp.n, row, col))
            cout << "|" << qr_comp.qw[i] << "|" << " ";
        else
            cout << qr_comp.qw[i] << " ";
    }
    cout << endl;
    cout << "dw: ";
    for (int i = 0; i < 2; i++){
        if((row == col && i == 0) || (row + col == qr_comp.n - 1 && i == 1))
            cout << "|" << qr_comp.dw[i] << "|" << " ";
        else
            cout << qr_comp.dw[i] << " ";
    }
    cout << endl;
    cout << "indef_l: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == row)
            cout << "|" << qr_comp.indef_l[i] << "|" << " ";
        else
            cout << qr_comp.indef_l[i] << " ";
    }
    cout << endl;
    cout << "indef_c: ";
    for (int i = 0; i < qr_comp.n; i++){
        if(i == col)
            cout << "|" << qr_comp.indef_c[i] << "|" << " ";
        else
            cout << qr_comp.indef_c[i] << " ";
    }
    cout << endl;
    cout << "indef_q: ";
    for (int i = 0; i < 4; i++){
        if(i == getQuadrant(qr_comp.n, row, col))
            cout << "|" << qr_comp.indef_q[i] << "|" << " ";
        else
            cout << qr_comp.indef_q[i] << " ";
    }
    cout << endl;
    cout << "indef_d: ";
    for (int i = 0; i < 2; i++){
        if((row == col && i == 0) || (row + col == qr_comp.n - 1 && i == 1))
            cout << "|" << qr_comp.indef_d[i] << "|" << " ";
        else
            cout << qr_comp.indef_d[i] << " ";
    }
    cout << endl;
    
}



//APENAS PARA SER USADA SOBRE CÉLULAS A -1!!! (ESTA A ALTERAR OS VALORES DE LB, CB, ETC ETC)
void fillCell(vector<vector<int>> & qr, qr_comp & qr_comp, int row, int col, int value, int & newCellFilledFlag){
    /*cout << "-------------------" << endl;
    cout << "Antes de fill cell: " << endl;
    print_qr(qr, qr_comp.n, false);
    print_qr_comp(qr_comp, row, col);
    cout << "---" << endl;*/
    
    newCellFilledFlag++;
    qr[row][col] = value;
    qr_comp.indef_l[row]--;
    qr_comp.indef_c[col]--;
    //indefs das diagonais decrementados dentro dos ifs abaixo
    int quadrante = getQuadrant(qr_comp.n, row, col);
    qr_comp.indef_q[quadrante]--;




    //visto q estou a passar por copia, posso decrementar tambem o numero de pretos na linha e coluna, para manter o numero de pretos q falta preencher e nao o numero total
    if(value == 1){ //se for preto //se for preto, tambem é preciso ter em conta a possibilidade de transições
        qr_comp.lb[row]--;
        qr_comp.cb[col]--;
        if(row == col) {
            qr_comp.db[0]--;
            qr_comp.indef_d[0]--;
        }
        if(row + col == qr_comp.n - 1){//se for a celula do meio vai decrementar para as duas diagonais, e é suposto
            qr_comp.db[1]--;
            qr_comp.indef_d[1]--;
        }
        qr_comp.qb[quadrante]--;
        //tratar das transições
        //colunas


        if(row > 0 && row < qr_comp.n - 1) {
            if(qr[row - 1][col] == 1) qr_comp.ct[col]++;
            else qr_comp.ct[col]--;
            if(qr[row + 1][col] == 1) qr_comp.ct[col]++;
            else qr_comp.ct[col]--;
        }
        else if(row == qr_comp.n - 1) {
            if(qr[row - 1][col] == 1) qr_comp.ct[col]++;
            else qr_comp.ct[col]--;
        }
        else if(row == 0) {
            if(qr[1][col] == 1) qr_comp.ct[col]++;
            else qr_comp.ct[col]--;
        }

        //linhas
        
        if(col > 0 && col < qr_comp.n - 1) {
            if(qr[row][col - 1] == 1) qr_comp.lt[row]++;
            else qr_comp.lt[row]--;
            if(qr[row][col + 1] == 1) qr_comp.lt[row]++;
            else qr_comp.lt[row]--;
        }
        else if(col == qr_comp.n - 1) {
            if(qr[row][col - 1] == 1) qr_comp.lt[row]++;
            else qr_comp.lt[row]--;
        }
        else if(col == 0) {
            if(qr[row][1] == 1) qr_comp.lt[row]++;
            else qr_comp.lt[row]--;
        }
        
    } else if(value == 0){ //se for branco
        qr_comp.lw[row]--;
        qr_comp.cw[col]--;
        if(row == col){
            qr_comp.dw[0]--;
            qr_comp.indef_d[0]--;
        }
        if(row + col == qr_comp.n - 1) {
            qr_comp.dw[1]--;
            qr_comp.indef_d[1]--;
        }
        qr_comp.qw[quadrante]--;
    }/*
    cout << "Depois de fill cell: " << endl;
    print_qr_comp(qr_comp, row, col);
    print_qr(qr, qr_comp.n, false);
    cout << "-------------------" << endl;*/
}

//TODO: trocar as referencias a filas vazias ou cheias de 0 para n - indef e de n para indef, respetivamente
//TODO: dar update ao preprocessing das diagonais e acrescentar o dos quadrantes
//TODO: pensar se a preprocess devera ser chamada em cada chamada recursiva a generate_check ou se devera ser ela propria recusiva, ou os dois
int  pre_proccess(qr_comp & qr_comp,  vector<vector<int>> & qr){
    int newCellFilledFlag = 0;
    //Pre process linhas //i é row e j é col
    //inicialmente indef = n
    //depois, indef decrementa sempre q se pinta uma celula, quer de preto quer de branco
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.lb[i] == qr_comp.indef_l[i]){ //se o numero de pretos na linha for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                //if(qr_comp.lb[i] == 0) break;
                //if(qr[i][j] == 0) return -1;
                if (qr[i][j] == -1) { //TODO: talvez extrair este if para uma funcao para n estar sempre a repetir codigo
                    fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                }
            } if(qr_comp.lb[i] != 0) return -1;
        }else if(qr_comp.lw[i] == qr_comp.indef_l[i]){ //se o numero de brancos na linha for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                //if(qr_comp.lw[i] == 0) break;
                // if(qr[i][j] == 1) return -1;
                if(qr[i][j] == -1){
                    fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                }
            } if(qr_comp.lw[i] != 0) return -1;
        }
    }
    
    //Pre process colunas //j é row e i é col
    for (int i = 0; i < qr_comp.n; i++){
        if(qr_comp.cb[i] == qr_comp.indef_c[i]){ //se o numero de pretos na coluna for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                // if(qr_comp.cb[i] == 0) break; //se o numero de pretos na coluna for 0, nao ha mais pretos para pintar (nao ha mais indefinidos
                // if(qr[j][i] == 0) return -1;
                if(qr[j][i] == -1) {
                    fillCell(qr, qr_comp, j, i, 1, newCellFilledFlag);
                }
            } if(qr_comp.cb[i] != 0) return -1;
        } else if(qr_comp.cw[i] == qr_comp.indef_c[i]){ //se o numero de brancos na coluna for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                // if(qr_comp.cw[i] == 0) break; //se o numero de brancos na coluna for 0, nao ha mais brancos para pintar (nao ha mais indefinidos
                // if(qr[j][i] == 1) return -1;
                if(qr[j][i] == -1) {
                    fillCell(qr, qr_comp, j, i, 0, newCellFilledFlag);
                }
            } if(qr_comp.cw[i] != 0) return -1;
        }
    }
    
    //Pre process diagonais
    //i = 0 -> diagonal principal, i = 1 -> diagonal secundaria, j = row e k = col
    for(int i = 0; i < 2; i++){
        if(qr_comp.db[i] == qr_comp.indef_d[i]){ //se o numero de pretos na diagonal for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                for(int k = 0; k < qr_comp.n; k++){
                    if (i == 0 && j == k){ //se for a diagonal principal
                        // if(qr_comp.db[i] == 0) break; //se o numero de pretos na diagonal for 0, nao ha mais pretos para pintar (nao ha mais indefinidos)
                        // if(qr[j][k] == 0) return -1;
                        if(qr[j][k] == -1) {
                            fillCell(qr, qr_comp, j, k, 1, newCellFilledFlag);
                        }
                    }
                    else if (i == 1 && qr_comp.n - j - 1 == k){ //se for a diagonal secundaria
                        // if(qr_comp.db[i] == 0) break; //se o numero de pretos na diagonal for 0, nao ha mais pretos para pintar (nao ha mais indefinidos
                        // if(qr[j][qr_comp.n - 1 - k] == 0) return -1;
                        if(qr[j][k] == -1) {
                            fillCell(qr, qr_comp, j, k, 1, newCellFilledFlag);
                        }
                    }
                }
            } if(qr_comp.db[i] != 0) return -1;
        }else if(qr_comp.dw[i] == qr_comp.indef_d[i]){ //se o numero de brancos na diagonal for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                for(int k = 0; k < qr_comp.n; k++){
                    if (i == 0 && j == k){
                        // if(qr[j][k] == 1) return 1;
                        if(qr[j][k] == -1) {
                            fillCell(qr, qr_comp, j, k, 0, newCellFilledFlag);
                        }
                    }
                    else if (i == 1 && qr_comp.n - j - 1 == k ){
                        // if(qr[j][qr_comp.n - 1 - k] == 1) return -1;
                        if(qr[j][k] == -1) {
                            fillCell(qr, qr_comp, j, k, 0, newCellFilledFlag);
                        }
                    }
                    //qr_comp.db[i]--;
                }
            } if(qr_comp.dw[i] != 0) return -1;
        }
    }

    //pre process quadrantes
    for(int q = 0; q < 4; q++) { //q é o quadrante
        if(qr_comp.qb[q] == qr_comp.indef_q[q]) { //se o numero de pretos for igual ao numero de indefinidos no quadrante
            if(q == 0) {//quadrante superior direito
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qb[0] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 1) {//quadrante superior esquerdo
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qb[1] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 2) {//quadrante inferior esquerdo
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qb[2] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 3) {//quadrante inferior direito
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qb[3] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            }
        } else if(qr_comp.qw[q] == qr_comp.indef_q[q]) { //se o numero de brancos for igual ao numero de indefinidos no quadrante
            if(q == 0) {//quadrante superior direito
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qw[0] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 1) {//quadrante superior esquerdo
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qw[1] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 2) {//quadrante inferior esquerdo
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qw[2] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            } else if(q == 3) {//quadrante inferior direito
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                } if(qr_comp.qw[3] != 0) return -1; //se nao for possivel preencher todos entao é invalido
            }
        }
    }
    



    #ifdef DEBUG
    cout << "Pre process" << endl;
    print_qr(qr, qr_comp.n, false);
    cout << "Filled cells: " << newCellFilledFlag << endl;
    for(int i = 0; i < 2; i++){
        cout << qr_comp.db[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < 2; i++){
        cout << qr_comp.indef_d[i] << " ";
    }
    cout << endl;
    #endif
    
    //se preencheu alguma celula, entra em recursao com o qr_comp atualizado
    if(newCellFilledFlag) return pre_proccess(qr_comp, qr);

    return newCellFilledFlag;
}



//Check if the qr code is valid. If it is valid return 2, if it is not valid return 0 (can´t proceed), if it is valid but not complete return 1
//TODO: corrigir as falhas de validação. nao deteta todos os invalidos
//TODO: adicionar validacao para os brancos tb, neste momento estao a passar ao lado
//*****AVISO****** esta isValid() NAO PODE ALTERAR NADA NO QR_COMP
int isValid(qr_comp & qr_comp) {//TODO: , int & level, int & sum){
    int n = qr_comp.n;
    #ifdef DEBUG
    cout << "Validating" << endl;
    print_qr_comp(qr_comp, -1, -1);
    #endif
    //verificar pretos e brancos e transicoes q faltam
    for(int i = 0; i < n; i++){
        if(qr_comp.lb[i] < 0) return 0;
        if(qr_comp.lw[i] < 0) return 0;
        if(qr_comp.cb[i] < 0) return 0;
        if(qr_comp.cw[i] < 0) return 0;
        //transicoes
        //if(qr_comp.lt[i] < -2) return 0;
        //if(qr_comp.ct[i] < -2) return 0;
        if (qr_comp.lt[i] > qr_comp.lb[i]*2) return 0;
        if (qr_comp.ct[i] > qr_comp.cb[i]*2) return 0;
    }
    for(int i = 0; i < 2; i++){
        if(qr_comp.db[i] < 0) return 0;
        if(qr_comp.dw[i] < 0) return 0;
    }
    for(int i = 0; i < 4; i++){
        if(qr_comp.qb[i] < 0) return 0;
        if(qr_comp.qw[i] < 0) return 0;
    }

    //verificar se, nao havendo invalidez, e estando no nivel correto, esta tudo certo
    //TODO: if(level == sum) {
        for(int i = 0; i < n; i++){
            if(qr_comp.lb[i] != 0) return 1;
            if(qr_comp.cb[i] != 0) return 1;
            //transicoes
            if (qr_comp.lt[i] != 0) return 1;
            if (qr_comp.ct[i] != 0) return 1;
        }
        for(int i = 0; i < 2; i++){
            if(qr_comp.db[i] != 0) return 1;
        }
        for(int i = 0; i < 4; i++){
            if(qr_comp.qb[i] != 0) return 1;
        }
        return 2; //se chegou aqui, esta tudo certo
    //}
    
    //senao, retornar 1 (valido mas ainda nao é o correto)
    return 1;

}

bool isValidDescendant(qr_comp & qr_comp, int row, int col) { //deteta se a celula pode ser pintada de acordo com os pretos disponiveis
    if (qr_comp.lb[row] == 0 || qr_comp.cb[col] == 0) return false;
    if (qr_comp.qb[getQuadrant(qr_comp.n, row, col)] == 0) return false;
    if (row == col && qr_comp.db[0] == 0) return false;
    if (qr_comp.n - row -1 == col && qr_comp.db[1] == 0) return false;
    return true;
}


//TODO fazer backtracking ( validar se pode pintar a preto ou a branco , caso se puder pinta senão volta atrás)
//Fuction that fills the empty spaces of the qr code verifying if it can paint it black or white using backtracking
int generate_check(qr_comp & qr_comp_pai, vector<vector<int>> & qr_pai, vector<vector<int>> & valid_qr, int & counter, int row, int col, int level){
    #ifdef DEBUG
    cout << "level: " << level << endl;
    cout << "row: " << row << " col: " << col << endl;
    print_qr(qr_pai, qr_comp_pai.n, false);
    #endif

    
    // check if the qr code is valid
    //qr_comp copy_qr_comp(original_qr_comp);
    int valid = isValid(qr_comp_pai);//, level, qr_comp.sum);
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
                valid_qr = qr_pai;
            }
            counter++;
            return 1; // @luis aqui parece-me q é return como tinhas mas quero ver isso, n tenho a certeza se estou a pensar bem. os filhos tem obrigatoriamente os pretos dos pais, pelo que acho que mais abaixo na arvore vamos infringir regras (se corresponder ao encoding entao é pq ja nao pode ter mais pretos)
    }

    //limite de profundidade
    // if(level == qr_comp.sum) return 0;
    // generate descendants
    vector<int> next{row, col};
    
    while((next = nextCell(qr_comp_pai.n, next[0], next[1], qr_pai))[0] != qr_comp_pai.n){
        //if(isValidDescendant(copy_qr_comp, next[0], next[1]) == false) continue;
        
        //print_qr(qr, original_qr_comp.n, false);
        
        // qr[next[0]][next[1]] = 1;
        vector<vector<int>> qr_filho(qr_pai);
        qr_comp qr_comp_filho(qr_comp_pai);
        int newCellFilled;
        fillCell(qr_filho, qr_comp_filho, next[0], next[1], 1, newCellFilled);
        //print_qr_comp(qr_comp_filho, next[0], next[1]);
        //print_qr(qr_filho, qr_comp_filho.n, false);
        // dar um preprocess ao filho acabado de gerar para saltar os invalidos
        int cellsFilled = 0;
        if((cellsFilled = pre_proccess(qr_comp_filho, qr_filho)) == -1) {
            #ifdef DEBUG
            cout << "Invalid on preprocessing" << endl;
            #endif
            continue;
        } else if(cellsFilled > 0) {
            #ifdef DEBUG
            cout << "Used preprocessing" << endl;
            #endif
        }



        level++;
        generate_check(qr_comp_filho, qr_filho, valid_qr, counter, next[0], next[1], level);
        fillCell(qr_pai, qr_comp_pai, next[0], next[1], 0, newCellFilled);//qr[next[0]][next[1]] = 0; // @luis tive q por isto para evitar um wrapper. assim a primeira chamada à funcao pode ser feita com row=-1 e col=n-1
        // aceitam-se ideias melhores
    }
    return 0;
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


        // initialize indefs
        qr_comp.indef_l.assign(qr_comp.n, qr_comp.n); //n valores a n porque o qr é quadrado e no inicio todos sao indefs
        qr_comp.indef_c.assign(qr_comp.n, qr_comp.n);
        qr_comp.indef_d.assign(2, qr_comp.n);
        // para os quadrantes nao é tao facil pq cada quadrante tem um tamanho diferente
        int l_quadrante, a_quadrante; //largura, altura
        for(int i = 1; i <= 4; i++){
            l_quadrante = a_quadrante = qr_comp.n/2;
            if(i == 1 || i == 4) l_quadrante += qr_comp.n%2;
            if(i == 3 || i == 4) a_quadrante += qr_comp.n%2;
            qr_comp.indef_q.push_back(l_quadrante * a_quadrante);
        }


        //Read the amount of black squares in each row
        int sum = 0;
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.lb.push_back(value);
            qr_comp.lw.push_back(qr_comp.n - value);
            sum += value;
        }
        qr_comp.sum = sum;


        //Read the amount of black squares in each column
        for(int i = 0; i < qr_comp.n; i++){
            cin >> value;
            qr_comp.cb.push_back(value);
            qr_comp.cw.push_back(qr_comp.n - value);
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
            for(int j = 0; j < 4; j++){
                if(i == j) qr_comp.qw.push_back(qr_comp.indef_q[j] - value);
            }
        }

        //Read the amount of transitions in each diagonal - main  and antidiagonal
        for(int i = 0; i < 2; i++){
            cin >> value;
            qr_comp.db.push_back(value);
            qr_comp.dw.push_back(qr_comp.n - value);
        }

        //Detect if the qr code is more black or more white and invert if the later is true (isValid and others get considerably faster)
        bool inverted = false;
        int aux;
        if(qr_comp.sum > qr_comp.n * qr_comp.n / 2){
            inverted = true;
            #ifdef DEBUG
            cout << "QR is mostly white: Solving inverted" << endl;
            #endif
            qr_comp.sum = qr_comp.n * qr_comp.n - qr_comp.sum;
            for(int i = 0; i < qr_comp.n; i++){
                aux = qr_comp.lb[i];
                qr_comp.lb[i] = qr_comp.lw[i];
                qr_comp.lw[i] = aux;

                aux = qr_comp.cb[i];
                qr_comp.cb[i] = qr_comp.cw[i];
                qr_comp.cw[i] = aux;
            }
            for(int i = 0; i < 4; i++){
                aux = qr_comp.qb[i];
                qr_comp.qb[i] = qr_comp.qw[i];
                qr_comp.qw[i] = aux;
            }
            for(int i = 0; i < 2; i++){
                aux = qr_comp.db[i];
                qr_comp.db[i] = qr_comp.dw[i];
                qr_comp.dw[i] = aux;
            }
        }
        

        vector<vector<int>> qr(qr_comp.n, vector<int>(qr_comp.n, -1));
        //cout << "BEFORE pre process" << endl;
        //print_qr(qr, qr_comp.n, false);
        //cout << endl;
        vector<vector<int>> valid_qr;
        int counter = 0;
       
        int cellsFilled = 0;
        if((cellsFilled = pre_proccess(qr_comp,qr)) == -1) {
            cout << "DEFECT: No QR Code generated!" << endl;
            continue;
        }


        /*vector<vector<int>> vect {
            {0,0,0,0,0,0,-1,0,-1,-1,-1,-1},
            {0,0,0,0,0,0,-1,0,-1,-1,-1,-1},
            {0,0,0,0,0,0,-1,0,-1,-1,-1,-1},
            {0,0,0,0,0,0,-1,0,-1,-1,-1,-1},
            {0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0},
            {-1,-1,-1,0,0,0,-1,0,-1,-1,-1,-1},
            {-1,-1,-1,0,0,0,-1,0,-1,-1,-1,-1},
            {-1,-1,-1,0,0,0,-1,0,-1,-1,-1,-1},
            {-1,-1,-1,0,0,0,-1,0,-1,-1,-1,-1},
            {-1,-1,-1,0,0,0,-1,0,-1,-1,-1,-1},
            {0,0,0,0,0,0,0,0,0,0,0,0}
        };
        qr = vect;*/
        #ifdef DEBUG
        cout << "AFTER pre process" << endl;
        print_qr(qr, qr_comp.n, false);
        print_qr_comp(qr_comp, -1, -1);
        #endif


        generate_check(qr_comp, qr, valid_qr, counter, -1, qr_comp.n - 1, cellsFilled);

        if (counter == 1)
        {
            cout << "VALID: 1 QR Code generated!" << endl;
            //print qr code
            print_qr(valid_qr, qr_comp.n, inverted);
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