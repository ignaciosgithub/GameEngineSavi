import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np

# Load the image
img = mpimg.imread('frames/frame4.png')

# Display information about the image
print(f"Image shape: {img.shape}")
print(f"Image data type: {img.dtype}")

# Check for red pixels (RGB: 255,0,0)
red_pixels = np.sum((img[:,:,0] > 200) & (img[:,:,1] < 50) & (img[:,:,2] < 50))
print(f"Number of red pixels (R>200, G<50, B<50): {red_pixels}")

# Save the image for viewing
plt.figure(figsize=(10, 8))
plt.imshow(img)
plt.title('Frame 4')
plt.savefig('/tmp/frame4_view.png')
print('Frame saved to /tmp/frame4_view.png')
