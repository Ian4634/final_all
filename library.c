#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
int obtain_booster(int *arr, int len, int new){
    int temp;
    int popped = arr[len-1];
    for (int i=len;i>=2;i--){
        arr[i-1] = arr[i-2];
    }
    arr[0] = new;
    return popped;
}

int random2(){
    static int srand_value = 5;
    srand(srand_value);
    srand_value++;
    printf("%d\n", srand_value);
    return (rand()%63)+1;
}

// this func sqush items together. e.g. {1,4,2} --> {1,2,4} which 4 means space
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

void boosters_pop_relations(int *boosters,int *saving, int popped){
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

void map_game(int *saving){
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
    int map[64] = {0}; // if 1, print P, 2 print M, 3 print B
    int input;
    printf("M:%d B:%d\n", money, booster);
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
        printf("you still have %d\n", *saving);
    }
    
}

int main(){
    int saving = 1000;
    map_game(&saving);
    
}










