/**
 * @file Texture.h
 * @brief Aquí defino la clase Texture, que me sirve para manejar las texturas del motor.
 *
 * @details
 *  Esta clase la uso para cargar imágenes, crear texturas desde cero o duplicar otras texturas.
 *  Básicamente, aquí controlo toda la parte de cómo la GPU almacena y usa imágenes para dibujar.
 *  Me gusta tenerla separada porque así puedo reutilizarla tanto para materiales, como para render targets.
 */

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class Texture
 * @brief Clase encargada de manejar texturas 2D dentro del motor.
 *
 * @details
 *  Aquí manejo la creación, carga, y destrucción de texturas en Direct3D 11.
 *  La puedo inicializar con una imagen externa, con medidas personalizadas
 *  (por ejemplo, un render target), o incluso copiar otra textura con distinto formato.
 *  También incluyo funciones update/render/destroy para mantener el ciclo de vida ordenado.
 */
class
  Texture {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí, solo dejo la estructura lista para cuando llame `init()`.
   *  Así mantengo la inicialización limpia.
   */
  Texture() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No destruyo nada aquí directamente, prefiero que se haga con `destroy()`
   *  para tener más control sobre cuándo libero memoria de GPU.
   */
  ~Texture() = default;

  /**
   * @brief Inicializo la textura cargando una imagen desde disco.
   *
   * @param device         Referencia al dispositivo Direct3D para crear la textura.
   * @param textureName    Nombre o ruta de la textura a cargar (por ejemplo "brick.jpg").
   * @param extensionType  Tipo de extensión (jpg, png, dds, etc) para manejarla correctamente.
   *
   * @return HRESULT       `S_OK` si todo bien, o código de error si falló la carga.
   *
   * @details
   *  Aquí cargo la imagen desde el archivo y creo una textura 2D con su respectivo SRV
   *  (Shader Resource View) para poder usarla en el pipeline. Es la forma normal de cargar assets.
   */
  HRESULT
    init(Device& device,
      const std::string& textureName,
      ExtensionType extensionType);

  /**
   * @brief Inicializo la textura creando un buffer vacío (por ejemplo, render target o depth map).
   *
   * @param device         Dispositivo Direct3D donde creo la textura.
   * @param width          Ancho en píxeles.
   * @param height         Alto en píxeles.
   * @param Format         Formato de la textura (por ejemplo DXGI_FORMAT_R8G8B8A8_UNORM).
   * @param BindFlags      Banderas de enlace (render target, shader resource, etc).
   * @param sampleCount    Cantidad de samples (para MSAA, por default 1).
   * @param qualityLevels  Niveles de calidad del muestreo (por default 0).
   *
   * @return HRESULT       `S_OK` si se creó bien, o error si falló.
   *
   * @details
   *  Con este método creo texturas vacías que no vienen de archivo.
   *  Me sirve cuando quiero renderizar directamente a una textura (como en postprocesos o sombras).
   */
  HRESULT
    init(Device& device,
      unsigned int width,
      unsigned int height,
      DXGI_FORMAT Format,
      unsigned int BindFlags,
      unsigned int sampleCount = 1,
      unsigned int qualityLevels = 0);

  /**
   * @brief Inicializo una textura copiando el contenido o formato de otra textura.
   *
   * @param device      Dispositivo Direct3D donde se crea la nueva textura.
   * @param textureRef  Referencia a la textura que quiero copiar.
   * @param format      Formato de la nueva textura (por ejemplo, para cambiar de tipo o compatibilidad).
   *
   * @return HRESULT    `S_OK` si se clonó bien, o error si algo falló.
   *
   * @details
   *  Uso esto cuando necesito crear una textura basada en otra (por ejemplo, para downsampling o efectos).
   */
  HRESULT
    init(Device& device, Texture& textureRef, DXGI_FORMAT format);

  /**
   * @brief Actualizo el estado de la textura.
   *
   * @details
   *  Normalmente las texturas no cambian cada frame, pero dejo este método por si después
   *  quiero implementar actualizaciones dinámicas (como escribirle datos desde CPU).
   */
  void
    update();

  /**
   * @brief Enlazo la textura al pipeline para que el shader pueda usarla.
   *
   * @param deviceContext  Contexto donde aplico la textura.
   * @param StartSlot      Slot inicial en el que la voy a bindear.
   * @param NumViews       Número de vistas (por si tengo varias texturas seguidas).
   *
   * @details
   *  Aquí básicamente le digo al shader “usa esta textura”.
   *  Es donde realmente la activo en la GPU para que se vea en pantalla.
   */
  void
    render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);

  /**
   * @brief Destruyo y libero los recursos de la textura.
   *
   * @details
   *  Aquí libero los punteros de la textura y del SRV para no dejar memoria colgando.
   *  Lo uso siempre antes de cerrar el motor o cambiar de escena.
   */
  void
    destroy();

public:

  /// @brief Puntero a la textura base (ID3D11Texture2D). Es la que almacena los datos en GPU.
  ID3D11Texture2D* m_texture = nullptr;

  /// @brief Shader Resource View, o sea la vista que los shaders usan para acceder a la textura.
  ID3D11ShaderResourceView* m_textureFromImg = nullptr;

  /// @brief Nombre o ruta del archivo de textura, útil para depurar o recargar assets.
  std::string m_textureName;
};
