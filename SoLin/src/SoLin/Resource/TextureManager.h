#pragma once
#include<glm/glm.hpp>


namespace SoLin {
    class Renderer2D;
    class Texture2D;

    class TextureManager final {
    public:
        TextureManager();

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;
    private:
        // @brief 加载纹理
        // @param filepath 纹理路径
        Ref<Texture2D> loadTexture(const std::string& filepath);
        // @brief 获取纹理
        // @param filepath 纹理路径
        Ref<Texture2D> getTexture(const std::string& filepath);
        // @brief 获取纹理尺寸
        glm::vec2 getTextureSize(const std::string& filepath);
        // @brief 卸载纹理
        void unloadTexture(const std::string& filepath);
        // @brief 清空所有纹理
        void clearTextures();
    private:
        // @breif 2D纹理映射表
        std::unordered_map<std::string, Ref<Texture2D>> m_textures;
        // @brief 2D渲染器指针
        Renderer2D* m_renderer = nullptr;

        friend class ResourceManager;
    };
}
