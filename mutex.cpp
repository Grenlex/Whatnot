#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::once_flag flag;
std::vector<int> attendance;
std::timed_mutex mutex;
std::mutex in_out_mutex;
auto time_for_answer = std::chrono::seconds(10); //10 секунд отводится на то, чтобы ответили все ученики, а не на каждого в отдельности

void check_attendance(int i){
  int answer;
  bool state;
  
  auto now=std::chrono::steady_clock::now();
  state = mutex.try_lock_until(now + time_for_answer);
  if (state){
  {
  std::lock_guard<std::mutex> _(in_out_mutex);
  std::cout << "Teacher: Student " << i << ", are you here? (input any number)" << std::endl;
  std::cin >> answer;
  std::cout << "Student: I'm here!" << std::endl;
  if (state && (std::chrono::steady_clock::now() - now) < time_for_answer){
    std::cout << "Teacher: Okay!" << std::endl;
    std::call_once(flag, [](){std::cout << "Then you should clear whiteboard!" << std::endl;});
    std::cout << std::endl;
    mutex.unlock();
  }
  else std::cout << "Teacher: no, time is over, you're absent" << std::endl;
  std::cout << std::endl;
  }
  }
  else{
    std::lock_guard<std::mutex> _(in_out_mutex);
    std::cout << "Teacher: time is over, student " << i << " is absent" << std::endl;
  }
}

int main(){
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(check_attendance, i);
    }
 
    for (auto& i: threads) {
        i.join();
    }
    return 0;
}
