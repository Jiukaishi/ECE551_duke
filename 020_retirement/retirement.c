#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;

double calculation(int* startAge, double balance, retire_info state) {
  for (int m = 1; m <= state.months; m++) {
    int age = *startAge / 12;
    int month = *startAge % 12;
    printf("Age %3d month %2d you have $%.2f\n", age, month, balance);
    balance += balance * state.rate_of_return + state.contribution;
    *(startAge) = *(startAge)+1;
  }
  return balance;
}

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  double balance = initial;
  balance = calculation(&startAge, balance, working);
 // startAge = startAge + working.months;
  balance = calculation(&startAge, balance, retired);
}

int main(void) {
  retire_info working;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045 / 12;
  retire_info retired;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01 / 12;

  int startAge = 327;
  double initial = 21345;
  retirement(startAge, initial, working, retired);
  return 0;
}
