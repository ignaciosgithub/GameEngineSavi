#!/usr/bin/env python3
"""
Frame Display for GameEngineSavi Frame Saving Editor

This script uses pygame to display the frames saved by the frame saving editor.
It only supports quitting the application, as all other controls are handled by the C++ engine.
It also handles cleaning up old frames to prevent the engine and viewer from getting out of sync.
"""

import os
import sys
import time
import pygame
from pygame.locals import QUIT, KEYDOWN, K_ESCAPE

# Handle XDG_RUNTIME_DIR environment variable
if 'XDG_RUNTIME_DIR' not in os.environ:
    temp_dir = "/tmp"
    os.environ['XDG_RUNTIME_DIR'] = temp_dir
    print(f"XDG_RUNTIME_DIR not set, using {temp_dir} as fallback")

# Constants
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600
FPS = 30
FRAMES_DIR = "frames"
MAX_FRAMES_TO_KEEP = 100  # Maximum number of frames to keep

def cleanup_old_frames(current_frame_number):
    """Delete all frames except the most recent ones to prevent out-of-sync issues."""
    try:
        frame_files = [f for f in os.listdir(FRAMES_DIR) if f.startswith("frame") and f.endswith(".png")]
        frame_files.sort(key=lambda f: int(f[5:-4]))
        
        # Keep only the most recent frames
        frames_to_delete = frame_files[:-MAX_FRAMES_TO_KEEP] if len(frame_files) > MAX_FRAMES_TO_KEEP else []
        
        for frame_file in frames_to_delete:
            try:
                os.remove(os.path.join(FRAMES_DIR, frame_file))
                print(f"Deleted old frame: {frame_file}")
            except Exception as e:
                print(f"Error deleting frame {frame_file}: {e}")
    except Exception as e:
        print(f"Error during cleanup: {e}")

def main():
    # Initialize pygame
    pygame.init()
    
    # Create window
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("GameEngineSavi Frame Display")
    
    # Create clock for controlling frame rate
    clock = pygame.time.Clock()
    
    # Check if frames directory exists
    if not os.path.exists(FRAMES_DIR):
        print(f"Error: Frames directory '{FRAMES_DIR}' not found.")
        print("Run the frame saving editor first to generate frames.")
        return
    
    # Main loop
    running = True
    last_frame_number = -1
    cleanup_counter = 0  # Counter to periodically clean up old frames
    
    while running:
        # Handle events - only quit is supported
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False
        
        # Get list of frame files
        try:
            frame_files = [f for f in os.listdir(FRAMES_DIR) if f.startswith("frame") and f.endswith(".png")]
            
            # Sort frame files by number
            frame_files.sort(key=lambda f: int(f[5:-4]))
        except Exception as e:
            print(f"Error listing frame files: {e}")
            frame_files = []
        
        if not frame_files:
            # No frames available yet, wait and try again
            window.fill((0, 0, 0))
            font = pygame.font.Font(None, 36)
            text = font.render("Waiting for frames...", True, (255, 255, 255))
            window.blit(text, (WINDOW_WIDTH // 2 - text.get_width() // 2, WINDOW_HEIGHT // 2))
            pygame.display.flip()
            clock.tick(FPS)
            time.sleep(0.1)  # Short delay to avoid busy waiting
            continue
        
        # Get the latest frame
        latest_frame = frame_files[-1]
        current_frame_number = int(latest_frame[5:-4])
        
        # Only load if it's a new frame
        if current_frame_number != last_frame_number:
            last_frame_number = current_frame_number
            
            # Periodically clean up old frames
            cleanup_counter += 1
            if cleanup_counter >= 30:  # Clean up every ~1 second (30 frames at 30 FPS)
                cleanup_old_frames(current_frame_number)
                cleanup_counter = 0
            
            # Load and display current frame
            frame_path = os.path.join(FRAMES_DIR, latest_frame)
            try:
                frame = pygame.image.load(frame_path)
                window.blit(frame, (0, 0))
            except pygame.error as e:
                print(f"Error loading frame {frame_path}: {e}")
                # Display error message on screen
                font = pygame.font.Font(None, 36)
                text = font.render(f"Error loading frame {latest_frame}", True, (255, 0, 0))
                window.fill((0, 0, 0))
                window.blit(text, (WINDOW_WIDTH // 2 - text.get_width() // 2, WINDOW_HEIGHT // 2))
            
            # Display frame number
            font = pygame.font.Font(None, 24)
            text = font.render(f"Frame: {current_frame_number} | Press ESC to exit", True, (255, 255, 255))
            text_bg = pygame.Surface((text.get_width(), text.get_height()))
            text_bg.set_alpha(128)
            text_bg.fill((0, 0, 0))
            window.blit(text_bg, (10, 10))
            window.blit(text, (10, 10))
            
            # Update display
            pygame.display.flip()
        
        # Control frame rate
        clock.tick(FPS)
    
    # Clean up
    pygame.quit()

if __name__ == "__main__":
    main()
