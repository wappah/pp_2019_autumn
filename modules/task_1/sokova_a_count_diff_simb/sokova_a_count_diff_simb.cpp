#include "../../../modules/task_1/sokova_a_count_diff_simb/sokova_a_count_diff_simb.h";
#include <cstring>
#include <random>
#include <stdio.h>
#include <iostream>

int GetCountDiffSimb(char* Str1, char* Str2) {
  int ProcSize, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int Step;
  int Remainder;
  int Surplus = 0;

  if (strlen(Str1) <= strlen(Str2)) {
	Step = strlen(Str1) / ProcSize;
	Remainder = strlen(Str1) % ProcSize;
	Surplus = strlen(Str2) - strlen(Str1);
  }
  else {
	Step = strlen(Str2) / ProcSize;
	Remainder = strlen(Str2) % ProcSize;
	Surplus = strlen(Str1) - strlen(Str2);
  }

  int TempResult = 0;
  int Result = 0;
  char* TempStr1 = new char[Step + 1];
  char* TempStr2 = new char[Step + 1];

  if (ProcRank == 0) {
    if (Step > 0) {
      for (int Proc = 1; Proc < ProcSize; Proc++) {
	MPI_Send(&Str1[Proc * Step + Remainder], Step, MPI_CHAR, Proc, 0, MPI_COMM_WORLD);
	MPI_Send(&Str2[Proc * Step + Remainder], Step, MPI_CHAR, Proc, 1, MPI_COMM_WORLD);
      }
    }
  }

  if (ProcRank == 0) {
	delete[] TempStr1;
	delete[] TempStr2;
	TempStr1 = new char[Step + Remainder + 1];
	TempStr2 = new char[Step + Remainder + 1];
	for (int i = 0; i < Step + Remainder; i++) {
	  TempStr1[i] = Str1[i];
	  TempStr2[i] = Str2[i];
	}
	TempStr1[Step + Remainder] = 0;
	TempStr2[Step + Remainder] = 0;
  }
  else {
	MPI_Status status;
	if (Step > 0) {
	  MPI_Recv(&TempStr1[0], Step, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
	  MPI_Recv(&TempStr2[0], Step, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
	  TempStr1[Step] = 0;
	  TempStr2[Step] = 0;
	}
  }

  TempResult = Count(TempStr1, TempStr2);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&TempResult, &Result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  delete[] TempStr1;
  delete[] TempStr2;

  return Result + Surplus;
}

int Count(char* Str1, char* Str2) {
  int Result = 0;
  for (size_t i = 0; i < strlen(Str1); i++) {
	if (Str1[i] != Str2[i])
	Result++;
  }
  return Result;
}

int GetRandomStr(char* Str, int StrSize) {
  const char Arr[] = "abcdefghijklmnopqrstuvwxyz0123456789";

  if (StrSize <= 0)
	throw "Error. Invalid size.";
  for (int i = 0; i < StrSize; i++)
	Str[i] = Arr[rand() % (sizeof(Arr) - 1)];
  Str[StrSize] = 0;
}
