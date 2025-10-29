//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[64];
    int size;

    MinHeap() { size = 0; }

    void push(int idx, int weightArr[]) {
        int pos = size;
        data[pos] = idx;
        size++;
        upheap(pos, weightArr);
    }

    int pop(int weightArr[]) {
        // TODO: remove and return smallest index
        // Replace root with last element, then call downheap()
        return -1; // placeholder
    }

    void upheap(int pos, int weightArr[]) {
        while (pos > 0) {
            int par = (pos - 1) / 2;

            int childIdx = data[pos];
            int parIdx = data[par];

            int childW = weightArr[childIdx];
            int parW = weightArr[parIdx];

            bool childIsSmaller = (childW < parW) || (childW == parW && childIdx < parIdx);
            if (!childIsSmaller) break;

            int tmp = data[pos];
            data[pos] = data[par];
            data[par] = tmp;

            pos = par;
        }
    }

    void downheap(int pos, int weightArr[]) {
        // TODO: swap parent downward while larger than any child
    }
};

#endif