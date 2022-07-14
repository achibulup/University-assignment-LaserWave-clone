#include <chrono>
#include <string_view>
#include <fstream>

extern std::ofstream log_file;

class Benchmarker
{
  public:
    Benchmarker(std::string_view name) : m_name(name)
    {
        m_start = std::chrono::high_resolution_clock::now();
    }
    ~Benchmarker()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
        ::log_file << m_name << " " << duration.count() << "us" << std::endl;
    }

  private:
    std::string_view m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

template<typename Procedure>
auto benchmark(std::string_view name, Procedure &&procedure)
-> decltype(std::forward<Procedure>(procedure)())
{
    Benchmarker bench(name);
    return std::forward<Procedure>(procedure)();
}

#ifdef DEBUG
#define WHEN_DEBUG(...) __VA_ARGS__
#define BENCHMARK_IF_DEBUG(...) benchmark(#__VA_ARGS__, [&](){ return (__VA_ARGS__); })
#define LOG_IF_DEBUG(...) ((::log_file << #__VA_ARGS__ << std::endl), (__VA_ARGS__))
#else
#define WHEN_DEBUG(...)
#define BENCHMARK_IF_DEBUG(...) (__VA_ARGS__)
#define LOG_IF_DEBUG(...) (__VA_ARGS__)
#endif // DEBUG