#!/usr/bin/env python3
import numpy as np
from PIL import Image, ImageDraw, ImageFont

def create_editor_screenshot(width=1024, height=768, filename="editor_interface_color.png"):
    """Create a colorful editor interface screenshot with panels and a 3D scene view."""
    # Create a new RGB image with dark gray background
    img = Image.new('RGB', (width, height), color=(60, 60, 60))
    draw = ImageDraw.Draw(img)
    
    # Define colors
    panel_bg = (80, 80, 80)
    panel_header = (100, 100, 100)
    scene_bg = (40, 40, 40)
    text_color = (220, 220, 220)
    selected_color = (100, 150, 250)
    grid_color = (70, 70, 70)
    
    # Draw main panels
    # Hierarchy panel (left)
    draw.rectangle([(10, 10), (210, height-210)], fill=panel_bg, outline=(30, 30, 30), width=1)
    draw.rectangle([(10, 10), (210, 40)], fill=panel_header, outline=(30, 30, 30), width=1)
    draw.text((15, 15), "Hierarchy", fill=text_color)
    
    # Scene view panel (center)
    draw.rectangle([(220, 10), (width-220, height-210)], fill=panel_bg, outline=(30, 30, 30), width=1)
    draw.rectangle([(220, 10), (width-220, 40)], fill=panel_header, outline=(30, 30, 30), width=1)
    draw.text((225, 15), "Scene", fill=text_color)
    
    # Inspector panel (right)
    draw.rectangle([(width-210, 10), (width-10, height-210)], fill=panel_bg, outline=(30, 30, 30), width=1)
    draw.rectangle([(width-210, 10), (width-10, 40)], fill=panel_header, outline=(30, 30, 30), width=1)
    draw.text((width-205, 15), "Inspector", fill=text_color)
    
    # Project panel (bottom)
    draw.rectangle([(10, height-200), (width-10, height-10)], fill=panel_bg, outline=(30, 30, 30), width=1)
    draw.rectangle([(10, height-200), (width-10, height-170)], fill=panel_header, outline=(30, 30, 30), width=1)
    draw.text((15, height-195), "Project", fill=text_color)
    
    # Draw scene view content
    scene_rect = [(220, 40), (width-220, height-210)]
    draw.rectangle(scene_rect, fill=scene_bg)
    
    # Draw grid in scene view
    scene_width = scene_rect[1][0] - scene_rect[0][0]
    scene_height = scene_rect[1][1] - scene_rect[0][1]
    scene_center_x = scene_rect[0][0] + scene_width // 2
    scene_center_y = scene_rect[0][1] + scene_height // 2
    
    # Draw grid lines
    grid_spacing = 40
    for x in range(scene_rect[0][0], scene_rect[1][0], grid_spacing):
        draw.line([(x, scene_rect[0][1]), (x, scene_rect[1][1])], fill=grid_color)
    for y in range(scene_rect[0][1], scene_rect[1][1], grid_spacing):
        draw.line([(scene_rect[0][0], y), (scene_rect[1][0], y)], fill=grid_color)
    
    # Draw coordinate axes
    axis_length = 100
    # X axis (red)
    draw.line([(scene_center_x, scene_center_y), (scene_center_x + axis_length, scene_center_y)], fill=(255, 0, 0), width=2)
    # Y axis (green)
    draw.line([(scene_center_x, scene_center_y), (scene_center_x, scene_center_y - axis_length)], fill=(0, 255, 0), width=2)
    # Z axis (blue)
    draw.line([(scene_center_x, scene_center_y), (scene_center_x - axis_length//2, scene_center_y + axis_length//2)], fill=(0, 0, 255), width=2)
    
    # Draw a cube in the center of the scene
    cube_size = 80
    cube_half = cube_size // 2
    
    # Define cube vertices in 3D space (centered at origin)
    vertices = [
        (-1, -1, -1),  # 0: back bottom left
        (1, -1, -1),   # 1: back bottom right
        (1, 1, -1),    # 2: back top right
        (-1, 1, -1),   # 3: back top left
        (-1, -1, 1),   # 4: front bottom left
        (1, -1, 1),    # 5: front bottom right
        (1, 1, 1),     # 6: front top right
        (-1, 1, 1)     # 7: front top left
    ]
    
    # Define cube edges
    edges = [
        (0, 1), (1, 2), (2, 3), (3, 0),  # back face
        (4, 5), (5, 6), (6, 7), (7, 4),  # front face
        (0, 4), (1, 5), (2, 6), (3, 7)   # connecting edges
    ]
    
    # Simple perspective projection
    def project(x, y, z):
        # Apply perspective division
        scale = 4.0 / (z + 5)
        px = x * scale * cube_half + scene_center_x
        py = -y * scale * cube_half + scene_center_y
        return px, py
    
    # Draw cube edges
    for edge in edges:
        v1 = vertices[edge[0]]
        v2 = vertices[edge[1]]
        p1 = project(v1[0], v1[1], v1[2])
        p2 = project(v2[0], v2[1], v2[2])
        draw.line([p1, p2], fill=(200, 200, 200), width=2)
    
    # Draw cube faces (simplified)
    # Front face
    front_points = [project(vertices[i][0], vertices[i][1], vertices[i][2]) for i in [4, 5, 6, 7]]
    draw.polygon(front_points, fill=(180, 180, 180, 128), outline=(200, 200, 200))
    
    # Draw hierarchy content
    draw.text((20, 50), "Default Light", fill=text_color)
    draw.text((20, 75), "Default Cube", fill=selected_color)  # Selected item
    
    # Draw inspector content
    inspector_x = width - 200
    draw.text((inspector_x + 10, 50), "Transform", fill=text_color)
    draw.text((inspector_x + 20, 75), "Position: X: 0  Y: 0  Z: 0", fill=text_color)
    draw.text((inspector_x + 20, 100), "Rotation: X: 0  Y: 0  Z: 0", fill=text_color)
    draw.text((inspector_x + 20, 125), "Scale: X: 1  Y: 1  Z: 1", fill=text_color)
    
    draw.text((inspector_x + 10, 160), "Material", fill=text_color)
    draw.text((inspector_x + 20, 185), "Albedo: [Default]", fill=text_color)
    draw.text((inspector_x + 20, 210), "Normal Map: [None]", fill=text_color)
    draw.text((inspector_x + 20, 235), "Opacity: 1.0", fill=text_color)
    
    # Draw project content
    project_y = height - 170
    draw.text((20, project_y + 10), "Models", fill=text_color)
    draw.text((80, project_y + 10), "Textures", fill=text_color)
    draw.text((150, project_y + 10), "Scenes", fill=text_color)
    draw.text((220, project_y + 10), "Scripts", fill=text_color)
    
    # Save the image
    img.save(filename)
    print(f"Editor screenshot saved to {filename}")
    
    # Also save a version with scene view highlighted
    img_highlight = img.copy()
    draw_highlight = ImageDraw.Draw(img_highlight)
    draw_highlight.rectangle(scene_rect, outline=(255, 255, 0), width=3)
    draw_highlight.text((scene_center_x - 100, scene_center_y - 150), 
                        "Default Cube", fill=(255, 255, 0), stroke_width=1)
    img_highlight.save(filename.replace(".png", "_highlighted.png"))
    print(f"Highlighted version saved to {filename.replace('.png', '_highlighted.png')}")
    
    return True

if __name__ == "__main__":
    create_editor_screenshot()
