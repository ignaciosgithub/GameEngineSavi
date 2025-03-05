#!/usr/bin/env python3
"""
Create enhanced test frames with PBR shading and point light using pygame.
"""
import os
import sys
import math
import numpy as np
import pygame
from pygame.locals import *

# Initialize pygame
pygame.init()

# Constants
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600
FPS = 30
FRAME_COUNT = 10

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
DARK_GRAY = (50, 50, 50)
LIGHT_GRAY = (100, 100, 100)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
CYAN = (0, 255, 255)
MAGENTA = (255, 0, 255)

def normalize(v):
    """Normalize a vector."""
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm

def rotate_point(point, angle_x, angle_y, angle_z):
    """Rotate a point around the origin."""
    # Convert angles to radians
    angle_x = math.radians(angle_x)
    angle_y = math.radians(angle_y)
    angle_z = math.radians(angle_z)
    
    # Rotation around X axis
    cos_x, sin_x = math.cos(angle_x), math.sin(angle_x)
    y = point[1] * cos_x - point[2] * sin_x
    z = point[1] * sin_x + point[2] * cos_x
    point = np.array([point[0], y, z])
    
    # Rotation around Y axis
    cos_y, sin_y = math.cos(angle_y), math.sin(angle_y)
    x = point[0] * cos_y + point[2] * sin_y
    z = -point[0] * sin_y + point[2] * cos_y
    point = np.array([x, point[1], z])
    
    # Rotation around Z axis
    cos_z, sin_z = math.cos(angle_z), math.sin(angle_z)
    x = point[0] * cos_z - point[1] * sin_z
    y = point[0] * sin_z + point[1] * cos_z
    point = np.array([x, y, point[2]])
    
    return point

def project_point(point, fov, aspect, near, far, screen_width, screen_height):
    """Project a 3D point to 2D screen coordinates."""
    # Perspective projection
    fov_rad = math.radians(fov)
    f = 1.0 / math.tan(fov_rad / 2.0)
    
    # Projection matrix components
    a = f / aspect
    b = f
    c = (far + near) / (near - far)
    d = (2 * far * near) / (near - far)
    
    # Apply projection
    x, y, z = point
    
    # Prevent division by zero
    if z == 0:
        z = 0.0001
    
    # Apply perspective division
    w = -z
    x_clip = x * a / w
    y_clip = y * b / w
    
    # Convert to screen coordinates
    x_screen = (x_clip + 1.0) * 0.5 * screen_width
    y_screen = (1.0 - (y_clip + 1.0) * 0.5) * screen_height
    
    return np.array([x_screen, y_screen, -z])  # Return z for depth sorting

def calculate_pbr_lighting(normal, light_pos, view_pos, vertex_pos, albedo, metallic, roughness):
    """Calculate lighting using PBR (Physically Based Rendering)."""
    # Constants
    PI = 3.14159265359
    
    # Calculate vectors
    N = normalize(normal)
    L = normalize(light_pos - vertex_pos)
    V = normalize(view_pos - vertex_pos)
    H = normalize(L + V)
    
    # Calculate dot products
    NdotL = max(np.dot(N, L), 0.0)
    NdotV = max(np.dot(N, V), 0.0)
    NdotH = max(np.dot(N, H), 0.0)
    LdotH = max(np.dot(L, H), 0.0)
    
    # Calculate light attenuation
    distance = np.linalg.norm(light_pos - vertex_pos)
    attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance))
    
    # Calculate F0 (surface reflection at zero incidence)
    F0 = np.array([0.04, 0.04, 0.04])
    F0 = F0 * (1.0 - metallic) + albedo * metallic
    
    # Calculate Fresnel term (Schlick approximation)
    F = F0 + (1.0 - F0) * np.power(1.0 - LdotH, 5.0)
    
    # Calculate normal distribution function (GGX/Trowbridge-Reitz)
    alpha = roughness * roughness
    alpha2 = alpha * alpha
    denom = NdotH * NdotH * (alpha2 - 1.0) + 1.0
    D = alpha2 / (PI * denom * denom)
    
    # Calculate geometric attenuation (Smith's method with GGX)
    k = (roughness + 1.0) * (roughness + 1.0) / 8.0
    G1_L = NdotL / (NdotL * (1.0 - k) + k)
    G1_V = NdotV / (NdotV * (1.0 - k) + k)
    G = G1_L * G1_V
    
    # Calculate specular BRDF
    specular = (F * D * G) / max(4.0 * NdotL * NdotV, 0.001)
    
    # Calculate diffuse BRDF (Lambert)
    kD = (1.0 - F) * (1.0 - metallic)
    diffuse = kD * albedo / PI
    
    # Combine diffuse and specular
    BRDF = diffuse + specular
    
    # Apply light color, intensity, and attenuation
    light_color = np.array([1.0, 1.0, 1.0])  # White light
    light_intensity = 1.0
    radiance = light_color * light_intensity * attenuation
    
    # Calculate final lighting
    Lo = BRDF * radiance * NdotL
    
    # Add ambient lighting
    ambient = 0.03 * albedo
    
    # Final color with tone mapping and gamma correction
    color = ambient + Lo
    color = 1.0 - np.exp(-color * 1.0)  # Tone mapping
    color = np.power(color, 1.0 / 2.2)  # Gamma correction
    
    return color

def create_cube_vertices():
    """Create vertices for a cube."""
    # Cube vertices (8 corners)
    vertices = [
        np.array([-0.5, -0.5, -0.5]),  # 0: bottom-left-back
        np.array([0.5, -0.5, -0.5]),   # 1: bottom-right-back
        np.array([0.5, 0.5, -0.5]),    # 2: top-right-back
        np.array([-0.5, 0.5, -0.5]),   # 3: top-left-back
        np.array([-0.5, -0.5, 0.5]),   # 4: bottom-left-front
        np.array([0.5, -0.5, 0.5]),    # 5: bottom-right-front
        np.array([0.5, 0.5, 0.5]),     # 6: top-right-front
        np.array([-0.5, 0.5, 0.5])     # 7: top-left-front
    ]
    
    # Cube faces (6 faces, each with 2 triangles)
    faces = [
        # Front face
        [4, 5, 6], [4, 6, 7],
        # Back face
        [1, 0, 3], [1, 3, 2],
        # Left face
        [0, 4, 7], [0, 7, 3],
        # Right face
        [5, 1, 2], [5, 2, 6],
        # Top face
        [7, 6, 2], [7, 2, 3],
        # Bottom face
        [0, 1, 5], [0, 5, 4]
    ]
    
    # Cube face normals
    normals = [
        np.array([0.0, 0.0, 1.0]),   # Front face
        np.array([0.0, 0.0, -1.0]),  # Back face
        np.array([-1.0, 0.0, 0.0]),  # Left face
        np.array([1.0, 0.0, 0.0]),   # Right face
        np.array([0.0, 1.0, 0.0]),   # Top face
        np.array([0.0, -1.0, 0.0])   # Bottom face
    ]
    
    return vertices, faces, normals

def draw_cube(surface, vertices, faces, normals, light_pos, view_pos, rotation_angles, albedo, metallic, roughness):
    """Draw a cube with PBR shading."""
    # Rotate vertices
    rotated_vertices = []
    for vertex in vertices:
        rotated_vertex = rotate_point(vertex, rotation_angles[0], rotation_angles[1], rotation_angles[2])
        rotated_vertices.append(rotated_vertex)
    
    # Project vertices to screen coordinates
    projected_vertices = []
    for vertex in rotated_vertices:
        # Move the vertex away from the camera
        vertex_with_depth = vertex + np.array([0, 0, 3])
        projected_vertex = project_point(vertex_with_depth, 60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0, WINDOW_WIDTH, WINDOW_HEIGHT)
        projected_vertices.append(projected_vertex)
    
    # Sort faces by depth (painter's algorithm)
    face_depths = []
    for i, face in enumerate(faces):
        # Calculate face center
        face_center = np.zeros(3)
        for vertex_idx in face:
            face_center += rotated_vertices[vertex_idx]
        face_center /= 3
        
        # Calculate face depth
        depth = face_center[2] + 3  # Add camera distance
        
        # Store face index and depth
        face_depths.append((i, depth))
    
    # Sort faces by depth (back to front)
    face_depths.sort(key=lambda x: x[1], reverse=True)
    
    # Draw faces
    for face_idx, depth in face_depths:
        face = faces[face_idx]
        
        # Get face normal
        normal_idx = face_idx // 2
        normal = normals[normal_idx]
        
        # Rotate normal
        rotated_normal = rotate_point(normal, rotation_angles[0], rotation_angles[1], rotation_angles[2])
        
        # Calculate face center for lighting
        face_center = np.zeros(3)
        for vertex_idx in face:
            face_center += rotated_vertices[vertex_idx]
        face_center /= 3
        
        # Calculate lighting
        color = calculate_pbr_lighting(rotated_normal, light_pos, view_pos, face_center, albedo, metallic, roughness)
        
        # Convert color to pygame color
        pygame_color = (int(color[0] * 255), int(color[1] * 255), int(color[2] * 255))
        
        # Draw face as a filled triangle
        points = [
            (int(projected_vertices[face[0]][0]), int(projected_vertices[face[0]][1])),
            (int(projected_vertices[face[1]][0]), int(projected_vertices[face[1]][1])),
            (int(projected_vertices[face[2]][0]), int(projected_vertices[face[2]][1]))
        ]
        pygame.draw.polygon(surface, pygame_color, points)
        
        # Draw face outline
        pygame.draw.polygon(surface, (0, 0, 0), points, 1)

def draw_point_light(surface, light_pos, view_pos, color=(255, 255, 255), radius=5):
    """Draw a point light as a small sphere."""
    # Move the light away from the camera
    light_with_depth = light_pos + np.array([0, 0, 3])
    
    # Project light position to screen coordinates
    projected_light = project_point(light_with_depth, 60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0, WINDOW_WIDTH, WINDOW_HEIGHT)
    
    # Draw light
    pygame.draw.circle(surface, color, (int(projected_light[0]), int(projected_light[1])), radius)
    
    # Draw light rays
    for i in range(8):
        angle = i * math.pi / 4
        end_x = projected_light[0] + math.cos(angle) * radius * 2
        end_y = projected_light[1] + math.sin(angle) * radius * 2
        pygame.draw.line(surface, color, (int(projected_light[0]), int(projected_light[1])), (int(end_x), int(end_y)), 1)

def draw_editor_panels(surface, active_panel=1):
    """Draw editor panels."""
    # Draw panel borders
    pygame.draw.rect(surface, WHITE, (0, 0, 200, WINDOW_HEIGHT), 2)  # Hierarchy panel
    pygame.draw.rect(surface, WHITE, (WINDOW_WIDTH - 300, 0, 300, WINDOW_HEIGHT), 2)  # Inspector panel
    pygame.draw.rect(surface, WHITE, (200, WINDOW_HEIGHT - 200, WINDOW_WIDTH - 500, 200), 2)  # Project panel
    
    # Draw panel titles
    font = pygame.font.Font(None, 24)
    
    # Hierarchy panel
    text = font.render("Hierarchy Panel", True, WHITE)
    surface.blit(text, (10, 10))
    
    # Inspector panel
    text = font.render("Inspector Panel", True, WHITE)
    surface.blit(text, (WINDOW_WIDTH - 290, 10))
    
    # Project panel
    text = font.render("Project Panel", True, WHITE)
    surface.blit(text, (210, WINDOW_HEIGHT - 190))
    
    # Scene view panel
    text = font.render("Scene View Panel", True, WHITE)
    surface.blit(text, (210, 10))
    
    # Highlight active panel
    if active_panel == 0:  # Hierarchy panel
        pygame.draw.rect(surface, GREEN, (0, 0, 200, 30), 2)
    elif active_panel == 1:  # Scene view panel
        pygame.draw.rect(surface, GREEN, (200, 0, WINDOW_WIDTH - 500, 30), 2)
    elif active_panel == 2:  # Inspector panel
        pygame.draw.rect(surface, GREEN, (WINDOW_WIDTH - 300, 0, 300, 30), 2)
    elif active_panel == 3:  # Project panel
        pygame.draw.rect(surface, GREEN, (200, WINDOW_HEIGHT - 200, WINDOW_WIDTH - 500, 30), 2)

def create_frame(frame_number, frames_dir="frames"):
    """Create a frame with PBR shading and point light."""
    # Create frames directory if it doesn't exist
    os.makedirs(frames_dir, exist_ok=True)
    
    # Create surface
    surface = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT))
    
    # Fill background
    surface.fill(DARK_GRAY)
    
    # Calculate rotation angles based on frame number
    rotation_x = 20 + frame_number * 2
    rotation_y = 30 + frame_number * 3
    rotation_z = 0
    
    # Create cube
    vertices, faces, normals = create_cube_vertices()
    
    # Set material properties
    albedo = np.array([0.7, 0.7, 0.9])  # Slightly blue
    metallic = 0.1
    roughness = 0.3
    
    # Set light position
    light_pos = np.array([1.5, 1.0, 1.0])
    
    # Set view position (camera)
    view_pos = np.array([0.0, 0.0, 5.0])
    
    # Draw cube
    draw_cube(surface, vertices, faces, normals, light_pos, view_pos, (rotation_x, rotation_y, rotation_z), albedo, metallic, roughness)
    
    # Draw point light
    draw_point_light(surface, light_pos, view_pos, YELLOW)
    
    # Draw editor panels
    active_panel = 1  # Scene view panel is active by default
    draw_editor_panels(surface, active_panel)
    
    # Draw frame number
    font = pygame.font.Font(None, 24)
    text = font.render(f"Frame: {frame_number}", True, WHITE)
    surface.blit(text, (WINDOW_WIDTH - 150, WINDOW_HEIGHT - 30))
    
    # Save frame
    pygame.image.save(surface, os.path.join(frames_dir, f"frame{frame_number}.png"))
    
    # Save panel info
    panel_names = ["Hierarchy Panel", "Scene View Panel", "Inspector Panel", "Project Panel"]
    with open(os.path.join(frames_dir, "panel_info.txt"), "w") as f:
        f.write(f"Active Panel: {active_panel}\n")
        f.write(panel_names[active_panel])
    
    # Save hotkey state
    with open(os.path.join(frames_dir, "hotkey_state.txt"), "w") as f:
        f.write("W: Up\n")
        f.write("A: Left\n")
        f.write("S: Down\n")
        f.write("D: Right\n")
        f.write("Shift: Fast Movement\n")
        f.write("Space: Play/Pause\n")
        f.write("Ctrl+1: Hierarchy Panel\n")
        f.write("Ctrl+2: Scene View Panel\n")
        f.write("Ctrl+3: Inspector Panel\n")
        f.write("Ctrl+4: Project Panel\n")
    
    return surface

def main():
    """Main function to generate frames with PBR shading and point light."""
    print("Generating frames with PBR shading and point light...")
    
    # Create frames directory if it doesn't exist
    frames_dir = "frames"
    os.makedirs(frames_dir, exist_ok=True)
    
    # Generate frames
    for frame_number in range(FRAME_COUNT):
        print(f"Generating frame {frame_number}...")
        create_frame(frame_number, frames_dir)
    
    print(f"Generated {FRAME_COUNT} frames with PBR shading and point light.")
    print(f"Frames are saved in the '{frames_dir}' directory.")
    print("You can view the frames using the frame_reader.py script.")

if __name__ == "__main__":
    main()
