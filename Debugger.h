#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <vector>
#include <functional>
#include <memory>

class Debugger {
private:
    struct ScriptError {
        std::string message;
        std::string scriptName;
        std::string functionName;
        int lineNumber;
        
        ScriptError(const std::string& msg, const std::string& script, 
                   const std::string& func, int line)
            : message(msg), scriptName(script), functionName(func), lineNumber(line) {}
    };
    
    // New structure for tracking redundant declarations
    struct RedundancyInfo {
        std::string fileName;
        std::string symbolName;
        std::string type;  // "class", "method", "type", "include"
        int lineNumber;
        
        RedundancyInfo(const std::string& file, const std::string& symbol, 
                      const std::string& t, int line)
            : fileName(file), symbolName(symbol), type(t), lineNumber(line) {}
    };
    
    std::vector<ScriptError> errors;
    std::vector<RedundancyInfo> redundancies;  // Track redundant declarations
    bool showErrorPanel;
    bool showRedundancyPanel;  // New flag for redundancy panel
    static Debugger* instance;
    
    Debugger();  // Private constructor for singleton
    
public:
    static Debugger& GetInstance();
    
    // Log an error with full context
    void LogError(const std::string& message, const std::string& scriptName,
                 const std::string& functionName, int lineNumber);
    
    // Log a simple error message
    void LogError(const std::string& message);
    
    // New method to log redundant declarations
    void LogRedundancy(const std::string& fileName, const std::string& symbolName,
                      const std::string& type, int lineNumber);
    
    // Clear all logged errors
    void ClearErrors();
    
    // Clear all logged redundancies
    void ClearRedundancies();
    
    // Get all current errors
    const std::vector<ScriptError>& GetErrors() const;
    
    // Get all current redundancies
    const std::vector<RedundancyInfo>& GetRedundancies() const;
    
    // Update error display
    void Update();
    
    // New method to check for redundant declarations in the codebase
    void CheckForRedundancies();
    
    // Try-catch wrapper for script execution
    template<typename Func>
    bool TryExecute(Func&& func, const std::string& scriptName, 
                   const std::string& functionName) {
        try {
            func();
            return true;
        } catch (const std::exception& e) {
            LogError(e.what(), scriptName, functionName, -1);
            return false;
        } catch (...) {
            LogError("Unknown error occurred", scriptName, functionName, -1);
            return false;
        }
    }
};

#endif // DEBUGGER_H
