/* File:     exercise_test.c
 *
 * Purpose:  Unit tests for functions defined in exercise.c 
 *
 * Compile:  gcc -g -Wall -o exercise exercise.c exercise_test.c
 * Run:      ./exercise
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"
#include "exercise.h"


/*-------------------------------------------------------------------
 * Test exchange() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *exchange_test(void) {
  int a = 1, b = 2;
    int ret;

    ret = exchange(&a, &b);
    mu_assert("exchange returned FAIL on valid input", ret == SUCC);
    mu_assert("exchange did not swap a correctly", a == 2);
    mu_assert("exchange did not swap b correctly", b == 1);

    ret = exchange(NULL, &b);
    mu_assert("exchange should fail on NULL a", ret == FAIL);

    ret = exchange(&a, NULL);
    mu_assert("exchange should fail on NULL b", ret == FAIL);

    return NULL;
}  

/*-------------------------------------------------------------------
 * Test reverse_array() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *reverse_array_test(void) {
  int arr1[] = {1, 2, 3, 4, 5};
  int expected1[] = {5, 4, 3, 2, 1};
  int size1 = 5;

  int ret = reverse_array(arr1, size1);
  mu_assert("reverse_array failed on normal array", ret == SUCC);
  for (int i = 0; i < size1; i++) {
    mu_assert("reverse_array did not reverse correctly", arr1[i] == expected1[i]);
  }

  int arr2[] = {42};
  int expected2[] = {42};
  ret = reverse_array(arr2, 1);
  mu_assert("reverse_array failed on single element", ret == SUCC);
  mu_assert("reverse_array did not keep single element intact", arr2[0] == expected2[0]);

  // Edge case: NULL array or size <= 0
  ret = reverse_array(NULL, 0); // Should safely return FAIL
  mu_assert("reverse_array should fail on NULL input", ret == FAIL);

  return NULL;
}
  

/*-------------------------------------------------------------------
 * Test match_add() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *match_add_test(void) {
  mu_assert("match_add failed for add1", match_add("add1", 3) == 4);
  mu_assert("match_add failed for add2", match_add("add2", 3) == 5);
  mu_assert("match_add failed for add3", match_add("add3", 3) == 6);

  // Invalid command
  mu_assert("match_add failed for unknown command", match_add("foo", 10) == 10);

  // NULL command
  mu_assert("match_add failed for NULL command", match_add(NULL, 7) == 7);

  return NULL;
}
 

/*-------------------------------------------------------------------
 * Check the result of testing after calling  set_key_action() 
 * If failed, return a message string showing the failed point
 * If succesful, return NULL 
 */
char *check_key_action_return(char *expected_key,int (*expected_func)(int),  int ret, struct key_action *rec) {
  mu_assert("Error in set_key_action with del1 key", strcmp(expected_key, rec->cmd)==0);
  mu_assert("Error in set_key_action with del1 value", rec->func == expected_func);
  mu_assert("Error in set_key_action with del1 value", ret== SUCC);
  return NULL;
}

/*-------------------------------------------------------------------
 * Test set_key_action() 
 * If failed, return a message string showing the failed point
 * If succesful, return NULL 
 */
int del1(int x){
  return x-1;
}

int del2(int x){
  return x-2;
}

char *set_key_action_test(void){
    char *key = "del1";
    int ret = set_key_action(NULL, key, del1);
    mu_assert("set_key_action should fail on NULL rec", ret == FAIL);

    struct key_action rec;
    ret = set_key_action(&rec, key, del1);
    mu_assert("set_key_action failed on valid input", ret == SUCC);
    mu_assert("key mismatch", strcmp(rec.cmd, key) == 0);
    mu_assert("function mismatch", rec.func == del1);

    return NULL;
}


/*-------------------------------------------------------------------
 * Test match_action() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *match_action_test(void){
  // Create a local map to avoid global variable issues
  struct key_action local_map[] = {
    { "del1", del1 },
    { "del2", del2 },
    { 0, 0 }
  };
  
  // Normal cases
  mu_assert("match_action failed for del1", match_action(local_map, "del1", 5) == del1(5));
  mu_assert("match_action failed for del2", match_action(local_map, "del2", 5) == del2(5));

  // Invalid command: should return original arg
  mu_assert("match_action failed for unknown command", match_action(local_map, "del3", 10) == 10);

  // Edge case: NULL command
  mu_assert("match_action failed for NULL command", match_action(local_map, NULL, 7) == 7);

  // Edge case: NULL map
  mu_assert("match_action failed for NULL map", match_action(NULL, "del1", 3) == 3);

  // Edge case: NULL function pointer in map
  struct key_action map2[] = { {"delX", NULL}, {0, 0} };
  mu_assert("match_action failed for NULL function pointer", match_action(map2, "delX", 8) == 8);

  return NULL; // all tests passed
}


/*-------------------------------------------------------------------
 * Test if the  matrix-vector multiplicatioon result is expected.
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 * m is number of rows and n is the number of columns
 */
char*  test_vect(double y[], int m, int n){
  int i;
  double expected= n*(n-1)/2;
  for (i = 0; i < m; i++){
#ifdef DEBUG1
    printf("Expected %f actual %f in mat_vect_mult\n", expected, y[i]); 
#endif
    // Use epsilon comparison for floating point numbers
    double diff = y[i] - expected;
    if (diff < 0) diff = -diff; // absolute value
    mu_assert("Error in mat_vect_mult, one mismatch", diff < 1e-9); 
  }
  return NULL;
}

/*-------------------------------------------------------------------
 * Test matrix vector multiplciation 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char* mat_vect_mult_test1(int m, int n) {
  int i,j;
  char *msg;
  double *A = malloc(m*n*sizeof(double));
  double *x = malloc(n*sizeof(double));
  double *y = malloc(m*sizeof(double));
  
  // Check if memory allocation succeeded
  if (A == NULL || x == NULL || y == NULL) {
    if (A) free(A);
    if (x) free(x);
    if (y) free(y);
    return "Memory allocation failed in mat_vect_mult_test1";
  }
  
  for (j = 0; j < n; j++) {
    x[j]=j;
  }
  for (i = 0; i < m; i++) {
    y[i]=0;
  }
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) A[i*n+j]=1;
  }

  int ret=mat_vect_mult(A, x, y, m, n);
  msg=test_vect(y, m, n);  
  free(A);
  free(x);
  free(y);

  if(msg !=NULL)
    return msg;

  if(ret!=SUCC)
    return "Error in mat_vect_mult return value";

  return NULL;
}  

/*-------------------------------------------------------------------
 * Test matrix vector multiplication.
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *mat_vect_mult_test(void) {
  return  mat_vect_mult_test1(2,4);
}  

char *mat_vect_mult_test_null(void) {
  double A=1; 
  int n=1;  
  int ret=mat_vect_mult(NULL, NULL, NULL, n, n);
  mu_assert("Error in mat_vect_mult, NULL input", ret ==FAIL);
  ret=mat_vect_mult(&A, &A, &A, 0, n);
  mu_assert("Error in mat_vect_mult, zero size", ret ==FAIL);
  return  NULL;
}  

/*-------------------------------------------------------------------
 * Test matrix matrix multiplication 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */

char *mat_mat_mult_test(void) {
  double A2[4] = {1, 2, 3, 4};
  double B2[4] = {5, 6, 7, 8};
  double C2[4] = {0, 0, 0, 0};

  int ret = mat_mat_mult(A2, B2, C2, 2);
  mu_assert("mat_mat_mult failed return code for 2x2", ret == SUCC);
  
  // Use epsilon comparison for floating point numbers
  double expected_C0 = 1*5 + 2*7;
  double expected_C1 = 1*6 + 2*8;
  double expected_C2 = 3*5 + 4*7;
  double expected_C3 = 3*6 + 4*8;
  
  mu_assert("mat_mat_mult failed C[0]", (C2[0] - expected_C0) < 1e-9 && (C2[0] - expected_C0) > -1e-9);
  mu_assert("mat_mat_mult failed C[1]", (C2[1] - expected_C1) < 1e-9 && (C2[1] - expected_C1) > -1e-9);
  mu_assert("mat_mat_mult failed C[2]", (C2[2] - expected_C2) < 1e-9 && (C2[2] - expected_C2) > -1e-9);
  mu_assert("mat_mat_mult failed C[3]", (C2[3] - expected_C3) < 1e-9 && (C2[3] - expected_C3) > -1e-9);

  // Edge case: NULL matrices or invalid size
  ret = mat_mat_mult(NULL, B2, C2, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_A", ret == FAIL);
  ret = mat_mat_mult(A2, NULL, C2, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_B", ret == FAIL);
  ret = mat_mat_mult(A2, B2, NULL, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_C", ret == FAIL);

  ret = mat_mat_mult(A2, B2, C2, 0);
  mu_assert("mat_mat_mult failed to handle zero size", ret == FAIL);
  ret = mat_mat_mult(A2, B2, C2, -1);
  mu_assert("mat_mat_mult failed to handle negative size", ret == FAIL);

  return NULL;
}


/*-------------------------------------------------------------------
 * Run all tests.  Ignore returned messages.
 */
void run_all_tests(void) {
  /* Call all tests.  You can add more tests*/
  mu_run_test(exchange_test);
  mu_run_test(reverse_array_test);
  mu_run_test(match_add_test);
  mu_run_test(set_key_action_test);
  mu_run_test(match_action_test);
  mu_run_test(mat_vect_mult_test);
  mu_run_test(mat_vect_mult_test_null);
  mu_run_test(mat_mat_mult_test);
}

/*-------------------------------------------------------------------
 * The main entrance to run all tests.  
 * If failed, return a message string showing the first failed point
 * Print the test stats
 */
int main(int argc, char* argv[]) {
  run_all_tests();
  
  mu_print_test_summary("Summary:");  
  return 0;
}
