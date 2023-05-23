#include "system_help.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static result_t system_help_append_command(help_t* help, help_cmd_t* cmd) {
    if(help->commands_count >= HELP_COMMANDS_MAX_COUNT) {
        error_printf("help commands register is full\n");
        return RESULT_ERROR;
    }

    memcpy(&help->commands_register[help->commands_count], cmd, sizeof(help_cmd_t));
    help->commands_count++;
    return RESULT_OK;
}

static void system_help_cmd_print(const help_cmd_t* cmd, const char* indentation) {
    if(!cmd) {
        error_printf("cmd is NULL\n");
        return;
    }

    info_printf("%s", indentation);

    switch(cmd->type) {
        case HELP_COMMAND_TYPE_KEYWORD: {
            const keyword_t* keyword = &cmd->keyword;
            info_printf("%s", keyword->text);
            break;
        }
        case HELP_COMMAND_TYPE_KEYWORDS_LIST: {
            const keywords_list_t* keywords_list = &cmd->keywords_list;
            for(int i = 0; i < keywords_list->count; ++i) {
                const keyword_t* keyword = &keywords_list->keywords[i];
                info_printf("%s", keyword->text);
                if(i < keywords_list->count - 1) {
                    info_printf(", ");
                }
            }

            break;
        }
        default: {
            error_printf("unknown command type\n");
            break;
        }
    }
    info_printf(" - %s\n", cmd->description);
}

result_t system_help_create_cmd_with_keyword(help_cmd_t* cmd, const keyword_t* keyword, const char* description) {
    if(!cmd) {
        error_printf("cmd is NULL\n");
        return RESULT_ERROR;
    }

    if(!keyword) {
        error_printf("keyword is NULL\n");
        return RESULT_ERROR;
    }

    if(!description) {
        error_printf("description is NULL\n");
        return RESULT_ERROR;
    }

    /* Description */
    memcpy(&cmd->keyword, keyword, sizeof(keyword_t));
    if(strlen(description) >= HELP_COMMAND_DESCRIPTION_MAX_LENGTH) {
        error_printf("description is too long\n");
        return RESULT_ERROR;
    }
    memset(cmd->description, 0, HELP_COMMAND_DESCRIPTION_BUFFER_SIZE);
    memcpy(cmd->description, description, strlen(description) + 1);

    /* Type */
    cmd->type = HELP_COMMAND_TYPE_KEYWORD;

    /* Clone keyword */
    memcpy(&cmd->keyword, keyword, sizeof(keyword_t));

    return RESULT_OK;
}

result_t system_help_create_cmd_with_keywords_list(help_cmd_t* cmd, const keywords_list_t* keywords_list, const char* description) {
    if(!cmd) {
        error_printf("cmd is NULL\n");
        return RESULT_ERROR;
    }

    if(!keywords_list) {
        error_printf("keywords list is NULL\n");
        return RESULT_ERROR;
    }

    if(!description) {
        error_printf("description is NULL\n");
        return RESULT_ERROR;
    }

    /* Description */
    memcpy(&cmd->keywords_list, keywords_list, sizeof(keywords_list_t));
    if(strlen(description) >= HELP_COMMAND_DESCRIPTION_MAX_LENGTH) {
        error_printf("description is too long\n");
        return RESULT_ERROR;
    }
    memset(cmd->description, 0, HELP_COMMAND_DESCRIPTION_BUFFER_SIZE);
    memcpy(cmd->description, description, strlen(description) + 1);

    /* Type */
    cmd->type = HELP_COMMAND_TYPE_KEYWORDS_LIST;

    /* Clone keywords list */
    memcpy(&cmd->keywords_list, keywords_list, sizeof(keywords_list_t));

    return RESULT_OK;
}



result_t system_help_init(help_t* help) {
    if(!help) {
        error_printf("help is NULL\n");
        return RESULT_ERROR;
    }
    memset(help, 0, sizeof(help_t));
    help->commands_count = 0;
    return RESULT_OK;
}

result_t system_help_append_command_with_keyword(help_t* help, const keyword_t* keyword, const char* description) {
    if(!help) {
        error_printf("help is NULL\n");
        return RESULT_ERROR;
    }

    if(!keyword) {
        error_printf("keyword is NULL\n");
        return RESULT_ERROR;
    }

    if(!description) {
        error_printf("description is NULL\n");
        return RESULT_ERROR;
    }
  
    help_cmd_t cmd;
    if(system_help_create_cmd_with_keyword(&cmd, keyword, description) != RESULT_OK) {
        error_printf("failed to create command\n");
        return RESULT_ERROR;
    }
    
    if(system_help_append_command(help, &cmd) != RESULT_OK) {
        error_printf("failed to append command\n");
        return RESULT_ERROR;
    }

    return RESULT_OK;
}


result_t system_help_append_command_with_keywords_list(help_t* help, const keywords_list_t* keywords_list, const char* description) {
        if(!help) {
        error_printf("help is NULL\n");
        return RESULT_ERROR;
    }

    if(!keywords_list) {
        error_printf("keyword is NULL\n");
        return RESULT_ERROR;
    }

    if(!description) {
        error_printf("description is NULL\n");
        return RESULT_ERROR;
    }
  
    help_cmd_t cmd;
    if(system_help_create_cmd_with_keywords_list(&cmd, keywords_list, description) != RESULT_OK) {
        error_printf("failed to create command\n");
        return RESULT_ERROR;
    }
    
    if(system_help_append_command(help, &cmd) != RESULT_OK) {
        error_printf("failed to append command\n");
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

result_t system_help_set_initial_text(help_t* help, const char* initial_text) {
    if(!help) {
        error_printf("help is NULL\n");
        return RESULT_ERROR;
    }

    if(!initial_text) {
        error_printf("initial text is NULL\n");
        return RESULT_ERROR;
    }

    if(strlen(initial_text) >= HELP_COMMAND_INITIAL_TEXT_MAX_LENGTH) {
        error_printf("initial text is too long\n");
        return RESULT_ERROR;
    }

    memcpy(help->initial_text, initial_text, strlen(initial_text) + 1);
    return RESULT_OK;
}

result_t system_help_set_ending_text(help_t* help, const char* ending_text) {
    if(!help) {
        error_printf("help is NULL\n");
        return RESULT_ERROR;
    }

    if(!ending_text) {
        error_printf("ending text is NULL\n");
        return RESULT_ERROR;
    }

    if(strlen(ending_text) >= HELP_COMMAND_ENDING_TEXT_MAX_LENGTH) {
        error_printf("ending text is too long\n");
        return RESULT_ERROR;
    }

    memcpy(help->ending_text, ending_text, strlen(ending_text) + 1);
    return RESULT_OK;
}

void system_help_print(const help_t* help) {
    if(!help) {
        error_printf("help is NULL\n");
        return;
    }

    info_printf("%s\n", help->initial_text);
    for(int i = 0; i < help->commands_count; ++i) {
        const help_cmd_t* cmd = &help->commands_register[i];
        system_help_cmd_print(cmd, " * ");
    }
    info_printf("%s\n", help->ending_text);
}


void system_help_print_hint(world_t* world, const player_t* player) {
    if(! world || !player) {
        return;
    }

    if(player_is_in_conversation(player) == BOOL_TRUE) {
        npc_t* npc_in_conversation = NULL;
        if(world_get_npc_by_id(world, &npc_in_conversation, player->current_conversation_npc_id) != OPTION_SOME) {
            return;
        }

        /* List all possible keywords */
        for(int i=0; i<npc_in_conversation->dialog_blocks_count; ++i) {
            //todo check conditions
            dialog_cond_if_t* cond_if = &npc_in_conversation->dialog_blocks[i].cond_if;
            if(system_dialog_match_dialog_stage(cond_if, npc_in_conversation) == OPTION_SOME) {
                keywords_list_printf(&cond_if->keywords, NULL);
            }
        }
        printf("\n");

    } else {
        info_printf("To start conversation type \'hi\' with NPC name.\n");
    }
}