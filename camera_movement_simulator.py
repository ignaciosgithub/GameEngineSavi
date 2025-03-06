#!/usr/bin/env python3
"""
Camera Movement Simulator for GameEngineSavi Headless Editor

This script generates frames showing camera movement around a stationary red cube,
simulating the effect of pressing A+W keys simultaneously.
"""

import os
import math
import time
from PIL import Image, ImageDraw, ImageFont

# Create frames directory if it doesn't exist
os.makedirs('frames', exist_ok=True)

# Clean up old frames
for f in os.listdir('frames'):
    if f.startswith('frame') and f.endswith('.png'):
        os.remove(os.path.join('frames', f))

# Configuration
WIDTH = 800
HEIGHT = 600
TOTAL_FRAMES = 102  # 0-101
CUBE_SIZE = 100
CENTER_X, CENTER_Y = 400, 300

# Camera movement parameters
camera_position = [0.0, 0.0, -300.0]  # x, y, z
camera_rotation = [0.0, 0.0, 0.0]     # pitch, yaw, roll
camera_speed = 5.0
camera_rotation_speed = 2.0

# Key press simulation
keys_pressed = {
    'W': False,
    'A': False,
    'S': False,
    'D': False,
    'Q': False,
    'E': False,
    'R': False,
    'F': False
}

def update_camera(frame_num):
    """Update camera position and rotation based on simulated key presses"""
    # For this simulation, we'll press A+W simultaneously
    keys_pressed['A'] = True
    keys_pressed['W'] = True
    
    # Update camera position based on key presses
    if keys_pressed['W']:
        # Move forward (along camera's forward vector)
        camera_position[0] += math.sin(math.radians(camera_rotation[1])) * camera_speed
        camera_position[2] += math.cos(math.radians(camera_rotation[1])) * camera_speed
    
    if keys_pressed['A']:
        # Strafe left and rotate slightly (combined effect)
        camera_position[0] -= math.cos(math.radians(camera_rotation[1])) * camera_speed * 0.7
        camera_position[2] += math.sin(math.radians(camera_rotation[1])) * camera_speed * 0.7
        camera_rotation[1] += camera_rotation_speed  # Rotate camera left
    
    # Return the updated camera state
    return camera_position.copy(), camera_rotation.copy()

def project_3d_to_2d(point_3d, camera_pos, camera_rot):
    """Project a 3D point to 2D screen coordinates"""
    # Translate point relative to camera
    x = point_3d[0] - camera_pos[0]
    y = point_3d[1] - camera_pos[1]
    z = point_3d[2] - camera_pos[2]
    
    # Apply camera rotation (simplified)
    yaw = math.radians(camera_rot[1])
    
    # Rotate around Y axis (yaw)
    x_rotated = x * math.cos(yaw) - z * math.sin(yaw)
    z_rotated = x * math.sin(yaw) + z * math.cos(yaw)
    
    # Simple perspective projection
    if z_rotated <= 0:
        z_rotated = 0.1  # Avoid division by zero
    
    # Perspective division
    scale = 400.0 / z_rotated
    screen_x = CENTER_X + x_rotated * scale
    screen_y = CENTER_Y - y * scale
    
    return (screen_x, screen_y, z_rotated > 0)

def draw_cube(draw, vertices_2d, faces, colors):
    """Draw a 3D cube using 2D projected vertices"""
    # Draw each face
    for i, face in enumerate(faces):
        # Get the vertices for this face
        face_vertices = [vertices_2d[idx] for idx in face]
        
        # Check if face is visible (all vertices are in front of camera)
        if all(v[2] for v in face_vertices):
            # Extract just the x,y coordinates
            points = [(v[0], v[1]) for v in face_vertices]
            draw.polygon(points, fill=colors[i], outline=(255, 255, 255, 255))

def create_frame(frame_num):
    """Create a single frame with the cube and editor UI"""
    # Update camera based on simulated key presses
    camera_pos, camera_rot = update_camera(frame_num)
    
    # Create a new image
    img = Image.new('RGBA', (WIDTH, HEIGHT), color=(50, 50, 80, 255))
    draw = ImageDraw.Draw(img)
    
    # Define cube vertices (centered at origin)
    half_size = CUBE_SIZE / 2
    vertices = [
        [-half_size, -half_size, -half_size],  # 0: back bottom left
        [half_size, -half_size, -half_size],   # 1: back bottom right
        [half_size, half_size, -half_size],    # 2: back top right
        [-half_size, half_size, -half_size],   # 3: back top left
        [-half_size, -half_size, half_size],   # 4: front bottom left
        [half_size, -half_size, half_size],    # 5: front bottom right
        [half_size, half_size, half_size],     # 6: front top right
        [-half_size, half_size, half_size]     # 7: front top left
    ]
    
    # Define cube faces (indices of vertices)
    faces = [
        [0, 1, 2, 3],  # back
        [4, 5, 6, 7],  # front
        [0, 4, 7, 3],  # left
        [1, 5, 6, 2],  # right
        [3, 2, 6, 7],  # top
        [0, 1, 5, 4]   # bottom
    ]
    
    # Define colors for each face
    colors = [
        (180, 0, 0, 255),    # back: dark red
        (255, 0, 0, 255),    # front: bright red
        (220, 0, 0, 255),    # left: medium red
        (200, 0, 0, 255),    # right: medium-dark red
        (240, 0, 0, 255),    # top: medium-bright red
        (160, 0, 0, 255)     # bottom: darker red
    ]
    
    # Project 3D vertices to 2D
    vertices_2d = []
    for vertex in vertices:
        # Add the vertex to the world position (0,0,0 for this example)
        world_vertex = [vertex[0], vertex[1], vertex[2]]
        screen_vertex = project_3d_to_2d(world_vertex, camera_pos, camera_rot)
        vertices_2d.append(screen_vertex)
    
    # Draw the cube
    draw_cube(draw, vertices_2d, faces, colors)
    
    # Add editor UI elements
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
    draw.text((620, 80), f"Position: (0.0, 0.0, 0.0)", fill=(200, 200, 255, 255))
    draw.text((620, 100), f"Rotation: (0.0, 0.0, 0.0)", fill=(200, 200, 255, 255))
    draw.text((620, 120), "Scale: (1.0, 1.0, 1.0)", fill=(200, 200, 255, 255))
    
    # Camera info
    draw.text((210, 40), f"Camera Position: ({camera_pos[0]:.1f}, {camera_pos[1]:.1f}, {camera_pos[2]:.1f})", 
              fill=(200, 255, 200, 255))
    draw.text((210, 60), f"Camera Rotation: ({camera_rot[0]:.1f}, {camera_rot[1]:.1f}, {camera_rot[2]:.1f})", 
              fill=(200, 255, 200, 255))
    
    # Key press info
    key_text = "Keys Pressed: A+W"
    draw.text((210, 80), key_text, fill=(255, 255, 0, 255))
    
    # Frame info
    draw.text((10, 10), f'GameEngineSavi Headless Editor', fill=(255, 255, 255, 255))
    draw.text((210, 100), f'Frame {frame_num}', fill=(255, 255, 255, 255))
    
    # Save the image
    img.save(f'frames/frame{frame_num}.png')
    print(f'Created frame: frames/frame{frame_num}.png')

def main():
    """Generate all frames with camera movement"""
    print(f"Generating {TOTAL_FRAMES} frames with camera movement...")
    start_time = time.time()
    
    for i in range(TOTAL_FRAMES):
        create_frame(i)
    
    elapsed = time.time() - start_time
    print(f"Generated {TOTAL_FRAMES} frames in {elapsed:.2f} seconds")

if __name__ == "__main__":
    main()
