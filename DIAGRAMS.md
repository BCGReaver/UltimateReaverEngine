# Diagramas

## Clases (resumen rápido)

Window ──▶ SwapChain ──▶ backBuffer(Texture)
             │
             ├── uses Device (crear recursos)
             ├── uses DeviceContext (bind/comandos)
             ├── RenderTargetView (clear/bind)
             └── DepthStencilView (clear/bind)

## Flujo por frame

Update time
Clear RTV/DSV
Bind shaders + buffers
DrawIndexed
Present
