// tasks.h

#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Enum for task priority
typedef enum {
    LOW = 1,
    MEDIUM,
    HIGH,
    CRITICAL
} Priority;

// Forward declaration of Task
typedef struct Task Task;

// Definition of Task structure
struct Task {
    int id;
    char* name;
    char date[50];
    char time[15];
    char* description;
    Priority priority;
    Task* nextTask;
    Task* previousTask;
};

// Definition of TaskList structure (Doubly Linked List)
typedef struct {
    Task* firstTask;  // Pointer to the first task in the list (head)
    Task* lastTask;   // Pointer to the last task in the list (tail)
    size_t count;     // Number of tasks currently stored
} TaskList;

// Function Prototypes
void initializeTaskList(TaskList* list);
Task* createTask(int id, const char* name, const char* date, const char* time, const char* description, Priority priority);
void addTask(TaskList* list, Task* newTask);
void listTasks(const TaskList* list);
bool deleteTask(TaskList* list, int id);
bool updateTask(TaskList* list, int id);
void freeTaskList(TaskList* list);
bool saveTasksToFile(const TaskList* list, const char* filename);
bool loadTasksFromFile(TaskList* list, const char* filename);
void clearInputBuffer(void);

#endif // TASKS_H
