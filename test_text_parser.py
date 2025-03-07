#!/usr/bin/env python3
"""
Test Script for Headless Editor Text Parser

This script tests the functionality of the headless editor text parser by:
1. Creating a command file with sample key commands
2. Verifying that the commands are properly processed
3. Verifying that the file is cleared after command execution
"""

import os
import time
import sys
import argparse
import logging
import threading
from pathlib import Path

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler("test_text_parser.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger("TestTextParser")

# Import the text parser
try:
    from headless_editor_text_parser import HeadlessEditorTextParser
    logger.info("Successfully imported HeadlessEditorTextParser")
except ImportError as e:
    logger.error(f"Failed to import HeadlessEditorTextParser: {e}")
    sys.exit(1)

# Constants
DEFAULT_COMMAND_FILE = "frames/commands.txt"
TEST_KEYS = ["W", "A", "S", "D", "Q", "E", "R", "F", "SPACE", "SHIFT"]
TEST_DURATIONS = [0.1, 0.2, 0.5, 1.0]

class TextParserTester:
    """
    Tests the functionality of the headless editor text parser.
    """
    
    def __init__(self, command_file=DEFAULT_COMMAND_FILE):
        """
        Initialize the tester.
        
        Args:
            command_file (str): Path to the command file
        """
        self.command_file = command_file
        self.parser = HeadlessEditorTextParser(command_file=command_file)
        
        # Create the frames directory if it doesn't exist
        directory = os.path.dirname(self.command_file)
        if directory and not os.path.exists(directory):
            try:
                os.makedirs(directory)
                logger.info(f"Created directory: {directory}")
            except Exception as e:
                logger.error(f"Failed to create directory {directory}: {e}")
        
        logger.info(f"Initialized TextParserTester with command file: {command_file}")
    
    def test_command_file_creation(self):
        """Test that the command file is created if it doesn't exist."""
        # Delete the command file if it exists
        if os.path.exists(self.command_file):
            try:
                os.remove(self.command_file)
                logger.info(f"Deleted command file: {self.command_file}")
            except Exception as e:
                logger.error(f"Failed to delete command file {self.command_file}: {e}")
                return False
        
        # Initialize the parser, which should create the command file
        self.parser = HeadlessEditorTextParser(command_file=self.command_file)
        
        # Check if the command file was created
        if os.path.exists(self.command_file):
            logger.info(f"Command file was created: {self.command_file}")
            return True
        else:
            logger.error(f"Command file was not created: {self.command_file}")
            return False
    
    def test_command_writing(self, key, duration):
        """
        Test writing a command to the command file.
        
        Args:
            key (str): Key to press
            duration (float): Duration to press the key in seconds
            
        Returns:
            bool: True if the command was written successfully, False otherwise
        """
        # Write the command to the file
        try:
            with open(self.command_file, 'w') as f:
                f.write(f"{key},{duration}\n")
            logger.info(f"Wrote command to {self.command_file}: {key},{duration}")
            
            # Check if the command was written correctly
            with open(self.command_file, 'r') as f:
                content = f.read().strip()
            
            if content == f"{key},{duration}":
                logger.info(f"Command was written correctly: {content}")
                return True
            else:
                logger.error(f"Command was not written correctly. Expected: {key},{duration}, Got: {content}")
                return False
        except Exception as e:
            logger.error(f"Failed to write command to {self.command_file}: {e}")
            return False
    
    def test_command_parsing(self, key, duration):
        """
        Test parsing a command from the command file.
        
        Args:
            key (str): Key to press
            duration (float): Duration to press the key in seconds
            
        Returns:
            bool: True if the command was parsed successfully, False otherwise
        """
        # Write the command to the file
        if not self.test_command_writing(key, duration):
            return False
        
        # Start the parser
        self.parser.start()
        
        # Wait for the parser to process the command
        time.sleep(duration + 1.0)
        
        # Check if the command file was cleared
        try:
            with open(self.command_file, 'r') as f:
                content = f.read().strip()
            
            if content == "":
                logger.info(f"Command file was cleared after processing")
                return True
            else:
                logger.error(f"Command file was not cleared after processing. Content: {content}")
                return False
        except Exception as e:
            logger.error(f"Failed to read command file {self.command_file}: {e}")
            return False
        finally:
            # Stop the parser
            self.parser.stop()
    
    def run_tests(self):
        """Run all tests."""
        logger.info("Starting tests...")
        
        # Test command file creation
        logger.info("Testing command file creation...")
        if not self.test_command_file_creation():
            logger.error("Command file creation test failed")
            return False
        
        # Test command writing and parsing for various keys and durations
        for key in TEST_KEYS:
            for duration in TEST_DURATIONS:
                logger.info(f"Testing command writing and parsing for key={key}, duration={duration}...")
                if not self.test_command_parsing(key, duration):
                    logger.error(f"Command parsing test failed for key={key}, duration={duration}")
                    return False
        
        logger.info("All tests passed!")
        return True

def main():
    """Main function to run the tests."""
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Test Headless Editor Text Parser')
    parser.add_argument('--command-file', default=DEFAULT_COMMAND_FILE,
                        help=f'Path to the command file (default: {DEFAULT_COMMAND_FILE})')
    args = parser.parse_args()
    
    # Create and run the tester
    tester = TextParserTester(command_file=args.command_file)
    success = tester.run_tests()
    
    if success:
        logger.info("All tests passed!")
        sys.exit(0)
    else:
        logger.error("Some tests failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()
