#include"slpch.h"

#include"SoLin/Renderer/SubTexture2D.h"

namespace SoLin {
    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2& spritePos, const glm::vec2& spriteSize)
        :m_Texture(texture)
    {
        float sheetWidth = texture->GetWidth(), sheetHeight = texture->GetHeight();
        float spriteWidth = cellSize.x, spriteHeight = cellSize.y;

        // 精灵图块的左下角  相当于从(0,0)坐标开始计算
        glm::vec2 min = { spritePos.x * spriteWidth / sheetWidth,  spritePos.y * spriteHeight / sheetHeight };
        // 精灵图块的右上角  相当于从(1,1)坐标开始计算
        glm::vec2 max = { (spritePos.x + spriteSize.x) * spriteWidth / sheetWidth, (spritePos.y + spriteSize.y) * spriteHeight / sheetHeight };

        // 获取精灵表中对应精灵图块四个角的坐标
        m_TexCoords[0] = {min.x,min.y}; //左下
        m_TexCoords[1] = {max.x,min.y}; //右下
        m_TexCoords[2] = {max.x,max.y}; //右上
        m_TexCoords[3] = {min.x,max.y}; //左上
    }
    Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2& spritePos, const glm::vec2& spriteSize)
    {
        return CreateRef<SubTexture2D>(texture, cellSize, spritePos, spriteSize);
    }
}
