#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>

int createWaitingTime()
{

    // 乱数生成器を初期化
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    static std::uniform_int_distribution<> dist(1, 100);

    // 1〜100の乱数を生成
    int random_value = dist(engine);

    // 80%の確率で15〜20の値を生成
    if (random_value <= 80)
    {
        std::uniform_int_distribution<> dist_15_20(15, 20);
        return dist_15_20(engine);
    }
    else
    {
        // それ以外の値は5〜30の間で等確率に生成
        std::uniform_int_distribution<> dist_5_30(5, 30);
        int value = dist_5_30(engine);

        // 15〜20の値は既に生成済みなので除外
        while (value >= 15 && value <= 20)
        {
            value = dist_5_30(engine);
        }
        return value;
    }
}

int main(int argc, char *argv[])
{
    using namespace std;

    // コマンドライン引数のチェック
    if (argc != 2)
    {
        cerr << "引数の数が間違っています．" << endl;
        cerr << "./getline customers.txt" << endl;
        return 1;
    }

    // C++のファイル入力のための準備
    ifstream ifs(argv[1], ios::in);
    // ファイルを開くのに失敗したときの処理
    if (!ifs)
    {
        cerr << "Error: file not opened." << endl;
        return 1;
    }

    string tmp;
    int num;
    int sum = 0;
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3);

    // getline()で1行ずつ読み込む
    while (getline(ifs, tmp))
    {
        // cout << tmp << endl; // そのまま出力
        stringstream ss;
        ss << tmp;
        string a, b, c;
        ss >> a >> b >> c;
        cus.at(0) = stoi(a);
        cus.at(1) = stoi(b);
        cus.at(2) = stoi(c);
        // cout << "---------\n";
        data.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();

    

    // 出力ファイルを開く
    std::ofstream output_file("output.txt");
    if (!output_file.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    int cusNum = 25;
    // 乱数生成器を初期化
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    static std::uniform_int_distribution<> dist(0, 59);
    for(int i=0;i < cusNum;i++){
        
    }

    
        output_file << line << std::endl;

    // ファイルを閉じる
    input_file.close();
    output_file.close();

    return 0;
}
