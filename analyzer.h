#ifndef ANALYZER_H
#define ANALYZER_H

double averagepos(double data[7][750]);
void freqanal(double data[7][750],double*freq,double*sdmfreq,double refpos);
void ampanal(double data[7][750],double*amp,double*sdamp,double*vmax,double refpos);
void shiftanal(double data[7][750],double*pshift,double*sdmpshift);


#endif
