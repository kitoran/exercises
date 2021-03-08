#ifndef MATHEXT_H
#define MATHEXT_H



#define LOOKUP_POWER 15
#define LOOKUP_TABLE_SIZE (1 << LOOKUP_POWER)
inline double fastSin(double x);
double* sinLookupTable();
extern const double tau;
int intLog2(int l);
#endif // MATHEXT_H
