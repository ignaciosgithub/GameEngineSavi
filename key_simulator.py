#!/usr/bin/env python3
"""
Key Simulator for GameEngineSavi Headless Editor

This script uses pydirectinput to simulate pressing A and W keys simultaneously
to move the camera around the cube in the headless editor.
"""

import os
import time
import sys
import pydirectinput
import pygame
from pygame.locals import QUIT, KEYDOWN, K_ESCAPE

# Initialize pygame for event handling
pygame.init()
pygame.display.set_mode((400, 300))
pygame.display.set_caption("Camera Movement Simulator")

def main():
    print("Camera Movement Simulator for GameEngineSavi Headless Editor")
    print("Simulating A+W key presses to move camera around the cube")
    print("Press ESC to exit")
    
    # Wait for the headless editor to start
    print("Waiting 3 seconds for the headless editor to start...")
    time.sleep(3)
    
    # Simulate A+W key presses for camera movement
    print("Starting A+W key simulation...")
    
    running = True
    keys_pressed = False
    
    # Press A+W keys
    pydirectinput.keyDown('a')
    pydirectinput.keyDown('w')
    keys_pressed = True
    print("Pressing: A+W")
    
    # Run for 30 seconds or until ESC is pressed
    start_time = time.time()
    while running and (time.time() - start_time < 30):
        # Handle events
        for event in pygame.event.get():
            if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
                running = False
                break
        
        # Every 5 seconds, print a status message
        elapsed = time.time() - start_time
        if elapsed % 5 < 0.1:
            print(f"Still pressing A+W... ({int(elapsed)} seconds elapsed)")
        
        # Sleep to reduce CPU usage
        time.sleep(0.1)
    
    # Release keys
    if keys_pressed:
        pydirectinput.keyUp('a')
        pydirectinput.keyUp('w')
        print("Released: A+W")
    
    print("Camera movement simulation completed")
    pygame.quit()

if __name__ == "__main__":
    main()
