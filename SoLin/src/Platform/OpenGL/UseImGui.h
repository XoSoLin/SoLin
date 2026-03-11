#pragma once

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Platform/OpenGL/ImGuiGLFW.h"
#include "GLFW/glfw3.h"

ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int keycode, int scancode);

namespace SoLin {

	static ImGuiKey keyGlToImGui(int keycode) {

		return ImGui_ImplGlfw_KeyToImGuiKey(keycode, 0);
	}

}