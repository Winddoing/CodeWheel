
struct clientinput {
    string optype<>;
    string word<>;
};

program DICTIONARY_PROG {
    version DICTIONARY_VERS {
        int USEDICTIONARY(clientinput) = 1;
    } = 1;
} = 0x23451113;

