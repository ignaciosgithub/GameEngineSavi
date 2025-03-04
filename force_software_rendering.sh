#!/bin/bash

# Backup the original EditorMain.cpp
cp Editor/EditorMain.cpp Editor/EditorMain.cpp.backup

# Modify EditorMain.cpp to force software rendering
sed -i '/glXMakeCurrent(display, window, context);/a \
    // Force software rendering\n\
    putenv((char*)"LIBGL_ALWAYS_SOFTWARE=1");\n\
    std::cout << "Forcing software rendering" << std::endl;' Editor/EditorMain.cpp

# Check if build_editor_linux.sh exists
if [ -f "./build_editor_linux.sh" ]; then
    # Rebuild the editor
    echo "Rebuilding editor with software rendering forced..."
    ./build_editor_linux.sh
else
    echo "Warning: build_editor_linux.sh not found. You will need to rebuild the editor manually."
fi

echo "Done. Run the editor with:"
echo "LIBGL_ALWAYS_SOFTWARE=1 ./bin/linux/Editor"
echo ""
echo "If the editor still doesn't display, try setting the DISPLAY variable:"
echo "export DISPLAY=10.0.2.2:0.0"
echo "LIBGL_ALWAYS_SOFTWARE=1 ./bin/linux/Editor"
