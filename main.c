#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
// process : open/close booster --> ask actions --> result check --> continue_or_not --> lottery

//
// �i��: ���ϧ����A��24���ɭԦ�����C
// result check area 5�S���[�Wbooster�ĪG�A����ڤW���Ȩ�
// ���� : choosen_arr reallocate, open_cost = 0;
// �C�ѵ����O�o�[�@���K�O��
// ���D�G���booster �P�ɶ}�u�p���@��


// ************�i�઺���D*****************
// 1. user check results�ɿ�J���X�k
void bg_of_day(int *saving, int *speed_level, int *taste_level, int *time, int *price, int boosters[], int booster_status[]);

void bg_of_area(int *area, int *saving, int *speed_level, int *taste_level, int *time, int *price);

int ask_action(int price, int up_speed_cost, int up_taste_cost, int time, int actions[]);

void result_check(int results[], int results_sell[][2], int booster_status[]);

void take_action(int actions[], int *saving, int *time, int *price, int *day_earning, int *up_speed_cost, int *speed_level, int *up_taste_cost, int *taste_level, int booster_status[], int results[], int results_sell[][2]);

void sell(int *saving, int *time, int price, int *day_earning, int booster_status[]);

void up_speed(int *saving, int *up_speed_cost, int *speed_level);

void up_taste(int *saving, int *up_taste_cost, int *taste_level);

void ask_continue(int *continue_or_not, int *free_choices);

// ************* graphing funcions********************
int graph_main(int *free_choices, int *open_cost, int *n, int *choosen_arr, int *saving, int *boosters, int *price, int *cell_num, int *prizes_left);

int graph_input_validate(int input, int *n, int *choosen_arr);

void choosen_arr_initialize(int *n, int *choosen_arr, int *prizes_left);

int take_action_lottery(int prize_type, int *saving, int *choosen_arr, int *free_choices, int *boosters, int *price, int *n, int *input, int *prizes_left);

int digit_count(int n);

int reverse(int cell_num);

void graph_numline(int n, int *cell_num, int *choosen_arr);

void graphing(int n, int *cell_num, int *choosen_arr);

int power(int original, int n);

int cell_value_calculation(int cell_num);
// ************* graphing funcions**********************

int main(){
    printf("Welcome, young boss!\n");
    // initialize variables
    int up_speed_cost = 50, up_taste_cost = 100;
    int speed_level = 0, time = 15,  taste_level = 0, price = 0;
    int saving = 100;
    int continue_or_not = 1;
    int day_earning = 0;
    int boosters[3] = {0, 0, 0}; // [0] = speed, [1] = price, [2] = area

    int booster_status[3] = {0,0,0}; // [0] = 0 means speed booster closed.

    int actions[4]; // stores actions at each area

    // 1:�S���ɯ� 2:�L�k�A�� 3:�t�פɯ� 4:���D�ɯ� 5: �����
    int results[5] = {0}; int results_sell[5][2]; // [0] num of hotdogs made, [1] earn
    //price = taste*10 + 30, time = 15-speed_level

    // *********************lottery variables*****************************************
    int n = 3; int prizes_left = n*n;
    int free_choices = 0; int open_cost = 500; int *choosen_arr = malloc((n*n)*sizeof(int));
    int cell_num = 1;
    // initialize choosen_arr
    for (int i=0;i<n;i++){
        choosen_arr[i] = 0;
    }
    //************************************************************************************
    while(continue_or_not){
        day_earning = 0; // �N�C����Q�k�s
        bg_of_day(&saving, &speed_level, &taste_level, &time, &price, boosters, booster_status);
        ask_action(price, up_speed_cost, up_taste_cost, time, actions);

        // operations
        take_action(actions,&saving, &time, &price, &day_earning,&up_speed_cost, &speed_level, &up_taste_cost, &taste_level, booster_status, results, results_sell);
        result_check(results, results_sell, booster_status);
        ask_continue(&continue_or_not, &free_choices);
        if (continue_or_not == 0){
            return 0;
        }
        //************************************************************************************
        //graphing(3, &cell_num);
        graph_main(&free_choices, &open_cost, &n, choosen_arr, &saving, &boosters[0], &price, &cell_num, &prizes_left);

    }
    return 0;
}

void ask_continue(int *continue_or_not, int *free_choices){
    // continue or not
    int flag = 1;

    while(flag){
        printf("Do you want to continue or exit?\n");
        printf("  [1] Continue\n");
        printf("  [2] Exit\n");
        printf("Enter the number(s): ");
        int input;
        scanf("%d", &input);

        if (input == 1 || input == 2){
            if (input == 1){
                printf("You get one free choice.\n");
                (*free_choices)++;
                flag=0;
            }else{
                *continue_or_not = 0;
                flag = 0;
                printf("We will miss you. Bye!\n");
            }
        }else{
            printf("Invalid input!!!!\n");
        }
    }

}

int take_action_lottery(int prize_type, int *saving, int *choosen_arr, int *free_choices, int *boosters, int *price, int *n, int *input, int *prizes_left){
    // *input is cell num picked

    switch(prize_type){
        case 1:
            choosen_arr[*input-1] = 1; // ������L��
            (*prizes_left)--; // �����Ѿl���ƶq
            // prize type 1
            printf("Fortune, fortune! You get $%d!\n", 100*(*price));
            *saving += 100*(*price);
            break;
        case 2:
            choosen_arr[*input-1] = 1; // ������L��
            (*prizes_left)--; // �����Ѿl���ƶq
            // prize type 2
            printf("You get an extra choice!\n");
            *free_choices+=1;
            break;
        case 3: // open up

            if (*input > *n){ // not first
                int next_cell_picted = *input-*n; // ���ΦA��@�]�����O�nindex
                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(*input-*n), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }else if(*input <= *n){ // first line
                int next_cell_picted = (*n)*(*n-1) + *input; // ���ΦA��@�]�����O�nindex
                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }else{
                printf("sth unexpected in take action price type 3");
            }
            // prize type 3 ~ 6 success
            //printf("Another open on %d!\n", );
            break;
        case 4: // open underneath

            if (*input > (*n)*(*n - 1)){ // last line
                int next_cell_picted = *input % (*n); // ���ΦA��@�]�����O�nindex
                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }else{
                int next_cell_picted = *input + *n; // ���ΦA��@�]�����O�nindex
                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }
            break;
        case 5: // open left
            if (*input % *n == 1){ // left most column
                int next_cell_picted = *input+ (*n-1); // ���ΦA��@�]�����O�nindex


                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq

                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }else{
                int next_cell_picted = *input - 1; // ���ΦA��@�]�����O�nindex

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq

                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }
            break;
        case 6: // open right
            if(*input % *n == 0){ // right most column
                int next_cell_picted = *input - (*n-1); // ���ΦA��@�]�����O�nindex

                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq

                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }else{
                int next_cell_picted = *input+1; // ���ΦA��@�]�����O�nindex

                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU
                choosen_arr[*input-1] = 1; // ������L��
                (*prizes_left)--; // �����Ѿl���ƶq

                // �����A����bad luck�]���p�G�O�ϥΪ̿���L���A�~��graph_main�N�|�פU
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(cell_value_calculation(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left);
            }
            break;
        case 7:
            choosen_arr[*input-1] = 1; // ������L��
            boosters[0] += 1;
            (*prizes_left)--; // �����Ѿl���ƶq
            printf("You get a booster!!\n");
            break;
        case 8:
            choosen_arr[*input-1] = 1; // ������L��
            boosters[1] += 1;
            (*prizes_left)--; // �����Ѿl���ƶq
            printf("You get a booster!!\n");
            break;
        case 9:
            choosen_arr[*input-1] = 1; // ������L��
            boosters[2] += 1;
            (*prizes_left)--; // �����Ѿl���ƶq
            printf("You get a booster!!\n");
            break;
    }
    return 0;
}

int graph_main(int *free_choices, int *open_cost, int *n, int *choosen_arr, int *saving, int *boosters, int *price, int *cell_num, int *prizes_left){

    int flag = 1;
    while(flag){

        // �e�ϫe�ˬd�٦��L���|
        if (*prizes_left <= 0 ){
            *n += 2;
            *prizes_left = (*n) * (*n);
            *open_cost = 500;
            free(choosen_arr);
            choosen_arr = malloc(((*n)*(*n))*sizeof(int));
            choosen_arr_initialize(n, choosen_arr, prizes_left);
        }
        *cell_num = 1;
        graphing(*n, cell_num, choosen_arr);
        printf("You can choose\n");
        if (*free_choices > 0){    // if free choice remains
            printf("  [number on cell] to open (- $0)\n");

        }else{
            printf("  [number on cell] to open (- $%d)\n", *open_cost);

        }
        printf("  [0] to continue the game\n");
        printf("Enter the number(s): ");

        int input;
        scanf("%d", &input);

        switch (graph_input_validate(input, n, choosen_arr)){
            case 1: // valid inut
                if ((*free_choices <= 0) && (*saving < *open_cost)){
                printf("You have no money!\n");
                return 0;
                }
                if(*free_choices > 0){
                    *free_choices-= 1;
                }else{
                    *saving -= *open_cost;
                    *open_cost += 500;
                }
                //choosen_arr[input-1] = 1;
                take_action_lottery(cell_value_calculation(input), saving, choosen_arr, free_choices, boosters, price, n, &input, prizes_left);
                //printf("free choices %d\n", *free_choices);
                break;
            case 2: // choose to �����~��
                flag = 0;
                break;
            case 3: // invalid input
                printf("Invalid input!!!!\n");
                //printf("input:%d cell:%d", input, choosen_arr[input-1]);
                break;
        }
    }
    return 0;
}

int graph_input_validate(int input, int *n, int *choosen_arr){

    if (input > 0 && input <= (*n)*(*n) && choosen_arr[input-1] == 0){ // ���`���
        return 1;
    }else if(input == 0){ // ������}
        return 2;
    }else{
        return 3;
    }
}

// �T�{�O�_���ݭninitialize, �L�h���L
void choosen_arr_initialize(int *n, int *choosen_arr, int *prizes_left){

    if (prizes_left == 0){
        for (int i=0;i<(*n)*(*n);i++){
            choosen_arr[i] = 0;
            printf("time to reset choosen_arr\n");
        }
    }
}

void result_check(int results[], int results_sell[][2], int booster_status[]){
    int input;

    // Opeartions ���X�L�n�D���æL�X
    // �פ���� 1. booster�}�Binput = 6, 2. booster�S�}�B input = 5
    while ((booster_status[2] == 1 && input != 6) || (booster_status[2] == 0 && input != 5)){
        printf("Which result of the area you want to check?\n");
        printf("  [1] Area 1\n");
        printf("  [2] Area 2\n");
        printf("  [3] Area 3\n");
        printf("  [4] Area 4\n");
        if (booster_status[2] == 1){
            printf("  [5] Area 5\n");   // if booster open
            printf("  [6] Done\n");     // if booster open
        }else{
            printf("  [5] Done\n");     // if booster close
        }
        printf("Enter the number(s): ");
        scanf("%d", &input);
        if ((booster_status[2] == 1 && input == 6) || (booster_status[2] == 0 && input == 5)){break;}
        // operattion: �ھڭn�D�C�L����
        switch(results[input-1]){ // results[input-1] �O�ӰϪ�action
            case 1:
                // lack of money to upgrade
                printf("Can't you tell how poor you are?\n");
                printf("Go vending your hotdogs instead of thinking about self-improvement!\n");
                printf("You make %d hotdogs here!\n", results_sell[input-1][0]);
                printf("You earn $%d!\n", results_sell[input-1][1]);
                break;
            case 2:
                // cannot reduce any time
                printf("Do you want to travel through time?\n");
                printf("GO WORK!!\n");
                printf("You make %d hotdogs here!\n", results_sell[input-1][0]);
                printf("You earn $%d!\n", results_sell[input-1][1]);
                break;
            case 3:
                // speed upgrade
                printf("You glimpse the secret of wind.\n");
                printf("Your hands can move faster now.\n");
                break;
            case 4:
                // taste upgrade
                printf("You feel the soul of the ingredients.\n");
                printf("Your hotdogs are more appetizing now.\n");
                break;
            case 5:
                // sell hotdogs
                printf("You make %d hotdogs here!\n", results_sell[input-1][0]);
                printf("You earn $%d!\n", results_sell[input-1][1]);

        }
    }


}

void bg_of_day(int *saving, int *speed_level, int *taste_level, int *time, int *price, int boosters[], int booster_status[]){
    *price = *taste_level*10 + 30;
    *time = 15 - *speed_level;
    printf("Chop chop, It's dawn.\n");
    printf("You have %d dollars.\n", *saving);
    printf("You need %d minutes to make a hotdog.\n", *time);
    printf("The price of a hotdog is $%d.\n", *price);
    printf("You have %d speed booster(s), %d price booster(s), %d area booster(s).\n", boosters[0], boosters[1] , boosters[2] );

    // small func that prints out messages
    void booster_message(int booster_status[]){
        printf("Open/Close boosters:\n");
        (booster_status[0] == 0) ? printf("  [1] Speed booster (now close)\n") : printf("  [1] Speed booster (now open)\n"); // if booster open;
        (booster_status[1] == 0) ? printf("  [2] Price booster (now close)\n") : printf("  [2] Price booster (now open)\n");
        (booster_status[2] == 0) ? printf("  [3] Area  booster (now close)\n") : printf("  [3] Area  booster (now open)\n");
        printf("  [4] Finish\n");
        printf("Enter the number(s): ");
    }


    int input;
    int flag = 1;
    while(flag){
        booster_message(booster_status);
        scanf("%d", &input);
        switch(input){
            case 1:
                // if off turn on else turn off
                if (booster_status[0] == 0){booster_status[0] = 1;}else{booster_status[0] = 0;}
                break;
            case 2:
                if (booster_status[1] == 0){booster_status[1] = 1;}else{booster_status[1] = 0;}
                break;
            case 3:
                if (booster_status[2] == 0){booster_status[2] = 1;}else{booster_status[2] = 0;}
                break;
            case 4:
                flag = 0;
                break;
            default:
                printf("Invalid input!!!!\n");
                break;
        }

    }

    // �ϥΪ̨M�w�n���A��A�P�_�O�_������boosters

    // check speed booster
    if (booster_status[0] == 1 && boosters[0] < 1){
        booster_status[0] = 0;
    }else if(booster_status[0] == 1 && boosters[0] >= 1){ // �}�A�B������booster�C
        boosters[0] -= 1;
    }else if(booster_status[0] == 0){

    }else{
        printf("unexpected in booster related1\n");
    }

    // check taste booster
    if (booster_status[1] == 1 && boosters[1] < 1){
        booster_status[1] = 0;
    }else if (booster_status[1] == 1 && booster_status[1] >= 1){
        boosters[1] -= 1;
    }else if(booster_status[1] == 0){

    }else{
        printf("unexpected in booster related2\n");
    }

    // check area booster
    if (booster_status[2] == 1 && boosters[2] < 1){
        booster_status[2] = 0;
    }else if(booster_status[2] == 1 && boosters[2] >= 1){
        boosters[2] -= 1;
    }else if(booster_status[2] == 0){

    }else{
        printf("unexpected in booster related3\n");
    }
}

int ask_action(int price, int up_speed_cost, int up_taste_cost, int time, int actions[]){


    // price*(180/time) this is mpney made if sell hot dog
    // choose action
    printf("Actions you can take for each area:\n");
    printf("  [1] Sell the hotdogs\n");
    printf("  [2] Improve your cooking speed\n");
    printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n", up_speed_cost, 2*up_speed_cost, 4*up_speed_cost, 8*up_speed_cost);
    printf("  [3] Improve your hotdog flavor\n");
    printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n",up_taste_cost ,2*up_taste_cost ,4*up_taste_cost ,8*up_taste_cost );
    printf("Enter the number(s): ");
    scanf("%d %d %d %d", &actions[0], &actions[1], &actions[2], &actions[3]);

    // test validation
    for (int i=0;i<4;i++){
        //printf("actions[%d] = %d", i, actions[i]);
        while(actions[i] < 1 || actions[i] > 3){
            // re-enter action
            printf("Invalid input!!!!\n");
            printf("Actions you can take at Area %d:\n", i+1);
            printf("  [1] Sell the hotdogs\n");
            printf("  [2] Improve your cooking speed\n");
            printf("  [3] Improve your hotdog flavor\n");
            printf("Enter the number(s): ");
            int temp;
            scanf("%d", &temp);
            actions[i] = temp;
        }
    }
    return 0;
}

void sell(int *saving,int *time, int price, int *day_earning, int booster_status[]){

    if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster on taste off
        int profit = (180/ *time)*2*price; //
        *saving += profit;
        *day_earning += profit;
    }else if(booster_status[1] == 1){ // taste booster on speed off
        int profit = (180/ *time)*2*price;
        *saving += profit;
        *day_earning += profit;
    }else if(booster_status[0] == 1 && booster_status[1] == 1){ // both open
        int profit = (180/ *time)*4*price; //
        *saving += profit;
        *day_earning += profit;
    }else{ // no booster
        int profit = (180/ *time)*price;
        *saving += profit;
        *day_earning += profit;
    }
}

void up_speed(int *saving, int *up_speed_cost, int *speed_level){
    *saving -= *up_speed_cost;
    *up_speed_cost *= 2;
    *speed_level += 1;
    //printf("You glimpse the secret of wind.\nYour hands can move faster now.\n");
}

void up_taste(int *saving, int *up_taste_cost, int *taste_level){

    *saving -= *up_taste_cost;
    *up_taste_cost *= 2;
    *taste_level += 1;
    //printf("You feel the soul of the ingredients.\nYour hotdogs are more appetizing now.\n");
}

// results: [0][0] = area1 result , ... , [0][4] = area5 result. [1][0] = area1 earning(if any)
// [6] = area2 earning(if any) ...
void take_action(int actions[], int *saving, int *time, int *price, int *day_earning, int *up_speed_cost, int *speed_level, int *up_taste_cost, int *taste_level, int booster_status[], int results[], int results_sell[][2]){
    for (int i=0;i<4;i++){ // [2,1,2,2]
        if (actions[i] == 1){
            sell(saving, time, *price, day_earning, booster_status);
            results[i] = 5; // �Ĩ������

            if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time)*2; // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���

            }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                int profit = (180/ *time)*(*price)*2; // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���

            }else if(booster_status[1] == 1 && booster_status[0] == 1){

                int profit = (180/ *time)*(*price)*4; // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���

            }else{                              // no booster open

                int profit = (180/ *time)*(*price); // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���
            }

        }else if(actions[i] == 2){
            if (*saving >= *up_speed_cost && *time > 1){
                up_speed(saving, up_speed_cost, speed_level);
                results[i] =3; // �Ĩ������
                *time = 15 - *speed_level;
            }else if(*saving < *up_speed_cost){
                // �S���ɯ�
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 1; // �Ĩ������
                if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time)*2; // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���

                }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                    int profit = (180/ *time)*(*price)*2; // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���

                }else if(booster_status[1] == 1 && booster_status[0] == 1){

                    int profit = (180/ *time)*(*price)*4; // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���

                }else{                              // no booster open

                    int profit = (180/ *time)*(*price); // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���
                }
            }else{
                // ����b��
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 2; // �Ĩ������
            }

        }else if(actions[i] == 3){
            if (*saving >= *up_taste_cost){
                up_taste(saving, up_taste_cost, taste_level);
                *price = *taste_level*10 + 30;
                results[i] = 4; // �Ĩ������
            }else{
                // �S���ɯ�
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 1; // �Ĩ������
                if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // �Ȧs�A���F�s��results.
                results_sell[i][0] = (180/ *time)*2; // �b���Ͻ檺������
                results_sell[i][1] = profit; // �b�����Ȫ���

                }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                    int profit = (180/ *time)*(*price)*2; // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���

                }else if(booster_status[1] == 1 && booster_status[0] == 1){

                    int profit = (180/ *time)*(*price)*4; // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time)*2; // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���

                }else{                              // no booster open

                    int profit = (180/ *time)*(*price); // �Ȧs�A���F�s��results.
                    results_sell[i][0] = (180/ *time); // �b���Ͻ檺������
                    results_sell[i][1] = profit; // �b�����Ȫ���
                }
            }
        }
    }

    if (booster_status[2] == 1){ // area booster open
        sell(saving, time, *price, day_earning, booster_status);

        if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

        int profit = (180/ *time)*2* (*price); // �Ȧs�A���F�s��results.
        results_sell[4][0] = (180/ *time)*2; // �b���Ͻ檺������
        results_sell[4][1] = profit; // �b�����Ȫ���

        }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

            int profit = (180/ *time)*(*price)*2; // �Ȧs�A���F�s��results.
            results_sell[4][0] = (180/ *time); // �b���Ͻ檺������
            results_sell[4][1] = profit; // �b�����Ȫ���

        }else if(booster_status[1] == 1 && booster_status[0] == 1){

            int profit = (180/ *time)*(*price)*4; // �Ȧs�A���F�s��results.
            results_sell[4][0] = (180/ *time)*2; // �b���Ͻ檺������
            results_sell[4][1] = profit; // �b�����Ȫ���

        }else{                              // no booster open

            int profit = (180/ *time)*(*price); // �Ȧs�A���F�s��results.
            results_sell[4][0] = (180/ *time); // �b���Ͻ檺������
            results_sell[4][1] = profit; // �b�����Ȫ���
        }
        results[4] = 5;


    }
    printf("Well done, you earn $%d today.\n", *day_earning);
}


// **************************Lottery***********************************************************************

// ��X�����
int digit_count(int n){
    int digit = 0,remainder = n;
    while (remainder != 0){
        digit++;
        remainder /= 10;
    }
    return digit;
}
int reverse(int cell_num){
    int n, reverse = 0, remainder;

    n = cell_num;
    while (n != 0) {
    remainder = n % 10;
    reverse = reverse * 10 + remainder;
    n /= 10;
  }
    return reverse;
}

// �t�d�@��Ʀr��
void graph_numline(int n, int *cell_num, int *choosen_arr){
    //static int cell_num = 1;
    int num_of_minus = digit_count(n*n)+2;
    // �C��cell�}�l��|�B" "�M������" " ����printf()�L�X
    // �ҥH��spaces == digit of cell num�N�n��cell num�L�X�A���M�N�|�����Ů�
    for(int i=0;i<n;i++){
        int spaces = num_of_minus - 2;
        printf("| ");
        while(spaces != 0){
            if (choosen_arr[*cell_num - 1] == 1){
                int digits = 1; // digits = 2; spaces =
                if (spaces == digits){
                    printf("x");
                    spaces-= digits; // �]���@���L�F�n�X�ӼƦr�A�ҥH�n��
                }else if(spaces > digits){
                    printf(" ");
                    spaces--;
                }else{
                    printf("E");
                }
            }else{
                int digits = digit_count(*cell_num); // digits = 2; spaces =
                if (spaces == digits){
                    printf("%d", *cell_num);
                    spaces-= digits; // �]���@���L�F�n�X�ӼƦr�A�ҥH�n��
                }else if(spaces > digits){
                    printf(" ");
                    spaces--;
                }else{
                    printf("E");
                }
            }


        }
        printf(" ");
        *cell_num += 1;
    }
    printf("|\n");
}
void graphing(int n, int *cell_num, int *choosen_arr){


    // �p��ӦL�X���t����
    int num_of_minus = digit_count(n*n)+2;
    //int spaces_left_in_cell = num_of_minus -

    // �e��
    for (int row = 0; row<n; row++){ //���@�U�令��
        for (int i = 0; i<2;i++){
            // +-����
            if (i == 0){
                for (int p = 0;p<n;p++){
                    for (int j=0;j<(num_of_minus+1);j++){
                        if (j== 0){
                            printf("+");
                        }else{
                            printf("-");
                        }
                    }
                }
                printf("+\n");
            // �Ʀr��
            }else{
                graph_numline(n, cell_num, choosen_arr);

            }

        }

    }
    // �̫ᨺ�@��+-
    for (int p = 0;p<n;p++){
        for (int j=0;j<(num_of_minus+1);j++){
            if (j== 0){
                printf("+");
            }else{
                printf("-");
            }
        }
    }
    printf("+\n");
}


int power(int original, int n){
    int temp = 1;
    for (int i = 0; i<n;i++){
        temp = temp*original;
    }
    return temp;
}

// ��J:cell num, �^�Ǭ����~
int cell_value_calculation(int cell_num){
    int temp = reverse(cell_num); // reverse��

    int prize = 0, n = 0;
    // �Ntemp����hex�ú�X��decimal��
    while(temp !=0){
        prize += ((temp%10)*(power(16, n++)));
        temp /= 10;
    }
    return prize%9 + 1;
}

