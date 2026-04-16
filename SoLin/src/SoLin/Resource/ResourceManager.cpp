#include"slpch.h"
#include"ResourceManager.h"

namespace SoLin {
    ResourceManager::ResourceManager()
    {
        // 初始化各个子管理器
        m_textureManager = std::make_unique<TextureManager>();
    }
    void ResourceManager::clear()
    {
        // 清理各个子管理器的内容
        m_textureManager->clearTextures();
    }

    Ref<Texture2D> ResourceManager::loadTexture(const std::string& file_path)
    {
        return m_textureManager->loadTexture(file_path);
    }
    Ref<Texture2D> ResourceManager::getTexture(const std::string& file_path)
    {
        return m_textureManager->getTexture(file_path);
    }
    void ResourceManager::unloadTexture(const std::string& file_path)
    {
        m_textureManager->unloadTexture(file_path);
    }
    glm::vec2 ResourceManager::getTextureSize(const std::string& file_path)
    {
        return m_textureManager->getTextureSize(file_path);
    }
    void ResourceManager::clearTextures()
    {
        m_textureManager->clearTextures();
    }
}
