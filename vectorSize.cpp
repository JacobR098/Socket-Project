#include <iostream>
#include <vector>

using namespace std;
int main(){
  vector<int> vec;
  cout << "Size of vec before initialization: " << sizeof(vec) << endl;
  vec.push_back(5);
  cout << "Size of vec after intialization: "  << sizeof(vec) << endl;
  return 0;
}
