#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
// process : open/close booster --> ask actions --> result check --> continue_or_not --> lottery

// 
// 進度: 換圖完成，但24的時候有算錯。
// result check area 5沒有加上booster效果，但實際上有賺到
// 換圖 : choosen_arr reallocate, open_cost = 0;
// 每天結束記得加一次免費抽
// 問題：兩個booster 同時開只計算到一個


// ************可能的問題***************** 
// 1. user check results時輸入不合法
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

void result_check(int results[], int results_sell[][2], int booster_status[]);

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
    printf("Welcome, young boss!\n");
    // initialize variables
    int len_of_booster_slots = slot_len();
    int booster_slots[len_of_booster_slots];
    int up_speed_cost = 50, up_taste_cost = 100;
    int speed_level = 0, time = 15,  taste_level = 0, price = 0; 
    int saving = 1000000;
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
        result_check(results, results_sell, booster_status);
        ask_continue(&continue_or_not, &free_choices);
        if (continue_or_not == 0){
            return 0;
        }
        //************************************************************************************
        //graphing(3, &cell_num);
        graph_main(&free_choices, &open_cost, &n, choosen_arr, &saving, &boosters[0], &price, &cell_num, &prizes_left, booster_slots, len_of_booster_slots);
        
        // testing 
        printf("boosters : %d %d %d\nbooster slots: ", boosters[0], boosters[1], boosters[2]);
        for (int i=0;i<len_of_booster_slots;i++){
            printf("%d ", booster_slots[i]);
        }
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
            printf("case 3\n");
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
            printf("case 4\n");
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
            printf("case 5\n");
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
            printf("case 6\n");
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

void result_check(int results[], int results_sell[][2], int booster_status[]){
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
    printf("arr[0]:%d arr[1]:%d arr[2]:%d\n", arr[0], arr[1], arr[2]);
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

// 暫時完成前面的BOOSTER，但其實沒有完全RANDOM.