#ifndef ANALYZER_H
#define ANALYZER_H

void averageang(double data[7][750],double*refpos,double*sdmrefpos);
void averagepos(double data[7][750],double*refpos,double*sdmrefpos);
void freqanal(double data[7][750],double*freq,double*sdmfreq,double refpos);
void ampanal (double data[7][750], double *amp , double *sdamp , double refpos);
void vanal(double *freq,double *sdmfreq,double *amp,double *sdmamp,double *vmax,double *sdmvmax);
void shiftanal(double data[7][750],double*pshift,double*sdmpshift,double refpos,double refang);


#endif
