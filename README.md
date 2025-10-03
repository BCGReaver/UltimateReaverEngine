# UltimateReaverEngine
Este es un compendio de las clases vistas en la materia de Graficas Computacionales 3D.

Base DirectX 11 minimalista con arquitectura modular:
- **Window**: Win32 + dimensiones
- **Device / DeviceContext**: creación de recursos y comandos
- **SwapChain**: backbuffer, `Present()`
- **RenderTargetView / DepthStencilView**: bind y clear
- **Texture**: desde archivo o memoria
- **Shaders + Draw**: cubo texturizado

## Requisitos
- Visual Studio + Windows SDK
- DirectX 11 (d3d11, d3dcompiler)
- assets/shaders incluidos

## Compilar y correr
1. Abre la solución / CMake.
2. Compila x64 Debug o Release.
3. Ejecuta: ventana + cubo girando.

## Estructura del proyecto
Ver `docs/ARCHITECTURE.md`.

## Documentación
- Doxygen en headers/cpp.
- Docs en `docs/`.

## Licencia
Usage educacional.
