#include <stdio.h>
#include "solver.h"
#define ANCI(b) (((b)/3)*3)
#define ANCJ(b) (((b)%3)*3)
#define BLKNO(i,j) (((i)/3)*3+(j)/3)
#define BCI(b,n) (ANCI(b)+((n)/3))
#define BCJ(b,n) (ANCJ(b)+((n)%3))

mes(t)
{
	switch( t ) {
		case 0: printf("正常に解けました。\n"); break;
		case 1: printf("初めから空白セルがありません。\n"); break;
		case -1: printf("途中で行き詰まりました。\n"); break;
		case -2: printf("空白セルを埋めれませんでした。\n"); break;
		case -3: printf("ボードデータに矛盾があります。\n"); break;
		default: ;
	}
}
void printBoard( Board_t bd )  //ボードbdの内容をプリントする
{
	int i, j;

	for( i = 0; i < 9; i++ ){
		if( i%3 == 0 ) printf( "\n" );
		for(j = 0; j < 9; j++ ){	
			if( j%3 == 0 ) printf( " " );
			printf( "%2d", bd[i][j] );
		}
		printf( "\n" );	
	}
}

static void copyBoard( Board_t to, Board_t from )  // from のデータを to にコピーする
{
	int i, j;

	for( i = 0; i < 9; i++ ) {
	    for( j = 0; j < 9; j++ ) {
		to[i][j] = from[i][j];
	    }
	}
}

static int countZero( Board_t bd )  //ボード内の「0」の数を数える
{
	int i, j, cnt;

	cnt = 0;
	for( i = 0; i < 9; i++ ){
		for( j = 0; j < 9; j++ ){
			if( bd[i][j] == 0 ) cnt++;
		}
	}
	return cnt;
}

static void clrGRB( int gyo, int rts, Board_t map )  //セルを含むmap上の行、列、ブロックを0クリアする
{
	int i, b;

	for( i = 0; i < 9; i++ ){
	   map[gyo][i] = 0;
	}
	for( i = 0; i < 9; i++ ){
	   map[i][rts] = 0;
	}
	b = BLKNO( gyo, rts );
	for( i = 0; i < 9; i++ ){
	   map[BCI(b,i)][BCJ(b,i)] = 0;
	}
}

static int countG( Board_t bm, int dgt, int g )  // bd または map の g 行目にいくつの dgt が含まれているかを数える
{
	int i, cnt;

	for( cnt = i = 0; i < 9; i++ )
	    if( bm[g][i] == dgt ) cnt++;
	return cnt;
}

static int countR( Board_t bm, int dgt, int r )  // bd または map の r 列目にいくつの dgt が含まれているかを数える
{
	int i, cnt;

	for( cnt = i = 0; i < 9; i++ )
	    if( bm[i][r] == dgt ) cnt++;
	return cnt;
}

static int countB( Board_t bm, int dgt, int b )  // bd または map のブロックにいくつの dgt が含まれているかを数える
{
	int i, cnt;

	for( cnt = i = 0; i < 9; i++ )
	    if( bm[BCI(b,i)][BCJ(b,i)] == dgt ) cnt++;
	return cnt;
}

static int checkMap( Board_t bd, int dgt, Board_t map )  //配置可能な場所が無い。または dgt が２つ以上配置されている
{
	int i, j, cnt;

	for( i = 0; i < 9; i++ ) {
	    cnt = countG( bd, dgt, i );
	    if( cnt > 1 ) return -1;
	    else if( cnt == 0 && countG( map, 1, i ) == 0 ) return -2;
	}
	for( i = 0; i < 9; i++ ) {
	    cnt = countR( bd, dgt, i );
	    if( cnt > 1 ) return -3;
	    else if( cnt == 0 && countR( map, 1, i ) == 0 ) return -4;
	}
	for( i = 0; i < 9; i++ ) {
	    cnt = countB( bd, dgt, i );
	    if( cnt > 1 ) return -5;
	    else if( cnt == 0 && countB( map, 1, i ) == 0 ) return -6;
	}
	return 0;
}

static int makeMap( Board_t bd, int dgt, Board_t map )  //bdの各セルについて、dgtの配置可否をmapに戻す
{
	int i, j;

	for( i = 0; i < 9; i++ ){
	   for( j = 0; j < 9; j++ ){
	      map[i][j] = bd[i][j] ? 0 : 1;
	   }
	}
	for( i = 0; i < 9; i++ ){
	   for( j = 0; j < 9; j++ ){
		if( bd[i][j] == dgt ){
		   clrGRB( i, j, map );
		}
	   }
	}
	return checkMap( bd, dgt, map );
}

static void putDigit( int dgt, Board_t bd, Board_t map )	// map を調べて確定できるセルに dgt を配置する
{
	int i, cnt, g, r, b, c;
 /* 行の中から確定セルを見つける */
	for( g = 0; g < 9; g++ ) {			// g 行目を調べる
	    cnt = 0;
	    for( i = 0; i < 9; i++ ) {			// i 列目を調べる
		if( map[g][i] == 1 ) {			// 配置可能か？
		    if( ++cnt == 1 ) r = i; else break; // 最初だけ場所を覚える
		}
	    }
	    if( cnt == 1 ) {				// １つだけなら
		bd[g][r] = dgt;				// 数字を配置して
		clrGRB( g, r, map );			// map を更新する
	    }
	}
 /* 列の処理 */
	for( r = 0; r < 9; r++ ) {			// r 列目を調べる
	    cnt = 0;
	    for( i = 0; i < 9; i++ ) {
		if( map[i][r] == 1 ) {
		    if( ++cnt == 1 ) g = i; else break;
		}
	    }
	    if( cnt == 1 ) {
		bd[g][r] = dgt;
		clrGRB( g, r, map );
	    }
	}
 /* ブロックの処理（ブロック内セル番号を用いて調べる） */
	for( b = 0; b < 9; b++ ) {			// ブロック b を調べる
	    cnt = 0;
	    for( i = 0; i < 9; i++ ) {
		if( map[BCI(b,i)][BCJ(b,i)] == 1 ) {
		    if( ++cnt == 1 ) c = i; else break;
		}
	    }
	    if( cnt == 1 ) {
		bd[BCI(b,c)][BCJ(b,c)] = dgt;
		clrGRB( BCI(b,c), BCJ(b,c), map );
	    }
	}
}

static int xsolver( Board_t bd )  // 排他法で、盤面 bd の空白セルに数字を埋める
{
	int z0, z1, z2, z3, z4, z5, dgt;
	Board_t map;

	z0 = z3 = countZero( bd );
	if( z0 == 0 ) return 1;

	do {
	    z2 = z3;
	    for( dgt = 1; dgt < 10; dgt++ ) {
		if( makeMap( bd, dgt, map ) ) return -3;
		z5 = countZero( bd );
		do {
		    z4 = z5;
		    putDigit( dgt, bd, map );
		    z5 = countZero( bd );
		    if( z5 == 0 ) return 0;
		} while( z4 > z5 );
	    }
	    z3 = countZero( bd );
	} while( z2 > z3 );

	z1 = countZero( bd );
	if( z1 == 0 ) return 0;
	else if( z0 > z1 ) return -1;
	else return -2;
}
static int selDigit( Board_t bd )			// 仮置きに適した数字を選ぶ
{
	int cmax, cdgt, dgt, z;
	Board_t map;

	cmax = cdgt = -1;
	for( dgt = 1; dgt < 10; dgt++ ) {
	    if( makeMap( bd, dgt, map ) ) return -2;
	    z = countZero( map );
	    if( z < 80 && z > cmax ) {
		cmax = z;
		cdgt = dgt;
	    }
	}
	return cdgt;
}

int rsolver( Board_t bd )  // 再起呼び出しを用いて、bd の空白セルに数字を埋める
{
	int retc, ret2, cdgt, i, j;
	Board_t cmap, wkbd;

	if( ( retc = xsolver( bd ) ) >= 0 ) return retc;
	if( ( cdgt = selDigit( bd ) ) < 0 ) return retc;
	if(makeMap( bd, cdgt, cmap ) ) return -3;
	for( i = 0; i < 9; i++ ) {
	    for( j = 0; j < 9; j++ ) {
		if( cmap[i][j] ) {
		    copyBoard( wkbd, bd );
		    wkbd[i][j] = cdgt;
		    if( ( ret2 = rsolver( wkbd ) ) >= 0 ) {
			copyBoard( bd, wkbd );
			return ret2;
		    }
		}
	   }
	}
	return retc;
}
