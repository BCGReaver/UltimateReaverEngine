#pragma once
/**
 * @file Prerequisites.h
 * @brief Todo lo b�sico que usa el motor (UltimateReaverEngine): includes de Win/DirectX, macros �tiles y structs compartidos.
 *
 * @details
 * Aqu� juntamos:
 * - Librer�as est�ndar y de Windows.
 * - Headers de DirectX 11 (D3D11, D3DX11, HLSL compiler).
 * - Macros para liberar recursos COM y loguear cosas r�pido.
 * - Structs t�picos para los constant buffers y el layout de v�rtice.
 */

 // -----------------------------
 // Librer�as STD / Windows
 // -----------------------------
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>

// -----------------------------
// DirectX 11
// -----------------------------
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

// Recursos (iconos, ids, etc.)
#include "Resource.h"
#include "resource.h"

/**
 * @def SAFE_RELEASE(x)
 * @brief Libera un recurso COM de manera segura y lo deja en nullptr.
 *
 * @param x Puntero COM (por ejemplo, @c ID3D11Buffer*, @c ID3D11Texture2D*).
 *
 * @note Esta macro chequea @p x por nullptr antes de llamar @c Release().
 * �sala siempre que termines con un recurso de DirectX.
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

 /**
  * @def MESSAGE(classObj, method, state)
  * @brief Log de mensajito simple al OutputDebug con el estado de creaci�n/acci�n.
  *
  * @param classObj Nombre de la clase (string o literal wide).
  * @param method   Nombre del m�todo que reporta.
  * @param state    Texto libre para describir qu� pas� (por ejemplo, "OK", "created", etc.).
  *
  * @details
  * Se imprime algo tipo: `Clase::metodo : [CREATION OF RESOURCE : OK]`
  * �til para debug r�pido sin ensuciar consola.
  */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << L"::" << method << L" : " \
       << L"[CREATION OF RESOURCE : " << state << L"]\n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

  /**
   * @def ERROR(classObj, method, errorMSG)
   * @brief Loguea un error en el OutputDebug con formato fijo.
   *
   * @param classObj Nombre de la clase (wide).
   * @param method   Nombre del m�todo (wide).
   * @param errorMSG Mensaje de error ya en wide o literal L"...".
   *
   * @details
   * No tira excepci�n ni corta la app, solo deja el mensaje en el debugger.
   * Ej: `ERROR : Device::CreateBuffer : descripci�n...`
   */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

   // ============================================================================
   // Estructuras compartidas (coinciden con lo que espera el shader)
   // ============================================================================

   /**
    * @struct SimpleVertex
    * @brief Layout de v�rtice b�sico: posici�n y UV.
    *
    * @var SimpleVertex::Pos
    * Posici�n del v�rtice en espacio del objeto (x,y,z).
    *
    * @var SimpleVertex::Tex
    * Coordenadas de textura (u,v) en [0..1].
    */
struct SimpleVertex {
  XMFLOAT3 Pos;
  XMFLOAT2 Tex;
};

/**
 * @struct CBNeverChanges
 * @brief Constant buffer para cosas que casi no cambian (ej: View).
 *
 * @var CBNeverChanges::mView
 * Matriz de vista (c�mara), transpuesta cuando se manda al shader.
 */
struct CBNeverChanges {
  XMMATRIX mView;
};

/**
 * @struct CBChangeOnResize
 * @brief Constant buffer pensado para cuando cambias tama�o de ventana.
 *
 * @var CBChangeOnResize::mProjection
 * Matriz de proyecci�n (perspectiva/ortogr�fica), transpuesta para el shader.
 */
struct CBChangeOnResize {
  XMMATRIX mProjection;
};

/**
 * @struct CBChangesEveryFrame
 * @brief Constant buffer que s� se actualiza cada frame.
 *
 * @var CBChangesEveryFrame::mWorld
 * Matriz world (transform del objeto), transpuesta.
 *
 * @var CBChangesEveryFrame::vMeshColor
 * Color del mesh (RGBA) para tintar o debuggear r�pido.
 */
struct CBChangesEveryFrame {
  XMMATRIX mWorld;
  XMFLOAT4 vMeshColor;
};

/**
 * @enum ExtensionType
 * @brief Tipos de archivo soportados al cargar texturas desde disco.
 *
 * @var ExtensionType::DDS
 * Formato DDS (ideal para GPU, soporta mipmaps y compresi�n).
 *
 * @var ExtensionType::PNG
 * PNG con alpha (com�n para UI, sprites, etc.).
 *
 * @var ExtensionType::JPG
 * JPG (ligero, sin alpha, ojo con artefactos).
 */
enum ExtensionType {
  DDS = 0,
  PNG = 1,
  JPG = 2
};
