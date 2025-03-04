#!/usr/bin/env python3
"""
Simple frame viewer for GameEngineSavi emergency renderer.
This script loads PNG frames from the frames directory and displays them using pygame.
"""

import os
import sys
import time
import pygame

# Initialize pygame
pygame.init()

# Set up the display
SCREEN_WIDTH = 640
SCREEN_HEIGHT = 480
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("GameEngineSavi PNG Emergency Renderer Viewer")

# Frame directory
FRAME_DIR = "frames"

def load_frame(frame_number):
    """Load a frame from the frames directory."""
    frame_path = os.path.join(FRAME_DIR, f"frame{frame_number}.png")
    try:
        # Check if file exists and has content
        if os.path.exists(frame_path) and os.path.getsize(frame_path) > 100:  # Ensure file is not too small
            print(f"Loading frame: {frame_path} ({os.path.getsize(frame_path)} bytes)")
            return pygame.image.load(frame_path)
        print(f"Frame not found or too small: {frame_path}")
        return None
    except pygame.error as e:
        print(f"Error loading frame {frame_path}: {e}")
        return None

def main():
    """Main function."""
    # Check if frames directory exists
    if not os.path.exists(FRAME_DIR):
        print(f"Error: {FRAME_DIR} directory not found.")
        return

    # Count frames
    frame_files = [f for f in os.listdir(FRAME_DIR) if f.startswith("frame") and f.endswith(".png")]
    if not frame_files:
        print(f"Error: No frames found in {FRAME_DIR} directory.")
        return

    print(f"Found {len(frame_files)} frames.")
    
    # Sort frames by number
    frame_files.sort(key=lambda f: int(f.replace("frame", "").replace(".png", "")))
    
    # Print frame sizes
    for frame_file in frame_files:
        frame_path = os.path.join(FRAME_DIR, frame_file)
        print(f"Frame {frame_file}: {os.path.getsize(frame_path)} bytes")

    # Main loop
    running = True
    frame_number = 0
    clock = pygame.time.Clock()

    while running:
        # Handle events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False

        # Load frame
        frame = load_frame(frame_number)
        if frame:
            # Display frame
            screen.blit(frame, (0, 0))
            pygame.display.flip()
            
            # Move to next frame
            frame_number = (frame_number + 1) % len(frame_files)
        else:
            # If frame not found, wait a bit and try again
            time.sleep(0.1)

        # Cap at 30 FPS
        clock.tick(30)

    pygame.quit()

if __name__ == "__main__":
    main()
