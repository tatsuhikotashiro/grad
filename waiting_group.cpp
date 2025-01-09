// 待ち時間が発生している顧客のグループの人数を出力するプログラム

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    // ファイル名
    string waitingTimeFile = argv[1];
    string customerFile = argv[2];

    // ファイルを開く
    ifstream ifs(waitingTimeFile);

    // ファイルを開けなかった場合のエラー処理
    if (!ifs)
    {
        cerr << "ファイルを開けませんでした: " << waitingTimeFile << endl;
        return 1;
    }

    // 整数を格納するベクトル
    vector<int> waitingtimev;
    int num;
    string line;

    // ファイルから整数を読み込む
    while (getline(ifs, line))
    {
        waitingtimev.push_back(stoi(line));
    }
    // ファイルを閉じる
    ifs.close();

// ファイルを開く
    ifstream ifs2(customerFile);

    // ファイルを開けなかった場合のエラー処理
    if (!ifs2)
    {
        cerr << "ファイルを開けませんでした: " << customerFile << endl;
        return 1;
    }

    // ファイルから整数を読み込む
    std::vector<std::vector<int>> customers;
    std::vector<int> cus(3);
    
    // getline()で1行ずつ読み込む
    while(getline(ifs2, line)){
        // cout << tmp << endl; // そのまま出力
        stringstream ss;
        ss << line; 
        string a, b, c;
        ss >> a >> b >> c;
        cus.at(0) = stoi(a);
        cus.at(1) = stoi(b);
        cus.at(2) = stoi(c);
        customers.push_back(cus);
    }
    ifs2.close();

    string outputfilename = "groupnums.txt";
    ofstream ofs(outputfilename);
    for (size_t i = 0; i < waitingtimev.size(); ++i)
    {
        // std::cout << "test" << endl;
        if(waitingtimev.at(i)>0){
        // std::cout << i << endl;

            ofs << customers.at(i).at(2) << endl;
        }
    }

    return 0;
}