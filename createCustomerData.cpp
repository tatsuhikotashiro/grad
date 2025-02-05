#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>

int main(int argc, char *argv[])
{
    // 出力ファイルを開く
    std::string output_filename;
    double lambda, mu, sigma;

    std::cout << "出力ファイル名 指数分布のλ 正規分布の平均値 正規分布の標準偏差 の順に入力" << std::endl;
    std::cin >> output_filename >> lambda >> mu >> sigma;

    std::ofstream outputFile(output_filename);
    if (!outputFile.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    int cusNum = 200;
    int now = 0;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    // 到着時間の設定------------------------
    // 到着間隔を指数分布で作成し，0スタートの変数に加えたものを到着時刻と定義する

    std::exponential_distribution<> dist_a(1/lambda);
    std::vector<int> arriveTime;
    for (int i = 0; i < cusNum; i++)
    {
        // 指数分布で乱数を生成する
        double result_a = dist_a(engine);
        // std::cout << result_a << std::endl;
        now += (int)result_a;
        arriveTime.push_back(now);
        // 指数分布終わり
    }

    // サービス時間の設定----------------------
    std::normal_distribution<> dist_s(mu, sigma);
    std::vector<int> serviceTime;
    // 正規分布で乱数を生成する
    for (int i = 0; i < cusNum; i++)
    {
        double result_s = dist_s(engine);
        serviceTime.push_back((int)result_s);
    }

    // 集団サイズの設定------------------------
    // シードは共通
    // 1. 一様分布の場合
    // 1以上4以下の値を等確率で発生させる
    std::uniform_int_distribution<> dist_g(1, 4);
    std::vector<int> groupSize;
    for (int n = 0; n < cusNum; ++n)
    {
        // 一様整数分布で乱数を生成する
        int result_g = dist_g(engine);
        groupSize.push_back(result_g);
    }

    for (int i = 0; i < cusNum; i++)
    {
        outputFile << arriveTime.at(i) << ' ' << serviceTime.at(i) << ' ' << groupSize.at(i) << std::endl;
    }
    // ファイルを閉じる
    outputFile.close();

    return 0;
}
