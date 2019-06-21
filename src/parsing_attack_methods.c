#include "parsing.h"
#include "log.h"
#include "mon_attack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* c_attack_methods_file_name = "data/attack_methods.txt";

parsing_callback(_parse_attack_methods_finalise);
parsing_callback(_parse_attack_method_name_callback);
parsing_callback(_parse_attack_method_msg1_callback);
parsing_callback(_parse_attack_method_msg2_callback);

enum ParserCode parse_attack_methods(void)
{
    struct Parser* parser = parser_new();

    parser_register_field(parser, "name", "name string", &_parse_attack_method_name_callback);
    parser_register_field(parser, "msg-1", "msg string", &_parse_attack_method_msg1_callback);
    parser_register_field(parser, "msg-2", "msg string", &_parse_attack_method_msg2_callback);

    if(open_file_and_parse_all(parser, c_attack_methods_file_name))
    {
        _parse_attack_methods_finalise(parser);
    }

    enum ParserCode retval = parser_get_last_code(parser);
    parser_free(parser);
    return retval;
}

parsing_callback(_parse_attack_methods_finalise)
{
    List* attack_methods_data = parser_get_userdata(parser);
    ListNode* node;

    log_format_msg(DEBUG, "Parsed attack types count: %d", attack_methods_data->count);

    g_attack_methods = malloc(sizeof(struct AttackMethod) * attack_methods_data->count);
    g_attack_methods_count = attack_methods_data->count;

    int idx = 0;
    list_for_each(attack_methods_data, node)
    {
        memcpy(&g_attack_methods[idx++], node->data, sizeof(struct AttackMethod));
        free(node->data);
    }

    return PARSE_CALLBACK_OK;
}

parsing_callback(_parse_attack_method_name_callback)
{
    struct AttackMethod* type = malloc(sizeof(struct AttackMethod));
    memset(type, 0, sizeof(struct AttackMethod));

    parser_set_userdata(parser, type);
    snprintf(type->name, sizeof(type->name), "%s", parser_field_get_string(parser, "name", "name"));
    return PARSE_CALLBACK_OK;
}

parsing_callback(_parse_attack_method_msg1_callback)
{
    struct AttackMethod* type = parser_get_userdata_active(parser);
    snprintf(type->msg1, sizeof(type->msg1), "%s", parser_field_get_string(parser, "msg-1", "msg"));
    return PARSE_CALLBACK_OK;
}

parsing_callback(_parse_attack_method_msg2_callback)
{
    struct AttackMethod* type = parser_get_userdata_active(parser);
    snprintf(type->msg2, sizeof(type->msg2), "%s", parser_field_get_string(parser, "msg-2", "msg"));
    return PARSE_CALLBACK_OK;
}