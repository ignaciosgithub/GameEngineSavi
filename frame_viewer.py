#!/usr/bin/env python3
"""
Frame Viewer for GameEngineSavi Emergency Renderer

This script uses pygame to display the frames rendered by the emergency renderer.
It loads each frame from the frames directory and displays them in sequence.
"""

import os
import sys
import time
import pygame
from pygame.locals import *

# Constants
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720
FPS = 10  # Frames per second

def main():
    # Initialize pygame
    pygame.init()
    
    # Create window
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("GameEngineSavi Emergency Viewer")
    
    # Create clock for controlling frame rate
    clock = pygame.time.Clock()
    
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
    auto_play = True
    
    while running:
        # Handle events
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False
                elif event.key == K_SPACE:
                    # Toggle auto-play
                    auto_play = not auto_play
                elif event.key == K_LEFT:
                    # Previous frame
                    frame_index = (frame_index - 1) % len(frame_files)
                elif event.key == K_RIGHT:
                    # Next frame
                    frame_index = (frame_index + 1) % len(frame_files)
        
        # Load and display current frame
        frame_path = os.path.join(frames_dir, frame_files[frame_index])
        try:
            frame = pygame.image.load(frame_path)
            window.blit(frame, (0, 0))
        except pygame.error as e:
            print(f"Error loading frame {frame_path}: {e}")
            # Display error message on screen
            font = pygame.font.Font(None, 36)
            text = font.render(f"Error loading frame {frame_index}", True, (255, 0, 0))
            window.fill((0, 0, 0))
            window.blit(text, (WINDOW_WIDTH // 2 - text.get_width() // 2, WINDOW_HEIGHT // 2))
        
        # Display frame number and controls
        font = pygame.font.Font(None, 24)
        text = font.render(f"Frame {frame_index + 1}/{len(frame_files)} | Space: {'Pause' if auto_play else 'Play'} | Left/Right: Navigate | Esc: Exit", True, (255, 255, 255))
        text_bg = pygame.Surface((text.get_width(), text.get_height()))
        text_bg.set_alpha(128)
        text_bg.fill((0, 0, 0))
        window.blit(text_bg, (10, 10))
        window.blit(text, (10, 10))
        
        # Update display
        pygame.display.flip()
        
        # Control frame rate
        clock.tick(FPS)
        
        # Auto-advance to next frame if auto-play is enabled
        if auto_play:
            frame_index = (frame_index + 1) % len(frame_files)
    
    # Clean up
    pygame.quit()

if __name__ == "__main__":
    main()
