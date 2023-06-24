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
    
    /* Add 1 item to last stack */
    result_t add_ret_4 = container_add_item(&container, &item_stack_1, &item_data);
    EXPECT_EQ(add_ret_4, RESULT_OK);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 33);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);
    EXPECT_EQ(container.items[0].count, 16);
    EXPECT_EQ(container.items[1].count, 16);
    EXPECT_EQ(container.items[2].count, 1);
}

TEST(ContainerTest, ContainerStackableRemoval) {
    const int capacity = 3;
    container_t container;
    container_init(&container, capacity);

    item_data_t item_data;
    result_t creation_ret = item_data_create(&item_data, 1, "sth", 16);
    EXPECT_EQ(creation_ret, RESULT_OK);
    EXPECT_EQ(item_data_is_stackable(&item_data), BOOL_TRUE);

    item_t item_stack_1;
    item_create(&item_stack_1, item_data.id, 1);

    /* Add stacks to container */
    result_t add_ret_1 = container_add_item(&container, &item_stack_1, &item_data);
    EXPECT_EQ(add_ret_1, RESULT_OK);

    /* Removal */
    result_t remove_ret_1 = container_remove_item_by_id(&container, &item_stack_1, &item_data);
    EXPECT_EQ(remove_ret_1, RESULT_OK);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 0);
    EXPECT_EQ(container_is_empty(&container), BOOL_TRUE);

    result_t remove_ret_2 = container_remove_item_by_id(&container, &item_stack_1, &item_data);
    EXPECT_EQ(remove_ret_2, RESULT_ERROR);
    EXPECT_EQ(container_count_item(&container, item_stack_1.id), 0);
    EXPECT_EQ(container_is_empty(&container), BOOL_TRUE);
}

/* Automatic spaces removal procedure */
TEST(ContainerTest, ContainerGapRemoval) {
    const int capacity = 4;
    container_t container;
    container_init(&container, capacity);

    item_data_t item_data_st_a;
    item_data_t item_data_st_b;
    item_data_t item_data_nst_c;
    item_data_t item_data_nst_d;

    item_data_create(&item_data_st_a, 1, "a", 6);
    item_data_create(&item_data_st_b, 2, "b", 6);
    item_data_create(&item_data_nst_c, 3, "c", ITEM_NOT_STACKABLE);
    item_data_create(&item_data_nst_d, 4, "d", ITEM_NOT_STACKABLE);

    item_t item_a = {item_data_st_a.id, 3};
    item_t item_b = {item_data_st_b.id, 2};
    item_t item_c = {item_data_nst_c.id, ITEM_NOT_STACKABLE};
    item_t item_d = {item_data_nst_d.id, ITEM_NOT_STACKABLE};

    /* Add items in specyfic order */
    EXPECT_EQ(container_add_item(&container, &item_a, &item_data_st_a), RESULT_OK);
    EXPECT_EQ(container_add_item(&container, &item_c, &item_data_nst_c), RESULT_OK);
    EXPECT_EQ(container_add_item(&container, &item_d, &item_data_nst_d), RESULT_OK);
    EXPECT_EQ(container_add_item(&container, &item_b, &item_data_st_b), RESULT_OK);
    
    /* Full for stackable */
    EXPECT_EQ(container_add_item(&container, &item_d, &item_data_nst_d), RESULT_ERROR);
    EXPECT_EQ(container_add_item(&container, &item_c, &item_data_nst_c), RESULT_ERROR);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);
    
    EXPECT_EQ(container_count_item(&container, item_a.id), 3);
    EXPECT_EQ(container_count_item(&container, item_b.id), 2);
    EXPECT_EQ(container_count_item(&container, item_c.id), 1);
    EXPECT_EQ(container_count_item(&container, item_d.id), 1);

    /* Can add some stackables */
    EXPECT_EQ(container_add_item(&container, &item_a, &item_data_st_a), RESULT_OK);
    EXPECT_EQ(container_add_item(&container, &item_b, &item_data_st_b), RESULT_OK);

    /* Stack is full */
    EXPECT_EQ(container_add_item(&container, &item_a, &item_data_st_a), RESULT_ERROR);

    /* Check order */
    EXPECT_EQ(container.items[0].count, 6);
    EXPECT_EQ(container.items[0].id, item_data_st_a.id);
    EXPECT_EQ(container.items[1].count, 1);
    EXPECT_EQ(container.items[1].id, item_data_nst_c.id);
    EXPECT_EQ(container.items[2].count, 1);
    EXPECT_EQ(container.items[2].id, item_data_nst_d.id);
    EXPECT_EQ(container.items[3].count, 4);
    EXPECT_EQ(container.items[3].id, item_data_st_b.id);

    /* Remove nonstackable from middle */
    EXPECT_EQ(container_remove_item_by_id(&container, &item_c, &item_data_nst_c), RESULT_OK);

    /* There should be no gap */
    EXPECT_EQ(container.items[0].count, 6);
    EXPECT_EQ(container.items[0].id, item_data_st_a.id);
    EXPECT_EQ(container.items[1].count, 1); // moved from slot 2 after removal
    EXPECT_EQ(container.items[1].id, item_data_nst_d.id);
    EXPECT_EQ(container.items[2].count, 4);
    EXPECT_EQ(container.items[2].id, item_data_st_b.id);
    
    EXPECT_EQ(container.items[3].id, INVALID_ID);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);

    /* Add at the end nonstackable */
    EXPECT_EQ(container_add_item(&container, &item_d, &item_data_nst_d), RESULT_OK);
    EXPECT_EQ(container_add_item(&container, &item_d, &item_data_nst_d), RESULT_ERROR);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);
    
    /* Remove stackable from middle */
    EXPECT_EQ(container_remove_item_by_id(&container, &item_b, &item_data_st_b), RESULT_OK);
    EXPECT_EQ(container.items[2].count, 2);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_FALSE);
    EXPECT_EQ(container_remove_item_by_id(&container, &item_b, &item_data_st_b), RESULT_OK);
    EXPECT_EQ(container_has_free_slot(&container), BOOL_TRUE);
    
    EXPECT_EQ(container.items[0].count, 6);
    EXPECT_EQ(container.items[0].id, item_data_st_a.id);
    EXPECT_EQ(container.items[1].count, 1);
    EXPECT_EQ(container.items[1].id, item_data_nst_d.id);
    EXPECT_EQ(container.items[2].count, 1); // moved from slot 3 after removal
    EXPECT_EQ(container.items[2].id, item_data_nst_d.id);
    
    EXPECT_EQ(container.items[3].id, INVALID_ID);
}



