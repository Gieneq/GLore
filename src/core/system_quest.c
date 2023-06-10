#include "system_quest.h"



/**
 * Check if player fullfills quest related requirements
 * 
 * @return OPTIO_SOME if condition_if has quest requirement
 * @param is_matching - BOOL_TRUE if player fullfills quest related requirements
 * 
 * If return OPTION_NONE then requirements are not counted
*/
option_t system_quest_get_cond_if_mach_player_questlog(const dialog_cond_if_t* condition_if, player_t* player, bool_t* is_maching) {

    /* Check if cond_if has quest related requirements */
    if(dialog_cond_if_has_quest_reqirement(condition_if) == BOOL_TRUE) {
        /* Cond_if has some quest requirement */

        const quest_stage_t* required_quest_stage = &condition_if->quest_stage;
        questlog_t* players_questlog = &player->questlog;

        /* Check if player has the same quest in questlog */
        int questlog_stage;
        if(questlog_get_quest_stage(players_questlog, required_quest_stage->quest_id, &questlog_stage) == OPTION_SOME) {
            /* Check if questlog has the same quest stage */
            if(required_quest_stage->stage == questlog_stage) {
                *is_maching = BOOL_TRUE;
            } else {
                *is_maching = BOOL_FALSE;
            }
        } 
        /* No, player doesn't have this quest in questlog */
        else {
            *is_maching = BOOL_FALSE;
        }

        return OPTION_SOME;
    }

    /* Cond_if has nothing about quest */
    *is_maching = BOOL_FALSE;
    return OPTION_NONE;
}


/**
 * Fullfills quest stage effects/rewards
 * 
 * @return OPTIO_SOME if condition_then has quest effects/rewards
 * @param was_fullfilled - BOOL_TRUE if effects/rewards were given
 * 
 * If return OPTION_NONE then effects are not counted
*/
option_t system_quest_execute_cond_then_match_player_questlog(const dialog_cond_then_t* condition_then, player_t* player, bool_t* was_fullfilled) {

    /* Check if cond_then has quest related requirements */
    if(dialog_cond_then_has_quest_update(condition_then) == BOOL_TRUE) {
        /* Cond_then has some quest requirement */

        const quest_stage_t* quest_stage_update = &condition_then->update_quest_stage;
        questlog_t* players_questlog = &player->questlog;

        /* Check if player has the same quest in questlog */
        int questlog_stage;
        if(questlog_get_quest_stage(players_questlog, quest_stage_update->quest_id, &questlog_stage) == OPTION_SOME) {
            /* Update questlog */
            option_t result = questlog_update(players_questlog, quest_stage_update->quest_id, quest_stage_update->stage);
            UNUSED(result); //it should be updated - see outer condition 
            debug_printf("Questlog %d has been updated %d -> %d", quest_stage_update->quest_id, questlog_stage, quest_stage_update->stage);
            *was_fullfilled = BOOL_TRUE;
        } 
        /* No, player doesn't have this quest in questlog */
        else {
            *was_fullfilled = BOOL_FALSE;
            /* But... if player just started quest, it can be missing */
            if(quest_stage_update->stage == 1) {
                if(questlog_append_quest(players_questlog, quest_stage_update->quest_id, quest_stage_update->stage) != RESULT_OK) {
                    error_printf("Couldn't add new quest to questlog");
                } else {
                    *was_fullfilled = BOOL_TRUE;
                }
            }
        }
        return OPTION_SOME;
    }

    /* Cond_then has nothing about quest */
    *was_fullfilled = BOOL_FALSE;
    return OPTION_NONE;
}