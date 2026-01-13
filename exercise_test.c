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

  reverse_array(arr1, size1);
  for (int i = 0; i < size1; i++) {
    mu_assert("reverse_array failed on normal array", arr1[i] == expected1[i]);
  }

  int arr2[] = {42};
  int expected2[] = {42};
  reverse_array(arr2, 1);
  mu_assert("reverse_array failed on single element", arr2[0] == expected2[0]);

  // Edge case: empty array (use NULL)
  reverse_array(NULL, 0); // Should do nothing, not crash

  return NULL;
      
}  

/*-------------------------------------------------------------------
 * Test match_add() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
char *match_add_test(void) {
  // Normal 
  mu_assert("match_add failed for add1", match_add("add1", 3) == 4);
  mu_assert("match_add failed for add2", match_add("add2", 3) == 5);
  mu_assert("match_add failed for add3", match_add("add3", 3) == 6);

  // Invalid command
  mu_assert("match_add failed for unknown command", match_add("foo", 10) == 10);

  // NULL command
  mu_assert("match_add failed for NULL command", match_add(NULL, 7) == 7);

  return NULL;  // all tests passed
  
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

char * set_key_action_test(void){
  char *key="del1";
  int ret=set_key_action(NULL, key, del1);
  mu_assert("Error in set_key_action with NULL value", ret == FAIL);

  struct key_action *rec = (struct key_action *) malloc(sizeof(struct key_action));
  ret = set_key_action(rec, key, del1);
  char *msg = check_key_action_return(key, del1, ret, rec);
  free(rec);
  /*All comparisons/tests are valid*/
  return msg;
}

/*-------------------------------------------------------------------
 * Test match_action() 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */
struct key_action map[] = {
  { "del1", del1 },
  { "del2", del2 },
  { 0, 0 }
};

char *match_action_test(void){
  /*Your solution*/
  // Normal cases
  mu_assert("match_action failed for del1", match_action(map, "del1", 5) == del1(5));
  mu_assert("match_action failed for del2", match_action(map, "del2", 5) == del2(5));

  // Invalid command: should return original arg
  mu_assert("match_action failed for unknown command", match_action(map, "del3", 10) == 10);

  // Edge case: NULL command
  mu_assert("match_action failed for NULL command", match_action(map, NULL, 7) == 7);

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
    mu_assert("Error in mat_vect_mult, one mismatch", y[i] ==expected); 
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
  mu_assert("Error in mat_mat_mult, NULL input", ret ==FAIL);
  ret=mat_vect_mult(&A, &A, &A, 0, n);
  mu_assert("Error in mat_mat_mult, NULL input", ret ==FAIL);
  return  NULL;
}  

/*-------------------------------------------------------------------
 * Test matrix matrix multiplication 
 * If failed, return a message string showing the failed point
 * If successful, return NULL 
 */

char *mat_mat_mult_test(void) {
  // Normal 2x2 multiplication
  double A2[4] = {1, 2,
                   3, 4};
  double B2[4] = {5, 6,
                   7, 8};
  double C2[4] = {0, 0,
                   0, 0};
  int ret = mat_mat_mult(A2, B2, C2, 2);
  mu_assert("mat_mat_mult failed return code for 2x2", ret == SUCC);
  mu_assert("mat_mat_mult failed C[0]", C2[0] == 1*5 + 2*7); // 19
  mu_assert("mat_mat_mult failed C[1]", C2[1] == 1*6 + 2*8); // 22
  mu_assert("mat_mat_mult failed C[2]", C2[2] == 3*5 + 4*7); // 43
  mu_assert("mat_mat_mult failed C[3]", C2[3] == 3*6 + 4*8); // 50

  // Normal 3x3 multiplication
  double A3[9] = {1,2,3, 4,5,6, 7,8,9};
  double B3[9] = {9,8,7, 6,5,4, 3,2,1};
  double C3[9] = {0};
  ret = mat_mat_mult(A3, B3, C3, 3);
  mu_assert("mat_mat_mult failed return code for 3x3", ret == SUCC);
  // Check a few key entries
  mu_assert("mat_mat_mult failed C3[0]", C3[0] == 1*9 + 2*6 + 3*3); // 30
  mu_assert("mat_mat_mult failed C3[4]", C3[4] == 4*8 + 5*5 + 6*2); // 57
  mu_assert("mat_mat_mult failed C3[8]", C3[8] == 7*7 + 8*4 + 9*1); // 60

  // Edge case: NULL pointers
  ret = mat_mat_mult(NULL, B2, C2, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_A", ret == FAIL);
  ret = mat_mat_mult(A2, NULL, C2, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_B", ret == FAIL);
  ret = mat_mat_mult(A2, B2, NULL, 2);
  mu_assert("mat_mat_mult failed to handle NULL matrix_C", ret == FAIL);

  // Edge case: non-positive size
  ret = mat_mat_mult(A2, B2, C2, 0);
  mu_assert("mat_mat_mult failed to handle size=0", ret == FAIL);
  ret = mat_mat_mult(A2, B2, C2, -3);
  mu_assert("mat_mat_mult failed to handle negative size", ret == FAIL);

  return NULL; // all tests passed
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
