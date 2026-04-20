#pragma once

#include <filesystem>
#include"SoLin/Renderer/Texture.h"

namespace SoLin {
    class ContentBrowserPanel {
    public:
        ContentBrowserPanel();
        // @brief 渲染更新
        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;   // 当前目录

        Ref<Texture> m_FolderIcon;  // 文件夹图标
        Ref<Texture> m_FileIcon;    // 文件图标
    };
}
