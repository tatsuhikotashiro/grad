#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <string>

int main(int argc, char* args[]){
    std::string customersFileName = args[1];
    std::ifstream ifs(customersFileName, std::ios::in);
    std::string line;
    std::vector<std::vector<int>> customersData;
    std::vector<int> cus(3);

    std::string a, b, c;
    while (getline(ifs, line))
    {
        std::stringstream ss;
        ss << line;
        ss >> a >> b >> c;
        cus.at(0) = std::__cxx11::stoi(a); // 到着
        cus.at(1) = std::__cxx11::stoi(b); // 滞在
        cus.at(2) = std::__cxx11::stoi(c); // 人数
        customersData.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();

    std::ofstream ofs("customerDataSplit2.txt");
    if (!ofs.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    // 人数の回数分だけ到着，滞在，1を書き込めばいい
    for(int i = 0;i < (int)customersData.size();i++){
        for(int j = 0;j < customersData.at(i).at(2);j++){
        ofs << customersData.at(i).at(0) << ' ' << customersData.at(i).at(1) << ' ' << 1 << std::endl;
        }
    }
}