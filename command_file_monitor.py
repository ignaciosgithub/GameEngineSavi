#!/usr/bin/env python3
"""
Command File Monitor for Headless Editor

This module monitors a command file for changes and executes commands when detected.
It works with the headless_editor_text_parser.py module to provide a complete
text-based control system for the headless editor.
"""

import os
import time
import sys
import threading
import logging
import argparse
from pathlib import Path

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler("command_monitor.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger("CommandFileMonitor")

# Import the text parser
try:
    from headless_editor_text_parser import HeadlessEditorTextParser
    logger.info("Successfully imported HeadlessEditorTextParser")
except ImportError as e:
    logger.error(f"Failed to import HeadlessEditorTextParser: {e}")
    sys.exit(1)

# Constants
DEFAULT_COMMAND_FILE = "frames/commands.txt"
DEFAULT_CHECK_INTERVAL = 0.5  # seconds

class CommandFileMonitor:
    """
    Monitors a command file for changes and executes commands when detected.
    """
    
    def __init__(self, command_file=DEFAULT_COMMAND_FILE, check_interval=DEFAULT_CHECK_INTERVAL):
        """
        Initialize the command file monitor.
        
        Args:
            command_file (str): Path to the command file to monitor
            check_interval (float): Interval in seconds to check for file changes
        """
        self.command_file = command_file
        self.check_interval = check_interval
        self.running = False
        self.thread = None
        self.last_modified = 0
        self.parser = HeadlessEditorTextParser(command_file=command_file)
        
        logger.info(f"Initialized CommandFileMonitor with command file: {command_file}")
        logger.info(f"Check interval: {check_interval} seconds")
        
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
    
    def _monitor_file(self):
        """Monitor the command file for changes and process commands when detected."""
        logger.info(f"Starting to monitor command file: {self.command_file}")
        
        while self.running:
            try:
                # Check if the file exists
                if not os.path.exists(self.command_file):
                    time.sleep(self.check_interval)
                    continue
                
                # Check if the file has been modified
                current_modified = os.path.getmtime(self.command_file)
                file_size = os.path.getsize(self.command_file)
                
                if current_modified > self.last_modified and file_size > 0:
                    # File has been modified and has content
                    logger.info(f"Command file modified at {current_modified}")
                    self.last_modified = current_modified
                    
                    # Process the commands in the file
                    self._process_commands()
                
                # Sleep to avoid high CPU usage
                time.sleep(self.check_interval)
                
            except Exception as e:
                logger.error(f"Error monitoring command file: {e}")
                time.sleep(self.check_interval * 2)  # Sleep longer on error
    
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
                logger.info("Command file is empty, nothing to process")
                return
                
            logger.info(f"Processing {len(commands)} commands")
            
            # Process each command
            for command in commands:
                command = command.strip()
                if not command or command.startswith('#'):
                    continue  # Skip empty lines and comments
                
                logger.info(f"Processing command: {command}")
                
                # Parse the command
                parts = command.split(',')
                if len(parts) != 2:
                    logger.warning(f"Invalid command format: {command}")
                    continue
                
                key = parts[0].strip().upper()
                try:
                    duration = float(parts[1].strip())
                except ValueError:
                    logger.warning(f"Invalid duration in command: {command}")
                    continue
                
                if not key or duration < 0:
                    logger.warning(f"Invalid key or duration: {command}")
                    continue
                
                # Execute the command using the parser
                self._execute_command(key, duration)
            
            # Clear the command file after processing
            self._clear_command_file()
            
        except Exception as e:
            logger.error(f"Error processing commands: {e}")
    
    def _execute_command(self, key, duration):
        """
        Execute a key press command with the specified duration.
        
        Args:
            key (str): Key to press
            duration (float): Duration to press the key in seconds
        """
        try:
            # Import pydirectinput here to handle import errors gracefully
            import pydirectinput
            
            logger.info(f"Pressing key {key} for {duration} seconds")
            
            # Press the key down
            pydirectinput.keyDown(key)
            
            # Wait for the specified duration
            time.sleep(duration)
            
            # Release the key
            pydirectinput.keyUp(key)
            
            logger.info(f"Released key {key} after {duration} seconds")
        except ImportError:
            logger.error("pydirectinput not available, cannot execute command")
        except Exception as e:
            logger.error(f"Error executing command for key {key}: {e}")
            # Make sure to release the key in case of an error
            try:
                import pydirectinput
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
    
    def start(self):
        """Start monitoring the command file."""
        if self.running:
            logger.warning("Monitor is already running")
            return
            
        self.running = True
        self.thread = threading.Thread(target=self._monitor_file)
        self.thread.daemon = True
        self.thread.start()
        
        logger.info("Command file monitor started")
    
    def stop(self):
        """Stop monitoring the command file."""
        if not self.running:
            logger.warning("Monitor is not running")
            return
            
        self.running = False
        if self.thread:
            self.thread.join(timeout=2)
            self.thread = None
            
        logger.info("Command file monitor stopped")

def main():
    """Main function to run the command file monitor."""
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Command File Monitor for Headless Editor')
    parser.add_argument('--command-file', default=DEFAULT_COMMAND_FILE,
                        help=f'Path to the command file to monitor (default: {DEFAULT_COMMAND_FILE})')
    parser.add_argument('--check-interval', type=float, default=DEFAULT_CHECK_INTERVAL,
                        help=f'Interval in seconds to check for file changes (default: {DEFAULT_CHECK_INTERVAL})')
    args = parser.parse_args()
    
    # Create and start the monitor
    monitor = CommandFileMonitor(command_file=args.command_file, check_interval=args.check_interval)
    monitor.start()
    
    try:
        # Keep the main thread alive
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        logger.info("Keyboard interrupt received, stopping monitor")
    finally:
        monitor.stop()

if __name__ == "__main__":
    main()
