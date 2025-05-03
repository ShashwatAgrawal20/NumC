#include <numc/numc.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    {
        clock_t start, end;
        double cpu_time_used;

        start = clock();
        ndarray_t *array = nc_reshape(nc_arange(1, 100000001, 1, nc_double),
                                      SND_INLINE(100, 100, 100, 100), true);

        nc_sum_otps opts = NC_SUM_DEFAULT_OPTS();
        opts.axis = 3;
        ndarray_t *s3 = nc_sum(array, &opts);
        nc_free(&array);
        nc_free(&s3);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("CPU time used(reshape|arange|sum): %.6f seconds\n",
               cpu_time_used);
    }

    return 0;
}
