#pragma once
#include <unordered_map>
#include <memory>
#include <vector>

namespace CustomLang {

    class GarbageCollector {
    public:
        struct ObjectHeader {
            size_t size;
            bool marked;
            void* data;
        };

        static GarbageCollector& getInstance();
        void* allocate(size_t size);
        void addRoot(void* ptr);
        void removeRoot(void* ptr);
        void collect();

    private:
        std::unordered_map<void*, ObjectHeader*> allocatedObjects;
        std::vector<void*> roots;

        void markPhase();
        void markObject(void* ptr);
        void sweepPhase();

        GarbageCollector() = default;
        ~GarbageCollector();
    };

} // namespace CustomLang