#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

// 顧客数とリストを受け取って割り当て可能ならその先頭の添え字を渡す
int isOK(int CustomerNum, std::vector<int> li){
        int cnt=0;
        int ans=0;
        for(int i=0;i<li.size();i++){
            // 人がいるとき
            if(li.at(i)>0){
                cnt=0;
                continue;
            }//人がいないとき
            else{
                if(cnt==0){
                    ans = i;
                }
                cnt++;
            }
            if(cnt >= CustomerNum){
                return ans;
            }
        }
        return -1;
    }

bool assignCustomersEasyWay(int stayTime, std::vector<int>& customerSeatsStayTime){
    for(int i=0;i < customerSeatsStayTime.size();i++){
        if(customerSeatsStayTime.at(i)==0){
            customerSeatsStayTime.at(i) = stayTime;
            return true;
        }
    }
    return false;
}

bool vectorAllZero(std::vector<int> vec){
    for(int tmp : vec){
        if(tmp > 0){
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    
    // std::vector<int> array(std::vector<int>(12), 12); // 隣接行列
	// std::vector<int> customerSeatsCapacity(12); // 客席で0番から11番まで計12席。座れる人数
	// std::vector<int> customerWaiting(12); // 待っている顧客でそれぞれ人数を持っている
	std::vector<int> customerSeatsStayTime(12); // 客席で0番から11番まで計12席で客の残り滞在時間
    
    int waitingCustomerNumber = 0;

    // コマンドライン引数のチェック
    if(argc != 2){
        std::cerr << "引数の数が間違っています．" << std::endl;
        std::cerr << "./getline customers.txt" << std::endl;
        return 1;
    }

    // C++のファイル入力のための準備
    std::ifstream ifs(argv[1], std::ios::in);
    // ファイルを開くのに失敗したときの処理
    if(!ifs){
        std::cerr << "Error: file not opened." << std::endl;
        return 1;
    }

    std::string tmp;
    int num;
    int sum = 0;
    std::vector<std::vector<int>> data;
    std::vector<int> cus(3);
    std::string a, b, c;
    
    // getline()で1行ずつ読み込む
    while(getline(ifs, tmp)){
        // cout << tmp << endl; // そのまま出力
        std::stringstream ss;
        ss << tmp; 
        ss >> a >> b >> c;
        cus.at(0) = std::__cxx11::stoi(a);
        cus.at(1) = std::__cxx11::stoi(b);
        cus.at(2) = std::__cxx11::stoi(c);
        // cout << "---------\n";
        data.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();

    // 11時スタートで24時まで13時間=60 * 13分
    int endTime = 60;
    int timeMinutes = 0; // シミュレーション開始時刻は11時とする
    // std::cout << waitingCustomerNumber << "," << data.size() << std::endl;
    while(timeMinutes<endTime){
        // std::cout << timeMinutes << std::endl;

        // 顧客リストの先頭の顧客の到着時刻が現在の時刻以前だった場合、すでに到着しているとみなして顧客の割当てを行う
        while(waitingCustomerNumber >=0 && data.at(waitingCustomerNumber).at(1) <= timeMinutes){
            // 客の滞在時間をcustomerSeatsStayTimeに入れる
            if(assignCustomersEasyWay(data.at(waitingCustomerNumber).at(2), customerSeatsStayTime)){
                data.at(waitingCustomerNumber).push_back(timeMinutes-data.at(waitingCustomerNumber).at(1));
                waitingCustomerNumber++;
                if(waitingCustomerNumber>=data.size()){
                    waitingCustomerNumber = -1;
                }
                // std::cout << waitingCustomerNumber << std::endl;
            }
            else{
                break;
            }
        }
        // 全体の時間を一秒進める
        timeMinutes++;
        // 各座席の残り滞在時間を一秒減らす
        for(auto&& stayTime : customerSeatsStayTime){
            if(stayTime > 0){
                stayTime--;
            }
        }
        // 席の待ち時間の状況テスト表示コード
        std::cout << "time=" << timeMinutes << ":" << "待ち客=" << waitingCustomerNumber << ":";
        for(int i=0;i < customerSeatsStayTime.size();i++){
            std::cout << "(" << i << ")" << customerSeatsStayTime.at(i);
            (customerSeatsStayTime.at(i) > 9) ? std::cout << " ": std::cout << "  ";
        }
        std::cout << std::endl;
    }
    for(int i=0;i < data.size();i++){
        std::cout << "(" << i << ")待ち時間:" << data.at(i).at(3) << std::endl;
    }
	
}