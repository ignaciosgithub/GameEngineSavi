#!/usr/bin/env python3
"""
Create test frames for the enhanced emergency editor
"""

import os
import sys
from PIL import Image, ImageDraw, ImageFont
import numpy as np

def create_test_frame(frame_number, active_panel=1):
    """Create a test frame with panel layout."""
    # Create a new image with white background
    img = Image.new('RGB', (800, 600), color=(50, 50, 50))
    draw = ImageDraw.Draw(img)
    
    # Draw panel borders
    # Hierarchy panel (left)
    draw.rectangle([(0, 0), (200, 600)], outline=(255, 255, 255), width=2)
    # Scene view panel (center)
    draw.rectangle([(200, 0), (600, 400)], outline=(255, 255, 255), width=2)
    # Inspector panel (right)
    draw.rectangle([(600, 0), (800, 600)], outline=(255, 255, 255), width=2)
    # Project panel (bottom)
    draw.rectangle([(200, 400), (600, 600)], outline=(255, 255, 255), width=2)
    
    # Highlight active panel
    if active_panel == 0:  # Hierarchy panel
        draw.rectangle([(0, 0), (200, 30)], outline=(0, 255, 0), width=3)
    elif active_panel == 1:  # Scene view panel
        draw.rectangle([(200, 0), (600, 30)], outline=(0, 255, 0), width=3)
    elif active_panel == 2:  # Inspector panel
        draw.rectangle([(600, 0), (800, 30)], outline=(0, 255, 0), width=3)
    elif active_panel == 3:  # Project panel
        draw.rectangle([(200, 400), (600, 430)], outline=(0, 255, 0), width=3)
    
    # Draw panel titles
    draw.text((10, 10), "Hierarchy Panel", fill=(255, 255, 255))
    draw.text((300, 10), "Scene View Panel", fill=(255, 255, 255))
    draw.text((610, 10), "Inspector Panel", fill=(255, 255, 255))
    draw.text((300, 410), "Project Panel", fill=(255, 255, 255))
    
    # Draw a simple 3D cube in the scene view panel
    cube_center = (400, 200)
    cube_size = 100
    rotation = frame_number * 5  # Rotate the cube slightly in each frame
    
    # Define cube vertices
    vertices = [
        (-1, -1, -1),  # 0
        (1, -1, -1),   # 1
        (1, 1, -1),    # 2
        (-1, 1, -1),   # 3
        (-1, -1, 1),   # 4
        (1, -1, 1),    # 5
        (1, 1, 1),     # 6
        (-1, 1, 1)     # 7
    ]
    
    # Define cube edges
    edges = [
        (0, 1), (1, 2), (2, 3), (3, 0),  # Bottom face
        (4, 5), (5, 6), (6, 7), (7, 4),  # Top face
        (0, 4), (1, 5), (2, 6), (3, 7)   # Connecting edges
    ]
    
    # Apply rotation
    rotation_rad = np.radians(rotation)
    cos_r = np.cos(rotation_rad)
    sin_r = np.sin(rotation_rad)
    
    # Rotate around Y axis
    rotated_vertices = []
    for x, y, z in vertices:
        new_x = x * cos_r - z * sin_r
        new_z = x * sin_r + z * cos_r
        rotated_vertices.append((new_x, y, new_z))
    
    # Project 3D to 2D
    projected_vertices = []
    for x, y, z in rotated_vertices:
        # Simple perspective projection
        scale = 200 / (4 - z)
        px = cube_center[0] + x * scale
        py = cube_center[1] - y * scale  # Flip y for screen coordinates
        projected_vertices.append((px, py))
    
    # Draw edges
    for edge in edges:
        start = projected_vertices[edge[0]]
        end = projected_vertices[edge[1]]
        draw.line([start, end], fill=(255, 255, 255), width=2)
    
    # Draw frame number
    draw.text((700, 570), f"Frame: {frame_number}", fill=(255, 255, 255))
    
    # Save the image
    os.makedirs("frames", exist_ok=True)
    img.save(f"frames/frame{frame_number}.png")
    print(f"Created frame{frame_number}.png")

def main():
    """Main function."""
    # Create 10 test frames
    for i in range(10):
        create_test_frame(i)
    
    # Create panel info file
    with open("frames/panel_info.txt", "w") as f:
        f.write("Active Panel: 1\n")
        f.write("Scene View Panel\n")
    
    # Create hotkey state file
    with open("frames/hotkey_state.txt", "w") as f:
        f.write("Active Panel: 1\n")
        f.write("W: Released\n")
        f.write("A: Released\n")
        f.write("S: Released\n")
        f.write("D: Released\n")
        f.write("Shift: Released\n")
        f.write("Space: Released\n")
        f.write("Ctrl: Released\n")
        f.write("Ctrl+1: Released\n")
        f.write("Ctrl+2: Released\n")
        f.write("Ctrl+3: Released\n")
        f.write("Ctrl+4: Released\n")
        f.write("Escape: Released\n")
        f.write("F12: Released\n")

if __name__ == "__main__":
    main()
