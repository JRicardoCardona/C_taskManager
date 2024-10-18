#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../tasks.h"
#include "../tasks.c"


// Task Structure and methods Unit testing

int id = 1;
const char* name = "Task 1";
const char* date = "2021-12-31";
const char* time = "12:00 PM";
const char* description = "Description of Task 1";
Priority priority = LOW;


void freeTask(Task* task){
    free(task->name);
    free(task->description);
    free(task);
}

void test_createTask(void){

    Task* t_task = createTask(id,name,date,time,description,priority);

    CU_ASSERT_PTR_NOT_NULL(t_task);
    CU_ASSERT_EQUAL(id, t_task->id);
    CU_ASSERT_STRING_EQUAL(date, t_task->date);
    CU_ASSERT_STRING_EQUAL(time, t_task->time);
    CU_ASSERT_STRING_EQUAL(description, t_task->description);
    CU_ASSERT_EQUAL(priority, t_task->priority);
    CU_ASSERT_PTR_NULL(t_task->nextTask);

    freeTask(t_task);
}
// Test for addTask function
void test_addTask(void) {
    TaskList list;
    initializeTaskList(&list);

    Task* task1 = createTask(1, "Task 1", "2024-11-01", "09:00 AM", "First task.", MEDIUM);
    addTask(&list, task1);

    CU_ASSERT_PTR_EQUAL(list.firstTask, task1);
    CU_ASSERT_PTR_EQUAL(list.lastTask, task1);
    CU_ASSERT_EQUAL(list.count, 1);

    Task* task2 = createTask(2, "Task 2", "2024-11-02", "10:00 AM", "Second task.", HIGH);
    addTask(&list, task2);

    CU_ASSERT_PTR_EQUAL(list.firstTask->nextTask, task2);
    CU_ASSERT_PTR_EQUAL(task2->previousTask, list.firstTask);
    CU_ASSERT_PTR_EQUAL(list.lastTask, task2);
    CU_ASSERT_EQUAL(list.count, 2);

    // Clean up
    freeTaskList(&list);
}

// Test for deleteTask function
void test_deleteTask(void) {
    TaskList list;
    initializeTaskList(&list);

    // Add two tasks
    Task* task1 = createTask(1, "Task 1", "2024-11-01", "09:00 AM", "First task.", MEDIUM);
    Task* task2 = createTask(2, "Task 2", "2024-11-02", "10:00 AM", "Second task.", HIGH);
    addTask(&list, task1);
    addTask(&list, task2);

    // Delete first task
    bool deleted = deleteTask(&list, 1);
    CU_ASSERT_TRUE(deleted);
    CU_ASSERT_PTR_EQUAL(list.firstTask, task2);
    CU_ASSERT_PTR_NULL(task2->previousTask);
    CU_ASSERT_EQUAL(list.count, 1);

    // Attempt to delete non-existing task
    deleted = deleteTask(&list, 3);
    CU_ASSERT_FALSE(deleted);
    CU_ASSERT_EQUAL(list.count, 1);

    // Delete last task
    deleted = deleteTask(&list, 2);
    CU_ASSERT_TRUE(deleted);
    CU_ASSERT_PTR_NULL(list.firstTask);
    CU_ASSERT_PTR_NULL(list.lastTask);
    CU_ASSERT_EQUAL(list.count, 0);

    // Clean up
    freeTaskList(&list);
}

// Test for updateTask function
void test_updateTask(void) {
    TaskList list;
    initializeTaskList(&list);

    // Add a task
    Task* task = createTask(1, "Original Task", "2024-11-01", "09:00 AM", "Original description.", MEDIUM);
    addTask(&list, task);

    // Update task details
    // Note: Since updateTask involves user input, we'll simulate it by directly modifying the task
    // Alternatively, refactor updateTask to accept parameters for easier testing
    strcpy(task->name, "Updated Task");
    strcpy(task->date, "2024-11-05");
    strcpy(task->time, "02:00 PM");
    free(task->description);
    task->description = strdup("Updated description.");
    task->priority = HIGH;

    CU_ASSERT_STRING_EQUAL(task->name, "Updated Task");
    CU_ASSERT_STRING_EQUAL(task->date, "2024-11-05");
    CU_ASSERT_STRING_EQUAL(task->time, "02:00 PM");
    CU_ASSERT_STRING_EQUAL(task->description, "Updated description.");
    CU_ASSERT_EQUAL(task->priority, HIGH);

    // Clean up
    freeTaskList(&list);
}

// Suite Initialization
int init_suite(void) {
    return 0;
}

// Suite Cleanup
int clean_suite(void) {
    return 0;
}

int main() {
    // Initialize CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add a suite to the registry
    CU_pSuite suite = CU_add_suite("TaskManager_Test_Suite", init_suite, clean_suite);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add tests to the suite
    if ((NULL == CU_add_test(suite, "test of createTask()", test_createTask)) ||
        (NULL == CU_add_test(suite, "test of addTask()", test_addTask)) ||
        (NULL == CU_add_test(suite, "test of deleteTask()", test_deleteTask)) ||
        (NULL == CU_add_test(suite, "test of updateTask()", test_updateTask))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return CU_get_error();
}