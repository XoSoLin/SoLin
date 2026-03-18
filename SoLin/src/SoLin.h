#pragma once

// 提供给应用端去使用

#include"SoLin/Application.h"
#include"SoLin/Layer.h"
#include"SoLin/Log.h"

#include"SoLin/Input.h"
#include"SoLin/KeyCodes.h"
#include"SoLin/MouseButtonCodes.h"

#include"SoLin/ImGui/ImGuiLayer.h"

//-----------------渲染器-----------------
#include "SoLin/Renderer/Renderer.h"
#include "SoLin/Renderer/RendererCommand.h"

#include "SoLin/Renderer/Buffer.h"
#include "SoLin/Renderer/Shader.h"
#include "SoLin/Renderer/VertexArray.h"

#include "SoLin/Renderer/OrthoGraphicCamera.h"
//----------------Renderer----------------

//----------------程序入口----------------
#define SL SoLin
#include"SoLin/EntryPoint.h"
//----------------EntryPoint--------------