#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <numeric>
#include <exception>

#ifdef TBB_FOUND
  #include <tbb/parallel_for.h>
  #include <tbb/blocked_range.h>
  #include <tbb/global_control.h>
#endif

int main() {
    try {
        std::cout << "=== Parallelization Library Verification ===\n";

        // Test std::thread
        std::cout << "[std::thread] Launching 4 threads...\n";
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([i] {
                std::cout << "  Thread " << i << " is working\n";
            });
        }
        for (auto& t : threads) t.join();
        std::cout << "[std::thread] OK\n";

        // Test std::async
        std::cout << "[std::async] Launching 2 tasks...\n";
        auto f1 = std::async(std::launch::async, [] { return 21; });
        auto f2 = std::async(std::launch::async, [] { return 21; });
        if ((f1.get() + f2.get()) != 42)
            throw std::runtime_error("std::async failed");
        std::cout << "[std::async] OK\n";

#ifdef TBB_FOUND
        // Test oneTBB
        std::cout << "[oneTBB] Parallel for over vector...\n";
        const int size = 1000;
        std::vector<int> data(size, 1);
        std::atomic<int> total = 0;

        tbb::parallel_for(
            tbb::blocked_range<int>(0, size),
            [&](tbb::blocked_range<int> r) {
                int local_sum = 0;
                for (int i = r.begin(); i < r.end(); ++i)
                    local_sum += data[i];
                total += local_sum;
            }
        );

        if (total != size)
            throw std::runtime_error("oneTBB parallel sum failed");
        std::cout << "[oneTBB] OK\n";
#else
        std::cout << "[oneTBB] Not available.\n";
#endif

        std::cout << "=== All available parallel libraries verified successfully ===\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
