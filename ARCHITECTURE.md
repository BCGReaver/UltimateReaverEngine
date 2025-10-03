# Arquitectura

## Módulos
- **Window**: crea/gestiona HWND, tamaño. 
- **Device**: wrap de `ID3D11Device` + “Create*” helpers.
- **DeviceContext**: wrap de `ID3D11DeviceContext` + comandos (IA/VS/PS/OM/...).
- **SwapChain**: crea `IDXGISwapChain`, configura MSAA, obtiene backbuffer.
- **Texture**: `ID3D11Texture2D` + `ID3D11ShaderResourceView`.
- **RenderTargetView**: crea/bindea RTV y limpia color.
- **DepthStencilView**: crea/bindea DSV y limpia depth/stencil.
- **UltimateReaverEngine.cpp**: `wWinMain`, `InitDevice`, `Render`, loop.

## Flujo (arranque)
1. `Window::init()`
2. `SwapChain::init(device, deviceContext, backBuffer, window)`
3. `RenderTargetView::init(device, backBuffer, format)`
4. `Texture depth` + `DepthStencilView::init(...)`
5. `RSSetViewports`
6. Compilar/crear shaders + input layout
7. Crear vertex/index buffers + constant buffers
8. Set pipeline y `Present()` por frame

## Responsabilidades
- Crear ↔ Destruir recursos: cada clase libera lo suyo.
- Estado explícito: RTV/DSV se bindean antes de dibujar.
