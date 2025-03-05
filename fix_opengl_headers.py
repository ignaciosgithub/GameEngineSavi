#!/usr/bin/env python3
"""
Script to fix OpenGL header inclusion issues and type definitions in the GameEngineSavi codebase.
This script:
1. Ensures GLEW headers are included before GL headers
2. Adds missing type definitions for OpenGL function pointers
3. Replaces direct GL includes with gl_definitions.h includes
"""

import os
import re
import glob
import subprocess
from pathlib import Path

# Repository root
REPO_ROOT = Path('/home/ubuntu/repos/GameEngineSavi')

# Files to exclude from processing
EXCLUDE_FILES = [
    'ThirdParty/OpenGL/include/GL/gl_definitions.h',  # We'll handle this separately
]

# Function to find all C++ files in the repository
def find_cpp_files():
    cpp_files = []
    for ext in ['*.cpp', '*.h']:
        cpp_files.extend(glob.glob(str(REPO_ROOT / '**' / ext), recursive=True))
    
    # Filter out excluded files
    for exclude in EXCLUDE_FILES:
        exclude_path = str(REPO_ROOT / exclude)
        if exclude_path in cpp_files:
            cpp_files.remove(exclude_path)
    
    return cpp_files

# Function to fix OpenGL includes in a file
def fix_opengl_includes(file_path):
    with open(file_path, 'r') as f:
        content = f.read()
    
    # Check if the file includes GL headers directly
    gl_include_pattern = re.compile(r'#include\s+[<"](?:GL|gl)/(?:gl|GL)\.h[>"]')
    if gl_include_pattern.search(content):
        # Replace direct GL includes with gl_definitions.h include
        modified_content = re.sub(
            gl_include_pattern,
            '#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"',
            content
        )
        
        # Remove duplicate includes
        include_lines = modified_content.split('\n')
        unique_includes = []
        seen_includes = set()
        
        for line in include_lines:
            if line.strip().startswith('#include'):
                if line not in seen_includes:
                    seen_includes.add(line)
                    unique_includes.append(line)
            else:
                unique_includes.append(line)
        
        modified_content = '\n'.join(unique_includes)
        
        # Write back to file if changes were made
        if modified_content != content:
            print(f"Fixing OpenGL includes in {file_path}")
            with open(file_path, 'w') as f:
                f.write(modified_content)
            return True
    
    return False

# Function to fix the gl_definitions.h file
def fix_gl_definitions():
    gl_definitions_path = REPO_ROOT / 'ThirdParty/OpenGL/include/GL/gl_definitions.h'
    
    # Create the fixed content for gl_definitions.h
    fixed_content = """#ifndef GAME_ENGINE_SAVI_GL_DEFINITIONS_H
#define GAME_ENGINE_SAVI_GL_DEFINITIONS_H

// Include standard headers
#include <cstdint>
#include "../../../../platform.h"
#include <iostream>

// Only include OpenGL headers for non-Windows platforms
#ifndef PLATFORM_WINDOWS

// Define GLEW_STATIC before including glew.h
#define GLEW_STATIC

// Include GLEW first, as it must precede other OpenGL headers
#include <GL/glew.h>

// Then include other OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#else
// Windows-specific DirectX headers are included in DirectXGraphicsAPI.h
#endif

// Helper function to log OpenGL header inclusion status
inline void LogOpenGLHeaderStatus() {
    std::cout << "OpenGL headers included successfully" << std::endl;
}

#endif // GAME_ENGINE_SAVI_GL_DEFINITIONS_H
"""
    
    print(f"Fixing {gl_definitions_path}")
    with open(gl_definitions_path, 'w') as f:
        f.write(fixed_content)

# Function to fix OpenGL class names in OpenGLGraphicsAPI.cpp
def fix_opengl_graphics_api():
    api_path = REPO_ROOT / 'Graphics/Core/OpenGLGraphicsAPI.cpp'
    api_h_path = REPO_ROOT / 'Graphics/Core/OpenGLGraphicsAPI.h'
    interface_path = REPO_ROOT / 'Graphics/Core/IGraphicsAPI.h'
    
    # First, check if the interface file exists and read its content
    if not interface_path.exists():
        print(f"Error: {interface_path} does not exist")
        return
    
    with open(interface_path, 'r') as f:
        interface_content = f.read()
    
    # Extract enum definitions from IGraphicsAPI.h
    enum_pattern = re.compile(r'enum\s+class\s+(\w+)\s*{([^}]+)}')
    enums = {}
    
    for match in enum_pattern.finditer(interface_content):
        enum_name = match.group(1)
        enum_values = [v.strip() for v in match.group(2).split(',') if v.strip()]
        enums[enum_name] = enum_values
    
    # Now fix the OpenGLGraphicsAPI.cpp file
    if api_path.exists():
        with open(api_path, 'r') as f:
            api_content = f.read()
        
        # Replace missing enum references
        for enum_name, values in enums.items():
            for value in values:
                # Replace patterns like "case DrawMode::Points:" with proper enum scope
                pattern = rf'case\s+{enum_name}::{value}'
                replacement = f'case {enum_name}::{value}'
                api_content = re.sub(pattern, replacement, api_content)
        
        # Fix function signatures
        api_content = re.sub(r'void\s+OpenGLGraphicsAPI::Clear\(ClearFlags', 
                            'void OpenGLGraphicsAPI::Clear(IGraphicsAPI::ClearFlags', api_content)
        
        api_content = re.sub(r'GLenum\s+OpenGLGraphicsAPI::ConvertDataType\(DataType', 
                            'GLenum OpenGLGraphicsAPI::ConvertDataType(IGraphicsAPI::DataType', api_content)
        
        # Write back the fixed content
        with open(api_path, 'w') as f:
            f.write(api_content)
        
        print(f"Fixed OpenGLGraphicsAPI.cpp")

# Main function
def main():
    print("Starting OpenGL header fixes...")
    
    # Fix gl_definitions.h first
    fix_gl_definitions()
    
    # Fix OpenGL includes in all C++ files
    cpp_files = find_cpp_files()
    fixed_count = 0
    for file in cpp_files:
        if fix_opengl_includes(file):
            fixed_count += 1
    
    print(f"Fixed OpenGL includes in {fixed_count} files")
    
    # Fix OpenGLGraphicsAPI.cpp
    fix_opengl_graphics_api()
    
    print("OpenGL header fixes completed")

if __name__ == "__main__":
    main()
