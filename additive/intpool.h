#ifndef INTPOOL_H
#define INTPOOL_H

struct IntPool {
    _Bool* pool;
    int size;
    int index;
    int m_occupation;
};

void initIntPool(struct IntPool* self, int size_);
void destroyIntPool(struct IntPool* self);
int getNewContinuity(struct IntPool* self);
void releaseContinuity(struct IntPool* self, int ind);
int occupation(struct IntPool* self);
#endif // INTPOOL_H
