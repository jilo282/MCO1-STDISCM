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
            numThreads = 4;
            maxNumber = 65536;
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
    
    // Variant 4 Thread Function
    void variant4IndividualThread(std::queue<int>& numbers, std::mutex& queueMutex, 
                                std::condition_variable& cv, bool& finished, 
                                std::vector<int>& localPrimes, int threadId) {
        while (true) {
            int num;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [&] { return !numbers.empty() || finished; });
                
                if (numbers.empty() && finished) break;
                if (numbers.empty()) continue;
                
                num = numbers.front();
                numbers.pop();
                
                // CRITICAL FOR COMPLETION: If this worker emptied the queue, notify the main thread.
                if (numbers.empty()) {
                    cv.notify_all(); 
                }
            }
            
            if (isPrime(num)) {
                localPrimes.push_back(num);
            }
        }
    }
    
    // Variant 4 Run Function
    void runVariant4(int numThreads, int maxNumber) {
        std::cout << "\n=== VARIANT 4: Individual Number Testing + Collect and Print ===\n";
        std::cout << "Start Time: " << getCurrentTimestamp() << std::endl;
        
        std::queue<int> numbers;
        std::mutex queueMutex;
        std::condition_variable cv;
        bool finished = false;
        std::vector<std::vector<int>> threadResults(numThreads);
        
        for (int i = 1; i <= maxNumber; i++) {
            numbers.push(i);
        }
        
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back(&PrimeFinder::variant4IndividualThread, this,
                               std::ref(numbers), std::ref(queueMutex), 
                               std::ref(cv), std::ref(finished), 
                               std::ref(threadResults[i]), i);
        }
        
        cv.notify_all();
        
        // ** ROBUST COMPLETION LOGIC **
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Wait for a worker thread to signal that numbers.empty() is true.
            cv.wait(lock, [&] { return numbers.empty(); });
            finished = true; // Queue is empty, now set flag
        } 
        
        // This final notify_all() is crucial to wake up all workers so they can check the 'finished' flag and exit.
        cv.notify_all();
        
        for (auto& t : threads) {
            t.join();
        }
        
        // Print all collected results
        for (int i = 0; i < numThreads; i++) {
            for (int prime : threadResults[i]) {
                std::cout << "[Thread " << i << "] Prime found: " << prime << std::endl;
            }
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
    finder.runVariant4(config.numThreads, config.maxNumber);
    
    return 0;
}