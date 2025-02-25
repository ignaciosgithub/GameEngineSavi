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
    
    std::vector<ScriptError> errors;
    bool showErrorPanel;
    static Debugger* instance;
    
    Debugger();  // Private constructor for singleton
    
public:
    static Debugger& GetInstance();
    
    // Log an error with full context
    void LogError(const std::string& message, const std::string& scriptName,
                 const std::string& functionName, int lineNumber);
    
    // Log a simple error message
    void LogError(const std::string& message);
    
    // Clear all logged errors
    void ClearErrors();
    
    // Get all current errors
    const std::vector<ScriptError>& GetErrors() const;
    
    // Update error display
    void Update();
    
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
