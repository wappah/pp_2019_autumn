// Copyright 2019 Sokova Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <string>
#include "../../../modules/task_1/sokova_a_count_diff_simb/sokova_a_count_diff_simb.h"
TEST(Count_Diff_Simb_MPI, count_diff_char_in_identical_str) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  char Str1[] = "helloworld";
  char Str2[] = "helloworld";

  int ExpectedResult = 0;

  int RealResult;
  RealResult = GetCountDiffSimb(Str1, Str2);

  if (ProcRank == 0) {
    ASSERT_EQ(ExpectedResult, RealResult);
  }
}

TEST(Count_Diff_Simb_MPI, count_diff_char_in_diff_size_str) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  char Str1[] = "abcdefghi";
  char Str2[] = "abc";

  int ExpectedResult = 6;

  int RealResult;
  RealResult = GetCountDiffSimb(Str1, Str2);

  if (ProcRank == 0) {
    ASSERT_EQ(ExpectedResult, RealResult);
  }
}

TEST(Count_Diff_Simb_MPI, count_diff_char_in_same_size_expected_result_even) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  char Str1[] = "abcdefg";
  char Str2[] = "abcxxxx";

  int ExpectedResult = 4;

  int RealResult;
  RealResult = GetCountDiffSimb(Str1, Str2);

  if (ProcRank == 0) {
    ASSERT_EQ(ExpectedResult, RealResult);
  }
}

TEST(Count_Diff_Simb_MPI, count_diff_char_in_same_size_expected_result_odd) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  char Str1[] = "abcdefghik";
  char Str2[] = "xxcxexxxix";

  int ExpectedResult = 7;

  int RealResult;
  RealResult = GetCountDiffSimb(Str1, Str2);

  if (ProcRank == 0) {
    ASSERT_EQ(ExpectedResult, RealResult);
  }
}

TEST(Count_Diff_Char_MPI, get_random_str) {
  int Size = 17;
  char* Str = new char[Size];
  ASSERT_NO_THROW(GetRandomStr(Str, Size));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
