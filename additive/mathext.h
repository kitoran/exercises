#ifndef MATHEXT_H
#define MATHEXT_H



#define LOOKUP_POWER 15
#define LOOKUP_TABLE_SIZE (1 << LOOKUP_POWER)
inline double fastSin(double x);
double* sinLookupTable();
extern const double tau;
int intLog2(int l);
inline unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
#endif // MATHEXT_H
