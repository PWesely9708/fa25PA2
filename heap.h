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
        if (size == 0) return -1;
            int ret = data[0];
            data[0] = data[size - 1];
            size--;
        if (size > 0) downheap(0, weightArr);
            return ret;
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
        while (true) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            auto isLess = [&](int aIdx, int bIdx) {
                int aW = weightArr[aIdx], bW = weightArr[bIdx];
                if (aW < bW) return true;
                if (aW > bW) return false;
                return aIdx < bIdx;
            };

            if (left < size && isLess(data[left], data[smallest])) smallest = left;
            if (right < size && isLess(data[right], data[smallest])) smallest = right;

            if (smallest != pos) {
                int tmp = data[pos];
                data[pos] = data[smallest];
                data[smallest] = tmp;
                pos = smallest;
            }
            else {
                break;
            }
        }
    }
};

#endif