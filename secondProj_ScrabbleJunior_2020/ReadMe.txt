1MIEIC05 - André Pereira up201905650 50% / Matilde Oliveira up201906954 50%

____CommonConsiderations____
Class Word and Class Board are the "same" but some methods and arguments and others have been removed, because they were not used in the respective program

____BoardBuilder____
All of the objectives have been accomplished.
The program runs with all the given rules.
The 2d view of the board have been added in the .txt file
Is necessary to have a file named "WORDS.TXT"(dictionary) in order to let the program run

MyDecisions:
While adding a new word to the board if it creats a new one, by interception the existing words, the program ask to user if he want to make those changes, in order to add the given word.


____ScrabbleJunior____
Considering the given game rules, all of them were implemented.
A file named SmallTestBoard.txt goes in the folder with the .cpp and .h files. This board have been created with our boardBuilder 

Inputs:
FILENAME: it must be written the filename followed by .txt;
NUMBER OF PLAYERS: it only accepts numbers between 2-4 as in the rules;
LETTERS: Although the letters on the board are uppercases it accepts both lower and uppercase;
POSITION: For the position of the letter in board it may be asked to write similar to the example (the line in uppercase and the column in lower case) but it accepts both letters lower or upper, but assumes that the order is right: first the line and then the column;

Outputs:
BOARD: grey background for the indexs of the lines and columns and black background for the board with the letters white or green if they are already played;
POOL: shown in the the begin of each round (after every player doing his moves);
PLAYER INFORMATION: it is shown in the beginning of every move of each player with it’s name, points and hand in the player’s attributed color;
WINNERS: the ones with the most points (if there are the same points then there are two or more winners).

 
MyDecisions:
1. When the number of letters is less than 28 for 4 player, or 21 for 3, or 14 for 2, we came out with a function nLettersInHand=0.0195*NumberOfLines*NumberOfColumns +2.7 that gives a more reasonable amount of letters for each player and taking in account that when the has an odd number it gives one less in order to give the same amount to each player.

Rules:
1. You must play 2 letters if you have the possibility. If you only have 1 letter to play you must play it and if the pool has letters you must draw one from your hand and take other from the pool (if the pool doesn’t have it you only do the move). If you don’t have possible letters to play (0) you must draw back two or one depending on how much letters the pool has (if the pool is empty then you don’t play anything). 
2. If you finish a word you receive 1 point and if you finish 2 word you receive 2 points.
3. The game ends when the pool doesn’t have letters and the players finish all their letters.