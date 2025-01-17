#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <algorithm> // std::maxのために追加

// 顧客情報を保持する構造体
struct Customer {
    int arrivalTime;   // 到着時刻
    int stayTime;      // 滞在時間
    int groupSize;     // グループの人数
    int waitingTime;  // 待ち時間
};

// 滞在時刻と席ごとにあとどれだけ顧客が座っているのかを受け取って空いている席に割り当てる
// 入力: 顧客情報, 各座席の顧客の残り滞在時間, 各座席の収容人数
bool assignCustomerToSeat(const Customer& customer, std::vector<int>& seatsRemainingTime, const std::vector<int>& seatCapacity) {
    int groupSize = customer.groupSize;
    int requiredSeats = (groupSize + seatCapacity[0] - 1) / seatCapacity[0]; // 必要な席の数

    for (int i = 0; i <= (int)seatsRemainingTime.size() - requiredSeats; ++i) {
        bool available = true;
        for (int j = 0; j < requiredSeats; ++j) {
            if (seatsRemainingTime[i + j] != 0) {
                available = false;
                break;
            }
        }
        if (available) {
            for (int j = 0; j < requiredSeats; ++j) {
                seatsRemainingTime[i + j] = customer.stayTime;
            }
            return true;
        }
    }
    return false;
}

void decreaseRemainingTime(std::vector<int>& seatsRemainingTime) {
    for (auto& remainingTime : seatsRemainingTime) {
        if (remainingTime > 0) {
            remainingTime--;
        }
    }
}

int main(int argc, char* argv[]) {
    // 席数と収容人数
    const int numSeats = 12;
    std::vector<int> seatCapacity = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    std::vector<double> occupancyRate;
    std::vector<Customer> customers;
    std::vector<int> seatsRemainingTime(numSeats, 0);

    // シミュレーション終了時刻（分）
    const int endTime = 1600; 
    int currentTime = 0;

    // コマンドライン引数のチェック
    if (argc != 2) {
        std::cerr << "引数の数が間違っています．" << std::endl;
        std::cerr << "./getline customers.txt" << std::endl;
        return 1;
    }

    // ファイル入力
    std::ifstream ifs(argv[1], std::ios::in);
    if (!ifs) {
        std::cerr << "Error: file not opened." << std::endl;
        return 1;
    }

    std::string line;
    while (getline(ifs, line)) {
        std::stringstream ss(line);
        Customer customer;
        ss >> customer.arrivalTime >> customer.stayTime >> customer.groupSize;
        customers.push_back(customer);
    }
    ifs.close();

    // シミュレーション
    int nextCustomerIndex = 0;
    std::vector<int> waitingCustomersIndexes;
    while (currentTime < endTime) {
        // 占有率を計算
        int usedSeats = std::count_if(seatsRemainingTime.begin(), seatsRemainingTime.end(), [](int time) { return time > 0; });
        occupancyRate.push_back(static_cast<double>(usedSeats) / numSeats);

        // 到着した顧客を待ち行列に追加
        while (nextCustomerIndex < (int)customers.size() && customers[nextCustomerIndex].arrivalTime <= currentTime) {
            waitingCustomersIndexes.push_back(nextCustomerIndex);
            nextCustomerIndex++;
        }

        // 待ち行列の顧客を席に割り当てる
        for (auto it = waitingCustomersIndexes.begin(); it != waitingCustomersIndexes.end(); ) {
            int customerIndex = *it;
            if (assignCustomerToSeat(customers[customerIndex], seatsRemainingTime, seatCapacity)) {
                // 待ち時間を計算
                customers[customerIndex].waitingTime = currentTime - customers[customerIndex].arrivalTime;

                // 待ち行列から削除
                it = waitingCustomersIndexes.erase(it); 
            }
            else {
                ++it;
            }
        }

        // 時間を進める
        currentTime++;
        decreaseRemainingTime(seatsRemainingTime);
    }

    // 結果出力
    std::ofstream waitingTimeFile("waitingTime.txt");
    if (!waitingTimeFile.is_open()) {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    for (const auto& customer : customers) {
        for (int i = 0; i < customer.groupSize; ++i) {
            waitingTimeFile << customer.waitingTime << std::endl;
        }
    }
    waitingTimeFile.close();
    std::cout << "待ち時間は waitingTime.txt に書き込みました" << std::endl;

    std::ofstream resultFile("./result/rslt.txt");
    if (!resultFile.is_open()) {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    for (int i = 0; i < (int)customers.size(); ++i) {
        for (int j = 0; j < customers[i].groupSize; ++j) {
            resultFile << i + 1 << ' ' << customers[i].arrivalTime << ' ' << customers[i].waitingTime << std::endl;
        }
    }
    resultFile.close();

    std::ofstream occupancyFile("./result/occupancy.txt");
    if (!occupancyFile.is_open()) {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }
    for (double occupancy : occupancyRate) {
        occupancyFile << occupancy << std::endl;
    }
    occupancyFile.close();

    return 0;
}