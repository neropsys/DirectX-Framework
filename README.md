# DirectX-Framework
DX11 framework based on http://www.rastertek.com/. Modified &amp; optimized as best as I could. 

Main goal of this repository is to update and use the SDK recommended from MSDN.

This repository will be removed if the owner of the website mentioned above requests. 

# Notes

1. I'm using Visual Studio 2013 Professional Edition on Windows 7
2. This code does not use legacy DirectX SDK(June 2010) as well as D3DX
3. This code will only use the DirectX SDK included in Windows 8.0 SDK.
4. I will add URLs to the libraries that are not included in SDK.(along with installation instructions)
5. Any modification on the DirectX SDK will be commented on the code.

# External Libraries
1. [DirectXTK](http://directxtk.codeplex.com/)
  
   [How to install](https://www.nuget.org/packages/directxtk_desktop_2013/)
    
# Modifications of the SDK

1. Added + operator on XMFLOAT3 struct(declared in DirectXMath.h).

    `XMFLOAT3& operator+ (const XMFLOAT3& Float3) { x = x + Float3.x; y = y + Float3.y; z = z + Float3.z; return *this; }`