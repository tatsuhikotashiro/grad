#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

int main(int argc, char* argv[])
{
    // C++のファイル入力のための準備
    std::ifstream ifs(argv[1], std::ios::in);
    std::vector<int> arrive;
    std::vector<int> service;
    std::vector<int> people;
    std::string listr;
    std::string a, b, c;
    // ファイルを開くのに失敗したときの処理
    if (!ifs)
    {
        std::cerr << "Error: file not opened." << std::endl;
        return 1;
    }

    // getline()で1行ずつ読み込む
    while (getline(ifs, listr))
    {
        std::stringstream ss;
        ss << listr;
        ss >> a >> b >> c;
        arrive.push_back(std::__cxx11::stoi(a));  // 到着
        service.push_back(std::__cxx11::stoi(b)); // 到着
        people.push_back(std::__cxx11::stoi(c));  // 到着
    }
    // ファイルを閉じる
    ifs.close();
    int cnt = 0;
    for (int tmp : people)
    {
        cnt += tmp;
    }
    std::cout << cnt << std::endl;
    std::sort(people.begin(), people.end());

    // 出力ファイルを開く
    std::ofstream outputFile("sortdata.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    for (int i = 0; i < (int)arrive.size(); i++)
    {
        outputFile << arrive.at(i) << ' ' << service.at(i) << ' ' << people.at(i) << std::endl;
    }
    // ファイルを閉じる
    outputFile.close();
}
