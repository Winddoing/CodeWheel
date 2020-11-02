
#define RPC_TEST_NUM         0x38000022

program FILETRANSPROG {
    version FILETRANSVERS {
        string READFILE(string) = 1;
    } = 1;
} = RPC_TEST_NUM;
