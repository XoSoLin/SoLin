#pragma once

#include"TextureManager.h"
namespace SoLin{

    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager() = default;

        // @brief 清理所有资源
        void clear();

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

    public:
        // 纹理管理接口
        Ref<Texture2D> loadTexture(const std::string& file_path);
        Ref<Texture2D> getTexture(const std::string& file_path);
        void unloadTexture(const std::string& file_path);      
        glm::vec2 getTextureSize(const std::string& file_path);
        void clearTextures();
    private:
        std::unique_ptr<TextureManager> m_textureManager;
    };
}
