/**
 * @file Device.h
 * @brief Aquí defino la clase Device, que representa el corazón de Direct3D: el dispositivo de render.
 *
 * @details
 *  Esta clase la uso para crear todos los recursos gráficos del motor: texturas, buffers, shaders,
 *  render targets, samplers, etc.
 *  El `Device` básicamente es el que tiene permiso de hablar directamente con la GPU para crear cosas.
 *  Todo lo que después usa el `DeviceContext` se genera aquí.
 */

#pragma once
#include "Prerequisites.h"

 /**
  * @class Device
  * @brief Clase encargada de manejar el dispositivo de Direct3D 11.
  *
  * @details
  *  Esta clase es la que crea todos los objetos base de Direct3D.
  *  Cuando necesito una textura, un shader o un render target, se lo pido a este compa.
  *  En pocas palabras: el `Device` fabrica, y el `DeviceContext` usa lo que se fabricó.
  */
class
  Device {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí todavía porque el dispositivo se inicializa con `init()`
   *  cuando configuro Direct3D.
   */
  Device() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente; todo se limpia en `destroy()` para tener control completo.
   */
  ~Device() = default;

  /**
   * @brief Inicializo el dispositivo.
   *
   * @details
   *  Aquí normalmente se crea el `ID3D11Device` y se inicializan las capacidades del hardware.
   *  Lo dejo como método separado para mantener el flujo de inicialización ordenado.
   */
  void
    init();

  /**
   * @brief Actualizo el estado del dispositivo.
   *
   * @details
   *  Este método no suele hacer mucho porque el dispositivo en sí no cambia durante la ejecución,
   *  pero lo mantengo por consistencia en el ciclo del motor.
   */
  void
    update();

  /**
   * @brief Renderizo (si fuera necesario) o coordino creación de recursos.
   *
   * @details
   *  Este método se usa más como parte estructural del motor, no para render directo.
   */
  void
    render();

  /**
   * @brief Destruyo el dispositivo y libero todos los recursos asociados.
   *
   * @details
   *  Libero el `m_device` y dejo el puntero en `nullptr` para evitar fugas de memoria.
   */
  void
    destroy();

  // ---------------------------------------------------------------------
  // Métodos para crear objetos del pipeline gráfico
  // ---------------------------------------------------------------------

  /**
   * @brief Creo un Render Target View.
   *
   * @param pResource  Recurso base (como una textura o back buffer).
   * @param pDesc      Descripción del RTV.
   * @param ppRTView   Doble puntero donde guardo el resultado.
   *
   * @return HRESULT   `S_OK` si se creó correctamente.
   *
   * @details
   *  Este método lo uso para decirle a Direct3D dónde va a dibujar (por ejemplo, en pantalla o en una textura).
   */
  HRESULT
    CreateRenderTargetView(ID3D11Resource* pResource,
      const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
      ID3D11RenderTargetView** ppRTView);

  /**
   * @brief Creo una textura 2D.
   *
   * @param pDesc         Descripción de la textura (ancho, alto, formato, etc.).
   * @param pInitialData  Datos iniciales opcionales para rellenarla.
   * @param ppTexture2D   Puntero donde guardo la textura creada.
   *
   * @return HRESULT      `S_OK` si se creó correctamente.
   *
   * @details
   *  Este método lo uso para crear texturas normales, render targets o depth maps.
   */
  HRESULT
    CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Creo una vista de profundidad y stencil (Depth Stencil View).
   *
   * @param pResource          Recurso base (usualmente una textura de profundidad).
   * @param pDesc              Descripción del depth stencil.
   * @param ppDepthStencilView Doble puntero donde guardo la vista creada.
   *
   * @return HRESULT           `S_OK` si se creó correctamente.
   *
   * @details
   *  Esta vista me sirve para manejar la profundidad en el render, evitando que objetos se atraviesen visualmente.
   */
  HRESULT
    CreateDepthStencilView(ID3D11Resource* pResource,
      const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
      ID3D11DepthStencilView** ppDepthStencilView);

  /**
   * @brief Creo un Vertex Shader.
   *
   * @param pShaderBytecode   Datos compilados del shader.
   * @param BytecodeLength    Tamaño del bytecode.
   * @param pClassLinkage     Enlace opcional para shaders con clases.
   * @param ppVertexShader    Puntero donde guardo el vertex shader creado.
   *
   * @return HRESULT          `S_OK` si se creó correctamente.
   */
  HRESULT
    CreateVertexShader(const void* pShaderBytecode,
      unsigned int BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11VertexShader** ppVertexShader);

  /**
   * @brief Creo un Input Layout.
   *
   * @param pInputElementDescs  Arreglo con la descripción de cada elemento del vértice.
   * @param NumElements         Número de elementos en el arreglo.
   * @param pShaderBytecodeWithInputSignature  Bytecode del vertex shader (para validación).
   * @param BytecodeLength      Tamaño del bytecode.
   * @param ppInputLayout       Puntero donde guardo el layout creado.
   *
   * @return HRESULT            `S_OK` si se creó correctamente.
   */
  HRESULT
    CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
      unsigned int NumElements,
      const void* pShaderBytecodeWithInputSignature,
      unsigned int BytecodeLength,
      ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Creo un Pixel Shader.
   *
   * @param pShaderBytecode  Datos compilados del pixel shader.
   * @param BytecodeLength   Tamaño del bytecode.
   * @param pClassLinkage    Enlace opcional para shaders con clases.
   * @param ppPixelShader    Puntero donde guardo el pixel shader creado.
   *
   * @return HRESULT         `S_OK` si se creó correctamente.
   *
   * @details
   *  Este es el shader que define el color final de cada píxel.
   */
  HRESULT
    CreatePixelShader(const void* pShaderBytecode,
      unsigned int BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11PixelShader** ppPixelShader);

  /**
   * @brief Creo un buffer genérico (como vertex buffer o index buffer).
   *
   * @param pDesc         Descripción del buffer (tamaño, uso, tipo, etc.).
   * @param pInitialData  Datos iniciales opcionales.
   * @param ppBuffer      Puntero donde guardo el buffer creado.
   *
   * @return HRESULT      `S_OK` si se creó correctamente.
   */
  HRESULT
    CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Buffer** ppBuffer);

  /**
   * @brief Creo un Sampler State.
   *
   * @param pSamplerDesc  Descripción del sampler (filtros, modos de dirección, etc.).
   * @param ppSamplerState  Puntero donde guardo el sampler creado.
   *
   * @return HRESULT        `S_OK` si se creó correctamente.
   *
   * @details
   *  Este objeto le dice a la GPU cómo debe leer las texturas (por ejemplo, filtrado lineal o punto).
   */
  HRESULT
    CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
      ID3D11SamplerState** ppSamplerState);

public:

  /// @brief Puntero principal al dispositivo Direct3D 11, el que crea todos los recursos del motor.
  ID3D11Device* m_device = nullptr;
};
