// Viewport.cpp
#include "Viewport.h"
#include "Window.h"
#include "DeviceContext.h"
#include <d3d11.h>
#include <windows.h>

HRESULT Viewport::init(const Window& window)
{
  // Forma robusta: medir el área de cliente actual del HWND
  RECT rc{};
  if (!GetClientRect(window.m_hWnd, &rc))
    return E_FAIL;

  const float w = static_cast<float>(rc.right - rc.left);
  const float h = static_cast<float>(rc.bottom - rc.top);
  if (w <= 0.0f || h <= 0.0f) return E_INVALIDARG;

  m_viewport.TopLeftX = 0.0f;
  m_viewport.TopLeftY = 0.0f;
  m_viewport.Width = w;
  m_viewport.Height = h;
  m_viewport.MinDepth = 0.0f;
  m_viewport.MaxDepth = 1.0f;
  return S_OK;
}

HRESULT Viewport::init(unsigned int width, unsigned int height)
{
  if (width == 0 || height == 0) return E_INVALIDARG;

  m_viewport.TopLeftX = 0.0f;
  m_viewport.TopLeftY = 0.0f;
  m_viewport.Width = static_cast<float>(width);
  m_viewport.Height = static_cast<float>(height);
  m_viewport.MinDepth = 0.0f;
  m_viewport.MaxDepth = 1.0f;
  return S_OK;
}

void Viewport::update()
{
  // Si luego manejas WM_SIZE, puedes recalcular aquí con el HWND actual
}

void Viewport::render(DeviceContext& deviceContext)
{
  if (!deviceContext.m_deviceContext) return;
  deviceContext.m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Viewport::destroy()
{
  // D3D11_VIEWPORT no requiere liberar recursos
}
