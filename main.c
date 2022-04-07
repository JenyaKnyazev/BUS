#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct station_{
    char* name;
    int line_number;
    struct station_* next;
}station_node;
typedef enum line_type{
    BUS,
    METRO,
    TRAIN
} line_type;

typedef struct schedule_line_node_s{
    line_type type;
    int line_number;
    float price;
    station_node* head_stations;
    struct schedule_line_node_s* next;

} schedule_line_node_t;

//because to remove the first node of linked list we need DIRECTLY THE HEAD and NOT a copy of what the heed contains as argument
//OR WE NEEd POINTER TO POINTER TO CHANGE THE HEAD 
//because this the first node will be "empty" and all functions will ignore the first node "empty"
//BUT in station liked list we dont need first node to be "empty" because we can directly change the head of stations 

schedule_line_node_t* schedule_create(){
    schedule_line_node_t* result;
    result = (schedule_line_node_t*)malloc(sizeof(schedule_line_node_t));
    if (result == NULL) {
        puts("SCHEDULE_OUT_OF_MEMORY");
        return NULL;
    }
    puts("SCHEDULE_SUCCESS");
    result -> next = NULL;
    result -> head_stations = NULL;
    result -> line_number = -1;
    return result;
}

void schedule_destroy(schedule_line_node_t* head){
    schedule_line_node_t* next_lines;
    station_node* next_stations;
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        return;
    }
    while (head != NULL) {
        while (head->head_stations != NULL) {
            next_stations = head->head_stations->next;
            free(head->head_stations->name);
            free(head->head_stations);
            head->head_stations = next_stations;
        }
        next_lines = head->next;
        free(head);
        head = next_lines;
    }
    puts("SCHEDULE_SUCCESS");
}

schedule_line_node_t* schedule_add_line(schedule_line_node_t* head, line_type type, unsigned int line_number, float price){
    schedule_line_node_t* run;
    schedule_line_node_t* element = (schedule_line_node_t*)malloc(sizeof(schedule_line_node_t));
    if (element == NULL) {
        puts("SCHEDULE_OUT_OF_MEMORY");
        return NULL;
    }
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        free(element);
        return NULL;
    }
    if (type != BUS && type != METRO && type != TRAIN) {
        puts("SCHEDULE_INVALID_LINE_TYPE");
        free(element);
        return NULL;
    }
    if (line_number > 100 || line_number < 0) {
        puts("SCHEDULE_INVALID_LINE_NUMBER");
        free(element);
        return NULL;
    }
    run = head->next;
    while (run != NULL) {
        if (run->line_number == line_number) {
            puts("SCHEDULE_ALREADY_EXISTS");
            free(element);
            return NULL;
        }
        run = run->next;
    }
    if (price < 0.01) {
        puts("SCHEDULE_INVALID_PRICE");
        free(element);
        return NULL;
    }
    run = head;
    while (run->next != NULL)
        run = run->next;
    element->price = price;
    element->line_number = line_number;
    element->type = type;
    element->next = NULL;
    element->head_stations = NULL;
    run->next = element;
    puts("SCHEDULE_SUCCESS");
    return head;
}

schedule_line_node_t* schedule_remove_line(schedule_line_node_t* head, unsigned int line_number){
    schedule_line_node_t* run;
    schedule_line_node_t* prev;
    station_node* next_station;
    if (head == NULL){
        puts("SCHEDULE_NULL_ARG");
        return NULL;
    }
    if (line_number > 100 || line_number < 0) {
        puts("SCHEDULE_INVALID_LINE_NUMBER");
        return NULL;
    }
    run = head;
    while (run != NULL) {
        if (run->line_number == line_number)
            break;
        run = run->next;
    }
    if (run == NULL) {
        puts("SCHEDULE_LINE_DOESNT_EXIST");
        return NULL;
    }
    prev = head;
    while (prev->next != run)
        prev = prev->next;
    prev->next = run->next;
    while (run->head_stations != NULL) {
        next_station = run->head_stations->next;
        free(run->head_stations->name);
        free(run->head_stations);
        run->head_stations = next_station;
    }
    free(run);
    puts("SCHEDULE_SUCCESS");
    return head;
}

schedule_line_node_t* schedule_add_station(schedule_line_node_t* head, unsigned int line_number, const char* station){
    schedule_line_node_t* run;
    station_node* element = (station_node*)malloc(sizeof(station_node));
    station_node* run_station;
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        if (element != NULL)
            free(element);
        return NULL;
    }
    if (element == NULL) {
        puts("SCHEDULE_OUT_OF_MEMORY");
        return NULL;
    }
    if (line_number > 100 || line_number < 0) {
        puts("SCHEDULE_INVALID_LINE_NUMBER");
        free(element);
        return NULL;
    }
    run = head;
    while (run != NULL) {
        if (run->line_number == line_number)
            break;
        run = run->next;
    }
    if (run == NULL) {
        puts("SCHEDULE_LINE_DOESNT_EXIST");
        free(element);
        return NULL;
    }
    element->next = NULL;
    element->line_number = line_number;
    element->name = (char*)malloc(sizeof(char) * (strlen(station) + 1));
    if (element->name == NULL) {
        puts("SCHEDULE_OUT_OF_MEMORY");
        free(element);
        return NULL;
    }
    strcpy(element->name, station);
    if (run->head_stations == NULL) 
        run->head_stations = element;
    else {
        run_station = run->head_stations;
        while (run_station->next != NULL)
            run_station = run_station->next;
        run_station->next = element;
    }
    puts("SCHEDULE_SUCCESS");
    return head;
}

schedule_line_node_t* schedule_remove_station(schedule_line_node_t* head, unsigned int line_number, unsigned int station_index){
    schedule_line_node_t* run;
    station_node* run_station;
    station_node* prev_station=head->head_stations;
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        return NULL;
    }
    if (line_number > 100 || line_number < 0) {
        puts("SCHEDULE_INVALID_LINE_NUMBER");
        return NULL;
    }
    run = head;
    while (run != NULL) {
        if (run->line_number == line_number)
            break;
        run = run->next;
    }
    if (run == NULL) {
        puts("SCHEDULE_LINE_DOESNT_EXIST");
        return NULL;
    }
    run_station = run->head_stations;
    while (run_station != NULL && station_index > 0) {
        prev_station = run_station;
        run_station = run_station->next;
        station_index--;
    }
    if (run_station == NULL) {
        puts("SCHEDULE_STATION_DOESNT_EXIST");
        return NULL;
    }
    if (run_station == run->head_stations) {
        run->head_stations = run->head_stations->next;
        free(run_station->name);
        free(run_station);
    }else {
        prev_station->next = run_station->next;
        free(run_station->name);
        free(run_station);
    }
    puts("SCHEDULE_SUCCESS");
    return head;
}

char* type(line_type type ) {
    switch (type) {
        case BUS:
            return "BUS";
        case METRO:
            return "METRO";
        default:
            return "TRAIN";
    }
}
void schedule_print_line_route(schedule_line_node_t* head, unsigned int line_number){
    station_node* run;
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        return;
    }
    if (line_number > 100 || line_number < 0) {
        puts("SCHEDULE_INVALID_LINE_NUMBER");
        return;
    }
    while (head != NULL && head->line_number != line_number)
        head = head->next;
    if (head == NULL) {
        puts("SCHEDULE_LINE_DOESNT_EXIST");
        return;
    }
    printf("Line Route %s, line number: %d, price: %f\n", type(head->type), head->line_number, head->price);
    run = head->head_stations;
    while (run != NULL) {
        puts(run->name);
        run = run->next;
    }
    puts("SCHEDULE_SUCCESS");
}
char** first_and_last_stations(schedule_line_node_t* line) {
    char** res=(char**)malloc(sizeof(char*)*2);
    station_node* run = line->head_stations;
    if (run == NULL) {
        res[0] = (char*)malloc(sizeof(char)*4);
        res[0][0] = 'N'; res[0][1] = '/'; res[0][2] = 'A'; res[0][3] = '\0';
        res[1] = (char*)malloc(sizeof(char) * 4);
        res[1][0] = 'N'; res[1][1] = '/'; res[1][2] = 'A'; res[1][3] = '\0';
        return res;
    }
    res[0] = run->name;
    while (run->next != NULL)
        run = run->next;
    res[1] = run->name;
    return res;
}
void schedule_print_all_lines(schedule_line_node_t* head){
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        return;
    }
    head = head->next;
    while (head != NULL) {
        char** fl_stations = first_and_last_stations(head);
        printf("Line Route %s, line number: %d, price: %f, frist station %s,last station %s\n",
               type(head->type), head->line_number, head->price,fl_stations[0],fl_stations[1]);
        free(fl_stations[0]); free(fl_stations[1]); free(fl_stations);
        head = head->next;
    }
    puts("SCHEDULE_SUCCESS");
}
int schedule_get_station_index(schedule_line_node_t* line,const char *station) {
    station_node* run = line->head_stations;
    int index = 0;
    while (run != NULL) {
        if (strcmp(run->name, station) == 0)
            return index;
        index++;
        run = run->next;
    }
    return -1;
}
void schedule_print_lines_for_stations(schedule_line_node_t* head, const char* from, const char* to){
    int index1, index2;
    if (head == NULL) {
        puts("SCHEDULE_NULL_ARG");
        return;
    }
    head = head->next;
    while (head != NULL) {
        index1 = schedule_get_station_index(head, from);
        index2 = schedule_get_station_index(head, to);
        if (index1 < index2 && index1 != -1) {
            char** fl_stations = first_and_last_stations(head);
            printf("Line Route %s, line number: %d, price: %f, frist station %s,last station %s\n",
                type(head->type), head->line_number, head->price, fl_stations[0], fl_stations[1]);
            free(fl_stations[0]); free(fl_stations[1]); free(fl_stations);
        }
        head = head->next;
    }
    puts("SCHEDULE_SUCCESS");
}
void print_menu() {
    puts("1 = ADD LINE");
    puts("2 = REMOVE LINE");
    puts("3 = ADD STATION");
    puts("4 = REMOVE STATION");
    puts("5 = PRINT LINE ROUTE");
    puts("6 = PRINT ALL LINES");
    puts("7 = PRINT LINES BETWEEN STATIONS");
    puts("8 = EXIT");
}
void run() {
    int choose;
    int type;
    float price;
    int line_number;
    char name[100];
    char from[100];
    char to[100];
    int index;
    schedule_line_node_t* head = schedule_create();
    do {
        print_menu();
        scanf("%d", &choose);
        switch (choose) {
            case 1:
                puts("Enter line type 0 = BUS ,1 = METRO,3=TRAIN");
                scanf("%d", &type);
                puts("Enter line number");
                scanf("%d", &line_number);
                puts("Enter price");
                scanf("%f", &price);
                schedule_add_line(head, type, line_number, price);
                break;
            case 2:
                puts("Enter line number to remove");
                scanf("%d", &line_number);
                schedule_remove_line(head, line_number);
                break;
            case 3:
                puts("Enter line number");
                scanf("%d", &line_number);
                while (getchar() != '\n');
                puts("Enter station name");
                gets(name);
                schedule_add_station(head, line_number, name);
                break;
            case 4:
                puts("Enter line number");
                scanf("%d", &line_number);
                puts("Enter station index");
                scanf("%d", &index);
                schedule_remove_station(head, line_number, index);
                break;
            case 5:
                puts("Enter line number");
                scanf("%d", &line_number);
                schedule_print_line_route(head, line_number);
                break;
            case 6:
                schedule_print_all_lines(head);
                break;
            case 7:
                while (getchar() != '\n');
                puts("Enter start station");
                gets(from);
                puts("Enter end station");
                gets(to);
                schedule_print_lines_for_stations(head, from, to);
                break;
        }
    } while (choose != 8);
    schedule_destroy(head);
    puts("Good Bye");
    system("pause");
}
int main(){
    run();
    return 0;
}
