#pragma once

#include<string>
#include<chrono>
#include<algorithm>
#include<fstream>

#include<thread>

namespace SoLin {

    //@brief 时间剖面结果数据
    struct ProfileResult {
        std::string Name;           //名称
        long long Start, End;       //时间终始点
        uint32_t ThreadID;          
    };

    struct InstrumentationSession {
        std::string Name;
    };

    //@brief 性能剖析器
    class Instrumentor {
    private:
        InstrumentationSession* m_CurrentSession;   // 当前执行期指针
        std::ofstream m_OutputStream;               // 信息输出流
        int m_ProfileCount;                         // 计数
    public:
        Instrumentor()
            :m_CurrentSession(nullptr),m_ProfileCount(0)
        {}

        // @brief 开始分析
        // @param name 阶段名
        // @param filepath 存储路径
        void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession{ name };
        }

        // @brief 结束分析
        void EndSession() {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result) {
            if (m_ProfileCount++ > 0) {
                m_OutputStream << ",";
            }

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        void WriteHeader() {
            m_OutputStream << "{\"otherData\":{},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter() {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        static Instrumentor& Get() {
            static Instrumentor instance;
            return instance;
        }
    };

    //----------------------------Timer---------------------------------
    class InstrumentationTimer {
    public:
        InstrumentationTimer(const char* name)
            :m_Name(name),m_Stopped(false) {
            m_StartTimePoint = std::chrono::high_resolution_clock::now();
        }
        ~InstrumentationTimer() {
            if (!m_Stopped) {
                Stop();
            }
        }
        void Stop() {
            auto endTimePoint = std::chrono::high_resolution_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name,start,end,threadID });

            m_Stopped = true;
        }

    private:
        bool m_Stopped;
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
    };
    //---------------------------------------------------------------------------------
}

#define SL_PROFILE 1
#if SL_PROFILE
    #define SL_PROFILE_BEGIN_SESSION(name,filepath) ::SoLin::Instrumentor::Get().BeginSession(name,filepath)
    #define SL_PROFILE_END_SESSION() ::SoLin::Instrumentor::Get().EndSession()
    #define SL_PROFILE_SCOPE(name) ::SoLin::InstrumentationTimer timer##__LINE__(name)
    //__FUNCSIG__ 获取当前函数的完整签名信息
    #define SL_PROFILE_FUNCTION() SL_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define SL_PROFILE_BEGIN_SESSION(name,filepath)
    #define SL_PROFILE_END_SESSION()
    #define SL_PROFILE_SCOPE(name)
    #define SL_PROFILE_FUNCTION() 
#endif
