#include "Profiler.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

Profiler* Profiler::instance = nullptr;

Profiler::Profiler() : isEnabled(false) {
    // Initialize profiler
}

Profiler& Profiler::GetInstance() {
    if (!instance) {
        instance = new Profiler();
    }
    return *instance;
}

void Profiler::Enable() {
    isEnabled = true;
}

void Profiler::Disable() {
    isEnabled = false;
}

bool Profiler::IsEnabled() const {
    return isEnabled;
}

void Profiler::TakeMemorySnapshot(const std::string& label) {
    if (!isEnabled) return;
    
    MemorySnapshot snapshot;
    snapshot.totalMemoryUsage = GetCurrentMemoryUsage();
    snapshot.timestamp = std::chrono::system_clock::now();
    
    // Update peak memory usage if needed
    if (snapshot.totalMemoryUsage > GetPeakMemoryUsage()) {
        snapshot.peakMemoryUsage = snapshot.totalMemoryUsage;
    } else {
        snapshot.peakMemoryUsage = GetPeakMemoryUsage();
    }
    
    memorySnapshots.push_back(snapshot);
    
    std::cout << "Memory snapshot taken";
    if (!label.empty()) {
        std::cout << " (" << label << ")";
    }
    std::cout << ": " << (snapshot.totalMemoryUsage / 1024.0f / 1024.0f) << " MB" << std::endl;
}

size_t Profiler::GetCurrentMemoryUsage() const {
#ifdef PLATFORM_WINDOWS
    // Windows implementation
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize; // Physical memory used by process
    }
    return 0;
#elif defined(PLATFORM_LINUX)
    // Linux implementation
    FILE* file = fopen("/proc/self/statm", "r");
    if (file) {
        long size, resident, share, text, lib, data, dt;
        fscanf(file, "%ld %ld %ld %ld %ld %ld %ld", &size, &resident, &share, &text, &lib, &data, &dt);
        fclose(file);
        return resident * sysconf(_SC_PAGESIZE); // Resident set size in bytes
    }
    return 0;
#else
    return 0;
#endif
}

size_t Profiler::GetPeakMemoryUsage() const {
    size_t peak = 0;
    for (const auto& snapshot : memorySnapshots) {
        if (snapshot.totalMemoryUsage > peak) {
            peak = snapshot.totalMemoryUsage;
        }
    }
    return peak;
}

void Profiler::TrackComponentMemory(const std::string& componentName, size_t size) {
    if (!isEnabled || memorySnapshots.empty()) return;
    
    auto& latestSnapshot = memorySnapshots.back();
    latestSnapshot.componentMemoryUsage[componentName] += size;
}

size_t Profiler::GetComponentMemoryUsage(const std::string& componentName) const {
    if (memorySnapshots.empty()) return 0;
    
    const auto& latestSnapshot = memorySnapshots.back();
    auto it = latestSnapshot.componentMemoryUsage.find(componentName);
    if (it != latestSnapshot.componentMemoryUsage.end()) {
        return it->second;
    }
    return 0;
}

void Profiler::PrintMemoryReport() const {
    if (memorySnapshots.empty()) {
        std::cout << "No memory snapshots available." << std::endl;
        return;
    }
    
    std::cout << "\n===== Memory Usage Report =====\n";
    std::cout << "Current memory usage: " << (GetCurrentMemoryUsage() / 1024.0f / 1024.0f) << " MB\n";
    std::cout << "Peak memory usage: " << (GetPeakMemoryUsage() / 1024.0f / 1024.0f) << " MB\n";
    
    std::cout << "\nMemory snapshots:\n";
    for (size_t i = 0; i < memorySnapshots.size(); ++i) {
        const auto& snapshot = memorySnapshots[i];
        auto time = std::chrono::system_clock::to_time_t(snapshot.timestamp);
        std::cout << "Snapshot " << i << " [" << std::ctime(&time) << "]: "
                  << (snapshot.totalMemoryUsage / 1024.0f / 1024.0f) << " MB\n";
        
        if (!snapshot.componentMemoryUsage.empty()) {
            std::cout << "  Component breakdown:\n";
            for (const auto& component : snapshot.componentMemoryUsage) {
                std::cout << "    " << component.first << ": " 
                          << (component.second / 1024.0f) << " KB\n";
            }
        }
    }
    
    std::cout << "==============================\n";
}

void Profiler::SaveMemoryReportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for memory report: " << filename << std::endl;
        return;
    }
    
    file << "===== Memory Usage Report =====\n";
    file << "Current memory usage: " << (GetCurrentMemoryUsage() / 1024.0f / 1024.0f) << " MB\n";
    file << "Peak memory usage: " << (GetPeakMemoryUsage() / 1024.0f / 1024.0f) << " MB\n";
    
    file << "\nMemory snapshots:\n";
    for (size_t i = 0; i < memorySnapshots.size(); ++i) {
        const auto& snapshot = memorySnapshots[i];
        auto time = std::chrono::system_clock::to_time_t(snapshot.timestamp);
        file << "Snapshot " << i << " [" << std::ctime(&time) << "]: "
             << (snapshot.totalMemoryUsage / 1024.0f / 1024.0f) << " MB\n";
        
        if (!snapshot.componentMemoryUsage.empty()) {
            file << "  Component breakdown:\n";
            for (const auto& component : snapshot.componentMemoryUsage) {
                file << "    " << component.first << ": " 
                     << (component.second / 1024.0f) << " KB\n";
            }
        }
    }
    
    file << "==============================\n";
    file.close();
    
    std::cout << "Memory report saved to " << filename << std::endl;
}

void Profiler::ClearSnapshots() {
    memorySnapshots.clear();
}
