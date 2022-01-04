#include "intpool.h"
#include <stdio.h>
#define false 0
void initIntPool(struct IntPool* self, int size_)
{
    self->pool = ((_Bool*)malloc(sizeof(_Bool)*size_));
    self->size = size_;
    self->index = 0;
    self->m_occupation = 0;
   for(int i = 0; i < self->size; i++) {
         self->pool[i] = false;
     }
}
void destroyIntPool(struct IntPool* self) {
    free(self->pool);
}
int getNewContinuity(struct IntPool* self) {
    int oldInd = self->index;
    while(self->pool[self->index] && self->index < self->size) {
        self->index++;
    }
    if(self->index == self->size) {
        self->index = 0;
        while(self->pool[self->index] && self->index < oldInd) {
            self->index++;
        }
        if(self->index == oldInd) {
            abort();
        }
    }
    assert(!self->pool[self->index]);
    self->pool[self->index] = 1/*true*/;
    if(self->index == 5) {
        fprintf(stderr, "get  %d",  self->index);
    }
    self->m_occupation++;
    return self->index;
}

void releaseContinuity(struct IntPool* self, int ind) {
    assert(self->pool[ind]);
    self->pool[ind] = 0/*false*/;
    if(ind == 5) {
        fprintf(stderr, "free %d",  ind);
    }
    self->m_occupation--;
}

int occupation(struct IntPool* self) {
    return self->m_occupation;
}
