#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <condition_variable>
#include <queue> 

class Config {
public:
    int numThreads;
    int maxNumber;
    
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Could not open config file. Using defaults: threads=4, max=65536\n";
            numThreads = 4; // Updated default value
            maxNumber = 65536; // Updated default value
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("threads=") == 0) {
                try {
                    numThreads = std::stoi(line.substr(8));
                } catch (const std::exception& e) {
                    numThreads = 255;
                }
            } else if (line.find("max_number=") == 0) {
                try {
                    maxNumber = std::stoi(line.substr(11));
                } catch (const std::exception& e) {
                    maxNumber = 65536;
                }
            }
        }
        return true;
    }
};

class PrimeFinder {
private:
    std::mutex printMutex;
    
public:
    bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }
    
    void variant1RangeThread(int start, int end, int threadId) {
        for (int i = start; i <= end; i++) {
            if (isPrime(i)) {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "[Thread " << threadId << "] [" 
                         << getCurrentTimestamp() << "] Prime found: " << i << std::endl;
            }
        }
    }
    
    void runVariant1(int numThreads, int maxNumber) {
        std::cout << "\n=== VARIANT 1: Range Division + Immediate Print ===\n";
        std::cout << "Start Time: " << getCurrentTimestamp() << std::endl;
        
        std::vector<std::thread> threads;
        int range = maxNumber / numThreads;
        
        for (int i = 0; i < numThreads; i++) {
            int start = (i * range) + (i == 0 ? 1 : 0);
            int end = (i == numThreads - 1) ? maxNumber : (i + 1) * range;
            
            threads.emplace_back(&PrimeFinder::variant1RangeThread, this, start, end, i);
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        std::cout << "End Time: " << getCurrentTimestamp() << std::endl;
    }
};

int main() {
    Config config;
    config.loadFromFile("config.txt");
    
    std::cout << "Configuration: " << config.numThreads << " threads, searching up to " 
              << config.maxNumber << std::endl;
    
    PrimeFinder finder;
    finder.runVariant1(config.numThreads, config.maxNumber);
    
    return 0;
}
