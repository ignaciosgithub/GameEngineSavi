#!/bin/bash

# Update include paths for gl.h
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <gl\/gl.h>/#include "ThirdParty\/OpenGL\/include\/GL\/platform_gl.h"/'
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <GL\/gl.h>/#include "ThirdParty\/OpenGL\/include\/GL\/platform_gl.h"/'

# Update include paths for glu.h
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <gl\/glu.h>//'
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <GL\/glu.h>//'

# Update include paths for glut.h
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <GL\/glut.h>/#include "ThirdParty\/OpenGL\/include\/GL\/glut.h"/'

# Update include paths for glx.h
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include <GL\/glx.h>//'

# Update include paths for gl_types.h
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include "GL\/gl_types.h"/#include "ThirdParty\/OpenGL\/include\/GL\/gl_types.h"/'
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include "..\/..\/GL\/gl_types.h"/#include "ThirdParty\/OpenGL\/include\/GL\/gl_types.h"/'
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include "..\/gl_types.h"/#include "ThirdParty\/OpenGL\/include\/GL\/gl_types.h"/'
find . -type f -name "*.h" -o -name "*.cpp" | xargs sed -i 's/#include "gl_types.h"/#include "ThirdParty\/OpenGL\/include\/GL\/gl_types.h"/'
