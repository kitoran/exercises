#ifndef MATHEXT_H
#define MATHEXT_H



#define LOOKUP_POWER 15
#define LOOKUP_TABLE_SIZE (1 << LOOKUP_POWER)

void initializeMathExt();

inline double fastSin(double x);
inline int fastReducedSinInt(int x);
extern double* sinLookupTable;
extern int* sinLookupTableInt;
extern const double tau;
int intLog2(int l);
inline unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
#endif // MATHEXT_H
