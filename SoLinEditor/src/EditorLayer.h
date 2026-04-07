#pragma once

#include "SoLin.h"

namespace SoLin {

    class EditorLayer :public Layer {
    public:
        EditorLayer();
	    virtual ~EditorLayer() = default;

	    virtual void OnAttach() override;
	    virtual void OnDetach()override;

	    virtual void OnUpdate(Timestep ts) override;
	    virtual void OnImGuiRender()override;
	    virtual void OnEvent(Event& event)override;
    private:
	    Ref<Texture2D> m_Texture;

        Ref<SoLin::FrameBuffer> m_Framebuffer;

        glm::vec2 m_ViewportSize = { 0.0f,0.0f };

	    OrthoGraphicCameraController m_CameraController;

	    glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };
    };
}
