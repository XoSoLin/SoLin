#pragma once

#include<string>
#include<chrono>
#include<algorithm>
#include<fstream>
#include<iomanip>

#include<thread>

namespace SoLin {

    //@brief 时间剖面结果数据
    struct ProfileResult {
        std::string Name;                                       //名称
        std::chrono::duration<double,std::micro> Start;         //时间起始点
        std::chrono::microseconds ElapsedTime;                  //已过去时间
        std::thread::id ThreadID;                               //线程id
    };

    struct InstrumentationSession {
        std::string Name;
    };

    //@brief 性能剖析器
    class Instrumentor {
    private:
        std::mutex m_Mutex;                         // 加入互斥锁

        InstrumentationSession* m_CurrentSession;   // 当前执行期指针
        std::ofstream m_OutputStream;               // 信息输出流
    public:
        Instrumentor()
            :m_CurrentSession(nullptr)
        {}

        // @brief 开始分析
        // @param name 阶段名
        // @param filepath 存储路径
        void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                // 如果当前会话已经存在，则在开始新会话之前将其关闭。
                // 针对原始会话的后续分析输出将最终出现在新打开的会话中。
                // 这比格式错误的分析输出要好。
                if (Log::GetCoreLogger()) {
                    SL_CORE_ERROR("Instrumentor::BeginSession('{0}') is creating a new session when session '{1}' already open.", name, m_CurrentSession->Name);
                }
                InternalEndSession();
            }
            m_OutputStream.open(filepath);
            if (m_OutputStream.is_open()) {
                // InstrumentationSession 首先创建互斥锁，然后 WriteHeader 通过 std::lock_guard 管理锁
                m_CurrentSession = new InstrumentationSession({ name });
                WriteHeader();
            }
            else {
                // Edge case : BeginSession() might be before Log::Init() !!!
                if (Log::GetCoreLogger()) {
                    SL_CORE_ERROR("Instrumentor can't open result file '{0}'.", filepath);
                }
            }
        }

        // @brief 结束分析
        void EndSession() {
            // 调用InternalEndSession时，必须确保互斥锁已经被创建
            std::lock_guard lock(m_Mutex);
            InternalEndSession();
        }
    private:
        void InternalEndSession() {
            if (m_CurrentSession) {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }

        void WriteHeader() {
            m_OutputStream << "{\"otherData\":{},\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void WriteFooter() {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

    public:
        void WriteProfile(const ProfileResult& result) {
            std::stringstream json;

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            // 设置json 字符串流中浮点数的输出都是小数点后保留三位，且切小数点固定的格式
            json << std::setprecision(3) << std::fixed;
            // 默认添加一个','，故不用判断是否需要','。由于Header中多写了一个{}，故初始化时第一个','并不多余。
            json << ",{";                                      
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << result.ElapsedTime.count() << ',';
            json << "\"name\":\"" << name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start.count();
            json << "}";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }
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
            // steady_clock 代替 high_resolution_clock
            // 单调时钟永远不会向后调整，高精度时钟可能受系统时调整影响出现负数
            m_StartTimePoint = std::chrono::steady_clock::now();
        }
        ~InstrumentationTimer() {
            if (!m_Stopped) {
                Stop();
            }
        }
        void Stop() {
            auto endTimePoint = std::chrono::steady_clock::now();
            auto highResStart = std::chrono::duration<double, std::micro>{ m_StartTimePoint.time_since_epoch() };
            auto elapsedTime =
                std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch();

            Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

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

    //根据不同机器上的编辑器及其版本确定对应的合适的获取方式，
    //将其定义为 SL_FUNC_SIG，自动获取函数签名
    // 请注意，这只有在预编译器启动时才会解析，
    // 因此语法高亮可能会在您的编辑器中标记错误的一个！

    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define SL_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define SL_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
    #define SL_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define SL_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define SL_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define SL_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define SL_FUNC_SIG __func__
    #else
    #define SL_FUNC_SIG "SL_FUNC_SIG unknown!"
    #endif
    
    #define SL_PROFILE_BEGIN_SESSION(name,filepath) ::SoLin::Instrumentor::Get().BeginSession(name,filepath)
    #define SL_PROFILE_END_SESSION() ::SoLin::Instrumentor::Get().EndSession()
    #define SL_PROFILE_SCOPE(name) ::SoLin::InstrumentationTimer timer##__LINE__(name)
    #define SL_PROFILE_FUNCTION() SL_PROFILE_SCOPE(SL_FUNC_SIG)
#else
    #define SL_PROFILE_BEGIN_SESSION(name,filepath)
    #define SL_PROFILE_END_SESSION()
    #define SL_PROFILE_SCOPE(name)
    #define SL_PROFILE_FUNCTION() 
#endif
