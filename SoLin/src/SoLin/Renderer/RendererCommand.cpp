#include"slpch.h"
#include"RendererCommand.h"

namespace SoLin {

	Scope<RendererAPI> RendererCommand::s_RendererAPI = RendererAPI::Create();
}
