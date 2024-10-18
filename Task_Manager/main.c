// main.c

#include "tasks.c"

#define DATA_FILE "tasks.csv"

// Function Prototypes
void displayMenu(void);
void handleAddTask(TaskList* list);
void handleListTasks(const TaskList* list);
void handleDeleteTask(TaskList* list);
void handleUpdateTask(TaskList* list);
int getNextTaskID(const TaskList* list);

int main() {
    TaskList myTaskList;
    initializeTaskList(&myTaskList);

    // Load existing tasks from file
    loadTasksFromFile(&myTaskList, DATA_FILE);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer(); // Remove any remaining input

        switch (choice) {
            case 1:
                handleAddTask(&myTaskList);
                break;
            case 2:
                handleListTasks(&myTaskList);
                break;
            case 3:
                handleDeleteTask(&myTaskList);
                break;
            case 4:
                handleUpdateTask(&myTaskList);
                break;
            case 5:
                if (saveTasksToFile(&myTaskList, DATA_FILE)) {
                    printf("Tasks saved successfully to '%s'.\n", DATA_FILE);
                } else {
                    printf("Failed to save tasks.\n");
                }
                break;
            case 6:
                running = false;
                break;
            default:
                printf("Invalid choice. Please select a number between 1 and 6.\n");
        }
    }

    // Save tasks before exiting
    saveTasksToFile(&myTaskList, DATA_FILE);

    // Free allocated memory
    freeTaskList(&myTaskList);

    printf("Exiting Task Manager. Goodbye!\n");
    return 0;
}

// Function to display the menu
void displayMenu(void) {
    printf("\n=== Task Manager ===\n");
    printf("1. Add Task\n");
    printf("2. List Tasks\n");
    printf("3. Delete Task\n");
    printf("4. Update Task\n");
    printf("5. Save Tasks\n");
    printf("6. Exit\n");
}

// Function to handle adding a task
void handleAddTask(TaskList* list) {
    char name[100];
    char date[50];
    char time[15];
    char description[256];
    int priorityVal;

    printf("Enter task name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline

    printf("Enter task date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0'; // Remove newline

    printf("Enter task time (HH:MM AM/PM): ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = '\0'; // Remove newline

    printf("Enter task description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0'; // Remove newline

    printf("Enter task priority (1=Low, 2=Medium, 3=High, 4=Critical): ");
    if (scanf("%d", &priorityVal) != 1 || priorityVal < LOW || priorityVal > CRITICAL) {
        printf("Invalid priority. Task not added.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer(); // Remove any remaining input

    int id = getNextTaskID(list);
    Task* newTask = createTask(id, name, date, time, description, (Priority)priorityVal);
    addTask(list, newTask);

    printf("Task added successfully with ID %d.\n", id);
}

// Function to list all tasks
void handleListTasks(const TaskList* list) {
    printf("\n--- List of Tasks ---\n");
    listTasks(list);
}

// Function to handle deleting a task
void handleDeleteTask(TaskList* list) {
    int id;
    printf("Enter the Task ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a numeric Task ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer(); // Remove any remaining input

    if (deleteTask(list, id)) {
        printf("Task with ID %d deleted successfully.\n", id);
    } else {
        printf("Task with ID %d not found.\n", id);
    }
}

// Function to handle updating a task
void handleUpdateTask(TaskList* list) {
    int id;
    printf("Enter the Task ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a numeric Task ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer(); // Remove any remaining input

    if (updateTask(list, id)) {
        printf("Task with ID %d updated successfully.\n", id);
    } else {
        printf("Task with ID %d not found.\n", id);
    }
}

// Function to get the next Task ID
int getNextTaskID(const TaskList* list) {
    int maxID = 0;
    Task* current = list->firstTask;
    while (current != NULL) {
        if (current->id > maxID) {
            maxID = current->id;
        }
        current = current->nextTask;
    }
    return maxID + 1;
}
