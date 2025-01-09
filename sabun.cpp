#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    // ファイル名
    string filename = argv[1];

    // ファイルを開く
    ifstream ifs(filename);

    // ファイルを開けなかった場合のエラー処理
    if (!ifs)
    {
        cerr << "ファイルを開けませんでした: " << filename << endl;
        return 1;
    }

    // 整数を格納するベクトル
    vector<int> numbers;
    int num;
    string line;

    // ファイルから整数を読み込む
    while (getline(ifs, line))
    {
        numbers.push_back(stoi(line));
    }
    string outputfilename = "difference.txt";
    ofstream ofs(outputfilename);
    // 差を計算してファイルに出力
    for (size_t i = 1; i < numbers.size(); ++i)
    {
        int diff = numbers[i] - numbers[i - 1];
        ofs << diff << endl;
    }

    return 0;
}