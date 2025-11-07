#pragma once
#include "Prerequisites.h"

// Forward declarations
class Window;
class DeviceContext;


class 
Viewport {
public:
	/**
	 * @brief Default constructor.
	 */
	Viewport() = default;

	/**
	 * @brief Default destructor.
	 */
	~Viewport() = default;

	/**
	 * @brief Initializes the viewport to match the dimensions of a given window.
	 * @param window The application window to get the dimensions from.
	 * @return HRESULT S_OK if successful, otherwise an error code.
	 */
	HRESULT
	init(const Window& window);

	/**
	 * @brief Initializes the viewport with explicit dimensions.
	 * @param width The width of the viewport in pixels.
	 * @param height The height of the viewport in pixels.
	 * @return HRESULT S_OK if successful.
	 */
	HRESULT
	init(unsigned int width, unsigned int height);

	/**
	 * @brief Per-frame update logic for the viewport.
	 * @note Can be used to handle window resize events and update viewport dimensions accordingly.
	 */
	void
	update();

	/**
	 * @brief Binds the viewport to the rasterizer stage of the graphics pipeline.
	 * @param deviceContext The device context used to issue the binding command.
	 */
	void
	render(DeviceContext& deviceContext);

	/**
	 * @brief Cleans up any resources associated with the viewport.
	 * @note As D3D11_VIEWPORT is a struct, this may not be strictly necessary unless
	 * other dynamically allocated resources are added.
	 */
	void
	destroy();

public:
	/**
	 * @brief The underlying D3D11_VIEWPORT structure.
	 * This struct holds the top-left corner coordinates (TopLeftX, TopLeftY),
	 * dimensions (Width, Height), and depth range (MinDepth, MaxDepth).
	 */
	D3D11_VIEWPORT m_viewport;
};