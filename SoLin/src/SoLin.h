#pragma once

// 提供给应用端去使用

#include"SoLin/Core/Application.h"
#include"SoLin/Core/Layer.h"
#include"SoLin/Core/Log.h"

#include"SoLin/Core/Timestep.h"

#include"SoLin/Core/Input.h"
#include"SoLin/Core/KeyCodes.h"
#include"SoLin/Core/MouseButtonCodes.h"

#include"SoLin/Renderer/OrthoGraphicCameraController.h"

#include"SoLin/ImGui/ImGuiLayer.h"

//-----------------渲染器-----------------
#include "SoLin/Renderer/Renderer.h"
#include "SoLin/Renderer/RendererCommand.h"

#include "SoLin/Renderer/Buffer.h"
#include "SoLin/Renderer/Shader.h"
#include "SoLin/Renderer/Texture.h"
#include "SoLin/Renderer/VertexArray.h"

//----------------Renderer----------------

//----------------程序入口----------------
#define SL SoLin
//----------------EntryPoint--------------