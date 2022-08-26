#ifndef SOLVER_H  //他で「SOLVER_H」が定義済、（solver.hがインクルード）されていれば名前の衝突を避けるためendifまでジャンプ
#define SOLVER_H
typedef int Board_t[9][9];
void printBoard( Board_t bd );
int rsolver( Board_t bd );
#endif
