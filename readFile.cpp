#include "record.h"
#include <fstream>

#define FILE_NAME "StatsCountry.csv"

using namespace std;


enum State {
  Free, Leader, InDHT
};

int main(int argc, char* argv[]){
  ifstream in;
  in.open(FILE_NAME);
  int numLines = 0;
  string counterString;
  if(in){ //File opened successfully
    getline(in, counterString);
    while(getline(in, counterString)){
      numLines++;
    }

    in.close();
    //    return 0;
    cout << "File read. " << numLines << " lines of data (not including header.)" << endl;
    
  }
  
  record* array = new record[241];
  //cout << "size of array " << sizeof(array) * 241 << endl;
  //in.clear(); //Clear EOF flag
  //in.seekg(ios::beg);
  in.open(FILE_NAME);
  getline(in, counterString);
  for(int i = 0; i < numLines; i++){
    array[i] = record(in);
  } 
  for(int i = 0; i < numLines; i++){
    cout << "Printing index: " << i << "\n";
    array[i].print();
    cout << "\n";
  }
  return 0;
}

