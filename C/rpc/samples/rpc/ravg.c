#include "avg.h"
#include <stdlib.h>

void averageprog_1( char* host, int argc, char *argv[])
{
    CLIENT *clnt;
    double  *result_1, *dp, f;
    char *endptr;
    int i;
    input_data  average_1_arg;
    average_1_arg.input_data.input_data_val =
        (double*) malloc(MAXAVGSIZE*sizeof(double));
    dp = average_1_arg.input_data.input_data_val;
    average_1_arg.input_data.input_data_len =
        argc - 2;
    for (i=1;i<=(argc - 2);i++) {
        f = strtod(argv[i+1],&endptr);
        printf("value   = %e\n",f);
        *dp = f;
        dp++;
    }
    clnt = clnt_create(host, AVERAGEPROG,
            AVERAGEVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    result_1 = average_1(&average_1_arg, clnt);
    if (result_1 == NULL) {
        clnt_perror(clnt, "call failed:");
    }
    clnt_destroy( clnt );
    printf("average = %e\n",*result_1);
}


void main( int argc, char* argv[] )
{
    char *host;

    if(argc < 3) {
        printf(
                "usage: %s server_host value ...\n",
                argv[0]);
        exit(1);
    }
    if(argc > MAXAVGSIZE + 2) {
        printf("Two many input values\n");
        exit(2);
    }
    host = argv[1];
    averageprog_1( host, argc, argv);
}

