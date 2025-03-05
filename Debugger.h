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
    
    // Structure for tracking redundant declarations
    struct RedundancyInfo {
        std::string fileName;
        std::string symbolName;
        std::string type;  // "class", "method", "type", "include"
        int lineNumber;
        
        RedundancyInfo(const std::string& file, const std::string& symbol, 
                      const std::string& t, int line)
            : fileName(file), symbolName(symbol), type(t), lineNumber(line) {}
    };
    
    // Structure for tracking import errors
    struct ImportError {
        std::string message;
        std::string filePath;
        std::string importType;  // "shader", "texture", "model", etc.
        
        ImportError(const std::string& msg, const std::string& path, const std::string& type)
            : message(msg), filePath(path), importType(type) {}
    };
    
    static std::vector<ScriptError> errors;
    static std::vector<RedundancyInfo> redundancies;  // Track redundant declarations
    static std::vector<ImportError> importErrors;     // Track import errors
    bool showErrorPanel;
    bool showRedundancyPanel;  // New flag for redundancy panel
    bool showImportErrorPanel;  // New flag for import error panel
    static Debugger* instance;
    
    Debugger();  // Private constructor for singleton
    
public:
    static Debugger& GetInstance();
    
    // Log an error with full context
    void LogError(const std::string& message, const std::string& scriptName,
                 const std::string& functionName, int lineNumber);
    
    // Log a simple error message
    void LogError(const std::string& message);
    
    // Method to log redundant declarations
    void LogRedundancy(const std::string& fileName, const std::string& symbolName,
                      const std::string& type, int lineNumber);
    
    // Method to log import errors
    void LogImportError(const std::string& message, const std::string& filePath,
                       const std::string& importType);
    
    // Clear all logged errors
    void ClearErrors();
    
    // Clear all logged redundancies
    void ClearRedundancies();
    
    // Clear all logged import errors
    void ClearImportErrors();
    
    // Get all current errors
    static const std::vector<ScriptError>& GetErrors();
    
    // Get all current redundancies
    static const std::vector<RedundancyInfo>& GetRedundancies();
    
    // Get all current import errors
    static const std::vector<ImportError>& GetImportErrors();
    
    // Update error display
    void Update();
    
    // New method to check for redundant declarations in the codebase
    void CheckForRedundancies();
    
    // Check if there are any errors
    static bool HasErrors();
    
    // Check if there are any redundant declarations
    static bool HasRedundancies();
    
    // Check if there are any import errors
    static bool HasImportErrors();
    
    // Print all errors to the console
    static void PrintErrors();
    
    // Print all redundant declarations to the console
    static void PrintRedundancies();
    
    // Print all import errors to the console
    static void PrintImportErrors();
    
    // Methods to control display panels
    void ShowErrorPanel(bool show) { showErrorPanel = show; }
    void ShowRedundancyPanel(bool show) { showRedundancyPanel = show; }
    void ShowImportErrorPanel(bool show) { showImportErrorPanel = show; }
    
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
    
    // Try-catch wrapper for import operations
    template<typename Func>
    bool TryImport(Func&& func, const std::string& filePath, 
                  const std::string& importType) {
        try {
            return func();
        } catch (const std::exception& e) {
            LogImportError(e.what(), filePath, importType);
            return false;
        } catch (...) {
            LogImportError("Unknown error occurred during import", filePath, importType);
            return false;
        }
    }
};

#endif // DEBUGGER_H
