#include "pandemic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
country_t parseLine(char * line) {
  if (line == NULL) {
    perror("Null pointer!, check your input!");
    exit(EXIT_FAILURE);
  }
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;
  const char * p_input = line;  //pointer to the input string
  char * p_name = ans.name;     //pointer to where we want to write in
  size_t cnt = 0;  //counting how many chars have counted already in case of overflowing.
  while (*p_input != ',') {  //write in name until seeing a comma
    if (*p_input == '\n' ||
        *p_input == '\0') {  //if it ends without seeing population, throw exception
      perror("End without seeing population\n");
      exit(EXIT_FAILURE);
    }
    if (cnt >= 64 - 1) {
      perror("Input a too long name\n");
      exit(EXIT_FAILURE);
    }
    *p_name = *p_input;  //write a char in name
    cnt++;
    p_input++;
    p_name++;
  }
  ans.name[cnt] = '\0';  //always an \0 at the end of name
  //Dealing with population.
  p_input++;  //skip the comma
  cnt = 0;    //reset the counter for later use
  while (*p_input == 9 || *p_input == 32) {
    p_input++;
  }  //skip the whitespaces before the digits
  while (*p_input >= '0' && *p_input <= '9') {
    uint64_t
        cur = (*p_input - '0'),
        tmp = ans.population;  //atoi for digits and init tmp for later checking overflow
    ans.population = ans.population * 10 + cur;
    //check multiply and add overflow
    if ((tmp > UINT64_MAX / 10) || tmp * 10 > (UINT64_MAX - cur)) {
      perror("Input population exceed limits, must be a unsigned 64bits integer!\n");
      exit(EXIT_FAILURE);
    }
    cnt++;
    p_input++;
  }
  //if there are other chars follow the digits, throw exceptions
  if (*p_input != '\n' && *p_input != '\0') {
    perror("Input population must be uint_64 type: \n");
    exit(EXIT_FAILURE);
  }
  //if there are no digits in the population, throw exception
  if (cnt == 0) {
    perror("No digits found in the input population\n");
    exit(EXIT_FAILURE);
  }
  return ans;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  if (data == NULL || avg == NULL) {
    perror("Null pointer!, check your input!");
    exit(EXIT_FAILURE);
  }
  if (n_days < 7) {
    return;
  }
  size_t left = 0;  //indicating the earliest of the sevendays
  unsigned cur_sum = 0;
  for (size_t i = 0; i < 7; i++) {
    cur_sum += data[i];
  }  //init cur_sum to be the sum of first 7 days
  avg[0] = (double)cur_sum / 7.0;
  for (size_t i = 7; i < n_days; i++) {
    cur_sum =
        cur_sum - data[left] +
        data[i];  //only minus the earliest day and add the next day to update cur_sum.
    left++;       //update left boundry
    avg[left] = (double)cur_sum / 7.0;
  }
  return;
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  if (data == NULL || cum == NULL) {
    perror("Null pointer!, check your input!");
    exit(EXIT_FAILURE);
  }
  if (pop < 0) {
    perror("Population must be greater than 0!");
    exit(EXIT_FAILURE);
  }
  if (n_days < 0) {
    perror("Number of days must greater than 0!");
    exit(EXIT_FAILURE);
  }
  double cur_total_number = 0;
  for (size_t i = 0; i < n_days; i++) {
    cur_total_number += data[i];
    cum[i] = cur_total_number / pop * 100000;
  }
  return;
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  if (data == NULL || countries == NULL) {
    perror("Null pointer!, check your input!");
    exit(EXIT_FAILURE);
  }
  if (n_days <= 0) {
    perror("Number of days must be greater than 0!");
    exit(EXIT_FAILURE);
  }
  if (n_countries <= 0) {
    perror("Number of countries must be greater than 0!");
    exit(EXIT_FAILURE);
  }
  unsigned max_data = data[0][0], max_idx = 0;
  //storing the max of data so far and its corresponding country index.
  for (size_t i = 0; i < n_countries; i++) {
    for (size_t j = 0; j < n_days; j++) {
      if (max_data < data[i][j]) {  //find the maximum so far, update it!
        max_data = data[i][j];
        max_idx = i;
      }
    }
  }
  const char * country_name = countries[max_idx].name;
  unsigned number_cases = max_data;
  printf("%s has the most daily cases with %u\n", country_name, number_cases);
  return;
}
