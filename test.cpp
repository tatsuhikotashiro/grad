#include "gtest/gtest.h"

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

TEST(MaxWaitingTimeTest, Basic) {
  int idx_list[] = {0, 2, 4};
  int idx_size = 3;
  int waiting_time_for_seats[] = {10, 5, 15, 8, 12};
  EXPECT_EQ(max_waiting_time(idx_list, idx_size, waiting_time_for_seats), 15);
}

// TEST(MaxWaitingTimeTest, NegativeValues) {
//   int idx_list[] = {0, 2, 4};
//   int idx_size = 3;
//   int waiting_time_for_seats[] = {-10, -5, -15, -8, -12};
//   EXPECT_EQ(max_waiting_time(idx_list, idx_size, waiting_time_for_seats), -10);
// }

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