#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

int count = 0;

vector <vector <int>> matrix = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 9}, {1, 1, 1, 9, 9}, {1, 1, 9, 9, 9}, {1, 9, 9, 9, 9}};

class Count{
  std::mutex mutex;
  public:
    void Counter(int n, int m){
      std::lock_guard<std::mutex> _(mutex);
      for (int i = 0; i < 5; i++) if (matrix.at(n).at(i) == m) count++;
      cout << "We found " << count << " elements with value " << m << " in the row " << n << endl;
      count = 0;
    }
};

int main() {
  Count example;
  std::vector<std::thread> threads;
  for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(&Count::Counter, &example, i, 1));
   }
   for(int i = 0; i < threads.size() ; i++)
   {
       threads.at(i).join();
   }
}
