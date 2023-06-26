#include "system_items.h"

void system_items_container_print(item_database_t* item_database, const container_t* container, const char* ending) {
    info_printf("Container %d " , container->capacity);
    if (container_is_empty(container) == BOOL_TRUE) {
        printf("is empty.\n");
        return;
    }

    info_printf("slots:\n");

    /* Iterate over all occupied slots, items_count variable name is confusing */
    for (int i = 0; i < container->item_count; i++) {
        const item_t* item;
        item_data_t* item_data;

        item = &container->items[i];

        if(item_database_get_item_data_by_id(item_database, &item_data, item->id) == OPTION_NONE) {
            info_printf("- corrupted [%d]", item->id);
        } else {
            info_printf("- %s [%d]", item_data->name, item->id);
        }

        if(item_data_is_stackable(item_data) == BOOL_TRUE) {
            printf(" x%d", item->count);
        }

        if(i < container->item_count - 1) {
            printf(",%s",ending);
        } else {
            printf(".%s",ending);
        }
    }
}