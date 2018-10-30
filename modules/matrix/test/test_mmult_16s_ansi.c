#include <string.h>
#include "unity.h"
#include "test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_clk.h"
#include "soc/cpu.h"

#include "dslm_mult.h"
#include "esp_attr.h"

// Test dsls_dotprod_16s_ansi function
TEST_CASE("DSL check dslm_mult_16s_ansi functionality", "[dsl]")
{
    int m = 4;
    int n = 3;
    int k = 4;


    int16_t A[m][n];
    int16_t* A_ptr = (int16_t*)A;

    int16_t B[n][k];
    int16_t* B_ptr = (int16_t*)B;

    int16_t C[m][k];
    int16_t* C_ptr = (int16_t*)C;
    int16_t C_compare[m][k];
    int16_t* Cc_ptr = (int16_t*)C_compare;

    int shift = 0;
    for (int i=0 ; i< m*n; i++)
    {
        A_ptr[i] = 0x1000;
        B_ptr[i] = 0x200;
    }
    long long store_reg = 0;
    for (int i=0 ; i< m ; i++)
    {
        for (int j=0 ; j< k ; j++)
        {
            store_reg = (0x7fff>>shift);
            for (int s=0 ; s< n ; s++)
            {
                store_reg += ((int32_t)A[i][s]*(int32_t)B[s][j]);
            }
            C_compare[i][j] = store_reg >> (15 - shift);
        }
    }
    dslm_mult_16s_ansi(A_ptr, B_ptr, C_ptr, m, n, k, shift);

    // for (int i=0 ; i< m ; i++)
    // {
    //     for (int j=0 ; j< k ; j++)
    //     {
    //         printf("[%i][%i] calc=%i, expected =%i\n",i,j, C[i][j], C_compare[i][j]);
    //     }
    // }
    // Compare and check results
    for (int i = 0 ; i< m*k ; i++)
    {
        if (Cc_ptr[i] != C_ptr[i])
        {
            TEST_ASSERT_EQUAL(Cc_ptr[i], C_ptr[i]);
        }
    }
}

static portMUX_TYPE testnlock = portMUX_INITIALIZER_UNLOCKED;

TEST_CASE("DSL check dslm_mult_16s_ansi benchmark", "[dsl]")
{
    int m = 4;
    int n = 4;
    int k = 4;

    int16_t A[m][n];
    int16_t* A_ptr = (int16_t*)A;

    int16_t B[n][k];
    int16_t* B_ptr = (int16_t*)B;

    int16_t C[m][k];
    int16_t* C_ptr = (int16_t*)C;


    portENTER_CRITICAL(&testnlock);

    unsigned int start_b = xthal_get_ccount();
    int repeat_count = 1024;
    for (int i=0 ; i< repeat_count ; i++)
    {
        dslm_mult_16s_ansi(A_ptr, B_ptr, C_ptr, m, n, k, 0);
    }
    unsigned int end_b = xthal_get_ccount();
    portEXIT_CRITICAL(&testnlock);

    float total_b = end_b - start_b;
    float cycles = total_b/(repeat_count);
    printf("Benchmark dslm_mult_16s_ansi - %f per multiplication %ix%ix%i.\n", cycles, m,n,k);
    float min_exec = 2000;
    float max_exec = 3000;
    if (cycles >= max_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes more then expected!");
    }
    if (cycles < min_exec) { 
        TEST_ASSERT_MESSAGE (false, "Exec time takes less then expected!");
    }
}