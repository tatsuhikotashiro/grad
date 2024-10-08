#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

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

    int assignCustomers(){

    }

int main() {
    
    std::vector<int> array(std::vector<int>(12), 12); // 隣接行列
	std::vector<int> customerSeatsCapacity(12); // 客席で0番から11番まで計12席。座れる人数
	std::vector<int> customerSeatsUsed(12); // 客席で0番から11番まで計12席。正なら使用されている
	std::vector<int> customerSeatsStayTime(12); // 客席で0番から11番まで計12席で客の残り滞在時間
	std::vector<int> customerWaiting(12); // 待っている顧客でそれぞれ人数を持っている
    int timeSeconds;
    // 60s * 60 min * 24hで一日の終わりは86400まで
    int endTime = 86400;
    timeSeconds = 60 * 60 * 11; // シミュレーション開始時刻は11時とする
    while(timeSeconds<endTime){
        // 各座席の顧客が退店するかどうかを確認する
        for(int i = 0;i < customerSeatStayTime.size();i++){
            if(customerSeatsUsed.at(i) > 0 && customerSeatsStayTime <= 0){
                customerSeatsUsed.at(i) = 0;
            }
        }


        // 待ち行列に客がいるか確認する
        // もしいる場合
            // 割当てる顧客の選択方法を適用する

            // 選択された顧客を客席に割り当てる

            // 顧客待ち行列から待っていたが客席に移動した顧客を削除する
            // 待ち行列を一つずつ進める
            // 顧客の滞在時間を5~30分の間で確率で設定する
            // 設定した滞在時間を"customerSeatsStayTime"に設定する
        // いない場合
            // なにもしない
        
        // 全体の時間を一秒進める
        timeSeconds++;
        // 各座席の残り滞在時間を一秒減らす
        for(auto&& stayTime : customerSeatStayTime){
            if(stayTime > 0){
                stayTime--;
            }
        }
    }
    

    customerSeats = {1,1,1,1,0,1,1,1,0,0,1,1};
    int num;// 客の人数
    num=2;
    // 客を割当てる
    for(int i = isOK(num, customerSeats);i<isOK(num, customerSeats)+num;i++){
        customerSeats.at(i)=1;
    }
    
	
}