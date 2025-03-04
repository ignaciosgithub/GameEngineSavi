#ifndef REDUNDANCY_DETECTOR_H
#define REDUNDANCY_DETECTOR_H

#include <string>
#include <vector>
#include <map>
#include <set>

class RedundancyDetector {
private:
    struct SymbolInfo {
        std::string fileName;
        int lineNumber;
        std::string type;  // "class", "method", "type", "include"
        
        SymbolInfo(const std::string& file, int line, const std::string& t)
            : fileName(file), lineNumber(line), type(t) {}
    };
    
    std::map<std::string, std::vector<SymbolInfo>> symbols;
    std::set<std::string> processedFiles;
    
public:
    RedundancyDetector();
    
    // Process a file to extract symbol declarations
    void ProcessFile(const std::string& fileName);
    
    // Process all files in a directory recursively
    void ProcessDirectory(const std::string& dirPath);
    
    // Get redundant symbols (symbols declared multiple times)
    std::map<std::string, std::vector<SymbolInfo>> GetRedundantSymbols() const;
    
    // Check if a file has already been processed
    bool IsFileProcessed(const std::string& fileName) const;
    
    // Mark a file as processed
    void MarkFileAsProcessed(const std::string& fileName);
};

#endif // REDUNDANCY_DETECTOR_H
