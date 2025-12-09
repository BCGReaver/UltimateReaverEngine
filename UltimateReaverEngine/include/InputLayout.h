#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class
	InputLayout {
public:
	/**
	 * @brief Default constructor.
	 */
	InputLayout() = default;

	/**
	 * @brief Default destructor.
	 */
	~InputLayout() = default;

	/**
	 * @brief Initializes the Input Layout object.
	 * This function creates the ID3D11InputLayout object by describing the
	 * per-vertex data structure and validating it against a compiled vertex shader's
	 * input signature.
	 * @param device The graphics device used to create the input layout.
	 * @param layout A vector of D3D11_INPUT_ELEMENT_DESC structures that defines the vertex format.
	 * @param vertexShaderData A pointer to the compiled vertex shader bytecode, used for validation.
	 * @return HRESULT S_OK if the layout is created successfully and matches the shader signature.
	 */
	HRESULT
		init(Device& device,
			std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* vertexShaderData);

	/**
	 * @brief Per-frame update logic for the input layout.
	 */
	void
		update();

	/**
	 * @brief Binds the input layout to the Input-Assembler (IA) stage of the pipeline.
	 * @param deviceContext The device context used to issue the binding command.
	 */
	void
		render(DeviceContext& deviceContext);

	/**
	 * @brief Releases the underlying ID3D11InputLayout COM object.
	 */
	void
		destroy();

public:
	/**
	 * @brief Pointer to the underlying DirectX 11 Input Layout interface.
	 */
	ID3D11InputLayout* m_inputLayout = nullptr;
};