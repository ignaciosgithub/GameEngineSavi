#!/usr/bin/env python3
"""
Simple Frame Viewer for GameEngineSavi Emergency Renderer

This script uses pygame to display the frames rendered by the emergency renderer.
It loads each frame from the frames directory as quickly as possible.
"""

import os
import sys
import pygame
from pygame.locals import *

# Constants
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

def main():
    # Initialize pygame
    pygame.init()
    
    # Create window
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("GameEngineSavi Simple Emergency Viewer")
    
    # Get list of frame files
    frames_dir = "frames"
    if not os.path.exists(frames_dir):
        print(f"Error: Frames directory '{frames_dir}' not found.")
        print("Run the emergency renderer first to generate frames.")
        return
    
    frame_files = sorted([f for f in os.listdir(frames_dir) if f.startswith("frame") and f.endswith(".png")])
    
    if not frame_files:
        print(f"Error: No frame files found in '{frames_dir}'.")
        print("Run the emergency renderer first to generate frames.")
        return
    
    print(f"Found {len(frame_files)} frames.")
    
    # Main loop
    running = True
    frame_index = 0
    
    while running:
        # Handle events
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False
        
        # Load and display current frame
        frame_path = os.path.join(frames_dir, frame_files[frame_index])
        try:
            frame = pygame.image.load(frame_path)
            window.blit(frame, (0, 0))
        except pygame.error as e:
            print(f"Error loading frame {frame_path}: {e}")
            window.fill((0, 0, 0))
        
        # Display frame number
        font = pygame.font.Font(None, 24)
        text = font.render(f"Frame {frame_index + 1}/{len(frame_files)}", True, (255, 255, 255))
        text_bg = pygame.Surface((text.get_width(), text.get_height()))
        text_bg.set_alpha(128)
        text_bg.fill((0, 0, 0))
        window.blit(text_bg, (10, 10))
        window.blit(text, (10, 10))
        
        # Update display
        pygame.display.flip()
        
        # Advance to next frame as quickly as possible
        frame_index = (frame_index + 1) % len(frame_files)
    
    # Clean up
    pygame.quit()

if __name__ == "__main__":
    main()
