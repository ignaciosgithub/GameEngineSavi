#!/usr/bin/env python3
"""
Create a test frame for the GameEngineSavi frame display script.
This script generates a simple PNG image with a red cube and text.
"""

import os
from PIL import Image, ImageDraw

# Create a directory for frames if it doesn't exist
os.makedirs('frames', exist_ok=True)

# Create a simple image with text
img = Image.new('RGBA', (800, 600), color=(50, 50, 80, 255))
draw = ImageDraw.Draw(img)

# Draw a red cube (simplified)
draw.rectangle((300, 200, 500, 400), fill=(255, 0, 0, 255), outline=(255, 255, 255, 255))

# Add text
draw.text((10, 10), 'GameEngineSavi Headless Editor', fill=(255, 255, 255, 255))
draw.text((10, 30), 'Frame 0 - Press WASD to move camera', fill=(255, 255, 255, 255))

# Save the image
img.save('frames/frame0.png')
print('Created test frame: frames/frame0.png')
