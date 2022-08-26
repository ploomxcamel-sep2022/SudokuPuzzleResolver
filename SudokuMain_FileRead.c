#include <stdio.h>
#include "solver.h"

int getDigit( FILE *fp ) //�X�g���[�� fp ���琔�����P�ǂ�Ő����l�ɖ߂�
{
	int c;

	while( ( c = fgetc( fp ) ) != EOF ) {  //�ufgetc�v�̖߂�l�͓ǂݍ��񂾕����̕����R�[�h

	    if( isdigit( c ) ) return c - '0';  //�����𐮐��ɕϊ����Ė߂�
	}
	return -1;  //�u-1�v�̓t�@�C����ǂ݂��������Ƃ��Ӗ�����
}

int readBoard( FILE *fp, int section, Board_t bd ) //�X�g���[�� fp ���� section �Ԗڂ̃f�[�^�� bd �ɓǂݍ���
{
	char buf[3], c;
	int i, j;

	while(section > 0 ) {  //�w�肳�ꂽ�Z�N�V�����܂œǂݔ�΂�
	    if( !fgets( buf, sizeof(buf), fp ) ) return -1;  //�u-1�v�̓Z�N�V�����̈ʒu�t�����o���Ȃ��������ƈӖ�����
	    if( strchr( buf, '\n' ) ) continue;
	    if( !strcmp( buf, "//" ) ) section--;  //�ubuf�v�̓��e���u//�v�������� section ���P���炷
		while( ( c = fgetc( fp ) ) != '\n' )  //�ubuf�v�ɉ��s���܂܂�Ȃ��Ƃ��́A���s�R�[�h�������܂œǂݔ�΂�
		if( c == EOF ) return -1;
	}
	for( i = 0; i < 9; i++ ){
	    for( j = 0; j < 9; j++ ){
		if( (bd[i][j] = getDigit( fp ) ) < 0 ) return -2;  //�u-2�v�͐����̓ǂݍ��݂ŃG���[�ɂȂ���
	    }
	}
	return 0;  //�u0�v�͐������ǂݍ��܂ꂽ
}

//��蕶���t�@�C������ǂݍ���
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
