/*#############################################################
 *     File Name	: run-parser.c
 *     Author		: winddoing
 *     Created Time	: 2020年09月10日 星期四 09时55分21秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <yaml.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

int main(int argc, char *argv[])
{
    int number;

    if (argc < 2) {
        printf("Usage: %s file1.yaml ...\n", argv[0]);
        return 0;
    }

    for (number = 1; number < argc; number ++)
    {
        FILE *file;
        yaml_parser_t parser;
        yaml_event_t event;
        int done = 0;
        int count = 0;
        int error = 0;

        printf("[%d] Parsing '%s': ", number, argv[number]);
        fflush(stdout);

        file = fopen(argv[number], "rb");
        assert(file);

        assert(yaml_parser_initialize(&parser));

        yaml_parser_set_input_file(&parser, file);

        while (!done) {
            yaml_event_type_t type;

            if (!yaml_parser_parse(&parser, &event)) {
                error = 1;
                break;
            }
            printf("----event type: %d\n", event.type);
            type = event.type;

            if (type == YAML_MAPPING_START_EVENT) {
                printf("+MAP");
                int flow = 0;
                if (flow == 0 && event.data.mapping_start.style == YAML_FLOW_MAPPING_STYLE)
                        printf(" {}");
                else if (flow == 1)
                        printf(" {}");
                if (event.data.mapping_start.anchor)
                        printf(" &%s", event.data.mapping_start.anchor);
                if (event.data.mapping_start.tag)
                        printf(" <%s>", event.data.mapping_start.tag);
                printf("\n");
            }

            if (type == YAML_ALIAS_EVENT)                 
                printf("=ALI *%s\n", event.data.alias.anchor); 


            done = (event.type == YAML_STREAM_END_EVENT);

            yaml_event_delete(&event);

            count ++;
        }

        yaml_parser_delete(&parser);

        assert(!fclose(file));

        printf("%s (%d events)\n", (error ? "FAILURE" : "SUCCESS"), count);
    }

    return 0;
}
