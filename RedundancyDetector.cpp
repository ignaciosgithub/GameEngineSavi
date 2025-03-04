#include "RedundancyDetector.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <dirent.h> // Use dirent.h instead of filesystem for better compatibility

RedundancyDetector::RedundancyDetector() {
    // Initialize the detector
}

void RedundancyDetector::ProcessFile(const std::string& fileName) {
    if (IsFileProcessed(fileName)) {
        return;
    }
    
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }
    
    std::string line;
    int lineNumber = 0;
    
    // Regular expressions for detecting declarations
    std::regex classRegex("\\s*class\\s+(\\w+)");
    std::regex structRegex("\\s*struct\\s+(\\w+)");
    std::regex methodRegex("\\s*(\\w+)::(\\w+)\\s*\\(");
    std::regex typedefRegex("\\s*typedef\\s+.*\\s+(\\w+)\\s*;");
    std::regex includeRegex("\\s*#include\\s+[\"<](.*?)[\">]");
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Check for class declarations
        std::smatch match;
        if (std::regex_search(line, match, classRegex)) {
            std::string className = match[1];
            symbols[className].emplace_back(fileName, lineNumber, "class");
        }
        
        // Check for struct declarations
        if (std::regex_search(line, match, structRegex)) {
            std::string structName = match[1];
            symbols[structName].emplace_back(fileName, lineNumber, "struct");
        }
        
        // Check for method implementations
        if (std::regex_search(line, match, methodRegex)) {
            std::string className = match[1];
            std::string methodName = match[2];
            std::string fullName = className + "::" + methodName;
            symbols[fullName].emplace_back(fileName, lineNumber, "method");
        }
        
        // Check for typedef declarations
        if (std::regex_search(line, match, typedefRegex)) {
            std::string typeName = match[1];
            symbols[typeName].emplace_back(fileName, lineNumber, "type");
        }
        
        // Check for include directives
        if (std::regex_search(line, match, includeRegex)) {
            std::string includePath = match[1];
            symbols["include:" + includePath].emplace_back(fileName, lineNumber, "include");
        }
    }
    
    MarkFileAsProcessed(fileName);
}

void RedundancyDetector::ProcessDirectory(const std::string& dirPath) {
    // Use dirent.h for directory traversal instead of filesystem
    std::vector<std::string> directories;
    directories.push_back(dirPath);
    
    while (!directories.empty()) {
        std::string currentDir = directories.back();
        directories.pop_back();
        
        DIR* dir = opendir(currentDir.c_str());
        if (!dir) {
            std::cerr << "Failed to open directory: " << currentDir << std::endl;
            continue;
        }
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;
            
            // Skip . and ..
            if (name == "." || name == "..") {
                continue;
            }
            
            std::string path = currentDir + "/" + name;
            
            // Check if it's a directory
            DIR* subdir = opendir(path.c_str());
            if (subdir) {
                closedir(subdir);
                directories.push_back(path);
                continue;
            }
            
            // Check if it's a .h or .cpp file
            size_t dotPos = name.find_last_of('.');
            if (dotPos != std::string::npos) {
                std::string extension = name.substr(dotPos);
                if (extension == ".h" || extension == ".cpp") {
                    ProcessFile(path);
                }
            }
        }
        
        closedir(dir);
    }
}

std::map<std::string, std::vector<RedundancyDetector::SymbolInfo>> RedundancyDetector::GetRedundantSymbols() const {
    std::map<std::string, std::vector<SymbolInfo>> redundantSymbols;
    
    for (const auto& pair : symbols) {
        if (pair.second.size() > 1) {
            // Check if this is a genuine redundancy
            bool isRedundant = true;
            
            // Skip include directives (they're expected to appear multiple times)
            if (pair.second[0].type == "include") {
                isRedundant = false;
            }
            
            // Skip method declarations in header and implementations in source
            if (pair.second[0].type == "method") {
                bool hasHeader = false;
                bool hasSource = false;
                
                for (const auto& info : pair.second) {
                    // Get file extension without using filesystem
                    size_t dotPos = info.fileName.find_last_of('.');
                    if (dotPos != std::string::npos) {
                        std::string extension = info.fileName.substr(dotPos);
                        if (extension == ".h") {
                            hasHeader = true;
                        } else if (extension == ".cpp") {
                            hasSource = true;
                        }
                    }
                }
                
                // If we have exactly one header declaration and one source implementation,
                // this is not a redundancy
                if (hasHeader && hasSource && pair.second.size() == 2) {
                    isRedundant = false;
                }
            }
            
            if (isRedundant) {
                redundantSymbols[pair.first] = pair.second;
            }
        }
    }
    
    return redundantSymbols;
}

bool RedundancyDetector::IsFileProcessed(const std::string& fileName) const {
    return processedFiles.find(fileName) != processedFiles.end();
}

void RedundancyDetector::MarkFileAsProcessed(const std::string& fileName) {
    processedFiles.insert(fileName);
}
