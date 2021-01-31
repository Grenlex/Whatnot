#include <iostream>
#include <vector>
#include <thread>

using namespace std;

int count = 0;

vector <vector <int>> matrix = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 9}, {1, 1, 1, 9, 9}, {1, 1, 9, 9, 9}, {1, 9, 9, 9, 9}};

void Counter(int n, int m){
for (int i = 0; i < 5; i++) if (matrix.at(n).at(i) == m) count++;
}

int main() {
  Counter(0, 1);
  std::thread threadObj1(Counter, 1, 1);
  std::thread threadObj2(Counter, 2, 1);
  std::thread threadObj3(Counter, 3, 1);
  std::thread threadObj4(Counter, 4, 1);
  threadObj1.join();
  threadObj2.join();
  threadObj3.join();
  threadObj4.join();
  cout << "We found " << count << " such elements" << endl;
}
