// Minimal DirectXMath.h header for testing
#ifndef __DIRECTXMATH_H__
#define __DIRECTXMATH_H__
// DirectXMath namespace
namespace DirectX {
    // Basic vector and matrix types
    struct XMFLOAT3 { float x, y, z; };
    struct XMFLOAT4 { float x, y, z, w; };
    struct XMMATRIX { float m[4][4]; };
}
#endif // __DIRECTXMATH_H__
