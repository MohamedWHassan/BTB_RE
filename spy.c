/********************************************************************************
* File:			GenCode.c
* 
* Description:	This program generates C code
*				aimed to determine the characteristics of Intel's BTB:
*				the number of ways and address bits used as index
*
* Input:		number of iterations of the outer loop 
*				dist - distance between subsequent branch addresses)
*				branches - number of branches in the loop
*
* Date:			April 2002
* 
* Author:		A. Milenkovic - milenkovic@computer.org
*
* Revision 1.1:	Distance is implemented using assembly clc instructions as noops
*				Condition code for branch instructions is set at the begining.
*
* Revision 1.2: All branches are short - distance < 128,
*				except loop codition.
*				To get shorter code for distance,
*				noops are combination of mov (5B), cmp (3B), and clc instructions
**********************************************************************************/


#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv)
{ 
	
	int long unsigned	iterations;		/* number of iterations in the loop*/
	int					dist;			/* max distance in bytes between two subsequent branch instruction */										
	int					branches;		/* number of branches inside a loop */
	
	char file_name[30];					/* name of the generated C file */
	FILE * fout;						/* generated C file */

	int j, k;							/* loop indexes */
    int num_nops;		/* number of mov, cmp and clc instructions in the distance code*/

	if (argc != 4) {
		printf("Usage is: %s number_of_iterations distance number_of_branches\n",argv[0]);
		exit(1);
	}

	iterations = atoi(argv[1]);
	dist = atoi(argv[2]);
	branches = atoi(argv[3]);

    
	/* generate file name */

	// if (iterations%1000000 != 0) {
	// 	fprintf(stderr,"Number of iterations must be millions\n");
	// 	exit(1);
	// }

	sprintf(file_name, "T1ImNB.c");
    
	/* open output file */
	
	fout=fopen (file_name,"w");
	
	/* write the content of the program */
	
	fprintf(fout,"void main(void) { \n");
	fprintf(fout,"int long unsigned i;\n");
	fprintf(fout,"int long unsigned liter = %ld;\n", iterations);
	fprintf(fout,"for (i=0; i<liter; ++i){\n");
	fprintf(fout,"asm volatile ( \n");

	/* generate sequence of asm instructions */
	/* assume short in-loop branches, distance < 128  */
	/* distance between loop condition branch and first in-loop branch is fixed to 16:
	   6 for jle + 5 mov + 3 cmp + 2* 1 clc */
	
	dist = dist - 4;
	num_nops = dist/4;

	fprintf(fout, "\"nop;\" \\\n");
	fprintf(fout, "\"nop;\" \\\n");
	for (j = 0; j < branches - 1; j++) { 
		// NOPs and branches are 4 bytes each
		if(j==0){
			fprintf(fout, "\"addi t0, x0, 1;\" \\\n");
			fprintf(fout, "\"addi t1, x0, 1;\" \\\n");
			fprintf(fout, "\"");
		}
		fprintf(fout, "beq t0, t1, l%d;\" \\\n", j);

		for (k = 1; k <= num_nops; k++) { 
			fprintf(fout, "\"nop;\" \\\n");
		}
		
		/* we can always generate a label (even for the last iteration - no harm) */
		
		fprintf(fout, "\"l%d: ", j); /* no new line here */
	}
	
	fprintf(fout, "nop;\" \n"); /* last branch targert */
	fprintf(fout, ");\n");	/* close _asm { */
	fprintf(fout, "}\n");	/* close for loop */
	fprintf(fout, "}\n");	/* close main */
	fclose(fout);			/* close this file */
}