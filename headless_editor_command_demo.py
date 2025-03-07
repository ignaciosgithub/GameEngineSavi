#!/usr/bin/env python3
"""
Headless Editor Command Demo

This script demonstrates how to use the text parser with the headless editor.
It writes commands to the command file and monitors the frames directory for output.
"""

import os
import time
import sys
import argparse
import subprocess
import threading
import logging
from pathlib import Path

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler("headless_editor_command_demo.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger("HeadlessEditorCommandDemo")

# Constants
DEFAULT_COMMAND_FILE = "frames/commands.txt"
DEFAULT_FRAMES_DIR = "frames"
DEFAULT_RUNNING_FILE = "frames/running.txt"

class HeadlessEditorCommandDemo:
    """
    Demonstrates how to use the text parser with the headless editor.
    """
    
    def __init__(self, command_file=DEFAULT_COMMAND_FILE, frames_dir=DEFAULT_FRAMES_DIR, running_file=DEFAULT_RUNNING_FILE):
        """
        Initialize the demo.
        
        Args:
            command_file (str): Path to the command file
            frames_dir (str): Path to the frames directory
            running_file (str): Path to the running file
        """
        self.command_file = command_file
        self.frames_dir = frames_dir
        self.running_file = running_file
        self.editor_process = None
        
        logger.info(f"Initialized HeadlessEditorCommandDemo with command file: {command_file}")
        logger.info(f"Frames directory: {frames_dir}")
        logger.info(f"Running file: {running_file}")
        
        # Create the frames directory if it doesn't exist
        self._ensure_frames_dir_exists()
    
    def _ensure_frames_dir_exists(self):
        """Ensure the frames directory exists, create it if it doesn't."""
        if not os.path.exists(self.frames_dir):
            try:
                os.makedirs(self.frames_dir)
                logger.info(f"Created frames directory: {self.frames_dir}")
            except Exception as e:
                logger.error(f"Failed to create frames directory {self.frames_dir}: {e}")
    
    def _ensure_running_file_exists(self):
        """Ensure the running file exists, create it if it doesn't."""
        if not os.path.exists(self.running_file):
            try:
                with open(self.running_file, 'w') as f:
                    f.write("running\n")
                logger.info(f"Created running file: {self.running_file}")
            except Exception as e:
                logger.error(f"Failed to create running file {self.running_file}: {e}")
    
    def _ensure_command_file_exists(self):
        """Ensure the command file exists, create it if it doesn't."""
        if not os.path.exists(self.command_file):
            try:
                with open(self.command_file, 'w') as f:
                    pass  # Create an empty file
                logger.info(f"Created empty command file: {self.command_file}")
            except Exception as e:
                logger.error(f"Failed to create command file {self.command_file}: {e}")
    
    def start_editor(self):
        """Start the headless editor."""
        # Check if we're on Windows or Linux
        if os.name == 'nt':
            # Windows
            editor_script = "bin\\text_controlled_editor.exe"
        else:
            # Linux
            editor_script = "bin/text_controlled_editor"
        
        # Check if the editor executable exists
        if not os.path.exists(editor_script):
            logger.error(f"Editor executable not found: {editor_script}")
            logger.info("Please build the editor first using build_text_controlled_editor.sh or build_text_controlled_editor.bat")
            return False
        
        # Create the running file
        self._ensure_running_file_exists()
        
        # Start the editor process
        try:
            logger.info(f"Starting editor: {editor_script}")
            self.editor_process = subprocess.Popen(
                [editor_script],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            logger.info(f"Editor started with PID: {self.editor_process.pid}")
            
            # Start a thread to monitor the editor process output
            threading.Thread(target=self._monitor_editor_output, daemon=True).start()
            
            return True
        except Exception as e:
            logger.error(f"Failed to start editor: {e}")
            return False
    
    def _monitor_editor_output(self):
        """Monitor the editor process output."""
        if not self.editor_process:
            logger.error("No editor process to monitor")
            return
            
        # Read and log stdout
        for line in self.editor_process.stdout:
            logger.info(f"Editor: {line.strip()}")
        
        # Read and log stderr
        for line in self.editor_process.stderr:
            logger.error(f"Editor error: {line.strip()}")
    
    def stop_editor(self):
        """Stop the headless editor."""
        if not self.editor_process:
            logger.warning("No editor process to stop")
            return
            
        # Remove the running file to signal the editor to stop
        try:
            if os.path.exists(self.running_file):
                os.remove(self.running_file)
                logger.info(f"Removed running file: {self.running_file}")
        except Exception as e:
            logger.error(f"Failed to remove running file {self.running_file}: {e}")
        
        # Wait for the editor process to exit
        try:
            logger.info("Waiting for editor process to exit...")
            self.editor_process.wait(timeout=5)
            logger.info("Editor process exited")
        except subprocess.TimeoutExpired:
            logger.warning("Editor process did not exit within timeout, terminating...")
            self.editor_process.terminate()
            try:
                self.editor_process.wait(timeout=2)
                logger.info("Editor process terminated")
            except subprocess.TimeoutExpired:
                logger.error("Editor process did not terminate, killing...")
                self.editor_process.kill()
                logger.info("Editor process killed")
        
        self.editor_process = None
    
    def write_command(self, key, duration):
        """
        Write a command to the command file.
        
        Args:
            key (str): Key to press
            duration (float): Duration to press the key in seconds
        """
        # Ensure the command file exists
        self._ensure_command_file_exists()
        
        # Write the command to the file
        try:
            with open(self.command_file, 'w') as f:
                f.write(f"{key},{duration}\n")
            logger.info(f"Wrote command to {self.command_file}: {key},{duration}")
            return True
        except Exception as e:
            logger.error(f"Failed to write command to {self.command_file}: {e}")
            return False
    
    def run_demo(self):
        """Run the demo."""
        # Start the editor
        if not self.start_editor():
            logger.error("Failed to start editor, aborting demo")
            return
        
        try:
            # Wait for the editor to initialize
            logger.info("Waiting for editor to initialize...")
            time.sleep(3)
            
            # Write some commands to demonstrate movement
            commands = [
                ('W', 1.0),  # Move forward for 1 second
                ('A', 0.5),  # Move left for 0.5 seconds
                ('S', 1.0),  # Move backward for 1 second
                ('D', 0.5),  # Move right for 0.5 seconds
                ('W', 0.3),  # Move forward for 0.3 seconds
                ('A', 0.3),  # Move left for 0.3 seconds
                ('S', 0.3),  # Move backward for 0.3 seconds
                ('D', 0.3),  # Move right for 0.3 seconds
            ]
            
            for key, duration in commands:
                # Write the command
                self.write_command(key, duration)
                
                # Wait for the command to be processed
                # Wait a bit longer than the duration to ensure the command is fully processed
                wait_time = duration + 0.5
                logger.info(f"Waiting {wait_time} seconds for command to be processed...")
                time.sleep(wait_time)
            
            logger.info("Demo completed successfully")
        finally:
            # Stop the editor
            self.stop_editor()

def main():
    """Main function to run the demo."""
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Headless Editor Command Demo')
    parser.add_argument('--command-file', default=DEFAULT_COMMAND_FILE,
                        help=f'Path to the command file (default: {DEFAULT_COMMAND_FILE})')
    parser.add_argument('--frames-dir', default=DEFAULT_FRAMES_DIR,
                        help=f'Path to the frames directory (default: {DEFAULT_FRAMES_DIR})')
    parser.add_argument('--running-file', default=DEFAULT_RUNNING_FILE,
                        help=f'Path to the running file (default: {DEFAULT_RUNNING_FILE})')
    args = parser.parse_args()
    
    # Create and run the demo
    demo = HeadlessEditorCommandDemo(
        command_file=args.command_file,
        frames_dir=args.frames_dir,
        running_file=args.running_file
    )
    demo.run_demo()

if __name__ == "__main__":
    main()
