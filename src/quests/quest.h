#pragma once
#include "item.h"
#include "utils.h"

//QUEST_STAGE_BRIEF_BUFFER_SIZE
//change all create to init

#define QUEST_NAME_BUFFER_SIZE 32
#define QUEST_NAME_MAX_LENGTH (QUEST_NAME_BUFFER_SIZE - 1)
#define QUEST_BRIEF_BUFFER_SIZE 128
#define QUEST_BRIEF_MAX_LENGTH (QUEST_BRIEF_BUFFER_SIZE - 1)
#define QUEST_OBJECTIVE_BUFFER_SIZE 128
#define QUEST_OBJECTIVE_MAX_LENGTH (QUEST_OBJECTIVE_BUFFER_SIZE - 1)

#define QUEST_STAGES_MAX_COUNT 16
#define QUEST_REQUIREMENTS_ITEMS_MAX_COUNT 4
#define QUEST_REWARDS_ITEMS_MAX_COUNT 4

/* Quest ID */
typedef size_t quest_id_t;

/* Questlog */
typedef struct questlog_t {
    quest_id_t quest_id;
    int stage;
} questlog_t;

void questlog_create(questlog_t *questlog, quest_id_t quest_id);
//todo questlog get finished quests: need to share data from database


/* Quest Data */
typedef struct quest_requirements_t {
    item_stack_t items[QUEST_REQUIREMENTS_ITEMS_MAX_COUNT];
    size_t count;
} quest_requirements_t;

void quest_requirements_create(quest_requirements_t *requirements);
result_t quest_requirements_add_item(quest_requirements_t *requirements, int item_id, size_t count);

typedef struct quest_reward_t {
    item_stack_t items[QUEST_REWARDS_ITEMS_MAX_COUNT];
    size_t count;
    int exp;
} quest_reward_t;

void quest_reward_create(quest_reward_t *reward);
void quest_reward_set_exp(quest_reward_t *reward, int exp);
result_t quest_reward_add_item(quest_reward_t *reward, int item_id, size_t count);

typedef struct quest_stage_data_t {
    size_t stage_no;
    char objective[QUEST_OBJECTIVE_BUFFER_SIZE];
    quest_requirements_t requirements;
    int returning_npc_id;
    quest_reward_t reward;
} quest_stage_data_t;

void quest_stage_data_create(quest_stage_data_t *stage_data);

typedef struct quest_data_t {
    quest_id_t quest_id;
    char name[QUEST_NAME_BUFFER_SIZE];
    char brief[QUEST_BRIEF_BUFFER_SIZE];
    int starting_npc_id;
    quest_stage_data_t stages[QUEST_STAGES_MAX_COUNT];
    int stages_count;
} quest_data_t;

result_t quest_data_create(quest_data_t *quest_data, quest_id_t quest_id, const char *name, const char *brief, const int starting_npc_id);
result_t quest_data_add_stage(quest_data_t *quest_data, quest_stage_data_t *stage_data);

/* Printf functions */
void quest_requirements_printf(quest_requirements_t *requirements);
void quest_reward_printf(quest_reward_t *reward);
void quest_stage_data_printf(quest_stage_data_t *stage_data);
void quest_data_printf(quest_data_t *quest_data);

result_t quest_data_printf_attach_db(void* db);
//todo detach