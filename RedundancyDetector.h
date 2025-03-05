#pragma once
#include <string>
#include <vector>

class RedundancyDetector {
public:
    RedundancyDetector();
    ~RedundancyDetector();
    
    void ProcessDirectory(const std::string& path);
    std::string GetRedundantSymbols() const;
};
