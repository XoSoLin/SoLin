#pragma once

#include"SoLin.h"

class ExampleLayer :public SoLin::Layer {
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach()override;
    virtual void OnDetach()override;

    virtual void OnUpdate(SoLin::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(SoLin::Event& event)override;
    

private:
    SoLin::Ref<SoLin::Shader> m_Shader;
    SoLin::ShaderLibrary m_ShaderLibrary;
    SoLin::Ref<SoLin::VertexArray> m_VertexArray;

    SoLin::Ref<SoLin::Shader> m_SquareShader, m_TextureShader;
    SoLin::Ref<SoLin::VertexArray> m_SquareVA;
    SoLin::Ref<SoLin::Texture2D> m_Texture, m_Texture2;

    glm::vec3 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f };

    SoLin::OrthoGraphicCameraController m_CameraController;

};
