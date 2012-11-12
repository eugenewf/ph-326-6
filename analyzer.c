#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "analyzer.h"

int main (int argc , char** argv){

	FILE* input = fopen(argv[1],"r");
	FILE* output = fopen(argv[2], "r");
	double data[7][750];
	int i  = 0;
	char c;
	double refpos;
	double freq, sdmfreq,amp,sdmamp,vmax,sdmvmax, pshift,sdmpshift;

	if(output==NULL){
		output= fopen(argv[2], "w");
		fprintf(output, "frequency\tsdm-freq\tAmplitude\tsdm-Amp\tvmax\tphase-shift\tsdm-shift\n");
	}else{
		fclose( output);
		fopen(argv[2], "a");
	}

	for (i=0;i<7;i++){
		c=fgetc(input);
		while(c!='\n'){
			c=fgetc(input);		
		}
	}

	for (i=0;i<750;i++){
		fscanf(input,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",&data[0][i],&data[1][i],&data[2][i],&data[3][i],&data[4][i],&data[5][i],&data[6][i]);
		c=fgetc(input);
		while(c!='\n'){
			c=fgetc(input);		
		}

	}

	refpos = averagepos(data);
	freqanal(data,&freq,&sdmfreq,refpos);
	ampanal(data,&amp,&sdmamp,refpos);
	vanal(&freq,&sdmfreq,&amp,&sdmamp,&vmax,&sdmvmax);
	shiftanal(data,&pshift,&sdmpshift);

	fprintf(output, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", freq, sdmfreq, amp, sdmamp, vmax, sdmvmax, pshift, sdmpshift);
}
double averagepos(double data[7][750]){
	int i =0;
	double summation=0;
	for (i=0;i<750;i++){
		summation = summation + data[4][i];
	}
	printf("avpos = %g\n", summation / 750 );
	return summation / 750;
}
void freqanal(double data[7][750],double*freq,double*sdmfreq,double refpos){
	double sinks[100];
	double intervals[100];
	double summation=0;
	int i,j;

	i=j=0;

	/*	
	 *	for(i=0;i<100;i++){
	 *		intervals[i]=sinks[i]=-1;
	 *	}
	 */

	for (i=0; i<749;i++){
		if (data[4][i]-refpos>0 && data[4][i+1]-refpos<0){
			sinks[j]=data[0][i];
			j++;
		}
	}
	for(i=0;i<j-1;i++){
		intervals[i]=sinks[i+1]-sinks[i];
	}

	for(i=0;i<j-2;i++){
		intervals[i] = 2*M_PI/intervals[i];
		summation = summation + intervals[i];
	}

	*freq = summation/(j-2);

	summation=0;

	for(i=0;i<j-2;i++){
		intervals[i]= (intervals[i]-*freq)*(intervals[i]-*freq);
		summation = summation +intervals[i];
	}
	summation = summation / (j-2);
	summation = summation / (j-3);

	*sdmfreq = sqrt(summation);
}

void ampanal (double data[7][750], double *amp , double *sdamp , double refpos){
	double peaks[100];
	double summation=0;
	int i,j;
	double last,current;

	j=0;
	last = data[4][1]-data[4][0];

	for(i=1;i<749;i++){
		current = data[4][i+1]-data[4][i];
		if (current<0 && last>0){
			peaks[j]=data[4][i];
			j++;
		}
		last=current;
	}

	for (i=0;i<j;i++){
		summation = summation +peaks[i];
	}
	*amp = summation/j;

	summation =0;

	for (i=0;i<j;i++){
		peaks[i]= (peaks[i]-*amp)*(peaks[i]-*amp);
		summation = summation +peaks[i];
	}
	summation = summation / j;
	summation = summation / (j-1);

	*sdamp=sqrt(summation);
	*amp = *amp - refpos;
}


void vanal(double *freq,double *sdmfreq,double *amp,double *sdmamp,double *vmax,double *sdmvma){

}
void shiftanal(double data[7][750],double* pshift,double* sdmpshift){

}




