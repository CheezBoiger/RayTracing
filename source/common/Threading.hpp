// Raytracer
#pragma once

#include "common/Types.hpp"
#include <thread>
#include <functional>
#include <vector>

namespace rt {

struct ThreadID {
    struct { U32 x, y, z; } local;
    struct { U32 x, y, z; } global;
    U64 globalId_1D;
};


typedef std::function<void(const ThreadID& id)> KernelFunc;

struct Kernel {
    KernelFunc func;
    U32 localX;
    U32 localY;
    U32 localZ;
};


static void dispatch(const Kernel& kern, U32 x, U32 y, U32 z) {
    U64 totalThreadCount = kern.localX * kern.localY * kern.localZ * x * y * z;
    
    std::vector<std::thread> threads(x * y * z);
    {
        U64 i = 0;
        for (U64 workX = 0; workX < x; ++workX) {
            for (U64 workY = 0; workY < y; ++workY) {
                for (U64 workZ = 0; workZ < z; ++workZ) {
                threads[workX + x * (workY + y * workZ)] =
                    std::thread([=](U64 globalId_1D) -> void {
                        std::vector<ThreadID> ids(kern.localX * kern.localY * kern.localZ);
                        for (U64 localX = 0; localX < kern.localX; ++localX) {
                              for (U64 localY = 0; localY < kern.localY; ++localY) {
                                    for (U64 localZ = 0; localZ < kern.localZ; ++localZ) {
                                        U64 threadId = localX + kern.localX * (localY + kern.localY * localZ); 
                                        ids[threadId] = {};
                                        ids[threadId].local.x = localX;
                                        ids[threadId].local.y = localY;
                                        ids[threadId].local.z = localZ;
                                        ids[threadId].global.x = workX * kern.localX + localX;
                                        ids[threadId].global.y = workY * kern.localY + localY;
                                        ids[threadId].global.z = workZ * kern.localZ + localZ;
                                        ids[threadId].globalId_1D = i; 
                                        kern.func(ids[threadId]);
                                    }
                              }
                        }                   
                    }, i);
                    ++i;
                }
            }
        }
    }

    for (U32 i = 0; i < threads.size(); ++i) 
        threads[i].join();
}
} // rt