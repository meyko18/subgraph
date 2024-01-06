#include <vector>
#include <cmath>

class BloomFilter {
private:
    std::vector<bool> bitArray;
    int bitArraySize;
    int numHashFunctions;

    int hash(int n, int x) {
        // 一个简单的哈希函数，实际实现中应该使用更复杂的哈希函数
        return (x + n * x) % bitArraySize;
    }

public:
    BloomFilter(int size, int numHashes) : bitArraySize(size), numHashFunctions(numHashes) {
        bitArray.resize(size);
    }

    void add(int item) {
        for (int i = 0; i < numHashFunctions; i++) {
            bitArray[hash(i, item)] = true;
        }
    }

    bool possiblyContains(int item) {
        for (int i = 0; i < numHashFunctions; i++) {
            if (!bitArray[hash(i, item)]) {
                return false;
            }
        }
        return true;
    }
};