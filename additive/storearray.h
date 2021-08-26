#ifndef STOREARRAY_H
#define STOREARRAY_H

void save(void* data, int size, const unsigned char* hash, char* type, int version);

bool load(const unsigned char* hash, char* type, int version, void** data, int* size);
bool loadOne(const unsigned char* hash, char* type, int version, void* data);

#endif // STOREARRAY_H
