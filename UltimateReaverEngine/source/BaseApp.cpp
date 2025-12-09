/**
 * @file BaseApp.cpp
 * @brief Implementación de la clase BaseApp y su ciclo de vida.
 *
 * @details
 *  Aquí es donde realmente se ejecuta el motor: arranco la ventana, inicializo Direct3D,
 *  corro el game loop, actualizo todo frame con frame, renderizo la escena y al final
 *  destruyo y libero todos los recursos.
 *  También manejo la integración con ImGui y el WndProc de Windows.
 */

#include "BaseApp.h"
#include <ResourceManager.h>

 /// @brief WndProc especial que usa ImGui para procesar la entrada de Windows.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
  HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Flag local para saber si la UI/ImGui se inicializó correctamente
namespace
{
  /// @brief Bandera para saber si la interfaz de usuario (ImGui/UserInterface) ya está lista.
  bool g_UserInterfaceInitialized = false;
}

/**
 * @brief Arranco la aplicación base y manejo el game loop principal.
 *
 * @param hInst    Instancia de la aplicación (la que da Windows).
 * @param nCmdShow Cómo se va a mostrar la ventana (normal, maximizada, etc.).
 *
 * @return int     Código de salida de la aplicación.
 *
 * @details
 *  Aquí:
 *  - Inicializo la ventana.
 *  - Llamo a `init()` para levantar todo el motor.
 *  - Entro en el loop de mensajes de Windows.
 *  - Cuando no hay mensajes, actualizo (`update`) y dibujo (`render`) la escena.
 *  Uso `QueryPerformanceCounter` para calcular `deltaTime` de forma precisa.
 */
int
BaseApp::run(HINSTANCE hInst, int nCmdShow) {
  if (FAILED(m_window.init(hInst, nCmdShow, wndProc))) {
    return 0;
  }
  if (FAILED(init()))
    return 0;

  // Main message loop
  MSG msg = {};
  LARGE_INTEGER freq, prev;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&prev);

  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      LARGE_INTEGER curr;
      QueryPerformanceCounter(&curr);
      float deltaTime =
        static_cast<float>(curr.QuadPart - prev.QuadPart) / freq.QuadPart;
      prev = curr;

      // Cuando no hay mensajes de Windows, aprovecho para actualizar lógica y renderizar
      update(deltaTime);
      render();
    }
  }
  return (int)msg.wParam;
}

/**
 * @brief Inicializo todo el pipeline del motor: swap chain, RTV, depth, viewport, shaders, buffers, actor, ImGui, etc.
 *
 * @return HRESULT `S_OK` si todo se inicializó bien, o un código de error si algo falló.
 *
 * @details
 *  En este método:
 *  - Creo el swap chain y el back buffer.
 *  - Creo el render target view y el depth stencil.
 *  - Configuro el viewport.
 *  - Cargo el modelo (Aircraft.fbx) y su textura.
 *  - Creo y configuro el shader program y los constant buffers.
 *  - Configuro la cámara (View) y la proyección (Projection).
 *  - Inicializo la UI (UserInterface/ImGui) y marco que ya está lista.
 */
HRESULT
BaseApp::init() {
  HRESULT hr = S_OK;

  // Create Swap Chain
  hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize SwapChain. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // Create a render target view
  hr = m_renderTargetView.init(m_device,
    m_backBuffer,
    DXGI_FORMAT_R8G8B8A8_UNORM);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize RenderTargetView. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // Create depth stencil texture
  hr = m_depthStencil.init(m_device,
    m_window.m_width,
    m_window.m_height,
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    D3D11_BIND_DEPTH_STENCIL,
    4,
    0);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize DepthStencil. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // Create the depth stencil view
  hr = m_depthStencilView.init(m_device,
    m_depthStencil,
    DXGI_FORMAT_D24_UNORM_S8_UINT);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize DepthStencilView. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // Create the viewport
  hr = m_viewport.init(m_window);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize Viewport. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // --------------------------------------------------------------------
  //  Cargar modelo FBX y textura
  // --------------------------------------------------------------------
  // Creo un actor usando mi sistema de punteros inteligentes EU::MakeShared
  m_abeBowser = EU::MakeShared<Actor>(m_device);

  if (!m_abeBowser.isNull()) {

    // Cargar modelo FBX
    m_model = new Model3D("Aircraft.fbx", ModelType::FBX);
    std::vector<MeshComponent> abeBowserMeshes = m_model->GetMeshes();

    // Cargar textura (asegúrate de tener E_45_col.jpg en /bin)
    std::vector<Texture> abeBowserTextures;
    hr = m_abeBowserAlbedo.init(m_device,
      "E_45_col",           // nombre del archivo SIN extensión
      ExtensionType::JPG);  // porque es .jpg

    if (FAILED(hr)) {
      ERROR("Main", "InitDevice",
        ("Failed to initialize abeBowserAlbedo. HRESULT: " +
          std::to_string(hr)).c_str());
      return hr;
    }

    abeBowserTextures.push_back(m_abeBowserAlbedo);

    // Asignar mallas y texturas al actor
    m_abeBowser->setMesh(m_device, abeBowserMeshes);
    m_abeBowser->setTextures(abeBowserTextures);
    m_abeBowser->setName("Aircraft_E45");
    m_actors.push_back(m_abeBowser);

    // Transform del avión (ajusta a tu gusto)
    m_abeBowser->getComponent<Transform>()->setTransform(
      EU::Vector3(0.0f, 0.0f, 10.0f),   // posición
      EU::Vector3(0.0f, 0.0f, 0.0f),    // rotación
      EU::Vector3(1.0f, 1.0f, 1.0f));   // escala
  }
  else {
    ERROR("Main", "InitDevice", "Failed to create Aircraft Actor.");
    return E_FAIL;
  }

  // --------------------------------------------------------------------
  // Input Layout
  // --------------------------------------------------------------------
  std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

  D3D11_INPUT_ELEMENT_DESC position;
  position.SemanticName = "POSITION";
  position.SemanticIndex = 0;
  position.Format = DXGI_FORMAT_R32G32B32_FLOAT; // float3
  position.InputSlot = 0;
  position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  position.InstanceDataStepRate = 0;
  layout.push_back(position);

  D3D11_INPUT_ELEMENT_DESC texcoord;
  texcoord.SemanticName = "TEXCOORD";
  texcoord.SemanticIndex = 0;
  texcoord.Format = DXGI_FORMAT_R32G32_FLOAT; // float2 para UVs
  texcoord.InputSlot = 0;
  texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  texcoord.InstanceDataStepRate = 0;
  layout.push_back(texcoord);

  hr = m_shaderProgram.init(m_device, "UltimateReaverEngine.fx", layout);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize ShaderProgram. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // Const buffers
  hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges));
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize NeverChanges Buffer. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize));
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice",
      ("Failed to initialize ChangeOnResize Buffer. HRESULT: " +
        std::to_string(hr)).c_str());
    return hr;
  }

  // View & Projection
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_View = XMMatrixLookAtLH(Eye, At, Up);

  cbNeverChanges.mView = XMMatrixTranspose(m_View);
  m_Projection = XMMatrixPerspectiveFovLH(
    XM_PIDIV4,
    m_window.m_width / (FLOAT)m_window.m_height,
    0.01f,
    100.0f);
  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

  // Inicializar ImGui / UserInterface
  m_userInterface.init(m_window.m_hWnd,
    m_device.m_device,
    m_deviceContext.m_deviceContext);

  g_UserInterfaceInitialized = true;

  // Actor seleccionado en el inspector
  if (!m_abeBowser.isNull()) {
    m_userInterface.setSelectedActor(m_abeBowser.get());
  }

  return S_OK;
}

/**
 * @brief Actualizo la lógica del motor en cada frame.
 *
 * @param deltaTime Tiempo transcurrido desde el frame anterior (en segundos).
 *
 * @details
 *  Aquí:
 *  - Actualizo un tiempo local `t` (por si quiero animaciones dependientes de tiempo).
 *  - Actualizo la interfaz de usuario si ya está inicializada.
 *  - Actualizo las matrices de View y Projection y las mando a los constant buffers.
 *  - Recorro todos los actores y llamo su `update`.
 */
void
BaseApp::update(float deltaTime) {
  // Update time
  static float t = 0.0f;
  if (m_swapChain.m_driverType == D3D_DRIVER_TYPE_REFERENCE) {
    t += (float)XM_PI * 0.0125f;
  }
  else {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // UI frame
  if (g_UserInterfaceInitialized) {
    m_userInterface.update();
  }

  // Update view/projection
  cbNeverChanges.mView = XMMatrixTranspose(m_View);
  m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr,
    &cbNeverChanges, 0, 0);

  m_Projection = XMMatrixPerspectiveFovLH(
    XM_PIDIV4,
    m_window.m_width / (FLOAT)m_window.m_height,
    0.01f,
    100.0f);
  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
  m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr,
    &cbChangesOnResize, 0, 0);

  // Update actors
  for (auto& actor : m_actors) {
    actor->update(deltaTime, m_deviceContext);
  }
}

/**
 * @brief Renderizo la escena completa en cada frame.
 *
 * @details
 *  Aquí:
 *  - Limpio el render target y el depth stencil con un color base.
 *  - Seteo el viewport, la vista de profundidad y los shaders.
 *  - Envío los constant buffers que no cambian mucho (View/Projection).
 *  - Recorro y dibujo todos los actores de la escena.
 *  - Renderizo la UI (ImGui/UserInterface).
 *  - Llamo a `present()` para mostrar el frame en pantalla.
 */
void
BaseApp::render() {
  float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
  m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

  m_viewport.render(m_deviceContext);
  m_depthStencilView.render(m_deviceContext);
  m_shaderProgram.render(m_deviceContext);

  m_cbNeverChanges.render(m_deviceContext, 0, 1);
  m_cbChangeOnResize.render(m_deviceContext, 1, 1);

  for (auto& actor : m_actors) {
    actor->render(m_deviceContext);
  }

  if (g_UserInterfaceInitialized) {
    m_userInterface.render();
  }

  m_swapChain.present();
}

/**
 * @brief Destruyo y libero todos los recursos del motor.
 *
 * @details
 *  Aquí:
 *  - Limpio el estado del device context.
 *  - Destruyo la UI si estaba activa.
 *  - Destruyo constant buffers, shaders, depth, RTV, swap chain, etc.
 *  - Libero el modelo cargado si no estoy usando smart pointers.
 */
void
BaseApp::destroy() {
  if (m_deviceContext.m_deviceContext)
    m_deviceContext.m_deviceContext->ClearState();

  if (g_UserInterfaceInitialized) {
    m_userInterface.destroy();
    g_UserInterfaceInitialized = false;
  }

  m_cbNeverChanges.destroy();
  m_cbChangeOnResize.destroy();
  m_shaderProgram.destroy();
  m_depthStencil.destroy();
  m_depthStencilView.destroy();
  m_renderTargetView.destroy();
  m_swapChain.destroy();
  m_backBuffer.destroy();
  m_deviceContext.destroy();
  m_device.destroy();

  // opcional: liberar modelo si no usas smart pointer
  delete m_model;
  m_model = nullptr;
}

/**
 * @brief Procedimiento de ventana (WndProc) para manejar mensajes de Windows.
 *
 * @param hWnd    Handle de la ventana.
 * @param message Mensaje enviado por Windows (teclado, ratón, paint, destroy, etc.).
 * @param wParam  Parámetro adicional (depende del mensaje).
 * @param lParam  Parámetro adicional (depende del mensaje).
 *
 * @return LRESULT Resultado del procesamiento del mensaje.
 *
 * @details
 *  Aquí:
 *  - Primero dejo que ImGui procese el mensaje; si lo consume, regreso.
 *  - Manejo eventos como:
 *    - `WM_CREATE`: guardo datos de creación.
 *    - `WM_PAINT`: pinto lo mínimo para que Windows esté contento.
 *    - `WM_DESTROY`: envío `PostQuitMessage` para cerrar el game loop.
 *  - Todo lo demás lo mando al `DefWindowProc` por defecto.
 */
LRESULT
BaseApp::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    return true;

  switch (message)
  {
  case WM_CREATE:
  {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);
  }
  return 0;

  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}
