#!/usr/bin/env python3
"""
Headless Editor Text Parser

This module reads commands from a text file and simulates key presses with specified durations.
Format: KEY,DURATION where KEY is the key to press and DURATION is how long to press it in seconds.

Example:
    A,1   # Press 'A' key for 1 second
    W,0.5 # Press 'W' key for 0.5 seconds
"""

import os
import time
import sys
import threading
import logging

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler("headless_editor_parser.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger("HeadlessEditorTextParser")

try:
    import pydirectinput
    logger.info("Successfully imported pydirectinput")
except ImportError:
    logger.error("pydirectinput not found. Installing...")
    try:
        import subprocess
        subprocess.check_call([sys.executable, "-m", "pip", "install", "pydirectinput"])
        import pydirectinput
        logger.info("Successfully installed and imported pydirectinput")
    except Exception as e:
        logger.error(f"Failed to install pydirectinput: {e}")
        logger.warning("Falling back to keyboard simulation without pydirectinput")
        pydirectinput = None

# Constants
DEFAULT_COMMAND_FILE = "frames/commands.txt"
COMMAND_SEPARATOR = ","

class HeadlessEditorTextParser:
    """
    Parser for headless editor text commands.
    Reads commands from a text file and simulates key presses with specified durations.
    """
    
    def __init__(self, command_file=DEFAULT_COMMAND_FILE):
        """
        Initialize the parser with the command file path.
        
        Args:
            command_file (str): Path to the command file
        """
        self.command_file = command_file
        self.running = False
        self.thread = None
        logger.info(f"Initialized HeadlessEditorTextParser with command file: {command_file}")
        
        # Create the command file if it doesn't exist
        self._ensure_command_file_exists()
    
    def _ensure_command_file_exists(self):
        """Ensure the command file exists, create it if it doesn't."""
        directory = os.path.dirname(self.command_file)
        if directory and not os.path.exists(directory):
            try:
                os.makedirs(directory)
                logger.info(f"Created directory: {directory}")
            except Exception as e:
                logger.error(f"Failed to create directory {directory}: {e}")
                
        if not os.path.exists(self.command_file):
            try:
                with open(self.command_file, 'w') as f:
                    pass  # Create an empty file
                logger.info(f"Created empty command file: {self.command_file}")
            except Exception as e:
                logger.error(f"Failed to create command file {self.command_file}: {e}")
    
    def _parse_command(self, command_line):
        """
        Parse a command line into key and duration.
        
        Args:
            command_line (str): Command line in format "KEY,DURATION"
            
        Returns:
            tuple: (key, duration) or (None, None) if parsing fails
        """
        try:
            parts = command_line.strip().split(COMMAND_SEPARATOR)
            if len(parts) != 2:
                logger.warning(f"Invalid command format: {command_line}")
                return None, None
            
            key = parts[0].strip().upper()
            duration = float(parts[1].strip())
            
            if not key or duration < 0:
                logger.warning(f"Invalid key or duration: {command_line}")
                return None, None
                
            return key, duration
        except Exception as e:
            logger.error(f"Error parsing command {command_line}: {e}")
            return None, None
    
    def _execute_command(self, key, duration):
        """
        Execute a key press command with the specified duration.
        
        Args:
            key (str): Key to press
            duration (float): Duration to press the key in seconds
        """
        if not pydirectinput:
            logger.warning(f"Cannot execute command {key},{duration} without pydirectinput")
            return
            
        try:
            logger.info(f"Pressing key {key} for {duration} seconds")
            
            # Press the key down
            pydirectinput.keyDown(key)
            
            # Wait for the specified duration
            time.sleep(duration)
            
            # Release the key
            pydirectinput.keyUp(key)
            
            logger.info(f"Released key {key} after {duration} seconds")
        except Exception as e:
            logger.error(f"Error executing command for key {key}: {e}")
            # Make sure to release the key in case of an error
            try:
                pydirectinput.keyUp(key)
            except:
                pass
    
    def _clear_command_file(self):
        """Clear the command file after processing commands."""
        try:
            with open(self.command_file, 'w') as f:
                pass  # Truncate the file
            logger.info(f"Cleared command file: {self.command_file}")
        except Exception as e:
            logger.error(f"Failed to clear command file {self.command_file}: {e}")
    
    def _process_commands(self):
        """Process commands from the command file."""
        try:
            # Check if the file exists
            if not os.path.exists(self.command_file):
                logger.warning(f"Command file does not exist: {self.command_file}")
                return
                
            # Read commands from the file
            with open(self.command_file, 'r') as f:
                commands = f.readlines()
            
            if not commands:
                return  # No commands to process
                
            logger.info(f"Processing {len(commands)} commands")
            
            # Process each command
            for command in commands:
                if not self.running:
                    break  # Stop processing if the parser has been stopped
                    
                command = command.strip()
                if not command or command.startswith('#'):
                    continue  # Skip empty lines and comments
                
                key, duration = self._parse_command(command)
                if key and duration is not None:
                    self._execute_command(key, duration)
            
            # Clear the command file after processing
            self._clear_command_file()
            
        except Exception as e:
            logger.error(f"Error processing commands: {e}")
    
    def _monitor_file(self):
        """Monitor the command file for changes and process commands."""
        logger.info("Starting command file monitoring")
        
        last_modified = 0
        
        while self.running:
            try:
                # Check if the file exists
                if not os.path.exists(self.command_file):
                    time.sleep(0.5)
                    continue
                
                # Check if the file has been modified
                current_modified = os.path.getmtime(self.command_file)
                
                if current_modified > last_modified:
                    # File has been modified, process commands
                    logger.info(f"Command file modified at {current_modified}")
                    last_modified = current_modified
                    
                    # Wait a short time to ensure the file is fully written
                    time.sleep(0.1)
                    
                    # Process commands
                    self._process_commands()
                
                # Sleep to avoid high CPU usage
                time.sleep(0.5)
                
            except Exception as e:
                logger.error(f"Error monitoring command file: {e}")
                time.sleep(1)  # Sleep longer on error
    
    def start(self):
        """Start monitoring the command file."""
        if self.running:
            logger.warning("Parser is already running")
            return
            
        self.running = True
        self.thread = threading.Thread(target=self._monitor_file)
        self.thread.daemon = True
        self.thread.start()
        
        logger.info("Parser started")
    
    def stop(self):
        """Stop monitoring the command file."""
        if not self.running:
            logger.warning("Parser is not running")
            return
            
        self.running = False
        if self.thread:
            self.thread.join(timeout=2)
            self.thread = None
            
        logger.info("Parser stopped")

def main():
    """Main function to run the parser."""
    # Parse command line arguments
    import argparse
    parser = argparse.ArgumentParser(description='Headless Editor Text Parser')
    parser.add_argument('--command-file', default=DEFAULT_COMMAND_FILE,
                        help=f'Path to the command file (default: {DEFAULT_COMMAND_FILE})')
    args = parser.parse_args()
    
    # Create and start the parser
    text_parser = HeadlessEditorTextParser(command_file=args.command_file)
    text_parser.start()
    
    try:
        # Keep the main thread alive
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        logger.info("Keyboard interrupt received, stopping parser")
    finally:
        text_parser.stop()

if __name__ == "__main__":
    main()
