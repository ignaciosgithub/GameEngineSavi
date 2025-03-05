#!/usr/bin/env python3
"""
Convert PPM files to PNG format
"""

import os
import sys
from PIL import Image

def convert_ppm_to_png(ppm_file, png_file):
    """Convert a PPM file to PNG format."""
    try:
        img = Image.open(ppm_file)
        img.save(png_file)
        print(f"Converted {ppm_file} to {png_file}")
        return True
    except Exception as e:
        print(f"Error converting {ppm_file} to {png_file}: {e}")
        return False

def main():
    """Main function."""
    # Convert frame0.ppm to frame0.png
    if os.path.exists("frames/frame0.ppm"):
        convert_ppm_to_png("frames/frame0.ppm", "frames/frame0.png")
        
        # Create additional frames for testing
        for i in range(1, 10):
            os.system(f"cp frames/frame0.png frames/frame{i}.png")
            print(f"Created frames/frame{i}.png")
    else:
        print("frames/frame0.ppm not found")

if __name__ == "__main__":
    main()
