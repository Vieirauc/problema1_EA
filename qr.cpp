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

int getQuadrant(int n, int row, int col){
    if(row+1 <= n/2) { //quadrantes 1 e 2
        if(col+1 > n/2) return 0;
        else return 1;
    } else { //quadrantes 3 e 4
        if(col+1 <= n/2) return 2;
        else return 3;
    }
}

//APENAS PARA SER USADA DENTRO DO PREPROCESS!!! (ESTA A ALTERAR OS VALORES DE LB, CB, ETC ETC)
void fillCell(vector<vector<int>> & qr, qr_comp & qr_comp, int row, int col, int value, int & newCellFilledFlag){
    newCellFilledFlag++;
    qr[row][col] = value;
    qr_comp.indef_l[row]--;
    qr_comp.indef_c[col]--;
    //indefs das diagonais decrementados dentro dos ifs abaixo
    int quadrante = getQuadrant(qr_comp.n, row, col);
    qr_comp.indef_q[quadrante]--;



    //visto q estou a passar por copia, posso decrementar tambem o numero de pretos na linha e coluna, para manter o numero de pretos q falta preencher e nao o numero total
    if(value == 1){ //se for preto
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
    }
}

//TODO: trocar as referencias a filas vazias ou cheias de 0 para n - indef e de n para indef, respetivamente
//TODO: dar update ao preprocessing das diagonais e acrescentar o dos quadrantes
//TODO: pensar se a preprocess devera ser chamada em cada chamada recursiva a generate_check ou se devera ser ela propria recusiva, ou os dois
int  pre_proccess(qr_comp qr_comp,  vector<vector<int>> & qr){
    int newCellFilledFlag = 0;


    /*#ifdef DEBUG
    for(int i = 0; i < qr_comp.n; i++){
        cout << "lb[" << i << "] = " << qr_comp.lb[i] << endl;
        cout << "cb[" << i << "] = " << qr_comp.cb[i] << endl;
        cout << "lw[" << i << "] = " << qr_comp.lw[i] << endl;
        cout << "cw[" << i << "] = " << qr_comp.cw[i] << endl;
        cout << "indef_l[" << i << "] = " << qr_comp.indef_l[i] << endl;
        cout << "indef_c[" << i << "] = " << qr_comp.indef_c[i] << endl;
        cout << "lt[" << i << "] = " << qr_comp.lt[i] << endl;
        cout << "ct[" << i << "] = " << qr_comp.ct[i] << endl;
    }
    cout << "db[0] = " << qr_comp.db[0] << endl;
    cout << "db[1] = " << qr_comp.db[1] << endl;
    cout << "dw[0] = " << qr_comp.dw[0] << endl;
    cout << "dw[1] = " << qr_comp.dw[1] << endl;
    cout << "indef_d[0] = " << qr_comp.indef_d[0] << endl;
    cout << "indef_d[1] = " << qr_comp.indef_d[1] << endl;
    #endif*/
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
            }
        }else if(qr_comp.lw[i] == qr_comp.indef_l[i]){ //se o numero de brancos na linha for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                //if(qr_comp.lw[i] == 0) break;
                // if(qr[i][j] == 1) return -1;
                if(qr[i][j] == -1){
                    fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                }
            }        
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
            }    
        } else if(qr_comp.cw[i] == qr_comp.indef_c[i]){ //se o numero de brancos na coluna for igual ao numero de indefinidos
            for (int j = 0; j < qr_comp.n; j++){
                // if(qr_comp.cw[i] == 0) break; //se o numero de brancos na coluna for 0, nao ha mais brancos para pintar (nao ha mais indefinidos
                // if(qr[j][i] == 1) return -1;
                if(qr[j][i] == -1) {
                    fillCell(qr, qr_comp, j, i, 0, newCellFilledFlag);
                }
            }    
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
            }    
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
            }    
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
                }
            } else if(q == 1) {//quadrante superior esquerdo
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                }
            } else if(q == 2) {//quadrante inferior esquerdo
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                }
            } else if(q == 3) {//quadrante inferior direito
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qb[q] == 0) break;
                        //if(qr[i][j] == 0) return -1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 1, newCellFilledFlag);
                        }
                    }
                }
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
                }
            } else if(q == 1) {//quadrante superior esquerdo
                for(int i = 0; i < qr_comp.n/2; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                }
            } else if(q == 2) {//quadrante inferior esquerdo
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = 0; j < qr_comp.n/2; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                }
            } else if(q == 3) {//quadrante inferior direito
                for(int i = qr_comp.n/2; i < qr_comp.n; i++) {
                    for(int j = qr_comp.n/2; j < qr_comp.n; j++) {
                        //if(qr_comp.qw[q] == 0) break;
                        //if(qr[i][j] == 1) return 1;
                        if(qr[i][j] == -1) {
                            fillCell(qr, qr_comp, i, j, 0, newCellFilledFlag);
                        }
                    }
                }
            }
        }
    }
    
    //primeiro acrescentar o codigo em fillCell



    #ifdef DEBUG
    cout << "Pre process" << endl;
    print_qr(qr, qr_comp.n);
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
int isValid(qr_comp & qr_comp, vector<vector<int>> qr){
    int n = qr_comp.n;
    for (int i = 0; i < n; i++){ //iterar linhas
        for (int j = 0; j < n; j++){ //iterar colunas
            //transicoes
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

            //pretos
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
                if((i+1) <= n/2){
                    if((j+1) > n/2){
                        qr_comp.qb[0]--;
                        if (qr_comp.qb[0] < 0) return 0;
                    } else {
                        qr_comp.qb[1]--;
                        if (qr_comp.qb[1] < 0) return 0;
                    }
                } else {
                    if((j+1) <= n/2){
                        qr_comp.qb[2]--;
                        if (qr_comp.qb[2] < 0) return 0;
                    } else {
                        qr_comp.qb[3]--;
                        if (qr_comp.qb[3] < 0) return 0;
                    }
                }

            //brancos
            } else if (qr[i][j] == 0){
                qr_comp.lw[i]--;
                if (qr_comp.lw[i] < 0) return 0;
                qr_comp.cw[j]--;
                if (qr_comp.cw[j] < 0) return 0;
                //TODO: adicionar validacao para as diagonais e quadrantes, mas antes tratar da sua inicializacao e preprocessing
                //diagonais
                if (i == j){
                    qr_comp.dw[0]--;
                    if (qr_comp.dw[0] < 0) return 0;
                }
                if (n - i - 1 == j ){
                    qr_comp.dw[1]--;
                    if (qr_comp.dw[1] < 0) return 0;
                }

                //quadrantes
                if((i+1) <= n/2){
                    if((j+1) > n/2){
                        qr_comp.qw[0]--;
                        if (qr_comp.qw[0] < 0) return 0;
                    } else {
                        qr_comp.qw[1]--;
                        if (qr_comp.qw[1] < 0) return 0;
                    }
                } else {
                    if((j+1) <= n/2){
                        qr_comp.qw[2]--;
                        if (qr_comp.qw[2] < 0) return 0;
                    } else {
                        qr_comp.qw[3]--;
                        if (qr_comp.qw[3] < 0) return 0;
                    }
                }
            }
        } //fim de uma iteracao de colunas
        
    } //fim de uma iteracao de linhas

    // for(int i = 0; i < qr_comp.n; i++){
    //     cout << qr_comp.lb[i] << " ";
    // }
    // cout << endl;
    
    
    

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




//TODO fazer backtracking ( validar se pode pintar a preto ou a branco , caso se puder pinta senão volta atrás)
//Fuction that fills the empty spaces of the qr code verifying if it can paint it black or white using backtracking
int generate_check(qr_comp & original_qr_comp, vector<vector<int>> qr, vector<vector<vector<int>>> & valid_qrs, int & counter, int row, int col, int level){
    #ifdef DEBUG
    cout << "level: " << level << endl;
    cout << "row: " << row << " col: " << col << endl;
    print_qr(qr, original_qr_comp.n);
    #endif
    
    // check if the qr code is valid
    qr_comp copy_qr_comp(original_qr_comp);
    int valid = isValid(copy_qr_comp, qr);
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

    //limite de profundidade
    // if(level == qr_comp.sum) return 0;
    // generate descendants
    vector<int> next{row, col};
    #ifdef DEBUG
    for(int i = 0; i < copy_qr_comp.n; i++){
        cout << copy_qr_comp.lb[i] << " ";
    }
    cout << endl;
    #endif
    while((next = nextCell(original_qr_comp.n, next[0], next[1], qr))[0] != original_qr_comp.n){
        //if(copy_qr_comp.lb[next[0]] == 0 || copy_qr_comp.cb[next[1]] == 0) continue;// || copy_qr_comp.qb[getQuadrant(original_qr_comp.n, next[0], next[1])]) continue;
        
        //print_qr(qr, original_qr_comp.n);
        vector<vector<int>> copy_qr(qr);
        // qr[next[0]][next[1]] = 1;
        int newCellFilled;
        fillCell(copy_qr, copy_qr_comp, next[0], next[1], 1, newCellFilled);
        generate_check(original_qr_comp, copy_qr, valid_qrs, counter, next[0], next[1], level + 1);
        qr[next[0]][next[1]] = 0; // @luis tive q por isto para evitar um wrapper. assim a primeira chamada à funcao pode ser feita com row=-1 e col=n-1
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

        

        vector<vector<int>> qr(qr_comp.n, vector<int>(qr_comp.n, -1));
        //cout << "BEFORE pre process" << endl;
        //print_qr(qr, qr_comp.n);
        //cout << endl;
        vector<vector<vector<int>>> valid_qrs;
        int counter = 0;
       
        if(pre_proccess(qr_comp,qr) == -1) {
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
        print_qr(qr, qr_comp.n);
        #endif


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