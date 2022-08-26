#include <stdio.h>
#include "solver.h"

int getDigit( FILE *fp ) //ストリーム fp から数字を１つ読んで整数値に戻す
{
	int c;

	while( ( c = fgetc( fp ) ) != EOF ) {  //「fgetc」の戻り値は読み込んだ文字の文字コード

	    if( isdigit( c ) ) return c - '0';  //数字を整数に変換して戻す
	}
	return -1;  //「-1」はファイルを読みきったことを意味する
}

int readBoard( FILE *fp, int section, Board_t bd ) //ストリーム fp から section 番目のデータを bd に読み込む
{
	char buf[3], c;
	int i, j;

	while(section > 0 ) {  //指定されたセクションまで読み飛ばす
	    if( !fgets( buf, sizeof(buf), fp ) ) return -1;  //「-1」はセクションの位置付けが出来なかったこと意味する
	    if( strchr( buf, '\n' ) ) continue;
	    if( !strcmp( buf, "//" ) ) section--;  //「buf」の内容が「//」だったら section を１減らす
		while( ( c = fgetc( fp ) ) != '\n' )  //「buf」に改行が含まれないときは、改行コードが現れるまで読み飛ばす
		if( c == EOF ) return -1;
	}
	for( i = 0; i < 9; i++ ){
	    for( j = 0; j < 9; j++ ){
		if( (bd[i][j] = getDigit( fp ) ) < 0 ) return -2;  //「-2」は数字の読み込みでエラーになった
	    }
	}
	return 0;  //「0」は正しく読み込まれた
}

//問題文をファイルから読み込む
main( int argc, char *argv[] )
{
	Board_t bd;
	FILE *fp;
	int s;
	int z;

	if( argc < 3 ){
	    fprintf( stderr, "(USAGE)$ %s filename section\n", argv[0] );
	    return 1;
	} else {
	    if( sscanf( argv[2], "%d", &s ) == 0 ) s = 0;
	}
	if( ( fp = fopen( argv[1], "r" ) ) == NULL ) return 1;
	if( !readBoard( fp, s, bd ) ) {
		z = rsolver( bd );
		printf( "rsolver returns %d\n", z );
		mes( z );
		printBoard( bd );
	}
	fclose( fp );
	return 0;
}
