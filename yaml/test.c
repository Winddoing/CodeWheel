#include <stdio.h>
#include <yaml.h>

typedef struct _conf {
    char* db_server;
    char* db_pass;
    char* db_user;
    char* rail_user;
    char* rail_pass;
}Conf;


int readConf(char* filename, Conf* conf)
{
    FILE* fh = NULL;
    char** datap = NULL;
    char* tk = NULL;

    int state = 0;

    yaml_parser_t parser;
    yaml_token_t token;

    fh = fopen(filename, "r");
    if (fh == NULL)
        fputs("Failed to open file!\n", stderr);

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);

    yaml_parser_set_input_file(&parser, fh);

    do {
        /* As this is an example, I'll just use:
         *  state = 0 = expect key
         *  state = 1 = expect value
         */

        yaml_parser_scan(&parser, &token);

        switch(token.type) {
        case YAML_KEY_TOKEN:
            state = 0; break;
        case YAML_VALUE_TOKEN:
            state = 1; break;
        case YAML_SCALAR_TOKEN:
            tk = token.data.scalar.value;
            if (state == 0) {
                /* It's safe to not use strncmp as one string is a literal */
                if (!strcmp(tk, "db_server")) {
                    datap = &conf->db_server;
                } else if (!strcmp(tk, "db_password")) {
                    datap = &conf->db_pass;
                } else if (!strcmp(tk, "db_username")) {
                    datap = &conf->db_user;
                } else if (!strcmp(tk, "national_rail_username")) {
                    datap = &conf->rail_user;
                } else if (!strcmp(tk, "national_rail_password")) {
                    datap = &conf->rail_pass;
                } else {
                    printf("Unrecognised key: %s\n", tk);
                }
            } else {
                *datap = strdup(tk);
            }
            break;
        default:
            break;
        }
        if (token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);

    } while (token.type != YAML_STREAM_END_TOKEN);

    yaml_token_delete(&token);
    yaml_parser_delete(&parser);

    fclose(fh);

    return 0;
}

int main(int argc, const char *argv[])
{
    Conf* con = NULL;
    int ret = 0;

    con = malloc(sizeof(Conf));

    ret = readConf("tst.yaml", con);

    printf("db_server=%s, db_pass=%s, db_user=%s, rail_user=%s, rail_pass=%s\n",
            con->db_server, con->db_pass, con->db_user,
            con->rail_user, con->rail_pass);

    free(con);

    return 0;
}
