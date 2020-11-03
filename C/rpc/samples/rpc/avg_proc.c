#include <rpc/rpc.h>
#include "avg.h"
#include <stdio.h>

static double sum_avg;

double * average_1(input_data *input,
        CLIENT *client) {

    double *dp = input->input_data.input_data_val;
    u_int i;
    sum_avg = 0;
    for(i=1;i<=input->input_data.input_data_len;i++) {
        sum_avg = sum_avg + *dp; dp++;
    }
    sum_avg = sum_avg /
        input->input_data.input_data_len;
    return(&sum_avg);
}

double * average_1_svc(input_data *input,
        struct svc_req *svc) {
    CLIENT *client;
    return(average_1(input,client));
}

