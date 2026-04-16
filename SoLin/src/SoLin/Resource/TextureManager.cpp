#include"slpch.h"
#include"TextureManager.h"
#include"SoLin/Renderer/Renderer2D.h"

namespace SoLin {
    TextureManager::TextureManager()
    {
    }
    Ref<Texture2D> TextureManager::loadTexture(const std::string& filepath)
    {
        // 查看是否已经加载
        auto it = m_textures.find(filepath);
        if (it != m_textures.end()) {
            SL_CORE_WARN("{0}：纹理重复加载，直接返回已存储资源", filepath);
            return it->second;
        }

        Ref<Texture2D> texture = Texture2D::Create(filepath);

        if (!texture) {
            SL_CORE_WARN("{0}：纹理加载失败", filepath);
            return nullptr;
        }
        m_textures.emplace(filepath, texture);

        return texture;
    }
    Ref<Texture2D> TextureManager::getTexture(const std::string& filepath)
    {
        auto it = m_textures.find(filepath);
        if (it != m_textures.end()) {
            return it->second;
        }

        // 没找到就尝试加载
        SL_CORE_WARN("{0}：纹理获取失败，尝试加载", filepath);
        return loadTexture(filepath);
    }
    glm::vec2 TextureManager::getTextureSize(const std::string& filepath)
    {
        auto t = getTexture(filepath);
        if (!t) {
            return glm::vec2(0);
        }

        // 获取纹理尺寸
        glm::vec2 size;
        size.x = t->GetWidth();
        size.y = t->GetHeight();

        return size;
    }
    void TextureManager::unloadTexture(const std::string& filepath)
    {
        auto it = m_textures.find(filepath);
        if (it != m_textures.end()) {
            m_textures.erase(it);
        }
        else {
            SL_CORE_WARN("尝试卸载不存在的纹理：{0}", filepath);
        }
    }
    void TextureManager::clearTextures()
    {
        if (!m_textures.empty()) {
            m_textures.clear();
        }
    }
}
