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
        Ref<Texture2D> m_SpriteSheet;
        Ref<Texture2D> m_SpriteSheetRole;
        Ref<SubTexture2D> m_Role;
        std::unordered_map<char, Ref<SubTexture2D>> m_TilesMap;

        Ref<SoLin::FrameBuffer> m_Framebuffer;

	    OrthoGraphicCameraController m_CameraController;

	    glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };
    };
}
