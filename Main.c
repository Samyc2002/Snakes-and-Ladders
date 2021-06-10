#include<conio.h>
#include<time.h>
#include "Headers/position.h"

struct players {
   char name[100];
   int position; 
};

typedef struct players players;
int num;
players* player;

int long_ladders;
int long_ladders_length;
int short_ladders;
int short_ladders_length;
int long_snakes;
int long_snakes_length;
int short_snakes;
int short_snakes_length;
int* long_snakes_pos;
int* short_snakes_pos;
int* long_ladders_pos;
int* short_ladders_pos;

players search_winner(players player[], int n);

int search(int n, int arr[], int len);

int search_players(int n, players arr[], int len);

void delay(int number_of_seconds);

void file_input() 
{
   FILE* ladders;
   ladders = fopen("Ladders.txt","r");

   fscanf(ladders,"%d %d", &long_ladders, &short_ladders);
   fscanf(ladders,"%d %d", &long_ladders_length, &short_ladders_length);

   long_ladders_pos = (int*)malloc(long_ladders*sizeof(int));
   short_ladders_pos = (int*)malloc(short_ladders*sizeof(int));

   for(int i=0;i<long_ladders;i++) {
      fscanf(ladders,"%d ", &long_ladders_pos[i]);
   }

   for(int i=0;i<short_ladders;i++) {
      fscanf(ladders,"%d ", &short_ladders_pos[i]);
   }

   fclose(ladders);

   FILE* snakes;
   snakes = fopen("Snakes.txt","r");

   fscanf(snakes,"%d %d", &long_snakes, &short_snakes);
   fscanf(snakes,"%d %d", &long_snakes_length, &short_snakes_length);

   long_snakes_pos = (int*)malloc(long_snakes*sizeof(int));
   short_snakes_pos = (int*)malloc(short_snakes*sizeof(int));

   for(int i=0;i<long_snakes;i++) {
      fscanf(snakes,"%d ",&long_snakes_pos[i]);
   }

   for(int i=0;i<short_snakes;i++) {
      fscanf(snakes,"%d ",&short_snakes_pos[i]);
   }

   fclose(snakes);
}

void rulesOftheGame() {

   // Storing the rules
   char rules[8][500] = {
      "Each player starts with a position 0.",
      "Take turns to roll the dice and enter the value you get in the terminal. You move forward the number of spaces shown on the dice.",
      "To move forward from position 0, your dice must turn up 1.",
      "If you land at the bottom of a ladder, you can move up to the top of the ladder.",
      "If you land on the head of a snake, you must slide down to the bottom of the snake.",
      "If you get a six, you move forward by six places and then get a free turn. But if you get a six three times consecutively, all three sixes will be cancelled and you will be asked for an input again.",
      "If you land in a position after which your progress is limited, and you give an input which makes you go beyond a hundred by some places, then you will bounce back by the same number of places.",
      "The first player to get to the space 100 is the winner."
   };

   printf("Rules of the Game:\n");

   for(int i=0; i<8; i++) {

      // Printing the rules
      printf("%d. %s\n", i+1, rules[i]);
   }
   
   printf("For more information regarding the rules, refer to the README.md file in the project.\n\n");
}

void welcome() {

   system("cls");

   printf("\t\t\t\t\t\t   WELCOME TO THE SNAKES AND LADDERS GAME\n\n");

   rulesOftheGame();

   printf("How many players want to play ?\t");

   scanf("%d", &num);
   printf("\n");

   player = (players*)malloc(num*sizeof(players));

   for(int i=0; i<num; i++) {

      if(i==0) {
         printf("What do I call the 1st player ?\t");
      }
      else {
         printf("What do I call the %dth player ?\t", i+1);
      }

      scanf("%s", player[i].name);
      player[i].position = 0;
   }

   printf("Okay, so now that I know all of you, let's head on to the adventure.\n\n");
}

void usual_process(int *i, int option) {

   // For simplicity, we store *i in t for iterating;
   int t = *i;
	
   // This function will be called each time input is valid and dice number is not 6
   if(player[t].position+option <= 100) {

      // User can give any number as dice input without any issues
      player[t].position += option;

      if(search(player[t].position, long_snakes_pos, long_snakes)) {
      
         // Player has fallen on the mouth of a long snake
         player[t].position -= long_snakes_length;
         printf("Alas! there was a snake and you were eaten by it.\nYour current position is %d. Let's hope for the best from now.\n", player[t].position);
      }
      else if(search(player[t].position, short_snakes_pos, short_snakes)) {

         // Player has fallen on the mouth of a short snake
         player[t].position -= short_snakes_length;
         printf("Alas! there was a snake and you were eaten by it. Thankfully, it was a small one. \nYour current position is %d. Let's hope for the best from now.\n", player[t].position);
      }
      else if(search(player[t].position, long_ladders_pos, long_ladders)) {

         // Player has reached the bottom of a long ladder
         player[t].position += long_ladders_length;
         printf("Congrats! you reached a ladder.\nYour current position is %d. Keep up this good work.\n", player[t].position);
      }
      else if(search(player[t].position, short_ladders_pos, short_ladders)) {

         // Player has reached the bottom of a short ladder
         player[t].position += short_ladders_length;
         printf("Congrats! you reached a ladder (although short).\nYour current position is %d. Keep up this good work.\n", player[t].position);
      }
      else {

         // Player has neither reached a ladder nor has been eaten by a snake
         printf("Nice! You progressed by %d. Your current position is %d. Keep up the good work.\n", option, player[t].position);
      }
   }
   else {
      
      /* User can only give some limited inputs which are valid in the specific case. 
      For example, if user is at 97 and gives input as 5. Then, he goes to position 102, which doesn't even exist!!
      This is not possible in practical situation, hence, we need to handle this seperately,
      and in this case, the player bounces back by the extra amount, 2 as in the example */
      int bounceback = 100 - (player[t].position+option);
      player[t].position = 100;
      usual_process(i, bounceback);
   }

}

void corner_cases(int *i, int *option) {

   // The player has already entered a six. If he gives two more sixes, all these three sixes will be cancelled and he will be giving an input again.

   // For simplicity, we store *i in t for iterating;
   int t = *i;
   int p = *option;

   // Storing the initial position of the player in case three sixes come up and we need to restore the state
   int initialPosition = player[t].position;

   // Letting the player know about this opportunity
   printf("Yay! You gave a six. Now you get a free chance for another dice throw, but first you will move by six places.\nMoving six places....\n");

   // Calculating the position for the first six
   usual_process(i, 6);

   // Taking next dice input
   scanf("%d", &p);

   if(p>6 || p<1) {

      while(p>6 || p<1) {

         printf("Please enter a number between 1 to 6.\n");
         scanf("%d", option);
         p = *option;
      }
   }

   if(p == 6) {

      // Letting the player know about the oppotunity a second time
      printf("Yay! You gave another six. Now you get a free chance for another dice throw, but first you will move by six more places.\nMoving six places....\n");

      // Moving by six places
      usual_process(i, 6);

      // Taking next dice input
      scanf("%d", &p);

      if(p>6 || p<1) {

         // Invalid Input
         while(p>6 || p<1) {

            printf("Please enter a number between 1 to 6.\n");
            scanf("%d", option);
            p = *option;
         }
      }

      if(p == 6) {

         // Letting player know about the situation
         printf("Oops! You gave another six, which made three sixes in a row. You get another chance, but these three sixes will be cancelled.\nYour current position is %d\n", initialPosition);

         // Cancelling three sixes and restoring state
         player[t].position = initialPosition;

         // Taking next dice input
         scanf("%d", &p);

         if(p>6 || p<1) {

            // Invalid Input
            while(p>6 || p<1) {

               printf("Please enter a number between 1 to 6.\n");
               scanf("%d", option);
               p = *option;
            }
         }

         if(p == 6) {

            // The same process repeats, hence, calling the same function
            printf("Ah.....shit! Here we go again! :|\n");
            corner_cases(i, option);
         }
         else {

            // The player didn't get another six, hence, this is a usual case only
            printf("Oops bad luck. All these three sixes for nothing :(\n");
            usual_process(i, p);
         }
      }
      else {

         // Player got sixes, whixh were also calculated before. Hence, we call usual_process with the new input
         printf("Phew!!....That wasn't a six. You seem to have a crazy luck :)\n");
         usual_process(i, p);
      }
   }
   else {

      // Player got only one six, whixh was also calculated before. Hence, we call usual_process with the new input
      printf("Oops that wasn't a six, but nevermind you atleast progressed by %d places and got an extra turn too :D\n", p);
      usual_process(i, p);
   }
}

int main() {

   // Positioning snakes and ladders
   position();

   // Variable for taking dice input
   int option;

   // Variable for tracking current player
   int i=0;

   // Welcoming players to the game and taking their details
   welcome();
   
   //Taking input from Snakes.txt and Ladders.txt
   file_input();

   // Game begins here
   while(!search_players(100, player, num)) {

      // Taking dice input
      printf("Hey %s. Roll a dice and tell me what you get.\t", player[i].name);
      scanf("%d", &option);

      /* This is just an additional check to enable the feature that a player can progress with only a 6 at the beginning.
      That is, as long as a player doesn't have a six while starting the game, he/she cannot move to a non-zero potition */
      if(player[i].position == 0) {

         if(option>6 || option<1) {

            // Invalid input
            printf("Please enter a number between 1 to 6.\n");

            delay(2);
            system("cls");
            
            continue;
         }
         else if(option != 1) {

            printf("Oops! it wasn't a one. You need a one to start the game.\n");

            if( i == num-1 ) {

               printf("\n");
               i = 0;
            }
            else {

               i++;
            }

            delay(2);
            system("cls");
            
            continue;
         }
         else if(option == 1) {
            
            // Player is allowed to move in the next turn
            printf("Nice! You can go forward in the next turn!!\n");
            player[i].position = -1;

            if( i == num-1 ) {

               printf("\n");
               i = 0;
            }
            else {

               i++;
            }

            delay(2);
            system("cls");
            
            printf("\t\t\t\t\t\t\tSCOREBOARD\n");
            
            for(int j=0; j<num; j++) {

               (player[j].position == -1)?(printf("\t\t\t\t\t\t\t%s\t%d\n", player[j].name, player[j].position+1)):(printf("\t\t\t\t\t\t\t%s\t%d\n", player[j].name, player[j].position));
            }
            printf("\n");
            
            continue;
         }
      }

      if(option>6 || option<1) {

         // Invalid input
         printf("Please enter a number between 1 to 6.\n");
         continue;
      }
      else if (player[i].position == -1)
      {
         
         // Player was allowed to move in the last turn
         player[i].position++;
         
         if(option == 6) {

            // input is valid, but this is a corner case, hence, calling corner case_function
            corner_cases(&i, &option);
         }
         else {

            // Input is valid and this is an usual case, hence, calling usual_process function
            usual_process(&i, option);
         }
      }
      else if(option==6) {

         // input is valid, but this is a corner case, hence, calling corner case_function
         corner_cases(&i, &option);
      }
      else {

         // Input is valid and this is an usual case, hence, calling usual_process function
         usual_process(&i, option);
      }

      /* This is basically the iteration statement. 
      If i=n-1, i.e., if the end of player array has been reached and even now no winner is found,
      then, the whole array will just be repeated again. */
      if( i == num-1 ) {

         printf("\n");
         i = 0;
      }
      else {
         
         i++;
      }

      delay(2);
      system("cls");

      printf("\t\t\t\t\t\t\tSCOREBOARD\n");
      
      for(int j=0; j<num; j++) {

         (player[j].position == -1)?(printf("\t\t\t\t\t\t\t%s\t%d\n", player[j].name, player[j].position+1)):(printf("\t\t\t\t\t\t\t%s\t%d\n", player[j].name, player[j].position));
      }
      printf("\n");
   }

   // Declaring winner
   players winner = search_winner(player, num);
   printf("Winner is %s. Congratulations!!\nWe hope you had fun playing it. Do try it again sometime later.\n\n", winner.name);

   free(long_ladders_pos);
   free(short_ladders_pos);
   free(long_snakes_pos);
   free(short_snakes_pos);
   free(player);

   printf("Press any key to end the game.....");
   getch();

   return 0;
}

players search_winner(players player[], int n) {

   for(int i=0; i<n; i++) {

      if(player[i].position == 100) {

         return player[i];
      }
   }
}

int search_players(int n, players arr[], int len) {

   for(int i=0; i<len; i++) {

      if(arr[i].position==n) {

         // Value found
         return 1;
      }
   }

   // Value not found
   return 0;
}

void delay(int number_of_seconds) {
   
   // Converting time into milli_seconds
   int milli_seconds = 1000 * number_of_seconds;
  
   // Storing start time
   clock_t start_time = clock();
  
   // looping till required time is not achieved
   while (clock() < start_time + milli_seconds);
}