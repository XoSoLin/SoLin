#include"slpch.h"
#include"ContentBrowserPanels.h"

#include<imgui/imgui.h>

namespace SoLin {

    static float padding = 16.0f;       // 间隔大小
    static float thumbnailSize = 128.0f;// 缩略图尺寸
    // 资源内容的根目录
    extern const std::filesystem::path s_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel()
        :m_CurrentDirectory(s_AssetPath)
    {
        m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon3.png");
        m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon1.png");
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Contect Browser");
        ImGui::Columns(2);
        if (ImGui::Button("<-")) {
            if (m_CurrentDirectory != s_AssetPath) {
                // 回到上一级目录
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        ImGui::NextColumn();
        ImGui::SliderFloat("ThumbnailSize", &thumbnailSize, 64, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);
        ImGui::Columns(1);

        ImGui::Separator();

        // 单个文件按钮尺寸
        float cellSize = padding + thumbnailSize;
        // 面板界面宽
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false); // 设置适合的列数

        // 循环渲染当前目录的每一项
        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            auto const& path = directoryEntry.path();                           // 每一个目录项的路径
            auto relativePath = std::filesystem::relative(path, s_AssetPath);   // 记录目录项相对于 assets/ 的相对路径
            std::string& filenameString = relativePath.filename().string();     // 获得相对路径的文件名

            ImGui::PushID(filenameString.c_str());
            // 将按钮的普通状态颜色设置为完全透明的黑色（即完全透明）
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            Ref<Texture> icon = (directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon);
            ImGui::ImageButton(filenameString.c_str(), (ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

            if (ImGui::BeginDragDropSource()) {
                // 设置拖拽源
                const wchar_t* itemPath = relativePath.c_str();
                // 用于设置拖拽数据     类型标识符               数据指针    数据大小
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                // 如果是目录
                if (directoryEntry.is_directory())
                {
                    // 点击后将该目录叠加在当前目录之后，也就是进入该目录
                    m_CurrentDirectory /= path.filename();
                }
            }
            ImGui::TextWrapped(filenameString.c_str());										// 附加文本（可自动折叠）
            ImGui::NextColumn();

            ImGui::PopID();
        }
        ImGui::Columns(1);

        ImGui::End();
    }


}
