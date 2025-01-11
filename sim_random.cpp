#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <string>

// 滞在時刻と席ごとにあとどれだけ顧客が座っているのかを受け取って空いている席に割り当てる
// 入力: 待っている顧客の滞在時間, 各座席の顧客の残り滞在時間
bool fifo(int stayTime, int group, std::vector<int> &customerSeatsStayTime, std::vector<int> &cap)
{
    for (int i = 0; i < (int)customerSeatsStayTime.size(); i++)
    {
        if (customerSeatsStayTime.at(i) == 0 && cap.at(i) >= group)
        {
            customerSeatsStayTime.at(i) = stayTime;
            return true;
        }
    }
    return false;
}

bool siro(int stayTime, int group, std::vector<int> &customerSeatsStayTime, std::vector<int> &cap)
{
    std::vector<int> available; // 割当て可能な添え字はtrue
    for (int i = 0; i < (int)customerSeatsStayTime.size(); i++)
    {
        if (customerSeatsStayTime.at(i) == 0 && cap.at(i) >= group)
        {
            available.push_back(i);
        }
        // 乱数生成器を初期化
        if(!available.empty()){
            std::random_device rd;
            std::mt19937 gengen(rd());
            std::uniform_int_distribution<> distrib(0, available.size() - 1);

            // ランダムなインデックスを生成
            int randomIndex = distrib(gengen);

            // ランダムに選択された要素を出力
            customerSeatsStayTime.at(randomIndex) = stayTime;
            return true;
        }
        
    }
    return false;
}
void minusTime(std::vector<int> &SeatsStay)
{
    for (auto &&stayTime : SeatsStay)
    {
        if (stayTime > 0)
        {
            stayTime--;
        }
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> SeatsStay1(12);
    // for (int i : SeatsStay1){
    //     std::cout << i << std::endl;
    // }
    std::vector<int> SeatsStay2(11);                             // 2人隣接してやるとき
    std::vector<int> SeatsStay3(10);                             // 3人隣接してやるとき
    std::vector<int> SeatsStay4(9);                              // 3人隣接してやるとき
    std::vector<int> cap = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // 客数
    std::vector<int> cap2 = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};   // cap[0],cap[1]空いてる→cap2[0]空いてる，cap[1],cap[2]空いてる→cap2[1]空いてる，cap[i],cap[i+1]空いてる→cap2[i]空いてる
    std::vector<int> cap3 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};      // cap[0],cap[1],cap[2]空いてる→cap3[0]空いてる，cap[1],cap[2], cap[3]空いてる→cap2[1]空いてる，cap[i],cap[i+1]空いてる→cap2[i]空いてる
    std::vector<int> cap4 = {4, 4, 4, 4, 4, 4, 4, 4, 4};         // cap[i],cap[i+1], cap[i+2], cap[i+3]空いてる→cap4[i]空いてる

    // SeatsStay2[i] = max(SeatsStay[i], SeatsStay[i+1])
    // SeatsStay3[i] = max(SeatsStay[i], SeatsStay[i+1], SeatsStay[i+2])
    // SeatsStay4[i] = max(max(SeatsStay[i], SeatsStay[i+1]), std::max(SeatsStay[i+2],SeatsStay[i+3]))
    int waitingCustomerNumber = 0; // 初期値を0に変更
    std::string tmp;               // 文字列の入力に一時的に使う
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3); // 文字列の入力に一時的に使う
    std::string a, b, c, d;  // 文字列の入力に一時的に使う

    // シミュレーション終了時刻（分）
    // 11時から24時までだったら13時間のためそれを分に直して記述する
    int endTime = 1600;
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
        cus.at(0) = std::__cxx11::stoi(a); // 到着
        cus.at(1) = std::__cxx11::stoi(b); // 滞在
        cus.at(2) = std::__cxx11::stoi(c); // 人数
        data.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();
    // std::cout << data.at(0).size() << std::endl;

    // シミュレーションの現在時刻が終了時刻よりも小さければ時刻を1ずつ進めながら繰り返す
    while (timeMinutes < endTime)
    {
        // 顧客リストの先頭の顧客の到着時刻が現在の時刻以前だった場合、すでに到着しているとみなして顧客の割当てを行う
        // std::cout << waitingCustomerNumber << ":" << data.at(waitingCustomerNumber).at(0) << ":" << timeMinutes << std::endl;
        while (waitingCustomerNumber < (int)data.size() && data.at(waitingCustomerNumber).at(0) <= timeMinutes)
        {
            for (int i = 0; i < (int)SeatsStay2.size(); ++i)
            {
                SeatsStay2[i] = std::max(SeatsStay1[i], SeatsStay1[i + 1]);
            }
            for (int i = 0; i < (int)SeatsStay3.size(); ++i)
            {
                SeatsStay3[i] = std::max(std::max(SeatsStay1[i], SeatsStay1[i + 1]), SeatsStay1[i + 2]);
            }
            for (int i = 0; i < (int)SeatsStay4.size(); ++i)
            {
                SeatsStay4[i] = std::max(std::max(SeatsStay1[i], SeatsStay1[i + 1]), std::max(SeatsStay1[i + 2], SeatsStay1[i + 3]));
            }

            // 条件を変更
            std::cout << waitingCustomerNumber << ":" << data.at(waitingCustomerNumber).at(0) << ":" << data.at(waitingCustomerNumber).at(1) << ":" << data.at(waitingCustomerNumber).at(2) << std::endl;

            // 客の滞在時間をSeatsStayに入れる
            if (siro(data.at(waitingCustomerNumber).at(1), data.at(waitingCustomerNumber).at(2), SeatsStay1, cap))
            {
                // 現在時刻と到着時刻の差が待ち時間になるのでそれを後ろに追加する
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(0)); // 修正: at(0) を at(1) に変更
                waitingCustomerNumber++;                                                                      // 待ち顧客の先頭を次に進める
                if (waitingCustomerNumber >= (int)data.size())                                                // 最後まで行ったらマイナスにして顧客を案内しきったことを示す
                {
                    // waitingCustomerNumber = -1;
                    break;
                }
            }
            else if (siro(data.at(waitingCustomerNumber).at(1), data.at(waitingCustomerNumber).at(2), SeatsStay2, cap2) && data.at(waitingCustomerNumber).at(2) == 2)
            {
                // 2席占有されるので反映する
                for (int i = 0; i < (int)SeatsStay2.size(); ++i)
                {
                    std::cout << SeatsStay2[i] << ", ";
                    if (SeatsStay2[i] > 0)
                    {
                        SeatsStay1[i] = SeatsStay2[i];
                        SeatsStay1[i + 1] = SeatsStay2[i];
                    }
                }
                std::cout << std::endl;
                // 現在時刻と到着時刻の差が待ち時間になるのでそれを後ろに追加する
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(0));
                waitingCustomerNumber++;                       // 待ち顧客の先頭を次に進める
                if (waitingCustomerNumber >= (int)data.size()) // 最後まで行ったらマイナスにして顧客を案内しきったことを示す
                {
                    // waitingCustomerNumber = -1;
                    break;
                }
            }
            else if (siro(data.at(waitingCustomerNumber).at(1), data.at(waitingCustomerNumber).at(2), SeatsStay3, cap3) && data.at(waitingCustomerNumber).at(2) == 3)
            {
                for (int i = 0; i < (int)SeatsStay3.size(); ++i)
                {
                    if (SeatsStay3[i] > 0)
                    {
                        SeatsStay1[i] = SeatsStay3[i];
                        SeatsStay1[i + 1] = SeatsStay3[i];
                        SeatsStay1[i + 2] = SeatsStay3[i];
                    }
                }
                // 現在時刻と到着時刻の差が待ち時間になるのでそれを後ろに追加する
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(0)); // 修正: at(0) を at(1) に変更
                waitingCustomerNumber++;                                                                      // 待ち顧客の先頭を次に進める
                if (waitingCustomerNumber >= (int)data.size())                                                // 最後まで行ったらマイナスにして顧客を案内しきったことを示す
                {
                    // waitingCustomerNumber = -1;
                    break;
                }
            }
            else if (siro(data.at(waitingCustomerNumber).at(1), data.at(waitingCustomerNumber).at(2), SeatsStay4, cap4) && data.at(waitingCustomerNumber).at(2) == 4)
            {
                for (int i = 0; i < (int)SeatsStay4.size(); ++i)
                {
                    if (SeatsStay4[i] > 0)
                    {
                        SeatsStay1[i] = SeatsStay4[i];
                        SeatsStay1[i + 1] = SeatsStay4[i];
                        SeatsStay1[i + 2] = SeatsStay4[i];
                        SeatsStay1[i + 3] = SeatsStay4[i];
                    }
                }
                // 現在時刻と到着時刻の差が待ち時間になるのでそれを後ろに追加する
                data.at(waitingCustomerNumber).push_back(timeMinutes - data.at(waitingCustomerNumber).at(0)); // 修正: at(0) を at(1) に変更
                waitingCustomerNumber++;                                                                      // 待ち顧客の先頭を次に進める
                if (waitingCustomerNumber >= (int)data.size())                                                // 最後まで行ったらマイナスにして顧客を案内しきったことを示す
                {
                    // waitingCustomerNumber = -1;
                    break;
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
        minusTime(SeatsStay1);
        minusTime(SeatsStay2);
        minusTime(SeatsStay3);
        minusTime(SeatsStay4);

        // 席の待ち時間の状況テスト表示コード
        std::cout << "time=" << timeMinutes << ":" << "待ち客=" << waitingCustomerNumber << ":";
        for (int i = 0; i < (int)SeatsStay1.size(); i++)
        {
            std::cout << "(" << i << ")" << SeatsStay1.at(i);
            (SeatsStay1.at(i) > 9) ? std::cout << " " : std::cout << "  ";
        }
        std::cout << std::endl;
        for (int i = 0; i < (int)SeatsStay2.size(); i++)
        {
            std::cout << "(" << i << "-2)" << SeatsStay2.at(i);
            (SeatsStay2.at(i) > 9) ? std::cout << " " : std::cout << "  ";
        }
        std::cout << std::endl;
        for (int i = 0; i < (int)SeatsStay3.size(); i++)
        {
            std::cout << "(" << i << "-3)" << SeatsStay3.at(i);
            (SeatsStay3.at(i) > 9) ? std::cout << " " : std::cout << "  ";
        }
        std::cout << std::endl;
        for (int i = 0; i < (int)SeatsStay4.size(); i++)
        {
            std::cout << "(" << i << "-4)" << SeatsStay4.at(i);
            (SeatsStay4.at(i) > 9) ? std::cout << " " : std::cout << "  ";
        }
        std::cout << std::endl;
    }
    std::string outputFilename = "waitingTime.txt";
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }

    // std::cout << data.at(0).size() << std::endl;
    for (int i = 0; i < (int)data.size(); i++)
    {
        for (int j = 0; j < (int)data.at(i).at(2); j++)
        {
            outputFile << data.at(i).at(3) << std::endl;
        }
    }
    outputFile.close();
    std::cout << "待ち時間は" << outputFilename << "に書き込みました" << std::endl;
}