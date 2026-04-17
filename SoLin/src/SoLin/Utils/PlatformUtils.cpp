#include "slpch.h"
#include"SoLin/Utils/PlatformUtils.h"

#include<commdlg.h>
#include<GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include<GLFW/glfw3native.h>

#include"SoLin/Core/Application.h"

namespace SoLin {
    std::string FileDialogs::OpenFile(const char* filter) {

        // 定义 OPENFILENAMEA 结构体（ANSI 版本，用于多字节字符集）
        OPENFILENAMEA ofn;
        // 定义字符数组作为文件路径缓冲区，大小为 260（Windows 最大路径长度 MAX_PATH）
        CHAR szFile[260] = { 0 };
        // 将结构体内存清零初始化（防止残留数据导致问题）
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // 设置结构体大小（必须，用于版本检查）
        ofn.lStructSize = sizeof(OPENFILENAME);
        // 设置父窗口句柄（对话框的所有者窗口）
        // glfwGetWin32Window 将 GLFW 窗口句柄转换为 Win32 HWND
        ofn.hwndOwner = glfwGetWin32Window(
            (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()
        );
        // 设置文件路径缓冲区
        ofn.lpstrFile = szFile;
        // 设置缓冲区大小（字符数，非字节数）
        ofn.nMaxFile = sizeof(szFile);
        // 设置文件过滤器（例如："Text Files\0*.txt\0All Files\0*.*\0"）
        // Windows 文件过滤器
        // 使用 双空字符 (\0\0) 作为整个过滤器的结尾
        // 每对之间用 单个空字符 (\0) 分隔
        // 以上例子中：描述1(Text Files)-模式1(*.txt)  描述2(All Files)-模式2(*.*)
        ofn.lpstrFilter = filter;
        // 设置默认选中的过滤器索引（1 为第一个过滤器）
        ofn.nFilterIndex = 1;
        // 设置标志位：
        // OFN_PATHMUSTEXIST - 路径必须存在
        // OFN_FILEMUSTEXIST - 文件必须存在
        // OFN_NOCHANGEDIR   - 不改变当前工作目录
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // 显示打开文件对话框，如果用户点击"打开"按钮（返回 TRUE）
        if (GetOpenFileNameA(&ofn) == TRUE) {
            // 返回用户选择的文件路径（存储在 lpstrFile 中）
            return ofn.lpstrFile;
        }
        // 用户取消或出错，返回空字符串
        return std::string();
    }
    std::string FileDialogs::SaveFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetSaveFileNameA(&ofn) == TRUE)								
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }
}
