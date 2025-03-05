#!/usr/bin/env python3
"""
Analyze a frame image to verify shading and point light.
"""
import numpy as np
from PIL import Image
import sys

def analyze_frame(frame_path):
    """Analyze a frame image."""
    # Open image
    img = Image.open(frame_path)
    print(f'Image size: {img.size}')
    
    # Convert to numpy array
    pixels = np.array(img)
    print(f'Pixel data shape: {pixels.shape}')
    
    # Analyze colors
    unique_colors = len(np.unique(pixels.reshape(-1, 3), axis=0))
    print(f'Unique colors: {unique_colors}')
    print(f'Color range: min={pixels.min()}, max={pixels.max()}')
    
    # Sample pixels
    print('Sample pixels:')
    print(pixels[300:305, 400:405])
    
    # Check for shading
    if unique_colors > 100:
        print("Image has good color variation, indicating proper shading.")
    else:
        print("Image has limited color variation, shading might be insufficient.")
    
    # Check for point light
    # Look for bright pixels that could be the point light
    bright_pixels = np.sum(pixels > 200, axis=2) == 3
    if np.any(bright_pixels):
        print("Bright pixels detected, likely indicating point light.")
    else:
        print("No bright pixels detected, point light might be missing.")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        frame_path = sys.argv[1]
    else:
        frame_path = "frames/frame6.png"
    
    analyze_frame(frame_path)
