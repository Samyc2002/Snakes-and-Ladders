# Snakes-and-Ladder
A CS course project at IIT Dharwad to make a simple snakes and ladder game.

# To run the program...
open up the terminal and type the following
```
gcc Main.c Headers/position.c Headers/patwriting.c
a.exe
```
# Rules of the Game:
+ Each player starts with a position 0.
+ Take turns to roll the dice and enter the value you get in the terminal. You move forward the number of spaces shown on the dice.
+ To move forward from position 0, your dice must turn up 1.
+ If you land at the bottom of a ladder, you can move up to the top of the ladder.
+ If you land on the head of a snake, you must slide down to the bottom of the snake.
+ If a six turns up, you get another chance to roll the dice after moving six places. If you get another six, you can roll a dice again after moving six more places. However, if you get six a third time, all these 12 spaces along with this six will be cancelled, but you can give input again.
+ If you land in a position after which your progress is limited (i.e. 95-99), and you give an input which makes you go beyond a hundred by some places      (say, for example, n), then you will bounce back by the same number of places. For example, let’s say you are at 97, and you get a 5, then you would land on 102, so, you get bounced back by 2 places and end up at 98.
+ The first player to get to the space 100 is the winner.