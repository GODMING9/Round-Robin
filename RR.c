#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// PCB
typedef struct process {
    int pid;
    int priority;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int remaining_time;
} p;

// Check the average of waiting time
float checkWaiting(p process[], int num) {
    int sum = 0;
    for (int i = 0; i < num; i++) {
        sum = sum + process[i].waiting_time;
    }
    return (float)sum / num;
}

// Check the average of response time
float checkResponse(p process[], int num) {
    int sum = 0;
    for (int i = 0; i < num; i++) {
        sum = sum + process[i].response_time;
    }
    return (float)sum / num;
}

// Check the average of turnaround time
float checkTurnaround(p process[], int num) {
    int sum = 0;
    for (int i = 0; i < num; i++) {
        sum = sum + process[i].turnaround_time;
    }
    return (float)sum / num;
}

// Print time-flow and process execution
void printChart(p process[], int num, int time_quantum) {
    int currentTime = 0;
    int completed = 0;
    int n = num;
    int contextSwitches = 0;
    int nextArrivalIndex = 0;

    printf("\nScheduling : RR (Time Quantum: %d)\n", time_quantum);
    printf("==========================================\n");

    while (completed < n) {
        int idle = 1;

        // RR Scheduling
        for (int i = 0; i < n; i++) {

            if (nextArrivalIndex < n && currentTime == process[nextArrivalIndex].arrival_time) {
                printf("<time %d> [new arrival] process %d\n", currentTime, process[nextArrivalIndex].pid);
                nextArrivalIndex++;
                idle = 0; // New process arrival, system is not idle
            }

            if (process[i].arrival_time <= currentTime && process[i].remaining_time > 0) {
                idle = 0; // Process is running, system is not idle
                int executionTime = (process[i].remaining_time < time_quantum) ? process[i].remaining_time : time_quantum;
                process[i].remaining_time -= executionTime;
                currentTime += executionTime;

                printf("<time %d> process %d is running\n", currentTime, process[i].pid);

                // Check if the process is completed
                if (process[i].remaining_time == 0) {
                    completed++;
                    process[i].turnaround_time = currentTime - process[i].arrival_time;
                    process[i].waiting_time = process[i].turnaround_time - process[i].burst_time;
                    printf("<time %d> process %d is finished\n", currentTime, process[i].pid);
                }
            }
        }

        if (idle) {
            printf("<time %d> ---- system is idle ----\n", currentTime);
            currentTime++;
        }
    }

    printf("<time %d> all processes finish\n", currentTime);
    printf("==========================================\n");
}


int main() {
    p process[10];
    int process_number = 0;
    int time_quantum;

    FILE* file;
    file = fopen("input.dat", "r");
    if (file == NULL) {
        printf("File Open Error!\n");
        return 1;
    }

    // Get the value
    while (fscanf(file, "%d %d %d %d", &process[process_number].pid, &process[process_number].priority, &process[process_number].arrival_time, &process[process_number].burst_time) != EOF) {
        process[process_number].remaining_time = process[process_number].burst_time;
        process_number++;
    }

    fclose(file);

    // Arrange processes based on arrival time
    for (int i = 0; i < process_number - 1; i++) {
        for (int j = 0; j < process_number - (i + 1); j++) {
            if (process[j].arrival_time > process[j + 1].arrival_time) {
                p temp = process[j];
                process[j] = process[j + 1];
                process[j + 1] = temp;
            }
        }
    }

    // Get time quantum from user
    printf("Enter the time quantum: ");
    scanf_s("%d", &time_quantum);

    printChart(process, process_number, time_quantum);

    // Print statistical data
    float cpuUsage = (float)currentTime / currentTime * 100;
    float avg_waiting_time = checkWaiting(process, process_number);
    float avg_response_time = checkResponse(process, process_number);
    float avg_turnaround_time = checkTurnaround(process, process_number);

    printf("Average CPU usage: %.2f %%\n", cpuUsage);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
    printf("Average Turnaround Time : %.2f\n", avg_turnaround_time);

    return 0;
}
