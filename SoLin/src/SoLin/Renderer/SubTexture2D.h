#pragma once

#include<glm/glm.hpp>

#include "SoLin/Renderer/Texture.h"

namespace SoLin {
    class SubTexture2D {
    public:
        // @param texture 原纹理指针
        // @param cellSize 原纹理单元尺寸
        // @param spritePos 子纹理于原纹理位置
        // @param spriteSize 子纹理单元尺寸
        SubTexture2D(
            const Ref<Texture2D>& texture, const glm::vec2& cellSize,
            const glm::vec2& spritePos, const glm::vec2& spriteSize
        );

        // @param texture 原纹理指针
        // @param cellSize 原纹理单元尺寸
        // @param spritePos 子纹理于原纹理位置
        // @param spriteSize 子纹理单元尺寸
        static Ref<SubTexture2D> Create(
            const Ref<Texture2D>& texture, const glm::vec2& cellSize,
            const glm::vec2& spritePos, const glm::vec2& spriteSize = {1,1}
        );

        const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetCoords() const { return m_TexCoords; }
    private:
        // @brief 原纹理指针
        Ref<Texture2D> m_Texture;

        // @brief 子纹理坐标
        glm::vec2 m_TexCoords[4];
    };
}
