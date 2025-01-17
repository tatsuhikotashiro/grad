#include "gtest/gtest.h"
#include "sim_way2.h"  // max_waiting_time() の定義を含むヘッダーファイルをインクルード

TEST(MaxWaitingTimeTest, Basic) {
  int idx_list[] = {0, 2, 4};
  int idx_size = 3;
  int waiting_time_for_seats[] = {10, 5, 15, 8, 12};
  EXPECT_EQ(max_waiting_time(idx_list, idx_size, waiting_time_for_seats), 15);
}

TEST(MaxWaitingTimeTest, NegativeValues) {
  int idx_list[] = {0, 2, 4};
  int idx_size = 3;
  int waiting_time_for_seats[] = {-10, -5, -15, -8, -12};
  EXPECT_EQ(max_waiting_time(idx_list, idx_size, waiting_time_for_seats), -10);
}

TEST(MaxWaitingTimeTest, EmptyIdxList) {
  int idx_list[] = {};
  int idx_size = 0;
  int waiting_time_for_seats[] = {10, 5, 15, 8, 12};
  EXPECT_THROW(max_waiting_time(idx_list, idx_size, waiting_time_for_seats), std::invalid_argument);
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}