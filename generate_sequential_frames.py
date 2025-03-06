#!/usr/bin/env python3
"""
Generate sequential frames for the GameEngineSavi frame display script.
This script creates simple PNG images with a red cube that rotates over time.
"""

import os
import math
from PIL import Image, ImageDraw

# Create frames directory if it doesn't exist
os.makedirs('frames', exist_ok=True)

# Clean up old frames
for f in os.listdir('frames'):
    if f.startswith('frame') and f.endswith('.png'):
        os.remove(os.path.join('frames', f))

# Create 102 frames (0-101) with a rotating red cube
for i in range(102):
    # Create a simple image with text
    img = Image.new('RGBA', (800, 600), color=(50, 50, 80, 255))
    draw = ImageDraw.Draw(img)
    
    # Calculate rotation angle
    angle = i * 3.6  # 360 degrees / 100 frames
    
    # Calculate cube position based on rotation
    center_x, center_y = 400, 300
    radius = 100
    offset_x = radius * math.cos(math.radians(angle))
    offset_y = radius * math.sin(math.radians(angle))
    
    # Draw a red cube (simplified)
    cube_size = 100
    x1 = center_x - cube_size/2 + offset_x
    y1 = center_y - cube_size/2 + offset_y
    x2 = center_x + cube_size/2 + offset_x
    y2 = center_y + cube_size/2 + offset_y
    
    # Draw cube faces with different shades of red to simulate 3D
    # Back face (darker)
    draw.polygon([(x1, y1), (x2, y1), (x2, y2), (x1, y2)], 
                fill=(180, 0, 0, 255), outline=(255, 255, 255, 255))
    
    # Add some shading based on rotation
    shade = int(128 + 127 * math.sin(math.radians(angle)))
    
    # Right face
    draw.polygon([(x2, y1), (x2+20, y1-20), (x2+20, y2-20), (x2, y2)], 
                fill=(shade, 0, 0, 255), outline=(255, 255, 255, 255))
    
    # Top face
    draw.polygon([(x1, y1), (x2, y1), (x2+20, y1-20), (x1+20, y1-20)], 
                fill=(255, 0, 0, 255), outline=(255, 255, 255, 255))
    
    # Add text
    draw.text((10, 10), f'GameEngineSavi Headless Editor', fill=(255, 255, 255, 255))
    draw.text((10, 30), f'Frame {i} - Press WASD to move camera', fill=(255, 255, 255, 255))
    
    # Add key press simulation info
    key_info = []
    if i % 3 == 0: key_info.append("W")  # Move forward every 3rd frame
    if i % 5 == 1: key_info.append("S")  # Move backward every 5th frame
    if i % 4 == 2: key_info.append("A")  # Move left every 4th frame
    if i % 4 == 3: key_info.append("D")  # Move right every 4th frame
    
    if key_info:
        draw.text((10, 50), f'Key pressed: {", ".join(key_info)}', fill=(255, 255, 0, 255))
    
    # Add editor UI elements to simulate full editor
    # Panel borders
    draw.rectangle((0, 0, 799, 599), outline=(100, 100, 100, 255))
    draw.line((200, 0, 200, 600), fill=(100, 100, 100, 255))
    draw.line((600, 0, 600, 600), fill=(100, 100, 100, 255))
    draw.line((0, 500, 800, 500), fill=(100, 100, 100, 255))
    
    # Panel headers
    draw.rectangle((0, 0, 200, 30), fill=(70, 70, 90, 255))
    draw.rectangle((200, 0, 600, 30), fill=(70, 70, 90, 255))
    draw.rectangle((600, 0, 800, 30), fill=(70, 70, 90, 255))
    draw.rectangle((0, 500, 800, 530), fill=(70, 70, 90, 255))
    
    # Panel titles
    draw.text((10, 5), "Hierarchy", fill=(255, 255, 255, 255))
    draw.text((350, 5), "Scene View", fill=(255, 255, 255, 255))
    draw.text((650, 5), "Inspector", fill=(255, 255, 255, 255))
    draw.text((10, 505), "Project", fill=(255, 255, 255, 255))
    
    # Hierarchy panel content
    draw.text((10, 40), "Scene", fill=(255, 255, 255, 255))
    draw.text((20, 60), "└ Main Camera", fill=(200, 200, 255, 255))
    draw.text((20, 80), "└ Directional Light", fill=(255, 255, 200, 255))
    draw.text((20, 100), "└ Red Cube", fill=(255, 200, 200, 255))
    
    # Inspector panel content
    draw.text((610, 40), "Red Cube", fill=(255, 200, 200, 255))
    draw.text((610, 60), "Transform:", fill=(255, 255, 255, 255))
    draw.text((620, 80), f"Position: ({offset_x:.1f}, {offset_y:.1f}, 0.0)", fill=(200, 200, 255, 255))
    draw.text((620, 100), f"Rotation: (0.0, 0.0, {angle:.1f})", fill=(200, 200, 255, 255))
    draw.text((620, 120), "Scale: (1.0, 1.0, 1.0)", fill=(200, 200, 255, 255))
    draw.text((610, 150), "Material:", fill=(255, 255, 255, 255))
    draw.text((620, 170), "Color: (1.0, 0.0, 0.0, 1.0)", fill=(255, 200, 200, 255))
    
    # Project panel content
    draw.text((10, 540), "Assets/", fill=(255, 255, 255, 255))
    draw.text((20, 560), "└ Materials/", fill=(200, 255, 200, 255))
    draw.text((20, 580), "└ Models/", fill=(200, 200, 255, 255))
    
    # Save the image
    img.save(f'frames/frame{i}.png')
    print(f'Created frame: frames/frame{i}.png')

print('Generated 102 sequential frames (0-101)')
