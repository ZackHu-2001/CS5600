#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class Malloc {
public:
    Malloc(int size, int start, int headerSize, std::string policy, std::string order, bool coalesce, int align)
        : size(size), headerSize(headerSize), policy(policy), returnPolicy(order), coalesce(coalesce), align(align) {
        if (policy != "BEST" && policy != "WORST" && policy != "FIRST") {
            throw std::invalid_argument("Invalid policy");
        }
    }

    void addToMap(int addr, int size) {
        sizemap[addr] = size;
    }

    std::pair<int, int> malloc(int size) {
        if (align != -1) {
            int left = size % align;
            if (left != 0) {
                int diff = align - left;
                size += diff;
            }
        }

        size += headerSize;

        int bestIdx = -1;
        int bestSize = (policy == "BEST") ? size + 1 : -1;
        int count = 0;

        for (int i = 0; i < freelist.size(); ++i) {
            int eaddr = freelist[i].first;
            int esize = freelist[i].second;
            count++;
            if (esize >= size && ((policy == "BEST" && esize < bestSize) || (policy == "WORST" && esize > bestSize) || (policy == "FIRST"))) {
                bestIdx = i;
                bestSize = esize;
                if (policy == "FIRST") {
                    break;
                }
            }
        }

        if (bestIdx != -1) {
            if (bestSize > size) {
                freelist[bestIdx] = std::make_pair(freelist[bestIdx].first + size, bestSize - size);
                addToMap(freelist[bestIdx].first, size);
            } else if (bestSize == size) {
                freelist.erase(freelist.begin() + bestIdx);
                addToMap(freelist[bestIdx].first, size);
            } else {
                throw std::runtime_error("Should never get here");
            }
            return std::make_pair(freelist[bestIdx].first, count);
        }

        return std::make_pair(-1, count);
    }

    int free(int addr) {
        if (sizemap.find(addr) == sizemap.end()) {
            return -1;
        }

        int size = sizemap[addr];
        if (returnPolicy == "INSERT-BACK") {
            freelist.push_back(std::make_pair(addr, size));
        } else if (returnPolicy == "INSERT-FRONT") {
            freelist.insert(freelist.begin(), std::make_pair(addr, size));
        } else if (returnPolicy == "ADDRSORT") {
            freelist.push_back(std::make_pair(addr, size));
            std::sort(freelist.begin(), freelist.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.first < b.first;
            });
        } else if (returnPolicy == "SIZESORT+") {
            freelist.push_back(std::make_pair(addr, size));
            std::sort(freelist.begin(), freelist.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.second < b.second;
            });
        } else if (returnPolicy == "SIZESORT-") {
            freelist.push_back(std::make_pair(addr, size));
            std::sort(freelist.begin(), freelist.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.second > b.second;
            });
        }

        if (coalesce) {
            std::vector<std::pair<int, int>> newlist;
            std::pair<int, int> curr = freelist[0];
            for (int i = 1; i < freelist.size(); ++i) {
                int eaddr = freelist[i].first;
                int esize = freelist[i].second;
                if (eaddr == (curr.first + curr.second)) {
                    curr = std::make_pair(curr.first, curr.second + esize);
                } else {
                    newlist.push_back(curr);
                    curr = freelist[i];
                }
            }
            newlist.push_back(curr);
            freelist = newlist;
        }

        sizemap.erase(addr);
        return 0;
    }

    void dump() {
        std::cout << "Free List [ Size " << freelist.size() << " ]: ";
        for (const auto& e : freelist) {
            std::cout << "[ addr:" << e.first << " sz:" << e.second << " ]";
        }
        std::cout << std::endl;
    }

private:
    int size;
    int headerSize;
    std::string policy;
    std::string returnPolicy;
    bool coalesce;
    int align;
    std::vector<std::pair<int, int>> freelist;
    std::map<int, int> sizemap;
};

int main() {
    int seed = 0;
    int heapSize = 100;
    int baseAddr = 1000;
    int headerSize = 0;
    int alignment = -1;
    std::string policy = "BEST";
    std::string listOrder = "ADDRSORT";
    bool coalesce = false;
    int opsNum = 10;
    int opsRange = 10;
    int opsPAlloc = 50;
    std::string opsList = "";
    bool solve = false;

    Malloc m(heapSize, baseAddr, headerSize, policy, listOrder, coalesce, alignment);

    std::cout << "seed " << seed << std::endl;
    std::cout << "size " << heapSize << std::endl;
    std::cout << "baseAddr " << baseAddr << std::endl;
    std::cout << "headerSize " << headerSize << std::endl;
    std::cout << "alignment " << alignment << std::endl;
    std::cout << "policy " << policy << std::endl;
    std::cout << "listOrder " << listOrder << std::endl;
    std::cout << "coalesce " << coalesce << std::endl;
    std::cout << "numOps " << opsNum << std::endl;
    std::cout << "range " << opsRange << std::endl;
    std::cout << "percentAlloc " << opsPAlloc << std::endl;
    std::cout << "allocList " << opsList << std::endl;
    std::cout << "compute " << solve << std::endl;
    std::cout << std::endl;

    srand(seed);
    std::map<int, int> p;
    std::vector<int> L;
    int percent = opsPAlloc / 100.0;

    int c = 0;
    int j = 0;
    while (j < opsNum) {
        bool pr = false;
        if (rand() / (RAND_MAX + 1.0) < percent) {
            int size = rand() % opsRange + 1;
            std::pair<int, int> result = m.malloc(size);
            if (result.first != -1) {
                p[c] = result.first;
                L.push_back(c);
            }
            std::cout << "ptr[" << c << "] = Alloc(" << size << ")";
            if (solve) {
                std::cout << " returned " << result.first + headerSize << " (searched " << result.second << " elements)" << std::endl;
            } else {
                std::cout << " returned ?" << std::endl;
            }
            c++;
            j++;
            pr = true;
        } else {
            if (!p.empty()) {
                int d = rand() % L.size();
                int rc = m.free(p[L[d]]);
                std::cout << "Free(ptr[" << L[d] << "])";
                if (solve) {
                    std::cout << " returned " << rc << std::endl;
                } else {
                    std::cout << " returned ?" << std::endl;
                }
                p.erase(L[d]);
                L.erase(L.begin() + d);
                pr = true;
                j++;
            }
        }
        if (pr) {
            if (solve) {
                m.dump();
            } else {
                std::cout << "List? " << std::endl;
            }
        }
        std::cout << std::endl;
    }
}
