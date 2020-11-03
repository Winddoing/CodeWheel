
#define RPC_TEST_NUM         0x38000022

const MAXNAME=20;
const MAXLENGTH=1024;

struct file_data {
    char f_name[MAXNAME];
    char *str;
};

typedef struct file_data file_data;

struct check_file_args {
    string file_name<>;
    string str<>;
    file_data _file_data<>;
};

program FILETRANSPROG {
    version FILETRANSVERS {
        string READFILE(string) = 1;
        int CHECK_FILE(check_file_args) = 2;
    } = 1;
} = RPC_TEST_NUM;

