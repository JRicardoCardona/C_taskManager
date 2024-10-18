// tasks.c

#include "tasks.h"

// Function to initialize the TaskList
void initializeTaskList(TaskList* list) {
    list->firstTask = NULL;
    list->lastTask = NULL;
    list->count = 0;
}

// Function to create a new Task
Task* createTask(int id, const char* name, const char* date, const char* time, const char* description, Priority priority) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (newTask == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for new task.\n");
        exit(EXIT_FAILURE);
    }

    newTask->id = id;

    // Allocate and copy the name
    newTask->name = strdup(name);
    if (newTask->name == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for task name.\n");
        free(newTask);
        exit(EXIT_FAILURE);
    }

    // Copy date and ensure null-termination
    strncpy(newTask->date, date, sizeof(newTask->date) - 1);
    newTask->date[sizeof(newTask->date) - 1] = '\0';

    // Copy time and ensure null-termination
    strncpy(newTask->time, time, sizeof(newTask->time) - 1);
    newTask->time[sizeof(newTask->time) - 1] = '\0';

    // Allocate and copy the description
    newTask->description = strdup(description);
    if (newTask->description == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for task description.\n");
        free(newTask->name);
        free(newTask);
        exit(EXIT_FAILURE);
    }

    newTask->priority = priority;
    newTask->nextTask = NULL;
    newTask->previousTask = NULL;

    return newTask;
}

// Function to add a Task to the TaskList (appends to the end)
void addTask(TaskList* list, Task* newTask) {
    if (list->firstTask == NULL) {
        // List is empty; new task becomes the first and last task
        list->firstTask = newTask;
        list->lastTask = newTask;
    } else {
        // Append to the end of the list
        list->lastTask->nextTask = newTask;
        newTask->previousTask = list->lastTask;
        list->lastTask = newTask;
    }
    list->count++;
}

// Function to list all tasks
void listTasks(const TaskList* list) {
    if (list->firstTask == NULL) {
        printf("No tasks available.\n");
        return;
    }

    Task* current = list->firstTask;
    while (current != NULL) {
        printf("Task ID: %d\n", current->id);
        printf("Name: %s\n", current->name);
        printf("Date: %s\n", current->date);
        printf("Time: %s\n", current->time);
        printf("Description: %s\n", current->description);
        printf("Priority: ");
        switch (current->priority) {
            case LOW:
                printf("Low\n");
                break;
            case MEDIUM:
                printf("Medium\n");
                break;
            case HIGH:
                printf("High\n");
                break;
            case CRITICAL:
                printf("Critical\n");
                break;
            default:
                printf("Unknown\n");
        }
        printf("-------------------------\n");
        current = current->nextTask;
    }
}

// Function to delete a task by ID
bool deleteTask(TaskList* list, int id) {
    Task* current = list->firstTask;
    while (current != NULL) {
        if (current->id == id) {
            if (current->previousTask != NULL) {
                current->previousTask->nextTask = current->nextTask;
            } else {
                // Deleting the first task
                list->firstTask = current->nextTask;
            }

            if (current->nextTask != NULL) {
                current->nextTask->previousTask = current->previousTask;
            } else {
                // Deleting the last task
                list->lastTask = current->previousTask;
            }

            free(current->name);
            free(current->description);
            free(current);
            list->count--;
            return true;
        }
        current = current->nextTask;
    }
    return false; // Task not found
}

// Function to update a task by ID
bool updateTask(TaskList* list, int id) {
    Task* current = list->firstTask;
    while (current != NULL) {
        if (current->id == id) {
            printf("Updating Task ID: %d\n", id);

            // Update Name
            char name[100];
            printf("Enter new name (leave blank to keep unchanged): ");
            fgets(name, sizeof(name), stdin);
            if (name[0] != '\n') {
                name[strcspn(name, "\n")] = '\0'; // Remove newline
                free(current->name);
                current->name = strdup(name);
                if (current->name == NULL) {
                    fprintf(stderr, "Error: Unable to allocate memory for task name.\n");
                    return false;
                }
            }

            // Update Date
            char date[50];
            printf("Enter new date (YYYY-MM-DD) (leave blank to keep unchanged): ");
            fgets(date, sizeof(date), stdin);
            if (date[0] != '\n') {
                date[strcspn(date, "\n")] = '\0'; // Remove newline
                strncpy(current->date, date, sizeof(current->date) - 1);
                current->date[sizeof(current->date) - 1] = '\0';
            }

            // Update Time
            char time[15];
            printf("Enter new time (HH:MM AM/PM) (leave blank to keep unchanged): ");
            fgets(time, sizeof(time), stdin);
            if (time[0] != '\n') {
                time[strcspn(time, "\n")] = '\0'; // Remove newline
                strncpy(current->time, time, sizeof(current->time) - 1);
                current->time[sizeof(current->time) - 1] = '\0';
            }

            // Update Description
            char description[256];
            printf("Enter new description (leave blank to keep unchanged): ");
            fgets(description, sizeof(description), stdin);
            if (description[0] != '\n') {
                description[strcspn(description, "\n")] = '\0'; // Remove newline
                free(current->description);
                current->description = strdup(description);
                if (current->description == NULL) {
                    fprintf(stderr, "Error: Unable to allocate memory for task description.\n");
                    return false;
                }
            }

            // Update Priority
            char priorityInput[10];
            printf("Enter new priority (1=Low, 2=Medium, 3=High, 4=Critical) (leave blank to keep unchanged): ");
            fgets(priorityInput, sizeof(priorityInput), stdin);
            if (priorityInput[0] != '\n') {
                int priorityVal = atoi(priorityInput);
                if (priorityVal >= LOW && priorityVal <= CRITICAL) {
                    current->priority = (Priority)priorityVal;
                } else {
                    printf("Invalid priority value. Keeping previous priority.\n");
                }
            }

            printf("Task updated successfully.\n");
            return true;
        }
        current = current->nextTask;
    }
    return false; // Task not found
}

// Function to free all allocated memory in the TaskList
void freeTaskList(TaskList* list) {
    Task* current = list->firstTask;
    while (current != NULL) {
        Task* temp = current;
        current = current->nextTask;
        free(temp->name);
        free(temp->description);
        free(temp);
    }
    list->firstTask = NULL;
    list->lastTask = NULL;
    list->count = 0;
}

// Function to save tasks to a CSV file
bool saveTasksToFile(const TaskList* list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s' for writing.\n", filename);
        return false;
    }

    Task* current = list->firstTask;
    while (current != NULL) {
        fprintf(file, "%d,\"%s\",\"%s\",\"%s\",\"%s\",%d\n",
                current->id,
                current->name,
                current->date,
                current->time,
                current->description,
                current->priority);
        current = current->nextTask;
    }

    fclose(file);
    return true;
}

// Function to load tasks from a CSV file
bool loadTasksFromFile(TaskList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // File may not exist initially; handle gracefully
        return false;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[100];
        char date[50];
        char time[15];
        char description[256];
        int priorityVal;

        // Parse the CSV line
        // Format: id,"name","date","time","description",priority
        int numFields = sscanf(line, "%d,\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%d",
                               &id, name, date, time, description, &priorityVal);

        if (numFields == 6) {
            // Create and add the task
            Task* task = createTask(id, name, date, time, description, (Priority)priorityVal);
            addTask(list, task);
        }
    }

    fclose(file);
    return true;
}

// Function to clear the input buffer
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
