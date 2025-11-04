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

    // Insert an index into the min-heap.
    // - The heap stores INDICES into the global node arrays (not weights directly).
    // - Priority is determined by weightArr[index]; lower weight = higher priority.
    // - Tie-break: if weights are equal, the lower index wins (deterministic order).
    void push(int idx, int weightArr[]) {
        // Place the new element at the end (next free slot)...
        int pos = size;
        data[pos] = idx;
        size++;
        // Then restore the min-heap property by bubbling up.
        upheap(pos, weightArr);
    }

    // Remove and return the smallest element (root) by the ordering rule.
    // Steps:
    //  1) Save the root to return later.
    //  2) Move the last element to the root and shrink the size.
    //  3) Push that element down until the heap property is restored.
    // If the heap is empty, return -1 as a sentinel.
    int pop(int weightArr[]) {
        if (size == 0) return -1;
            int ret = data[0]; // smallest index by (weight, then index)
            data[0] = data[size - 1]; // move last element to root
            size--; // shrink heap
        if (size > 0) downheap(0, weightArr); // restore property by sifting down
            return ret; // return original root
    }


    // Bubble the element at position `pos` up the tree while it should be above its parent.
    // Comparison rule:
    //   - smaller weight comes first
    //   - if weights are equal, smaller index comes first (stable/orderly)
    void upheap(int pos, int weightArr[]) {
        while (pos > 0) {
            int par = (pos - 1) / 2; // parent index in array-based heap

            int childIdx = data[pos];
            int parIdx = data[par];

            int childW = weightArr[childIdx];
            int parW = weightArr[parIdx];

            // True if child should be above parent under our ordering rule.
            bool childIsSmaller = (childW < parW) || (childW == parW && childIdx < parIdx);
            if (!childIsSmaller) break;

            // Swap child with parent and continue bubbling up.
            int tmp = data[pos];
            data[pos] = data[par];
            data[par] = tmp;

            pos = par; // move up one level
        }
    }

    // Push the element at `pos` down until it is <= both of its children
    // under the same ordering rule used in upheap:
    //   - smaller weight wins; on ties, smaller index wins.
    // Repeats until the element is in a valid position or no children are smaller.
    void downheap(int pos, int weightArr[]) {
        while (true) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            // Helper: true if a should come before b under our ordering rule
            auto isLess = [&](int aIdx, int bIdx) {
                int aW = weightArr[aIdx], bW = weightArr[bIdx];
                if (aW < bW) return true;
                if (aW > bW) return false;
                return aIdx < bIdx;
            };

            // Pick the smallest among parent and valid children.
            if (left < size && isLess(data[left], data[smallest])) smallest = left;
            if (right < size && isLess(data[right], data[smallest])) smallest = right;

            // If a child is smaller, swap and continue; otherwise we're done.
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