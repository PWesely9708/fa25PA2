//
// Created by Manju Muralidharan on 10/19/25.
//
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"

using namespace std;

// Global arrays for node information
const int MAX_NODES = 64;
int weightArr[MAX_NODES];
int leftArr[MAX_NODES];
int rightArr[MAX_NODES];
char charArr[MAX_NODES];

// Function prototypes
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};

    // Step 1: Read file and count letter frequencies
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for each character with nonzero frequency
    int nextFree = createLeafNodes(freq);

    // Step 3: Build encoding tree using your heap
    int root = buildEncodingTree(nextFree);

    // Step 4: Generate binary codes using an STL stack
    string codes[26];
    generateCodes(root, codes);

    // Step 5: Encode the message and print output
    encodeMessage("input.txt", codes);

    return 0;
}

/*------------------------------------------------------
    Function Definitions (Students will complete logic)
  ------------------------------------------------------*/

// Step 1: Read file and count frequencies
void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            charArr[nextFree] = 'a' + i;
            weightArr[nextFree] = freq[i];
            leftArr[nextFree] = -1;
            rightArr[nextFree] = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build the encoding tree using heap operations

// Build the encoding tree using the MinHeap.
// Input: nextFree = number of leaf nodes already created at indices [0..nextFree-1].
// Process:
//   - Push all existing leaf indices into the heap (priority = weightArr[leaf]).
//   - Repeat: pop two smallest nodes, create a parent (sum weights), push parent.
//   - When one element remains, that's the root; return its index.
// Edge cases:
//   - If there are no leaves, return -1 (no tree).
//   - If there is exactly one leaf, return it directly (single-symbol input).
int buildEncodingTree(int nextFree) {
    if (nextFree == 0) return -1;

    MinHeap h;
    // Initialize heap with all leaves that have positive frequency.
    for (int i = 0; i < nextFree; ++i) {
        if (weightArr[i] > 0) {
            h.push(i, weightArr);
        }
    }

    int freeIdx = nextFree; // next available slot for internal nodes

    // Single distinct symbol: tree is just that node.
    if (h.size == 1) {
        int rootOnly = h.pop(weightArr);
        return rootOnly;
    }

    // Combine two lightest subtrees until only the root remains.
    while (h.size > 1) {
        int left = h.pop(weightArr);
        int right = h.pop(weightArr);

        int parent = freeIdx++;
        // Parent's weight is the sum of children.
        weightArr[parent] = weightArr[left] + weightArr[right];
        // Link children; mark as an internal node with '\0' (not a printable char).
        leftArr[parent]   = left;
        rightArr[parent]  = right;
        charArr[parent]   = '\0';

        // Push the new combined subtree back into the heap.
        h.push(parent, weightArr);
    }

    // The remaining node is the tree root.
    int root = h.pop(weightArr);
    return root;
}

// Step 4: Use an STL stack to generate codes

// Generate variable-length codes for each symbol with iterative DFS (no recursion).
// Rules:
//   - Going LEFT appends '0' to the path, RIGHT appends '1'.
//   - When a LEAF is reached, assign its accumulated path as its code.
//   - Single-symbol edge: if the root is also a leaf, its path is empty;
//     assign "0" so the encoded output isn’t empty.
//
// Implementation details:
//   - Uses std::stack of (nodeIndex, pathString) pairs to simulate recursion.
//   - Push RIGHT first, then LEFT, so LEFT is processed first when popping (typical DFS order).
void generateCodes(int root, string codes[]) {
    // Clear any previous codes to avoid stale entries.
    for (int i = 0; i < 26; ++i) codes[i].clear();
    if (root == -1) return;

    // Start DFS from root with an empty path.
    stack<pair<int, string>> st;
    st.push({root, ""});

    while (!st.empty()) {
        auto cur = st.top();
        st.pop();

        int node = cur.first;
        const string& path = cur.second;

        int L = leftArr[node];
        int R = rightArr[node];
        bool isLeaf = (L == -1 && R == -1);

        if (isLeaf) {
            // Leaf: write its code into table (a..z only).
            char ch = charArr[node];
            if (ch >= 'a' && ch <= 'z') {
                // If there was only one distinct symbol, path == "" → assign "0".
                codes[ch - 'a'] = path.empty() ? string("0") : path;
            }
        } else {
            // Internal node: continue DFS.
            // Push right first so that left is processed first after the next pop.
            if (R != -1) st.push({R, path + "1"});
            if (L != -1) st.push({L, path + "0"});
        }
    }
}

// Step 5: Print table and encoded message
void encodeMessage(const string& filename, string codes[]) {
    cout << "\nCharacter : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty())
            cout << char('a' + i) << " : " << codes[i] << "\n";
    }

    cout << "\nEncoded message:\n";

    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
        if (ch >= 'a' && ch <= 'z')
            cout << codes[ch - 'a'];
    }
    cout << "\n";
    file.close();
}