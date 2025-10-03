#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @file Texture.h
 * @brief Wrapper sencillito para manejar texturas 2D en D3D11 dentro de **UltimateReaverEngine**.
 *
 * @details
 * Esta clase se encarga de crear, usar y soltar texturas. Puedes:
 * - Cargar desde archivo (png, jpg, dds, etc.).
 * - Crear texturas “vacías” en GPU (para RT/DS o lo que necesites).
 * - Armar vistas como Shader Resource para pegarlas al Pixel Shader.
 *
 * Notas rápidas:
 * - camelCase para métodos/vars.
 * - Miembros con prefijo m_.
 * - Indent de 2 espacios para que se vea limpio.
 */
class
  Texture {
public:
  /**
   * @brief Ctor por defecto (no crea nada aún).
   */
  Texture() = default;

  /**
   * @brief Dtor por defecto (recursos se liberan con destroy()).
   */
  ~Texture() = default;

  /**
   * @brief Crea una textura desde un archivo de imagen y su SRV.
   *
   * @param device        Dispositivo D3D11 para crear los recursos.
   * @param textureName   Ruta/nombre del archivo (sin extensión o como tú lo manejes).
   * @param extensionType Tipo de extensión (PNG, JPG, DDS, etc.).
   * @return S_OK si todo ok; HRESULT de error si falla.
   *
   * @post Si sale bien, tendrás el recurso y la SRV listos para bindear.
   */
  HRESULT
    init(Device& device,
      const std::string& textureName,
      ExtensionType extensionType);

  /**
   * @brief Crea una textura 2D “desde cero” en GPU (útil para RT/DS/UAV).
   *
   * @param device        Dispositivo D3D11.
   * @param width         Ancho en píxeles.
   * @param height        Alto en píxeles.
   * @param Format        Formato (ej. DXGI_FORMAT_R8G8B8A8_UNORM).
   * @param BindFlags     Banderas (ej. D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE).
   * @param sampleCount   MSAA samples (1 si no usas MSAA).
   * @param qualityLevels Niveles de calidad para MSAA (0 si no aplica).
   * @return S_OK si ok; HRESULT si no.
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
   * @brief Crea una SRV (o textura relacionada) a partir de otra textura.
   *
   * @param device     Dispositivo D3D11.
   * @param textureRef Textura base desde la que armamos la vista.
   * @param format     Formato deseado para la SRV.
   * @return S_OK si ok; HRESULT si falla.
   */
  HRESULT
    init(Device& device, Texture& textureRef, DXGI_FORMAT format);

  /**
   * @brief Hook para actualizar data de la textura si luego haces streaming.
   *
   * @note Por ahora no hace nada, es más para dejar listo el contrato.
   */
  void
    update();

  /**
   * @brief Bindea la textura al Pixel Shader.
   *
   * @param deviceContext Contexto de device.
   * @param StartSlot     Slot inicial (ej. 0).
   * @param NumViews      Número de vistas a setear (normalmente 1).
   *
   * @pre m_textureFromImg debe existir (o sea, ya hiciste init()).
   */
  void
    render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);

  /**
   * @brief Suelta los recursos COM (textura y SRV).
   *
   * @post m_texture == nullptr y m_textureFromImg == nullptr.
   */
  void
    destroy();

public:
  /** @brief Recurso base de textura en GPU. */
  ID3D11Texture2D* m_texture = nullptr;

  /** @brief Vista como Shader Resource (para PS/CS, etc.). */
  ID3D11ShaderResourceView* m_textureFromImg = nullptr;

  /** @brief Nombre/ruta original por si quieres loggear o recargar. */
  std::string m_textureName;
};
