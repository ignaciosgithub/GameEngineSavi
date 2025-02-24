# Texture Support Test Assets

This directory contains test assets for demonstrating the texture support features of GameEngineSavi.

## Required Files (not included in repository)

To test the texture functionality, you need to provide the following files:

1. `cube.obj` - A simple cube model with texture coordinates
2. `albedo.png` - Color/diffuse texture (RGB)
3. `normal.png` - Normal map texture (RGB where RGB = XYZ of normal)
4. `opacity.png` - Opacity/transparency texture (grayscale or with alpha channel)

## Usage Example

The `TextureDemo.cpp` file demonstrates how to use the texture support features:

```cpp
// Create a model
Model* cube = new Model("cube.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));

// Load textures
cube->loadTexture("albedo.png", "albedo");     // Color/diffuse texture
cube->loadTexture("normal.png", "normal");     // Normal map for enhanced lighting
cube->loadTexture("opacity.png", "opacity");   // Opacity/transparency map

// Set texture tiling (repeat texture 2x horizontally, 1x vertically)
cube->setTiling(2.0f, 1.0f);
```

## Features Demonstrated

1. **Texture Loading**: Load PNG, JPG, TGA, BMP and other formats using stb_image
2. **Texture Tiling**: Control texture repetition with the `setTiling` method
3. **Normal Mapping**: Enhanced lighting using normal maps
4. **Opacity/Transparency**: Alpha blending for transparent or semi-transparent objects

## Integration with Existing Rendering

The texture support is fully integrated with the existing lighting system, allowing for:
- Textured models with dynamic lighting
- Normal mapping for more detailed surface lighting
- Transparency/opacity control
