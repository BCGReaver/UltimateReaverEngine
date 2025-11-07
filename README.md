# UltimateReaverEngine

Base DirectX 11 minimalista para prácticas de Gráficas Computacionales 3D, con arquitectura modular y **cargador manual de archivos OBJ**.

## Módulos principales
- **Window**: Win32 + dimensiones.
- **Device / DeviceContext**: creación de recursos y comandos.
- **SwapChain**: backbuffer, `present()`.
- **RenderTargetView / DepthStencilView**: bind & clear.
- **Viewport**: set del área de dibujo.
- **ShaderProgram**: VS/PS de `UltimateReaverEngine.fx`.
- **Texture**: carga desde **.dds**, **.jpg**, **.png**.
- **OBJ ModelLoader**: parsea `v`, `vt`, `vn`, `f` + triangulación → genera **VB/IB**.

## Requisitos
- Visual Studio 2022 + Windows SDK
- DirectX 11 (d3d11, d3dcompiler)
- Activos en `bin/` (shaders, texturas, modelos)

## Compilar y correr
1. Abre la solución.
2. Compila **x64** en Debug o Release.
3. Ejecuta (Working Directory = `$(ProjectDir)bin\` o `bin\x64\` según tu config).
4. Deberías ver el modelo **OBJ** cargado y texturizado.

## Cargar tu propio OBJ
1. Copia `miModelo.obj` (y `miModelo.mtl` si tienes) a `bin\` (o `bin\x64\`).
2. Asegúrate de incluir la textura (por ejemplo `miTex.jpg`) en el mismo folder.
3. En `BaseApp::init()`:
   ```cpp
   // Carga el OBJ (rellena m_mesh con vertices/indices)
   m_modelLoader.loadModel("miModelo.obj", m_mesh);

   // Crea buffers GPU
   m_vertexBuffer.init(m_device, m_mesh, D3D11_BIND_VERTEX_BUFFER);
   m_indexBuffer.init(m_device,  m_mesh, D3D11_BIND_INDEX_BUFFER);

   // Asigna textura (JPG/PNG/DDS)
   m_textureCube.init(m_device, "miTex", ExtensionType::JPG);


---

## 📘 Y luego, enlázalo desde tu README
Dentro del `README.md`, agrega una línea como esta donde hables del flujo o la arquitectura:

```markdown
## Diagrama del pipeline
Puedes consultar el flujo completo de inicialización y renderizado en [PIPELINE.md](PIPELINE.md).
