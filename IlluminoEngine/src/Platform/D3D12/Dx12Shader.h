#pragma once

#include <d3d12.h>
#include <map>

#include "Illumino/Renderer/Shader.h"
#include "Illumino/Renderer/GraphicsContext.h"

namespace IlluminoEngine
{
	class Dx12Shader : public Shader
	{
	public:
		Dx12Shader(const char* filepath, const BufferLayout& layout);
		virtual ~Dx12Shader() override;

		virtual void BindConstant(uint32_t slot, uint64_t handle) override;
		virtual void BindGlobal(uint32_t slot, uint64_t handle) override;
		virtual void BindPipeline() override;

		virtual uint64_t CreateBuffer(String&& name, size_t sizeAligned) override;
		virtual void UploadBuffer(String&& name, void* data, size_t size, size_t offsetAligned) override;

	private:
		void SetBufferLayout(const BufferLayout& layout);
		std::string ReadFile(const char* filepath);
		ID3D12Resource* GetConstantBuffer(String&& name);

	private:
		struct ConstantBufferData
		{
			size_t Size = 0;
			ID3D12Resource* Resource = nullptr;
		};

		String m_Filepath;
		ID3D12RootSignature* m_RootSignature;
		ID3D12PipelineState* m_PipelineState;
		std::map<String, ConstantBufferData> m_ConstantBuffers[g_QueueSlotCount];
	};
}
