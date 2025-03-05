#!/usr/bin/env python3
"""
Create enhanced test frames for the EmergencyEditor with proper shading and point light
"""

import os
import sys
import math
import numpy as np
from PIL import Image, ImageDraw, ImageFont

def normalize(v):
    """Normalize a vector."""
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm

def calculate_lighting(normal, light_pos, view_pos, vertex_pos):
    """Calculate lighting using Phong reflection model."""
    # Material properties
    ambient_strength = 0.2
    diffuse_strength = 0.7
    specular_strength = 0.5
    shininess = 32.0
    
    # Ambient component
    ambient = ambient_strength
    
    # Light properties
    light_color = np.array([1.0, 1.0, 1.0])  # White light
    
    # Calculate light direction (from vertex to light)
    light_dir = normalize(light_pos - vertex_pos)
    
    # Calculate view direction (from vertex to camera)
    view_dir = normalize(view_pos - vertex_pos)
    
    # Calculate diffuse component
    diff = max(np.dot(normal, light_dir), 0.0)
    diffuse = diffuse_strength * diff
    
    # Calculate reflection direction
    reflect_dir = normalize(2.0 * np.dot(normal, light_dir) * normal - light_dir)
    
    # Calculate specular component
    spec = max(np.dot(view_dir, reflect_dir), 0.0) ** shininess
    specular = specular_strength * spec
    
    # Calculate attenuation based on distance
    distance = np.linalg.norm(light_pos - vertex_pos)
    attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance))
    
    # Calculate final lighting
    lighting = (ambient + diffuse + specular) * attenuation
    
    # Apply light color
    return lighting * light_color

def create_test_frame(frame_number, active_panel=1):
    """Create a test frame with panel layout and shaded cube."""
    # Create a new image with dark gray background
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
    
    # Draw a 3D cube with shading in the scene view panel
    cube_center = np.array([400.0, 200.0, 0.0])
    cube_size = 100.0
    rotation = frame_number * 5.0  # Rotate the cube slightly in each frame
    
    # Define cube vertices (local coordinates)
    vertices = [
        np.array([-1.0, -1.0, -1.0]),  # 0
        np.array([1.0, -1.0, -1.0]),   # 1
        np.array([1.0, 1.0, -1.0]),    # 2
        np.array([-1.0, 1.0, -1.0]),   # 3
        np.array([-1.0, -1.0, 1.0]),   # 4
        np.array([1.0, -1.0, 1.0]),    # 5
        np.array([1.0, 1.0, 1.0]),     # 6
        np.array([-1.0, 1.0, 1.0])     # 7
    ]
    
    # Define cube faces (each face is defined by 4 vertices)
    faces = [
        [0, 1, 2, 3],  # Front face
        [4, 5, 6, 7],  # Back face
        [0, 4, 7, 3],  # Left face
        [1, 5, 6, 2],  # Right face
        [0, 1, 5, 4],  # Bottom face
        [3, 2, 6, 7]   # Top face
    ]
    
    # Define face normals (in local coordinates)
    face_normals = [
        np.array([0.0, 0.0, -1.0]),  # Front face normal
        np.array([0.0, 0.0, 1.0]),   # Back face normal
        np.array([-1.0, 0.0, 0.0]),  # Left face normal
        np.array([1.0, 0.0, 0.0]),   # Right face normal
        np.array([0.0, -1.0, 0.0]),  # Bottom face normal
        np.array([0.0, 1.0, 0.0])    # Top face normal
    ]
    
    # Define face colors (base colors before lighting)
    face_colors = [
        np.array([0.8, 0.2, 0.2]),  # Front face (red)
        np.array([0.2, 0.8, 0.2]),  # Back face (green)
        np.array([0.2, 0.2, 0.8]),  # Left face (blue)
        np.array([0.8, 0.8, 0.2]),  # Right face (yellow)
        np.array([0.8, 0.2, 0.8]),  # Bottom face (magenta)
        np.array([0.2, 0.8, 0.8])   # Top face (cyan)
    ]
    
    # Apply rotation
    rotation_rad = np.radians(rotation)
    cos_r = np.cos(rotation_rad)
    sin_r = np.sin(rotation_rad)
    
    # Rotation matrix around Y axis
    rot_y = np.array([
        [cos_r, 0.0, sin_r],
        [0.0, 1.0, 0.0],
        [-sin_r, 0.0, cos_r]
    ])
    
    # Rotation matrix around X axis (slight tilt)
    tilt_angle = np.radians(20.0)
    cos_t = np.cos(tilt_angle)
    sin_t = np.sin(tilt_angle)
    rot_x = np.array([
        [1.0, 0.0, 0.0],
        [0.0, cos_t, -sin_t],
        [0.0, sin_t, cos_t]
    ])
    
    # Combined rotation matrix
    rot_matrix = np.matmul(rot_x, rot_y)
    
    # Rotate vertices and normals
    rotated_vertices = []
    for v in vertices:
        rotated_v = np.matmul(rot_matrix, v)
        # Scale and translate
        rotated_v = rotated_v * cube_size / 2.0
        rotated_vertices.append(rotated_v)
    
    rotated_normals = []
    for n in face_normals:
        rotated_n = np.matmul(rot_matrix, n)
        rotated_normals.append(normalize(rotated_n))
    
    # Define camera/view position
    view_pos = np.array([400.0, 200.0, -500.0])
    
    # Define light position (point light)
    light_pos = np.array([400.0 + 200.0 * np.sin(np.radians(frame_number * 2.0)), 
                          100.0, 
                          -300.0 + 100.0 * np.cos(np.radians(frame_number * 2.0))])
    
    # Draw point light
    light_screen_pos = (int(light_pos[0]), int(light_pos[1]))
    draw.ellipse([light_screen_pos[0] - 5, light_screen_pos[1] - 5, 
                  light_screen_pos[0] + 5, light_screen_pos[1] + 5], 
                 fill=(255, 255, 255), outline=(255, 255, 0))
    
    # Project vertices to 2D and calculate face depths
    projected_vertices = []
    for v in rotated_vertices:
        # Translate to world space
        world_v = v + np.array([0.0, 0.0, 300.0])  # Move cube forward in Z
        
        # Simple perspective projection
        scale = 600.0 / (600.0 - world_v[2])
        px = cube_center[0] + world_v[0] * scale
        py = cube_center[1] - world_v[1] * scale  # Flip y for screen coordinates
        projected_vertices.append((px, py, world_v[2]))  # Store z for depth sorting
    
    # Calculate face centers and depths for depth sorting
    face_depths = []
    for i, face in enumerate(faces):
        # Calculate face center in 3D
        center = np.zeros(3)
        for idx in face:
            center += rotated_vertices[idx]
        center /= 4.0
        
        # Translate to world space
        center += np.array([0.0, 0.0, 300.0])
        
        # Store face index and depth (z-coordinate)
        face_depths.append((i, center[2]))
    
    # Sort faces by depth (back-to-front rendering)
    face_depths.sort(key=lambda x: x[1], reverse=True)
    
    # Draw faces with proper shading
    for face_idx, _ in face_depths:
        face = faces[face_idx]
        normal = rotated_normals[face_idx]
        base_color = face_colors[face_idx]
        
        # Get face vertices in screen space
        face_vertices = [projected_vertices[idx] for idx in face]
        
        # Calculate face center in world space for lighting calculation
        face_center = np.zeros(3)
        for idx in face:
            face_center += rotated_vertices[idx]
        face_center /= 4.0
        face_center += np.array([0.0, 0.0, 300.0])  # Move to world space
        
        # Calculate lighting
        lighting = calculate_lighting(normal, light_pos, view_pos, face_center)
        
        # Apply lighting to base color
        color = base_color * lighting
        
        # Clamp color values to [0, 1]
        color = np.clip(color, 0.0, 1.0)
        
        # Convert to RGB
        rgb = tuple(int(c * 255) for c in color)
        
        # Draw the face as a polygon
        polygon_points = [(v[0], v[1]) for v in face_vertices]
        draw.polygon(polygon_points, fill=rgb, outline=(255, 255, 255))
    
    # Draw frame number
    draw.text((700, 570), f"Frame: {frame_number}", fill=(255, 255, 255))
    
    # Draw light position info
    draw.text((610, 50), "Point Light", fill=(255, 255, 0))
    
    # Draw hierarchy panel content
    draw.text((10, 40), "Scene", fill=(255, 255, 255))
    draw.text((20, 70), "└─ Cube", fill=(255, 255, 255))
    draw.text((20, 90), "└─ PointLight", fill=(255, 255, 0))
    
    # Draw inspector panel content
    if frame_number % 2 == 0:
        # Show cube properties
        draw.text((610, 100), "Cube Properties:", fill=(255, 255, 255))
        draw.text((620, 130), f"Position: (0, 0, 0)", fill=(200, 200, 200))
        draw.text((620, 150), f"Rotation: (20, {int(rotation)}, 0)", fill=(200, 200, 200))
        draw.text((620, 170), f"Scale: (1, 1, 1)", fill=(200, 200, 200))
    else:
        # Show light properties
        draw.text((610, 100), "PointLight Properties:", fill=(255, 255, 0))
        draw.text((620, 130), f"Position: ({int(light_pos[0]-400)}, {int(light_pos[1]-200)}, {int(light_pos[2])})", fill=(200, 200, 200))
        draw.text((620, 150), "Color: (255, 255, 255)", fill=(200, 200, 200))
        draw.text((620, 170), "Intensity: 1.0", fill=(200, 200, 200))
    
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
