#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct soldier {
    int health;
    int strength;
} Soldier;

typedef struct stack {
    Soldier soldiers[100];
    int top;
} Stack;

int isEmpty(Stack* s);

void push(Stack *s, Soldier soldier) {
    if(s->top < 99) {
        s->top++;
        s->soldiers[s->top] = soldier;
    } else {
        printf("Stack is full, can't add soldier.\n");
    }
}


Soldier pop(Stack *s) {
    if(!isEmpty(s)) {
        return s->soldiers[s->top--];
    } else {
        printf("Stack is empty, can't pop soldier.\n");
        Soldier emptySoldier = {-1, -1};
        return emptySoldier;
    }
}

Soldier top(Stack *s) {
    return s->soldiers[s->top];
}



int isEmpty(Stack* s) {
    return s->top == -1;
}

int damageCalculation(int strength1, int strength2) {
    return (strength1 - strength2) * 0.05 + 50;
}

Soldier randomSoldier() {
    Soldier s;
    s.health = (rand() % 100) + 1;
    s.strength = (rand() % 100) + 1;
    return s;
}

void criticalShot(Stack *s1, Stack *s2, int *turn) {
    if (*turn == 1) {
        printf("Critical shot by 1\n~> 2 has a casualty!\n");
        while (!isEmpty(s2)) {
            pop(s2);
        }
        *turn = 2;
    } else {
        printf("Critical shot by 2\n~> 1 has a casualty!\n");
        while (!isEmpty(s1)) {
            pop(s1);
        }
        *turn = 1;
    }
}



void Summary(Stack *s1, Stack *s2) {
    printf("-*-*-*-*-*-finish-*-*-*-*-*-\n");
    if (s1->top > s2->top) {
        printf("Side 1 wins \n");
    } else if (s1->top < s2->top) {
        printf("Side 2 wins \n");
    } else {
        printf("It's a draw\n");
    }
}

int main() {
    srand(time(NULL));
    Stack side1, side2;
    side1.top = -1;
    side2.top = -1;
    int isBattleHappend = 0;

    int turn = 1;

    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char command[100];

    while (fscanf(file, "%s", command) != EOF) {
        if (command[0] == 'A' || command[0] == 'C' || command[0] == 'F' || command[0] == 'R') {
         switch(command[0]) {
            case 'A':
            {
                int side;
                Soldier soldier;
                fscanf(file, " %d %d,%d", &side, &soldier.health, &soldier.strength);
                if (side == 1) {
                    push(&side1, soldier);
                    printf("Add soldiers to side 1\n");
                    printf("S- H:%d S:%d\n", soldier.health, soldier.strength);
                } else {
                    push(&side2, soldier);
                    printf("Add soldiers to side 2\n");
                    printf("S- H:%d S:%d\n", soldier.health, soldier.strength);
                }
                break;
            }
            case 'F':
            {
                if (isEmpty(&side1) || isEmpty(&side2)) {
                    printf("One of the sides has no soldiers left. Can't fight.\n");
                    continue;;
                }
                isBattleHappend = 1;
                if (turn == 1) {
                    Soldier s1 = top(&side1);
                    Soldier s2 = top(&side2);
                    int damage = damageCalculation(s1.strength, s2.strength);
                    printf("1 hit  %d damage\n", damage);
                    s2.health -= damage;
                    if (s2.health <= 0) {
                        printf("->2 has a casualty\n");
                        pop(&side2);
                    }
                    turn = 2;
                } else {
                    Soldier s1 = top(&side1);
                    Soldier s2 = top(&side2);
                    int damage = damageCalculation(s2.strength, s1.strength);
                    printf("2 hit %d damage\n", damage);
                    s1.health -= damage;
                    if (s1.health <= 0) {
                        printf("->1 has a casualty\n");
                        pop(&side1);
                    }
                    turn = 1;
                }
                break;
            }
            case 'R':
            {
                int side;
                fscanf(file, " %d", &side);
                Soldier s = randomSoldier();
                if (side == 1) {
                    printf("Called reinforcements to side 1\n");
                    printf("S- H:%d S:%d\n", s.health, s.strength);
                    push(&side1, s);
                } else {
                    printf("Called reinforcements to side 2\n");
                    printf("S- H:%d S:%d\n", s.health, s.strength);
                    push(&side2, s);
                }
                break;
            }
            case 'C':
                criticalShot(&side1, &side2, &turn);
                break;
            default:
                printf("Invalid command: %s\n", command);
                printf("Please edit your input file and start over!");
                break;
        }
       } 
       else{
                printf("Invalid command: %s\n", command);
                printf("Game ends due to invalid command.\n");
                printf("Please edit your input file and start over!");
                break;
       }
    }

    if (isBattleHappend) {
        Summary(&side1, &side2);
    } else {
        printf("\n");
    }
    fclose(file);
    return 0;
}

