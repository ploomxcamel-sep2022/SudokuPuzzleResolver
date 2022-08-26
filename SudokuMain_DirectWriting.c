#include <stdio.h>
#include "solver.h"

//問題文をmainで直接与える
main()
{
	int z;
	Board_t map, bd = 
	          { { 0, 0, 2, 0, 0, 9, 8, 6, 0 } 
		      , { 6, 0, 0, 4, 0, 0, 0, 0, 0 }
		      , { 1, 0, 0, 0, 7, 0, 0, 0, 3 }
		      , { 0, 5, 1, 0, 0, 0, 0, 0, 7 }
		      , { 0, 0, 0, 3, 0, 5, 0, 0, 0 }
		      , { 3, 0, 0, 0, 0, 0, 9, 4, 0 }
		      , { 7, 0, 0, 0, 9, 0, 0, 0, 8 }
		      , { 0, 0, 0, 0, 0, 6, 0, 0, 1 }
		      , { 0, 3, 5, 8, 0, 0, 7, 0, 0 }
		      };
	z = rsolver( bd );
	printf( "rsolver returns %d\n", z );
	mes( z );
	printBoard( bd );
}
