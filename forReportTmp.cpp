#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<int> generateHourWaiting(double lambda)
{
    // 平均到着率 (人/時) = lambda

    // シミュレーション時間 (時)
    double simulation_time = 1.0;

    // 乱数生成器
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // 到着時刻を格納するリスト
    std::vector<double> arrival_times;

    // 最初の到着時刻を生成
    double current_time = -log(1 - dist(engine)) / lambda;

    // シミュレーション時間まで到着時刻を生成
    while (current_time <= simulation_time)
    {
        arrival_times.push_back(current_time);
        current_time += -log(1 - dist(engine)) / lambda;
    }

    // 到着時刻のリストを出力
    //   cout << "到着時刻リスト (時間):" << endl;
    for (double arrival_time : arrival_times)
    {
        cout << (int)(arrival_time * 60) << endl;
    }
}

std::vector<int> divideNumberFromRangeAB(int n, int a, int b)
{
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<> dist(a, b);

    std::vector<int> result;
    while (n > 0)
    {
        int i = dist(gen);
        if (i > n)
            i = n;
        result.push_back(i);
        n -= i;
    }

    return result;
}

// 滞在時刻と席ごとにあとどれだけ顧客が座っているのかを受け取って空いている席に割り当てる
// 入力: 待っている顧客の滞在時間, 各座席の顧客の残り滞在時間
bool assignCustomersEasyWay(int stayTime, int group, std::vector<int> &customerSeatsStayTime, std::vector<int> &cap)
{
    for (int i = 0; i < customerSeatsStayTime.size(); i++)
    {
        if (customerSeatsStayTime.at(i) == 0 && cap.at(i) >= group)
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
    std::vector<int> cap = {1, 2, 2, 2, 2, 2, 2, 4, 4, 4, 6, 6}; // 客数
    int waitingCustomerNumber = 0;
    std::string tmp; // 文字列の入力に一時的に使う
    int num;
    int sum = 0;
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3); // 文字列の入力に一時的に使う
    std::string a, b, c, d;  // 文字列の入力に一時的に使う

    // シミュレーション終了時刻（分）
    // 11時から24時までだったら13時間のためそれを分に直して記述する
    int endTime = 60 * 13;
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

    // getline()で1行ずつ読み込む
    while (getline(ifs, tmp))
    {
        std::stringstream ss;
        ss << tmp;
        ss >> a >> b >> c;
        cus.at(0) = std::__cxx11::stoi(a);// 到着
        cus.at(1) = std::__cxx11::stoi(b);// 滞在
        cus.at(2) = std::__cxx11::stoi(c);// 人数
        data.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();

    // シミュレーションの現在時刻が終了時刻よりも小さければ時刻を1ずつ進めながら繰り返す
    while (timeMinutes < endTime)
    {
        // 顧客リストの先頭の顧客の到着時刻が現在の時刻以前だった場合、すでに到着しているとみなして顧客の割当てを行う
        while (waitingCustomerNumber >= 0 && data.at(waitingCustomerNumber).at(1) <= timeMinutes)
        {
            // 客の滞在時間をcustomerSeatsStayTimeに入れる
            if (assignCustomersEasyWay(data.at(waitingCustomerNumber).at(1), data.at(waitingCustomerNumber).at(2), customerSeatsStayTime, cap))
            {
                // 現在時刻と到着時刻の差が待ち時間になるのでそれを後ろに追加する
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(0));
                waitingCustomerNumber++;// 待ち顧客の先頭を次に進める
                if (waitingCustomerNumber >= data.size())// 最後まで行ったらマイナスにして顧客を案内しきったことを示す
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