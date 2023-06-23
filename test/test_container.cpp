#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
#include "container.h"
}

/* Container */
TEST(ContainerTest, ContainerCreation) {
    const int capacity = 2;
    container_t container;
    container_init(&container, capacity);

    EXPECT_EQ(container_is_empty(&container), BOOL_TRUE);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);
    EXPECT_EQ(container.item_count, 0);
    EXPECT_EQ(container.capacity, capacity);
    EXPECT_EQ(container.items[0].id, INVALID_ID);
}

TEST(ContainerTest, ContainerNotStackable) {
    const int capacity = 2;
    container_t container;
    container_init(&container, capacity);

    item_data_t item_data;
    result_t creation_ret = item_data_create(&item_data, 1, "notstackablejunk", ITEM_NOT_STACKABLE);
    EXPECT_EQ(creation_ret, RESULT_OK);

    item_t item;
    item_create(&item, item_data.id, ITEM_NOT_STACKABLE);

    EXPECT_EQ(item_data_is_stackable(&item_data), BOOL_FALSE);

    result_t add_ret_1 = container_add_item(&container, &item, &item_data);
    EXPECT_EQ(add_ret_1, RESULT_OK);
    result_t add_ret_2 = container_add_item(&container, &item, &item_data);
    EXPECT_EQ(add_ret_2, RESULT_OK);
    result_t add_ret_3 = container_add_item(&container, &item, &item_data);
    EXPECT_EQ(add_ret_3, RESULT_ERROR);

    EXPECT_EQ(container_contains_item(&container, item.id), BOOL_TRUE);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);

    /* Not exists */
    item_t item_not_exists = { 122, ITEM_NOT_STACKABLE };
    result_t remove_ret_err_1 = container_remove_item_by_id(&container, &item_not_exists, &item_data);
    EXPECT_EQ(remove_ret_err_1, RESULT_ERROR);
    
    /* Invalid id */
    item_t item_invalid = { INVALID_ID, ITEM_NOT_STACKABLE };
    result_t remove_ret_err_2 = container_remove_item_by_id(&container, &item_not_exists, &item_data);
    EXPECT_EQ(remove_ret_err_2, RESULT_ERROR);

    /* Normal removal */
    result_t remove_ret_1 = container_remove_item_by_id(&container, &item, &item_data);
    EXPECT_EQ(remove_ret_1, RESULT_OK);
    EXPECT_EQ(container_contains_item(&container, item.id), BOOL_TRUE);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);
    EXPECT_EQ(container.item_count, 1);
    EXPECT_EQ(container_count_item(&container, item.id), 1);


    result_t remove_ret_2 = container_remove_item_by_id(&container, &item, &item_data);
    EXPECT_EQ(remove_ret_2, RESULT_OK);
    EXPECT_EQ(container_contains_item(&container, item.id), BOOL_FALSE);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);
    EXPECT_EQ(container.item_count, 0);
    EXPECT_EQ(container_count_item(&container, item.id), 0);

    /* Empty */
    result_t remove_ret_3 = container_remove_item_by_id(&container, &item, &item_data);
    EXPECT_EQ(remove_ret_3, RESULT_ERROR);
    EXPECT_EQ(container_is_empty(&container), BOOL_TRUE);
    EXPECT_EQ(container_contains_item(&container, item.id), BOOL_FALSE);
    EXPECT_EQ(container_count_item(&container, item.id), 0);
}


/* Stackable items */

TEST(ContainerTest, ContainerStackable) {
    const int capacity = 3;
    container_t container;
    container_init(&container, capacity);

    item_data_t item_data;
    result_t creation_ret = item_data_create(&item_data, 1, "stackablegoods", 16);
    EXPECT_EQ(creation_ret, RESULT_OK);
    EXPECT_EQ(item_data_is_stackable(&item_data), BOOL_TRUE);

    item_t item_stack_1;
    item_create(&item_stack_1, item_data.id, 1);
    item_t item_stack_15;
    item_create(&item_stack_15, item_data.id, 15);
    item_t item_stack_16;
    item_create(&item_stack_16, item_data.id, 16);

    /* Add stacks to container */
    result_t add_ret_1 = container_add_item(&container, &item_stack_1, &item_data);
    EXPECT_EQ(add_ret_1, RESULT_OK);
    result_t add_ret_2 = container_add_item(&container, &item_stack_15, &item_data);
    EXPECT_EQ(add_ret_2, RESULT_OK);
    EXPECT_EQ(container_contains_item(&container, item_stack_1.id), BOOL_TRUE);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 16);

    
    result_t add_ret_3 = container_add_item(&container, &item_stack_16, &item_data);
    EXPECT_EQ(add_ret_3, RESULT_OK);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 32);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);
    
    result_t add_ret_4 = container_add_item(&container, &item_stack_1, &item_data);
    EXPECT_EQ(add_ret_4, RESULT_OK);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 33);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);
    
    /* Removal */

    // result_t remove_ret_1 = container_remove_item_by_id(&container, &item_stack_16, &item_data);
    // EXPECT_EQ(remove_ret_1, RESULT_OK);
    // result_t remove_ret_2 = container_remove_item_by_id(&container, &item_stack_16, &item_data);
    // EXPECT_EQ(remove_ret_2, RESULT_OK);
    // result_t remove_ret_3 = container_remove_item_by_id(&container, &item_stack_1, &item_data);
    // EXPECT_EQ(remove_ret_3, RESULT_OK);
    // EXPECT_EQ(container_count_item(&container, item_stack_1.id), 0);
    // EXPECT_EQ(container_is_empty(&container), BOOL_TRUE);
}

/* Automatic spaces removal procedure */