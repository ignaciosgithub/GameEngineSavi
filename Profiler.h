#ifndef PROFILER_H
#define PROFILER_H

#include "platform.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

class Profiler {
private:
    struct MemorySnapshot {
        size_t totalMemoryUsage;
        size_t peakMemoryUsage;
        std::chrono::system_clock::time_point timestamp;
        std::unordered_map<std::string, size_t> componentMemoryUsage;
        
        MemorySnapshot() 
            : totalMemoryUsage(0), peakMemoryUsage(0), 
              timestamp(std::chrono::system_clock::now()) {}
    };
    
    std::vector<MemorySnapshot> memorySnapshots;
    static Profiler* instance;
    bool isEnabled;
    
    // Private constructor for singleton pattern
    Profiler();
    
public:
    static Profiler& GetInstance();
    
    // Enable/disable profiling
    void Enable();
    void Disable();
    bool IsEnabled() const;
    
    // Take a memory snapshot
    void TakeMemorySnapshot(const std::string& label = "");
    
    // Get current process memory usage
    size_t GetCurrentMemoryUsage() const;
    
    // Get peak memory usage
    size_t GetPeakMemoryUsage() const;
    
    // Track memory allocation for a specific component
    void TrackComponentMemory(const std::string& componentName, size_t size);
    
    // Get memory usage for a specific component
    size_t GetComponentMemoryUsage(const std::string& componentName) const;
    
    // Print memory usage report
    void PrintMemoryReport() const;
    
    // Save memory report to file
    void SaveMemoryReportToFile(const std::string& filename) const;
    
    // Clear all memory snapshots
    void ClearSnapshots();
};

#endif // PROFILER_H
