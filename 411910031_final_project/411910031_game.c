#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<mmsystem.h>
// process : open/close booster --> ask actions --> result check --> continue_or_not --> lottery

// ***********************************extra functions************************************
// 負責一天賭注運算輸出等
int gamble(int *saving);

int contract();

void decision_and_action();

int print_matchups(int *saving);

// 負責檢查是否猜對，是回傳一否回傳0
int inspect_win(int *match_ups, int choosed_team, int *saving, int principle);

// 負責檢查是否猜對，是回傳一否回傳0
int inspect_score(int *match_ups ,int *predicted, int *saving, int principle);
// ******************************************************************************************

void bg_of_day(int *saving, int *speed_level, int *taste_level, int *time, int *price, int boosters[], int booster_status[], int *booster_slots, int len_of_booster_slots);

// 如果有舊的booster被pop，則boosters也要扣除
void boosters_pop_relations(int *boosters, int popped);

// this func sqush items together. e.g. {1,4,2} --> {1,2,4} which 4 means space
void sort_out(int *arr, int len);

// delete last booster, store new obtained booster. returns deleted booster.
int obtain_booster(int *arr, int len, int new);

// must at least one desired booster inside, 不然前面就擋掉
void booster_slots_minus(int *booster_slots, int booster_used);

int slot_len();

int random1(int next_cell_picted);

void bg_of_area(int *area, int *saving, int *speed_level, int *taste_level, int *time, int *price);

int ask_action(int price, int up_speed_cost, int up_taste_cost, int time, int actions[]);

void map_game(int *saving, int *booster_slots, int *boosters, int len_of_booster_slots);

void result_check(int results[], int results_sell[][2], int booster_status[], int *saving, int *booster_slots, int *boosters, int len_of_booster_slots);

void take_action(int actions[], int *saving, int *time, int *price, int *day_earning, int *up_speed_cost, int *speed_level, int *up_taste_cost, int *taste_level, int booster_status[], int results[], int results_sell[][2]);

void sell(int *saving, int *time, int price, int *day_earning, int booster_status[]);

void up_speed(int *saving, int *up_speed_cost, int *speed_level);

void up_taste(int *saving, int *up_taste_cost, int *taste_level);

void ask_continue(int *continue_or_not, int *free_choices);

// ************* graphing funcions********************
int graph_main(int *free_choices, int *open_cost, int *n, int *choosen_arr, int *saving, int *boosters, int *price, int *cell_num, int *prizes_left, int *booster_slots, int len_of_booster_slots);

int graph_input_validate(int input, int *n, int *choosen_arr);

void choosen_arr_initialize(int *n, int *choosen_arr, int *prizes_left);

int take_action_lottery(int prize_type, int *saving, int *choosen_arr, int *free_choices, int *boosters, int *price, int *n, int *input, int *prizes_left, int *booster_slots, int len_of_booster_slots);

int digit_count(int n);

int reverse(int cell_num);

void graph_numline(int n, int *cell_num, int *choosen_arr);

void graphing(int n, int *cell_num, int *choosen_arr);

int power(int original, int n);

int cell_value_calculation(int cell_num);
// ************* graphing funcions**********************

int main(){

    //PlaySound(TEXT("song.wav"), NULL,SND_ASYNC);
    printf("Welcome, young boss!\n");
    // initialize variables
    int len_of_booster_slots = slot_len();
    int booster_slots[len_of_booster_slots];
    int up_speed_cost = 50, up_taste_cost = 100;
    int speed_level = 0, time = 15,  taste_level = 0, price = 0;
    int saving = 100;
    int continue_or_not = 1;
    int day_earning = 0;
    int boosters[3] = {0, 0, 0}; // [0] = speed, [1] = price, [2] = area

    int booster_status[3] = {0,0,0}; // [0] = 0 means speed booster closed.

    int actions[4]; // stores actions at each area

    // 1:沒錢升級 2:無法再快 3:速度升級 4:味道升級 5: 賣熱狗
    int results[5] = {0}; int results_sell[5][2]; // [0] num of hotdogs made, [1] earn
    //price = taste*10 + 30, time = 15-speed_level

    // *********************lottery variables*****************************************
    int n = 3; int prizes_left = n*n;
    int free_choices = 0; int open_cost = 500; int choosen_arr[10000] = {0};
    int cell_num = 1;
    printf("you have %d slots for booster storage\n", len_of_booster_slots);
    // initialize booster_slots
    for (int i=0;i<len_of_booster_slots;i++){
        booster_slots[i] = 4;
    }
    //************************************************************************************
    while(continue_or_not){
        day_earning = 0; // 將每日獲利歸零
        bg_of_day(&saving, &speed_level, &taste_level, &time, &price, boosters, booster_status, booster_slots, len_of_booster_slots);
        ask_action(price, up_speed_cost, up_taste_cost, time, actions);

        // operations
        take_action(actions,&saving, &time, &price, &day_earning,&up_speed_cost, &speed_level, &up_taste_cost, &taste_level, booster_status, results, results_sell);
        result_check(results, results_sell, booster_status, &saving, booster_slots, boosters,len_of_booster_slots);
        ask_continue(&continue_or_not, &free_choices);
        if (continue_or_not == 0){
            return 0;
        }
        //************************************************************************************
        //graphing(3, &cell_num);
        graph_main(&free_choices, &open_cost, &n, choosen_arr, &saving, &boosters[0], &price, &cell_num, &prizes_left, booster_slots, len_of_booster_slots);

        gamble(&saving);
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

int take_action_lottery(int prize_type, int *saving, int *choosen_arr, int *free_choices, int *boosters, int *price, int *n, int *input, int *prizes_left, int *booster_slots, int len_of_booster_slots){
    // *input is cell num picked

    switch(prize_type){
        case 1:
            choosen_arr[*input-1] = 1; // 紀錄抽過的
            (*prizes_left)--; // 紀錄剩餘獎數量
            // prize type 1
            printf("Fortune, fortune! You get $%d!\n", 100*(*price));
            *saving += 100*(*price);
            break;
        case 2:
            choosen_arr[*input-1] = 1; // 紀錄抽過的
            (*prizes_left)--; // 紀錄剩餘獎數量
            // prize type 2
            printf("You get an extra choice!\n");
            *free_choices+=1;
            break;
        case 3: // open up
            //printf("case 3\n");
            if (*input > *n){ // not first
                int next_cell_picted = *input-*n; // 不用再減一因為不是要index
                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }else if(*input <= *n){ // first line
                int next_cell_picted = (*n)*(*n-1) + *input; // 不用再減一因為不是要index
                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }else{
                printf("sth unexpected in take action price type 3");
            }
            // prize type 3 ~ 6 success
            //printf("Another open on %d!\n", );
            break;
        case 4: // open underneath
            //printf("case 4\n");
            if (*input > (*n)*(*n - 1)){ // last line
                int next_cell_picted = *input % (*n); // 不用再減一因為不是要index
                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }else{
                int next_cell_picted = *input + *n; // 不用再減一因為不是要index
                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }
            break;
        case 5: // open left
            //printf("case 5\n");
            if (*input % *n == 1){ // left most column
                int next_cell_picted = *input+ (*n-1); // 不用再減一因為不是要index


                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量

                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }else{
                int next_cell_picted = *input - 1; // 不用再減一因為不是要index

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量

                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }
            break;
        case 6: // open right
            //printf("case 6\n");
            if(*input % *n == 0){ // right most column
                int next_cell_picted = *input - (*n-1); // 不用再減一因為不是要index

                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量

                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }else{
                int next_cell_picted = *input+1; // 不用再減一因為不是要index

                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下
                choosen_arr[*input-1] = 1; // 紀錄抽過的
                (*prizes_left)--; // 紀錄剩餘獎數量

                // 此狀態必為bad luck因為如果是使用者選到抽過的，外面graph_main就會擋下
                if (graph_input_validate(next_cell_picted, n, choosen_arr) == 3){
                    // prize type 3 ~ 6 failed
                    printf("Bad Luck :(\n");
                    return 0;
                }

                printf("Another open on %d!\n", next_cell_picted);
                take_action_lottery(random1(next_cell_picted), saving, choosen_arr, free_choices, boosters, price, n, &next_cell_picted, prizes_left, booster_slots, len_of_booster_slots);
            }
            break;
        case 7:
            choosen_arr[*input-1] = 1; // 紀錄抽過的
            boosters[0] += 1;
            (*prizes_left)--; // 紀錄剩餘獎數量
            // **********************處理boosters中如果有被pop
            boosters_pop_relations(boosters, obtain_booster(booster_slots, len_of_booster_slots, 0));

            printf("You get a booster!!\n");
            break;
        case 8:
            choosen_arr[*input-1] = 1; // 紀錄抽過的
            boosters[1] += 1;
            (*prizes_left)--; // 紀錄剩餘獎數量

            // **********************處理boosters中如果有被pop
            boosters_pop_relations(boosters, obtain_booster(booster_slots, len_of_booster_slots, 1));

            printf("You get a booster!!\n");
            break;
        case 9:
            choosen_arr[*input-1] = 1; // 紀錄抽過的
            boosters[2] += 1;
            (*prizes_left)--; // 紀錄剩餘獎數量

            // **********************處理boosters中如果有被pop，boosters也要扣
            boosters_pop_relations(boosters, obtain_booster(booster_slots, len_of_booster_slots, 2));
            printf("You get a booster!!\n");
            break;
    }
    return 0;
}

int graph_main(int *free_choices, int *open_cost, int *n, int *choosen_arr, int *saving, int *boosters, int *price, int *cell_num, int *prizes_left, int *booster_slots, int len_of_booster_slots){

    int flag = 1;
    while(flag){

        // 畫圖前檢查還有無機會
        if (*prizes_left <= 0 ){
            *n += 2;
            *prizes_left = (*n) * (*n);
            *open_cost = 500;
            //free(choosen_arr);
            //choosen_arr = malloc(((*n)*(*n))*sizeof(int));
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
                take_action_lottery(random1(input), saving, choosen_arr, free_choices, boosters, price, n, &input, prizes_left, booster_slots, len_of_booster_slots);
                //printf("free choices %d\n", *free_choices);
                break;
            case 2: // choose to 不抽繼續玩
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

    if (input > 0 && input <= (*n)*(*n) && choosen_arr[input-1] == 0){ // 正常選擇
        return 1;
    }else if(input == 0){ // 選擇離開
        return 2;
    }else{
        return 3;
    }
}

// 確認是否有需要initialize, 無則跳過
void choosen_arr_initialize(int *n, int *choosen_arr, int *prizes_left){

    for (int i=0;i<(*n)*(*n);i++){
            choosen_arr[i] = 0;
        }
    // if (*prizes_left == 0){
    //     printf("reinitial called\n");

    // }
}

void result_check(int results[], int results_sell[][2], int booster_status[], int *saving, int *booster_slots, int *boosters, int len_of_booster_slots){
    int input;

    // Opeartions 取出他要求的並印出
    // 終止條件 1. booster開且input = 6, 2. booster沒開且 input = 5
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

        map_game(saving, booster_slots, boosters, len_of_booster_slots);

        // operattion: 根據要求列印答案
        switch(results[input-1]){ // results[input-1] 是該區的action
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

void bg_of_day(int *saving, int *speed_level, int *taste_level, int *time, int *price, int boosters[], int booster_status[], int *booster_slots, int len_of_booster_slots){
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

    // 使用者決定好狀態後，判斷是否有足夠boosters

    // check speed booster
    if (booster_status[0] == 1 && boosters[0] < 1){
        booster_status[0] = 0;
    }else if(booster_status[0] == 1 && boosters[0] >= 1){ // 開，且有足夠booster。
        boosters[0] -= 1;
        booster_slots_minus(booster_slots, 0); // 進去booster_slots，刪一個speed
        sort_out(booster_slots, len_of_booster_slots);
    }else if(booster_status[0] == 0){

    }else{
        printf("unexpected in booster related1\n");
    }

    // check taste booster
    if (booster_status[1] == 1 && boosters[1] < 1){
        booster_status[1] = 0;
    }else if (booster_status[1] == 1 && booster_status[1] >= 1){
        boosters[1] -= 1;
        booster_slots_minus(booster_slots, 1); // 進去booster_slots，刪一個taste
        sort_out(booster_slots, len_of_booster_slots);
    }else if(booster_status[1] == 0){

    }else{
        printf("unexpected in booster related2\n");
    }

    // check area booster
    if (booster_status[2] == 1 && boosters[2] < 1){
        booster_status[2] = 0;
    }else if(booster_status[2] == 1 && boosters[2] >= 1){
        boosters[2] -= 1;
        booster_slots_minus(booster_slots, 2); // 進去booster_slots，刪一個area
        sort_out(booster_slots, len_of_booster_slots);
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
            results[i] = 5; // 採取的行動

            if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time)*2; // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢

            }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                int profit = (180/ *time)*(*price)*2; // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢

            }else if(booster_status[1] == 1 && booster_status[0] == 1){

                int profit = (180/ *time)*(*price)*4; // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢

            }else{                              // no booster open

                int profit = (180/ *time)*(*price); // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢
            }

        }else if(actions[i] == 2){
            if (*saving >= *up_speed_cost && *time > 1){
                up_speed(saving, up_speed_cost, speed_level);
                results[i] =3; // 採取的行動
                *time = 15 - *speed_level;
            }else if(*saving < *up_speed_cost){
                // 沒錢升級
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 1; // 採取的行動
                if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time)*2; // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢

                }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                    int profit = (180/ *time)*(*price)*2; // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢

                }else if(booster_status[1] == 1 && booster_status[0] == 1){

                    int profit = (180/ *time)*(*price)*4; // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢

                }else{                              // no booster open

                    int profit = (180/ *time)*(*price); // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢
                }
            }else{
                // 不能在快
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 2; // 採取的行動
            }

        }else if(actions[i] == 3){
            if (*saving >= *up_taste_cost){
                up_taste(saving, up_taste_cost, taste_level);
                *price = *taste_level*10 + 30;
                results[i] = 4; // 採取的行動
            }else{
                // 沒錢升級
                sell(saving, time, *price, day_earning, booster_status);
                results[i] = 1; // 採取的行動
                if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

                int profit = (180/ *time)*2* (*price); // 暫存，為了存到results.
                results_sell[i][0] = (180/ *time)*2; // 在本區賣的熱狗數
                results_sell[i][1] = profit; // 在本區賺的錢

                }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

                    int profit = (180/ *time)*(*price)*2; // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢

                }else if(booster_status[1] == 1 && booster_status[0] == 1){

                    int profit = (180/ *time)*(*price)*4; // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time)*2; // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢

                }else{                              // no booster open

                    int profit = (180/ *time)*(*price); // 暫存，為了存到results.
                    results_sell[i][0] = (180/ *time); // 在本區賣的熱狗數
                    results_sell[i][1] = profit; // 在本區賺的錢
                }
            }
        }
    }

    if (booster_status[2] == 1){ // area booster open
        sell(saving, time, *price, day_earning, booster_status);

        if (booster_status[0] == 1 && booster_status[1] == 0){ // speed booster open

        int profit = (180/ *time)*2* (*price); // 暫存，為了存到results.
        results_sell[4][0] = (180/ *time)*2; // 在本區賣的熱狗數
        results_sell[4][1] = profit; // 在本區賺的錢

        }else if(booster_status[1] == 1 && booster_status[0] == 0){ // taste booster open

            int profit = (180/ *time)*(*price)*2; // 暫存，為了存到results.
            results_sell[4][0] = (180/ *time); // 在本區賣的熱狗數
            results_sell[4][1] = profit; // 在本區賺的錢

        }else if(booster_status[1] == 1 && booster_status[0] == 1){

            int profit = (180/ *time)*(*price)*4; // 暫存，為了存到results.
            results_sell[4][0] = (180/ *time)*2; // 在本區賣的熱狗數
            results_sell[4][1] = profit; // 在本區賺的錢

        }else{                              // no booster open

            int profit = (180/ *time)*(*price); // 暫存，為了存到results.
            results_sell[4][0] = (180/ *time); // 在本區賣的熱狗數
            results_sell[4][1] = profit; // 在本區賺的錢
        }
        results[4] = 5;


    }
    printf("Well done, you earn $%d today.\n", *day_earning);
}


// **************************Lottery***********************************************************************

// 輸出抽獎圖
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

// 負責一行數字行
void graph_numline(int n, int *cell_num, int *choosen_arr){
    //static int cell_num = 1;
    int num_of_minus = digit_count(n*n)+2;
    // 每個cell開始的|、" "和結束的" " 都用printf()印出
    // 所以當spaces == digit of cell num就要把cell num印出，不然就會不夠空格
    for(int i=0;i<n;i++){
        int spaces = num_of_minus - 2;
        printf("| ");
        while(spaces != 0){
            if (choosen_arr[*cell_num - 1] == 1){
                int digits = 1; // digits = 2; spaces =
                if (spaces == digits){
                    printf("x");
                    spaces-= digits; // 因為一次印了好幾個數字，所以要扣
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
                    spaces-= digits; // 因為一次印了好幾個數字，所以要扣
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


    // 計算該印出的負號數
    int num_of_minus = digit_count(n*n)+2;
    //int spaces_left_in_cell = num_of_minus -

    // 畫圖
    for (int row = 0; row<n; row++){ //等一下改成五
        for (int i = 0; i<2;i++){
            // +-那行
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
            // 數字行
            }else{
                graph_numline(n, cell_num, choosen_arr);

            }

        }

    }
    // 最後那一行+-
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

// 輸入:cell num, 回傳為獎品
int cell_value_calculation(int cell_num){
    int temp = reverse(cell_num); // reverse後

    int prize = 0, n = 0;
    // 將temp視為hex並算出其decimal值
    while(temp !=0){
        prize += ((temp%10)*(power(16, n++)));
        temp /= 10;
    }
    return prize%9 + 1;
}


int random1(int next_cell_picted){
    srand(next_cell_picted);
    return (rand()%9)+1;
}

int slot_len(){
    srand(time(0));
    return (rand() % 11)+5;
}

// delete last booster, store new obtained booster.
int obtain_booster(int *arr, int len, int new){
    int temp;
    int popped = arr[len-1];
    for (int i=len;i>=2;i--){
        arr[i-1] = arr[i-2];
    }
    *arr = new;
    //printf("arr[0]:%d arr[1]:%d arr[2]:%d\n", arr[0], arr[1], arr[2]);
    return popped;
}

void sort_out(int *arr, int len){
    for (int i=len-1;i>=1;i--){
        if (arr[i-1] == 4){
            for (int a = i-1; a<=len-2;a++){
                arr[a] = arr[a+1];
            }
            arr[len-1] = 4; // moved forward so the last becomes nothing
        }
    }
}

void boosters_pop_relations(int *boosters, int popped){
    switch(popped){
        case 0:
            boosters[0]--;
            break;
        case 1:
            boosters[1]--;
            break;
        case 2:
            boosters[2]--;
            break;
        case 3:
            break;
    }
}

// must at least one desired booster inside, 不然前面就擋掉
void booster_slots_minus(int *booster_slots, int booster_used){
    int flag = 1;int index = 0;
    while (flag){
        if (booster_slots[index] == booster_used){
            booster_slots[index] = 4; // delete it
            flag = 0;
        }
        index++;

    }
}

void map_game(int *saving, int *booster_slots, int *boosters, int len_of_booster_slots ){
    int check_money(int *saving){
        if (*saving >= 25){
            return 1;
        }else{
            return 0;
        }
    }
    srand(time(0));
    int position = 0, money, booster;
    money = rand() % 63 + 1;
    booster = rand() % 63 + 1;

    // make sure M and B are not at same location
    if(money == booster){
        booster = rand() % 63 + 1;
    }

    int map[64] = {0}; // if 1, print P, 2 print M, 3 print B
    int input = 8;

    while (input!=5){

        // show map
        for (int i=0;i<64;i++){
            if((i%8) != 7 ){ // not yet end of row
                if (i!=position && i!= money && i!= booster){
                    printf(". ");
                }else if(i == position){
                    printf("P ");
                }else if(i == money){
                    printf("M ");
                }else if(i == booster){
                    printf("B ");
                }else{
                    printf("sth weird");
                }
            }else{
                if (i!=position && i!= money && i!= booster){
                    printf(".\n");
                }else if(i == position){
                    printf("P\n");
                }else if(i == money){
                    printf("M\n");
                }else if(i == booster){
                    printf("B\n");
                }else{
                    printf("sth weird");
                }
            }
        }

        printf("\n[1] to move up\n[2] to move down\n[3] to move left\n[4] to move right\n[5] to leave map\n");
        printf("Your move: ");
        scanf("%d", &input);
        // perform actions


        switch(input){
            case 1:
                // check enough money
                if(check_money(saving) && position >= 8){ // valid
                    position -= 8;
                    *saving -= 25;
                }else if(check_money(saving) == 0){ // lack of money
                    printf("You are too poor\n");
                }else if(position < 8){
                    printf("out of boundary\n");
                }else {
                    printf("unexpected in case 1");
                }
                break;
            case 2:
                // check enough money
                if(check_money(saving) && position <= 55){ // valid
                    position += 8;
                    *saving -= 25;
                }else if(check_money(saving) == 0){ // lack of money
                    printf("You are too poor\n");
                }else if(position > 55){
                    printf("out of boundary\n");
                }else {
                    printf("unexpected in case 1");
                }
                break;
            case 3:
                // check enough money
                if(check_money(saving) && (position % 8) != 0){ // valid
                    position -= 1;
                    *saving -= 25;
                }else if(check_money(saving) == 0){ // lack of money
                    printf("You are too poor\n");
                }else if((position % 8) == 0){
                    printf("out of boundary\n");
                }else {
                    printf("unexpected in case 1");
                }
                break;
            case 4:
                // check enough money
                if(check_money(saving) && (position % 8) != 7){ // valid
                    position += 1;
                    *saving -= 25;
                }else if(check_money(saving) == 0){ // lack of money
                    printf("You are too poor\n");
                }else if((position % 8) == 7){
                    printf("out of boundary\n");
                }else {
                    printf("unexpected in case 1");
                }
                break;
        }

        // check if M and B are picked up
        if (position == money){
            // row number = (position / 8) + 1 column num = position % 8
            int row_num = (position / 8) + 1;
            int column_num = position % 8;
            *saving += row_num*column_num*10;
            money = 65; // just put it out of map
        }

        if (position == booster){
            printf("You have won a booster\n");
            int prize_type = (rand()%3);
            boosters[prize_type] += 1;
            boosters_pop_relations(boosters, obtain_booster(booster_slots, len_of_booster_slots, prize_type));
            booster = 66;
        }

        printf("you still have %d\n", *saving);
    }

}


int gamble(int *saving){

    print_matchups(saving);


    return 0;
}

int contract(){

    printf(" ---------------------------------------------------------------------------------- \n");
    printf("|  CONTRACT                                                                        |\n|                                                                                  |\n|  Gambling is not a good thing to do. It is not gauranteed that you will win any  | \n|  money at all. According to the law of hotdog game, losing money because of any  |\n|  form of gambling is considered to be idiocy at its best in this game.           |\n|  Thus you take full responsibility of the consequences                           |\n|  you might face entering the gamblling section of this game.                     |\n");
    printf(" ---------------------------------------------------------------------------------- \n");
    printf("[1] to continue(terms agreed)\n[2] to leave\n");
    int input;
    scanf("%d", &input);
    if (input == 1){
        return 1;
    }else{
        return 0;
    }
}


int print_country(int country){

        switch(country){
            case 1:
                printf("Netherlands");
                break;
            case 2:
                printf("USA");
                break;
            case 3:
                printf("Argentina");
                break;
            case 4:
                printf("Australia");
                break;
            case 5:
                printf("France");
                break;
            case 6:
                printf("Poland");
                break;
            case 7:
                printf("England");
                break;
            case 8:
                printf("Senegal");
                break;
            case 9:
                printf("Japan");
                break;
            case 10:
                printf("Croatia");
                break;
            case 11:
                printf("Brazil");
                break;
            case 12:
                printf("Korea Republic");
                break;
            case 13:
                printf("Morocco");
                break;
            case 14:
                printf("Spain");
                break;
            case 15:
                printf("Portugal");
                break;
            case 16:
                printf("Switzerland");
                break;
        }
    }
int print_matchups(int *saving){


    // date --> [0] date game num --> [0] date [1-2]team num [3-4] result -->
    // [0][][] -- [4][][] is round of 16
    // 1. Netherlands, 2. USA, 3. Argentina, 4.Australia, 5. Japan, 6. Croatia,7,Brazil 8. South Korea 9. England
    // 10. Senegal 11. France 12 Poland 13 Morocco 14 Spain 15. Portugal 16. Switzerland
    static int match_ups[10][2][5] = {/*16*/{{3,1,2,3,1},{0}},{{4,3,4,2,1},{4,5,6,3,1}},{{5,7,8,3,0},{5,9,10,1,1}},
                                    {{6,11,12,4,1},{6,13,14,0,0}},{{7,15,16,6,1},{0}}/*8*/,{{9,10,11,1,1},{0}},
                                    {{10,1,3,2,2},{10,13,15,1,0}},{{11,7,5,1,2},{0}},/*4*/{{14,3,10,3,0},{0}},{{15,5,13,2,0},{0}}};

    static int called_times = 0;
         // variables
    int input = 0;
    int choosed_team = 0; // 當使用者選擇猜勝隊，存取其猜測
    int predicted[2]; // 當使用者選擇猜比數，存取其猜測
    int choosed_matchup = 0; // 當一晚有兩場比賽，讓其選擇要賭場次
    int principle = 0;

    printf("\ntoday is %d Dec 2022. You have worked hard and mama said you can\ngamble a bit since FIFA is on. You only have a shoot, think twice.\n\n", match_ups[called_times][0][0]);
    if(contract() == 1){

    }else{
        return 0;
    }
    if (match_ups[called_times][1][0] != 0){ // two matches today
        printf("matches today will be\n");
        print_country(match_ups[called_times][0][1]);printf(" VS. ");print_country(match_ups[called_times][0][2]);printf("\n");
        print_country(match_ups[called_times][1][1]);printf(" VS. ");print_country(match_ups[called_times][1][2]);printf("\n\n");
    }else{ // one match today
        printf("\nmatch today will be\n");
        print_country(match_ups[called_times][0][1]);printf(" VS. ");print_country(match_ups[called_times][0][2]);printf("\n\n");
    }


    // Decide gamble type and enter input
    while(input <=0 || input >= 3){
        printf("Choose gambling type\n  [1] Win Team\n  [2] Correct Scores\n  [3] Information\n Enter Type: ");
        scanf("%d", &input);
        if(input <=0 || input >= 4){
            printf("Invalid input !!!\n");
        }
        if(input == 3){
            printf(" -------------------------------------------------------------------\n");
            printf("| Win Team: Guess the correct team that wins, earn principle * 2    |\n");
            printf("| Correct Scores: Guess the correct final scores, earn principle * 3|\n");
            printf(" -------------------------------------------------------------------\n");
        }
    }

    // 本金
    printf("Priniciple: ");
    scanf("%d", &principle);
    *saving -= principle;


    // Two senerio, one game tonight or two games tonight
    if (match_ups[called_times][1][0] != 0){ // two matches tonight

        // 選要賭的場次
        while(choosed_matchup <=0 || choosed_matchup >= 3){
            printf("Which matchup do you wanna gamble?\n[1] first matchup\n[2] second math up\n");
            scanf("%d", &choosed_matchup);
            if(choosed_matchup <=0 || choosed_matchup >= 3){
                printf("Invalid input !!!\n");
            }
        }

        switch(input){
        case 1: // choose to play win team
            while(choosed_team<=0 || choosed_team>=4){
                printf("Choose the team you think will win\n");
                printf("[1] ");print_country(match_ups[called_times][choosed_matchup-1][1]);printf("\n");
                printf("[2] ");print_country(match_ups[called_times][choosed_matchup-1][2]);printf("\n");
                printf("[3] Draw\n");
                scanf("%d", &choosed_team);

                if(choosed_team <=0 || choosed_team >= 4){
                printf("Invalid input !!!\n");
                }

                inspect_win(match_ups[called_times][choosed_matchup-1], choosed_team, saving, principle);
            }

            break;
        case 2:
            printf("Your predictions(e.g. 2:1 or 0:0):");
            scanf("%d:%d", &predicted[0], &predicted[1]);

            inspect_score(match_ups[called_times][choosed_matchup-1], predicted, saving, principle);
            break;
        }
    }else{ // one game tonight

        switch(input){
        case 1: // choose to play win team
            while(choosed_team<=0 || choosed_team >= 4){
                printf("Choose the team you think will win\n");
                printf("[1] ");print_country(match_ups[called_times][0][1]);printf("\n");
                printf("[2] ");print_country(match_ups[called_times][0][2]);printf("\n");
                printf("[3] Draw\n");
                scanf("%d", &choosed_team);

                if(choosed_team<=0 || choosed_team >= 4){
                    printf("Invalid input !!!\n");
                }
            }

            int result = inspect_win(match_ups[called_times][0], choosed_team, saving, principle);

            break;
        case 2:
            printf("Your predictions(e.g. 2:1 or 0:0), pleaze type carefully in accordance to the format:");
            scanf("%d:%d", &predicted[0], &predicted[1]);

            inspect_score(match_ups[called_times][0], predicted, saving, principle);

            break;

        }
    }
    called_times++;
    return 0;
}

// 賭勝隊才呼叫此func檢查是否正確並轉錢
int inspect_win(int *match_ups, int choosed_team, int *saving, int principle){
    // 如果選的是1，檢查[3] 是否>[4]
    // 如果選的是2，檢查[4] 是否>[3];
    // 如果選的是3，檢查[3]、[4]是否相等
    switch(choosed_team){
        case 1:
            if(match_ups[3] > match_ups[4]){
                printf("You have won$%d\n", principle*2);
                *saving += principle * 2;
                return 1;
            }else{
                printf("Told you to think twice! You have thrown $%d into the water\n", principle);
                return 0;
            }
        case  2:
            if(match_ups[4] > match_ups[3]){
                printf("You have won$%d\n", principle*2);
                *saving += principle * 2;
                return 1;
            }else{
                printf("Told you to think twice! You have thrown $%d into the water\n", principle);
                return 0;
            }
        case 3:
            if(match_ups[3] == match_ups[4]){
                *saving += principle * 2;
                return 1;
            }else{
                printf("Told you to think twice! You have thrown $%d into the water\n", principle);
                return 0;
            }
    }
}

int inspect_score(int *match_ups ,int *predicted, int *saving, int principle){
    if (predicted[0] == match_ups[3] && predicted[1] == match_ups[4]){
        printf("You have won $%d\n", principle*3);
        *saving += principle * 3;
        return 1;
    }else{
        printf("Told you to think twice! You have thrown $%d into the water\n", principle);
        return 0;
    }
}
// 檢查預測並計算結果



