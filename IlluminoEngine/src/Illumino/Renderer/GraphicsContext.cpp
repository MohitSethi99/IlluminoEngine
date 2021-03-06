#include "ipch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "Platform/D3D12/Dx12GraphicsContext.h"

namespace IlluminoEngine
{
	Scope<GraphicsContext> IlluminoEngine::GraphicsContext::Create(const Window& window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	ILLUMINO_ASSERT(false, "RendererAPI::None is currently not supported");
										return nullptr;
		case RendererAPI::API::DX12:	return CreateScope<Dx12GraphicsContext>(window);
		}

		ILLUMINO_ASSERT(false, "Unknown GraphicsContext");
		return nullptr;
	}
}
