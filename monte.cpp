#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

// 乱数生成器
random_device rd;
mt19937 gen(rd());

// 指数分布
exponential_distribution<> exp_dist(1.0 / 5); // 平均5

// 正規分布
normal_distribution<> norm_dist(30, 10); // 平均30, 標準偏差10

// 店舗レイアウトの設定
const int K = 9;  // テーブル数
const int cap[K] = {2, 2, 2, 4, 4, 2, 4, 4, 4};  // 各テーブルの収容可能人数
const int table_pairs[][2] = {
    {1, 2}, {1, 4}, {2, 3}, {2, 5}, {3, 6}, {4, 5}, {4, 7},
    {5, 6}, {5, 8}, {6, 9}, {7, 8}, {7, 9}
};  // 組み合わせ可能なテーブルのペア
const int table_triples[][3] = {
    {1, 2, 4}, {1, 4, 5}, {2, 3, 5}, {3, 5, 8}, {4, 5, 7},
    {5, 6, 9}, {7, 8, 9}
};  // 組み合わせ可能なテーブルのトリプル

// シミュレーション設定
const int T = 500;  // シミュレーション期間
const int P = 3;  // 割り当てルール適用グループの最大組数
const double beta1 = 100;  // ジニ係数の重み
const double beta2 = 5;  // 平均待ち時間の重み


// 顧客クラス
class Customer {
public:
    int arrival_time;
    int group_size;
    double waiting_time;
    int assigned_time;
    int leaving_time;

    Customer(int arrival_time, int group_size) : 
        arrival_time(arrival_time), 
        group_size(group_size), 
        waiting_time(0), 
        assigned_time(-1), 
        leaving_time(-1) 
    {}
};

// 座席割り当てルール
vector<Customer*> fifo_rule(vector<Customer*> &customers, set<int> &available_tables) {
    // """従来法1: FIFOで案内"""
    vector<Customer*> assigned_customers;
    for (int table : available_tables) {
        for (Customer* customer : customers) {
            if (customer->assigned_time == -1 && customer->group_size <= cap[table]) {
                assigned_customers.push_back(customer);
                customer->assigned_time = customer->arrival_time + customer->waiting_time;
                customer->leaving_time = customer->assigned_time + norm_dist(gen);
                available_tables.erase(table);
                break;
            }
        }
    }
    return assigned_customers;
}

vector<Customer*> small_group_first_rule(vector<Customer*> &customers, set<int> &available_tables) {
    // """従来法2: 少人数グループ優先"""
    vector<Customer*> assigned_customers;
    vector<Customer*> customers_sorted = customers;
    sort(customers_sorted.begin(), customers_sorted.end(), [](Customer* a, Customer* b) {
        return a->group_size < b->group_size;
    });
    for (int table : available_tables) {
        for (Customer* customer : customers_sorted) {
            if (customer->assigned_time == -1 && customer->group_size <= cap[table]) {
                assigned_customers.push_back(customer);
                customer->assigned_time = customer->arrival_time + customer->waiting_time;
                customer->leaving_time = customer->assigned_time + norm_dist(gen);
                available_tables.erase(table);
                break;
            }
        }
    }
    return assigned_customers;
}

vector<Customer*> proposed_method_rule(vector<Customer*> &customers, set<int> &available_tables) {
    // """提案法"""
    // 論文のアルゴリズムに基づいて実装
    // (組み合わせ可能なテーブルの探索、待ち時間の計算、ジニ係数の考慮など)
    // ここでは簡略化のため、FIFOルールと同じ処理を行う
    vector<Customer*> assigned_customers;
    for (int table : available_tables) {
        for (Customer* customer : customers) {
            if (customer->assigned_time == -1 && customer->group_size <= cap[table]) {
                assigned_customers.push_back(customer);
                customer->assigned_time = customer->arrival_time + customer->waiting_time;
                customer->leaving_time = customer->assigned_time + norm_dist(gen);
                available_tables.erase(table);
                break;
            }
        }
    }
    return assigned_customers;
}

// ジニ係数の計算
double calculate_gini(vector<double> &waiting_times) {
    // """待ち時間のジニ係数を計算"""
    sort(waiting_times.begin(), waiting_times.end());
    int n = waiting_times.size();
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += (2 * (i + 1) - n - 1) * waiting_times[i];
    }
    return sum / (n * accumulate(waiting_times.begin(), waiting_times.end(), 0.0));
}

// シミュレーションの実行
vector<double> run_simulation(vector<Customer*> (*rule)(vector<Customer*> &, set<int> &)) {
    // """指定された座席割り当てルールでシミュレーションを実行"""
    vector<Customer*> customers;
    set<int> available_tables;
    for (int i = 0; i < K; ++i) {
        available_tables.insert(i);
    }
    int current_time = 0;
    while (current_time < T) {
        // 顧客の到着
        int arrival_time = current_time + exp_dist(gen);
        int group_size = uniform_int_distribution<>(1, 7)(gen); 
        customers.push_back(new Customer(arrival_time, group_size));

        // 時間を進める
        current_time++;

        // 座席割り当て
        if (!available_tables.empty()) {
            vector<Customer*> assigned_customers = rule(customers, available_tables);
            for (Customer* customer : assigned_customers) {
                // 待ち時間の計算
                customer->waiting_time = customer->assigned_time - customer->arrival_time;
            }
        }

    cout << "sim" << endl;
        // 退店処理
        for (auto it = customers.begin(); it != customers.end(); ) {
            Customer* customer = *it;
            if (customer->leaving_time != -1 && current_time >= customer->leaving_time) {
                // テーブルを解放
                for (int i = 0; i < K; ++i) {
                    if (cap[i] >= customer->group_size && available_tables.find(i) == available_tables.end()) {
                        available_tables.insert(i);
                        break;
                    }
                }
                delete customer; // メモリ解放
                it = customers.erase(it);
            } else {
                ++it;
            }
        }
        cout << "sim2" << endl;
    }
    // 結果の集計
    vector<double> waiting_times;
    for (Customer* customer : customers) {
        if (customer->assigned_time != -1) {
            waiting_times.push_back(customer->waiting_time);
        }
        delete customer; // メモリ解放
    }
    return waiting_times;
}

int main() {
    // 各座席割り当てルールでシミュレーションを実行
    cout << "test1" << endl;
    vector<double> waiting_times_fifo = run_simulation(fifo_rule);// 範囲外アクセス
    vector<double> waiting_times_small = run_simulation(small_group_first_rule);
    vector<double> waiting_times_proposed = run_simulation(proposed_method_rule);


    // ジニ係数の計算
    double gini_fifo = calculate_gini(waiting_times_fifo);
    double gini_small = calculate_gini(waiting_times_small);
    double gini_proposed = calculate_gini(waiting_times_proposed);

    cout << "FIFO rule:" << endl;
    cout << "  Gini coefficient: " << gini_fifo << endl;
    // 他の評価指標の出力

    cout << "Small group first rule:" << endl;
    cout << "  Gini coefficient: " << gini_small << endl;
    // 他の評価指標の出力

    cout << "Proposed method rule:" << endl;
    cout << "  Gini coefficient: " << gini_proposed << endl;
    // 他の評価指標の出力

    return 0;
}