#include <numc/numc.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    {
        clock_t start, end;
        double cpu_time_used;

        start = clock();
        ndarray_t *test = nc_reshape(nc_arange(1, 100000001, 1, nc_double),
                                     SND_INLINE(100, 100, 100, 100), true);
        ndarray_t *sall = nc_sum(test, 3);
        // nc_display(test, false);
        // nc_display(sall, true);
        nc_free(&test);
        nc_free(&sall);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("CPU time used(reshape|arange|sum): %.6f seconds\n",
               cpu_time_used);
    }

    return 0;
}
