// 先頭が割当てられないなら次に並んでいる客を検討するやり方
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

struct Customer
{
    int arrival_time;
    int service_time;
    int group_size;
    int waiting_time = -1;
};

struct CombineTableInfo
{
    int table_idx;
    int combined_size;
    int combined_table_idx[4] = {-1, -1, -1, -1};
    CombineTableInfo(int a, int b, int c[4]) : table_idx(a), combined_size(b)
    {
        for (int i = 0; i < 4; ++i)
        {
            combined_table_idx[i] = c[i];
        }
    }
};

int max_waiting_time(int idx_list[], int idx_size, int waiting_time_for_seats[])
{
    int rslt = -1;
    for (int i = 0; i < idx_size; i++)
    {
        if (rslt <= waiting_time_for_seats[idx_list[i]])
        {
            rslt = waiting_time_for_seats[idx_list[i]];
        }
    }
    if (rslt < 0)
    {
        throw std::invalid_argument("値が負です");
    }
    return rslt;
}

// target_idx番のテーブルが組み合わせでできたものならば，
// target_idx番のテーブルの残り待ち時間を元となるテーブルにコピーする
// target_idx番のテーブルを使ってテーブルを組み合わせる場合は組み合わせ元の
// テーブルの残り待ち時間の最大値を組み合わせ先のテーブルの待ち時間にコピーする
void update_table_status(int target_idx, std::vector<CombineTableInfo> combine_list, int waiting_time_for_seats[])
{
    for (CombineTableInfo table : combine_list)
    {
        // target_idx番のテーブルが組み合わせでできたものならば，
        // target_idx番のテーブルの残り待ち時間を元となるテーブルにコピーする
        if (table.table_idx == target_idx)
        {
            for (int i = 0; i < table.combined_size; i++)
            {
                // std::cout << target_idx << "コピー" << waiting_time_for_seats[target_idx] << "to" << table.combined_table_idx[i] << std::endl;
                waiting_time_for_seats[table.combined_table_idx[i]] = waiting_time_for_seats[target_idx];
            }
        }
    }
    for (CombineTableInfo table : combine_list)
    {
        // target_idx番のテーブルを使ってテーブルを組み合わせる場合は組み合わせ元の
        // テーブルの残り待ち時間の最大値を組み合わせ先のテーブルの待ち時間にコピーする
        // for (int j = 0; j < table.combined_size; j++)
        // {
        //     // std::cout << table.combined_table_idx[j] <<" "<< target_idx << std::endl;
        //     if (table.combined_table_idx[j] == target_idx)
        //     {
                // std::cout << "グループ元からコピー" << max_waiting_time(table.combined_table_idx, table.combined_size, waiting_time_for_seats) << "to" << table.table_idx << std::endl;
                waiting_time_for_seats[table.table_idx] = max_waiting_time(table.combined_table_idx, table.combined_size, waiting_time_for_seats);
        //     }
        // }
        // std::cout << "----\n";
    }
}

int fifo(std::vector<int> &indexes_of_waiting_customers)
{
    for (int rst : indexes_of_waiting_customers)
    {
        return rst; // 先頭を即座に返す
    }
    return -1; // 並んでいる客がいなければ負の値を返す
}

int method_1(std::vector<int> &indexes_of_waiting_customers, std::vector<Customer> &customers, int waiting_time_for_seats[], int seats_capacity[], int size)
{
    for (int idx : indexes_of_waiting_customers)
    {
        std::cout << "idx=" << idx << std::endl;
        for (int i = 0; i < size; i++)
        {
            if (waiting_time_for_seats[i] == 0 && seats_capacity[i] >= customers[idx].group_size)
            {
                return idx;
            }
        }
    }
    return -1; // 選べなければ負の値を返す
}

bool simple_assign(int current_time, std::vector<int> &indexes_of_waiting_customers, int target, std::vector<Customer> &customers, int waiting_time_for_seats[], int seats_capacity[], int size, std::vector<CombineTableInfo> &combine_list)
{
    // std::cout << target << "の割り当てを試みています\n"<< std::endl;
    for (int i = 0; i < size; i++)
    {

        if (waiting_time_for_seats[i] == 0 && customers[target].group_size <= seats_capacity[i])
        {
            waiting_time_for_seats[i] = customers[target].service_time;
            std::cout << i << "番目の席に" << customers[target].service_time << "分を割当てました";
            customers[target].waiting_time = current_time - customers[target].arrival_time;

            indexes_of_waiting_customers.erase(std::remove(indexes_of_waiting_customers.begin(), indexes_of_waiting_customers.end(), target), indexes_of_waiting_customers.end());
            std::cout << "i=" << i << std::endl;
            update_table_status(i, combine_list, waiting_time_for_seats);
            return true;
        }
    }
    return false;
}

void minus_time(int waiting_time_for_seats[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (waiting_time_for_seats[i] > 0)
        {
            waiting_time_for_seats[i]--;
        }
    }
}

int main(int argc, char *argv[])
{
    // カウンター式
    int seats_capacity[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    int waiting_time_for_seats[42] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int table_num = 42;
    std::vector<CombineTableInfo> combine_list;
    for (int i = 0; i < 11; ++i)
    {
        int initial_indices2[4] = {i, i + 1, -1, -1};
        CombineTableInfo info2(i + 12, 2, initial_indices2);
        combine_list.push_back(info2);
    }
    for (int i = 0; i < 10; ++i)
    {
        int initial_indices3[4] = {i, i + 1, i + 2, -1};
        CombineTableInfo info3(i + 12 + 11, 3, initial_indices3);
        combine_list.push_back(info3);
    }
    for (int i = 0; i < 9; ++i)
    {
        int initial_indices4[4] = {i, i + 1, i + 2, i + 3};
        CombineTableInfo info4(i + 12 + 11 + 10, 4, initial_indices4);
        combine_list.push_back(info4);
    }

    // int seats_capacity[28] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    // int waiting_time_for_seats[28] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // int table_num = 28;
    // int seats_capacity[3] = {1, 2, 4};
    // int waiting_time_for_seats[3] = {0, 0, 0};
    // int table_num = 3;

    std::vector<int> indexes_of_available_seats;
    std::vector<int> indexes_of_waiting_customers;
    std::vector<Customer> customers;

    std::string tmp;     // 文字列の入力に一時的に使う
    std::string a, b, c; // 文字列の入力に一時的に使う

    // シミュレーション中に使用する時刻を保存する変数（分）
    int current_time = 0; // シミュレーション開始時刻は11時とする
    // シミュレーション終了時刻（分）
    int end_time = 1600;

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
        Customer cus;
        std::stringstream ss;
        ss << tmp;
        ss >> a >> b >> c;
        cus.arrival_time = std::__cxx11::stoi(a); // 到着
        cus.service_time = std::__cxx11::stoi(b); // 滞在
        cus.group_size = std::__cxx11::stoi(c);   // 人数
        customers.push_back(cus);
    }
    // ファイルを閉じる
    ifs.close();

    // シミュレーションの現在時刻が終了時刻よりも小さければ時刻を1ずつ進めながら繰り返す
    int next_index = 0;
    while (current_time < end_time)
    {
        // if (next_index < 20)
        // {
        //         std::cout <<current_time << ":";
        //     for (int i = 0; i < 12; i++)
        //     {
        //         std::cout << waiting_time_for_seats[i] << ' ';
        //     }
        //     std::cout << std::endl;
        // }
        // 現在到着して待っている顧客のリストの更新
        while (next_index < (int)customers.size() && customers[next_index].arrival_time <= current_time)
        {
            // std::cout << "到着リストに追加しましたarrival=" << customers[next_index].arrival_time << std::endl;
            indexes_of_waiting_customers.push_back(next_index);
            next_index++;
        }
        // std::cout << "待ち顧客インデックスのリスト: ";
        // for(int nom: indexes_of_waiting_customers){
        //     std::cout << nom << " ";
        // }
        // std::cout << '\n';

        bool flag = true; // 割当てできなくなるまでtrue

        while (flag)
        {
            // 割当てを試みる客のindexの選択

            // int selected_index = fifo(indexes_of_waiting_customers);

            int selected_index = method_1(indexes_of_waiting_customers, customers, waiting_time_for_seats, seats_capacity, table_num);
            // std::cout << selected_index << "が選択されています\n";
            if (selected_index < 0)
                break;
            // 割当て
            if (selected_index >= 0 && simple_assign(current_time, indexes_of_waiting_customers, selected_index, customers, waiting_time_for_seats, seats_capacity, table_num, combine_list))
            {
                flag = true;
            }
            else
            {
                flag = false;
            }
            for (int i = 0; i < table_num; i++)
            {
                std::cout << std::setw(3) << waiting_time_for_seats[i];
            }
            std::cout << std::endl;
        }
        // 全体の時間を一秒進める
        current_time++;
        // 各座席の残り滞在時間を一秒減らす
        minus_time(waiting_time_for_seats, table_num);
    }

    const std::string base_name = "waiting_time";
    const std::string extension = ".txt";
    int file_number = 1;
    // fs = std::filesystemであることに注意する
    fs::path current_dir = fs::current_path();

    // 重複したファイル名を出力しないようにしてファイルを比較できるようにする
    while (fs::exists(current_dir / (base_name + (file_number < 10 ? "0" :"") + std::to_string(file_number) + extension))) {
        file_number++;
    }
    std::string output_filename = base_name + (file_number < 10 ? "0" :"") + std::to_string(file_number) + extension;

    std::ofstream outputFile(output_filename);
    if (!outputFile.is_open())
    {
        std::cerr << "出力ファイルを開けませんでした。" << std::endl;
        return 1;
    }

    // 人数分で出力する場合
    for (int i = 0; i < (int)customers.size(); i++)
    {
        for (int j = 0; j < customers[i].group_size; j++)
        {
            outputFile << customers[i].waiting_time << std::endl;
        }
    }

    // // グループごとに記述する場合
    // for (int i = 0; i < (int)customers.size(); i++)
    // {
    //     outputFile << customers[i].waiting_time << std::endl;
    // }

    outputFile.close();
    std::cout << "待ち時間は" << output_filename << "に書き込みました" << std::endl;
}