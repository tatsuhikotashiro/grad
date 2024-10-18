#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// 滞在時刻と席ごとにあとどれだけ顧客が座っているのかを受け取って空いている席に割り当てる
bool assignCustomersEasyWay(int stayTime, std::vector<int> &customerSeatsStayTime)
{
    for (int i = 0; i < customerSeatsStayTime.size(); i++)
    {
        if (customerSeatsStayTime.at(i) == 0)
        {
            customerSeatsStayTime.at(i) = stayTime;
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    std::vector<int> customerSeatsStayTime(12); // 客席で0番から11番まで計12席で客の残り滞在時間
    int waitingCustomerNumber = 0;
    std::string tmp; // 入力に使う
    int num;
    int sum = 0;
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3);
    std::string a, b, c, d;

    // シミュレーション終了時刻（分）
    int endTime = 60;
    // シミュレーション中に使用する時刻を保存する変数（分）
    int timeMinutes = 0; // シミュレーション開始時刻は11時とする

    // コマンドライン引数のチェック
    if (argc != 2)
    {
        std::cerr << "引数の数が間違っています．" << std::endl;
        std::cerr << "./getline customers.txt" << std::endl;
        return 1;
    }

    // C++のファイル入力のための準備
    std::ifstream ifs(argv[1], std::ios::in);
    // ファイルを開くのに失敗したときの処理
    if (!ifs)
    {
        std::cerr << "Error: file not opened." << std::endl;
        return 1;
    }

    // // getline()で1行ずつ読み込む
    // while (getline(ifs, tmp))
    // {
    //     std::stringstream ss;
    //     ss << tmp;
    //     ss >> a >> b >> c;
    //     cus.at(0) = std::__cxx11::stoi(a);
    //     cus.at(1) = std::__cxx11::stoi(b);
    //     cus.at(2) = std::__cxx11::stoi(c);
    //     data.push_back(cus);
    // }

    // getline()で1行ずつ読み込む(4要素ver)    
    while (getline(ifs, tmp))
    {
        std::stringstream ss;
        ss << tmp;
        ss >> a >> b >> c >> d;
        cus.at(0) = std::__cxx11::stoi(a);
        cus.at(1) = std::__cxx11::stoi(b);
        cus.at(2) = std::__cxx11::stoi(c);
        cus.at(2) = std::__cxx11::stoi(d);
        data.push_back(cus);
    }
    ファイルを閉じる
    ifs.close();

    // シミュレーションの現在時刻が終了時刻よりも小さければ時刻を1ずつ進めながら繰り返す
    while (timeMinutes < endTime)
    {
        // 顧客リストの先頭の顧客の到着時刻が現在の時刻以前だった場合、すでに到着しているとみなして顧客の割当てを行う
        while (waitingCustomerNumber >= 0 && data.at(waitingCustomerNumber).at(1) <= timeMinutes)
        {
            // 客の滞在時間をcustomerSeatsStayTimeに入れる
            if (assignCustomersEasyWay(data.at(waitingCustomerNumber).at(2), customerSeatsStayTime))
            {
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(1));
                waitingCustomerNumber++;
                if (waitingCustomerNumber >= data.size())
                {
                    waitingCustomerNumber = -1;
                }
            }
            else
            {
                break;
            }
        }
        // 全体の時間を一秒進める
        timeMinutes++;
        // 各座席の残り滞在時間を一秒減らす
        for (auto &&stayTime : customerSeatsStayTime)
        {
            if (stayTime > 0)
            {
                stayTime--;
            }
        }
        // 席の待ち時間の状況テスト表示コード
        std::cout << "time=" << timeMinutes << ":" << "待ち客=" << waitingCustomerNumber << ":";
        for (int i = 0; i < customerSeatsStayTime.size(); i++)
        {
            std::cout << "(" << i << ")" << customerSeatsStayTime.at(i);
            (customerSeatsStayTime.at(i) > 9) ? std::cout << " " : std::cout << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "待ち時間以下の通り" << std::endl;
    for (int i = 0; i < data.size(); i++)
    {
        std::cout << "(" << i << ")" << data.at(i).at(3);
    }
    std::cout << std::endl;
}