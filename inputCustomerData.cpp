#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]){
    using namespace std;

    // コマンドライン引数のチェック
    if(argc != 2){
        cerr << "引数の数が間違っています．" << endl;
        cerr << "./getline customers.txt" << endl;
        return 1;
    }

    // C++のファイル入力のための準備
    ifstream ifs(argv[1], ios::in);
    // ファイルを開くのに失敗したときの処理
    if(!ifs){
        cerr << "Error: file not opened." << endl;
        return 1;
    }

    string tmp;
    int num;
    int sum = 0;
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3);
    
    // getline()で1行ずつ読み込む
    while(getline(ifs, tmp)){
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

    // 配列の中身チェック
    // for(int i=0;i<data.size();i++){
    //     for( int a: data.at(i)){
    //         cout << a << " ";
    //     }
    //     cout << "\n";
    // }
    
    return 0;
}
