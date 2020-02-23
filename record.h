#include <iostream>
struct record {
  std::string Country_Code, Short_Name, Table_Name, Long_Name, Alpha_Code, Currency, Region, WB_Code, Latest_Census;
    record(std::istream& stream){
     
    getline(stream, Country_Code, ',');
    getline(stream, Short_Name, ',');
    getline(stream, Table_Name, ',');
    getline(stream, Long_Name, ',');
    getline(stream, Alpha_Code, ',');
    getline(stream, Currency, ',');
    getline(stream, Region, ',');
    getline(stream, WB_Code , ',');
    getline(stream, Latest_Census, '\n');
  }
  record(){
    Country_Code = "";
    Short_Name = "";
    Table_Name = "";
    Long_Name = "";
    Alpha_Code = "";
    Currency = "";
    Region = "";
    WB_Code = "";
    Latest_Census = "";
  }
  void print(){
    std:: cout << Country_Code << ",";
    std:: cout << Short_Name << ",";
    std:: cout << Table_Name << ",";
    std:: cout << Long_Name << ",";
    std:: cout << Alpha_Code << ",";
    std:: cout << Currency << ",";
    std:: cout << Region << ",";
    std:: cout << WB_Code << ",";
    std:: cout << Latest_Census << "\n";

  }
};
