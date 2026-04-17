#pragma once
#include<string>
namespace SoLin {
    // @brief 文件对话框
    class FileDialogs {
    public:
        // @brief 打开文件
        // @param filter 过滤器
        static std::string OpenFile(const char* filter);
        // @brief 保存文件
        // @param filter 过滤器
        static std::string SaveFile(const char* filter);
    };
}
