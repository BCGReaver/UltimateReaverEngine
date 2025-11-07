# Pipeline del Engine

Este diagrama muestra el flujo de inicialización y renderizado dentro del motor **UltimateReaverEngine**.

```mermaid
flowchart TD
    A[WinMain] --> B[BaseApp::run]
    B --> C[m_window.init()]
    C --> D[BaseApp::init()]
    D --> E[m_swapChain.init()]
    E --> F[m_renderTargetView.init()]
    F --> G[m_depthStencil.init() + m_depthStencilView.init()]
    G --> H[m_viewport.init()]
    H --> I[m_shaderProgram.init()]
    I --> J[m_modelLoader.loadModel(.obj) → m_mesh]
    J --> K[m_vertexBuffer.init(m_mesh)]
    J --> L[m_indexBuffer.init(m_mesh)]
    K --> M[ConstBuffers.init()]
    L --> M
    M --> N[m_textureCube.init(.dds/.jpg/.png)]
    N --> O[m_samplerState.init()]
    O --> P{Main Loop}
    P --> Q[BaseApp::update(dt)\n- View/Projection\n- World (rotación)\n- CBs update]
    Q --> R[BaseApp::render()\n- RTV/DSV/Viewport\n- Shaders\n- VB/IB/CBs\n- Textura+Sampler\n- DrawIndexed]
    R --> S[m_swapChain.present()]
    S --> P
