#include "gc.hpp"
#include <algorithm>

namespace CustomLang {

    GarbageCollector& GarbageCollector::getInstance() {
        static GarbageCollector instance;
        return instance;
    }

    void* GarbageCollector::allocate(size_t size) {
        void* mem = malloc(size + sizeof(ObjectHeader));
        if (!mem) return nullptr;

        ObjectHeader* header = static_cast<ObjectHeader*>(mem);
        header->size = size;
        header->marked = false;
        header->data = static_cast<char*>(mem) + sizeof(ObjectHeader);

        allocatedObjects[header->data] = header;
        return header->data;
    }

    void GarbageCollector::addRoot(void* ptr) {
        if (ptr && allocatedObjects.find(ptr) != allocatedObjects.end()) {
            roots.push_back(ptr);
        }
    }

    void GarbageCollector::removeRoot(void* ptr) {
        roots.erase(std::remove(roots.begin(), roots.end(), ptr), roots.end());
    }

    void GarbageCollector::collect() {
        markPhase();
        sweepPhase();
    }

    void GarbageCollector::markPhase() {
        for (void* root : roots) {
            markObject(root);
        }
    }

    void GarbageCollector::markObject(void* ptr) {
        auto it = allocatedObjects.find(ptr);
        if (it != allocatedObjects.end() && !it->second->marked) {
            it->second->marked = true;
            // Here you would recursively mark any referenced objects
            // This depends on your object structure
        }
    }

    void GarbageCollector::sweepPhase() {
        auto it = allocatedObjects.begin();
        while (it != allocatedObjects.end()) {
            if (!it->second->marked) {
                free(it->second); // Free the memory
                it = allocatedObjects.erase(it);
            }
            else {
                it->second->marked = false; // Reset mark for next collection
                ++it;
            }
        }
    }

    GarbageCollector::~GarbageCollector() {
        // Clean up all remaining objects
        for (auto& pair : allocatedObjects) {
            free(pair.second);
        }
    }

} // namespace CustomLang