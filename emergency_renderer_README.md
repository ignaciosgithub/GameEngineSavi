# Emergency Renderer for GameEngineSavi

This is an emergency renderer for the GameEngineSavi editor. It renders each frame to a file instead of directly to the screen, and a separate Python script using pygame displays these rendered frames.

## Building and Running

1. Build the emergency renderer:
   ```bash
   ./build_emergency_renderer.sh
   ```

2. Run the emergency renderer to generate frames:
   ```bash
   ./bin/linux/Editor_emergency
   ```

3. Install pygame if you don't have it:
   ```bash
   pip install pygame
   ```

4. Run the Python viewer script to view the rendered frames:
   ```bash
   python frame_viewer.py
   ```

## Controls

- Left/Right Arrow Keys: Navigate between frames
- Escape: Exit the viewer

## Troubleshooting

- If you see no frames, make sure the emergency renderer has been run first to generate frames.
- If frames are not displaying correctly, check the console output for error messages.

## Viewer Options

Two viewer scripts are provided:

1. `frame_viewer.py` - A feature-rich viewer with playback controls:
   - Space: Toggle auto-play
   - Left/Right Arrow Keys: Navigate between frames
   - Escape: Exit the viewer

2. `simple_frame_viewer.py` - A simplified viewer that loads frames as quickly as possible:
   - Escape: Exit the viewer

The simple viewer is recommended for basic visualization needs.
