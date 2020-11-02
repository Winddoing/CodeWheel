enum color {ORANGE, PUCE, TURQUOISE};

struct list {
    string data<>;
    int key;
    color col;
    list *next;
};

program PRINTER {
    version PRINTER_V1 {
            int PRINT_LIST(list) = 1;
            int SUM_LIST(list) = 2;
        } = 1;
} = 0x2fffffff;
