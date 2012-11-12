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
	double refpos,sdmrefpos,refang,sdmrefang;
	double freq, sdmfreq,amp,sdmamp,vmax,sdmvmax, pshift,sdmpshift;

	if(output==NULL){
		output= fopen(argv[2], "w");
		fprintf(output, "filename\tangular-frequency\tsdm-freq\tAmplitude\tsdm-Amp\tvmax\tsdm-vmax\tphase-shift\tsdm-shift\tref position,\tsdm ref pos\n");
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
	averageang(data,&refang,&sdmrefang);
	averagepos(data,&refpos,&sdmrefpos);
	freqanal(data,&freq,&sdmfreq,refpos);
	ampanal(data,&amp,&sdmamp,refpos);
	sdmamp = sqrt(sdmamp*sdmamp+sdmrefpos*sdmrefpos);
	vanal(&freq,&sdmfreq,&amp,&sdmamp,&vmax,&sdmvmax);
	shiftanal(data,&pshift,&sdmpshift,refpos,refang);
	pshift=((pshift*freq/(2*M_PI))+1)*(-360);
	sdmpshift= sqrt(((sdmfreq)/(freq))*((sdmfreq)/(freq)) + ((sdmpshift)/(pshift))*((sdmpshift)/(pshift)));

	fprintf(output, "%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",argv[1], freq, sdmfreq, amp, sdmamp, vmax, sdmvmax, pshift, sdmpshift,refpos,sdmrefpos);
}
void averageang(double data[7][750],double*refpos,double*sdmrefpos){
	int i =0;
	double dat,ref;
	double summation=0;
	for (i=0;i<750;i++){
		summation = summation + data[1][i];
	}
	printf("avpos = %g\n", summation / 750 );
	*refpos= summation / 750;

	summation=0;
	ref= *refpos;
	for(i=0;i<750;i++){
		dat = data[1][i];
		summation = summation + (dat-ref)*(dat-ref);
	}
	summation = summation / (750);
	summation = summation / (749);

	*sdmrefpos = sqrt(summation);

}
void averagepos(double data[7][750],double*refpos,double*sdmrefpos){
	int i =0;
	double dat,ref;
	double summation=0;
	for (i=0;i<750;i++){
		summation = summation + data[4][i];
	}
	printf("avpos = %g\n", summation / 750 );
	*refpos= summation / 750;

	summation=0;
	ref= *refpos;
	for(i=0;i<750;i++){
		dat = data[4][i];
		summation = summation + (dat-ref)*(dat-ref);
	}
	summation = summation / (750);
	summation = summation / (749);

	*sdmrefpos = sqrt(summation);

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


void vanal(double *freq,double *sdmfreq,double *amp,double *sdmamp,double *vmax,double *sdmvmax){
	*vmax=(*freq) * (*amp);
	*sdmvmax = sqrt(((*sdmfreq)/(*freq))*((*sdmfreq)/(*freq))+((*sdmamp)/(*amp))*((*sdmamp)/(*amp)));


}
void shiftanal(double data[7][750],double* pshift,double* sdmpshift,double refpos,double refang){
	int i = 0;
	int j =0;
	int k =0;
	double lastout,lastin;
	double outsinks[100];
	double insinks[100];
	double shift[100];
	double summation =0;
	for (i=0;i<100;i++){
		outsinks[i] = insinks[i] =0;
		shift[i]=0;
	}

	lastout=-1;
	lastin=-1;

	for(i=0;i<750;i++){


		if(lastout>=refpos && data[4][i]<refpos){
			outsinks[j]=data[0][i-1];
				j++;
		}
		if(lastin>=refang && data[1][i]<refang){
			insinks[k]=data[0][i-1];
			k++;
		}
		lastout=data[4][i];
		lastin=data[1][i];
	}
	i=j=k=0;
	if (insinks[0]<outsinks[0]){
		j++;
	}

	while(insinks[j]!=0 && outsinks[i]!=0){
		shift[k]=outsinks[i]-insinks[j];
		i++;
		j++;
		k++;

	}

	for(i=0;i<k;i++){
		summation = summation +shift[i];
	}

	*pshift= summation/k;
	summation=0;
	for(i=0;i<k;i++){
		shift[i]=(shift[i]-*pshift)*(shift[i]-*pshift);
		summation = summation + shift[i];
	}
	summation = summation / k;
	summation = summation / (k-1);

	*sdmpshift = sqrt(summation);
}


