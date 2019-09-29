#define RPC_TEST_NUM         0x38000010

struct test_io_data {
    int type;
    int len;
    char data[1024];
};

typedef struct test_io_data test_data;

program rpc_func_test {
    version rpc_test_set_func {
            int test_func(test_data) = 1;
        } = 1;        /* Version number = 1 */
    version rpc_test_get_func {
            int test_func(test_data) = 1;
        } = 2;        /* Version number = 2 */
} = RPC_TEST_NUM;    /* Program number */
