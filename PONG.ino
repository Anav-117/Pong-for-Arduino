#include <Arduboy.h>
Arduboy arduboy;
 
//Variables declared here
int gamestate = 0;
int ballx = 62;
int bally = 0;
int ballsize = 4;
int ballright = 1;
int balldown = 1;
int paddlewidth = 4;
int paddleheight = 9;
int player1x = 0;
int player1y = 0;
int player2y = 0;
int player2x = 127 - paddlewidth;
int player1score = 0;
int player2score = 0;
bool singleplayer = true;
int selectory = 12;
int computerspeed = 2;

void resetGame() {
  ballx = 63;
  player1score = 0;
  player2score = 0;
}

void setup() {
  arduboy.beginNoLogo();
  //Seed the random number generator
  arduboy.initRandomSeed();
  //Set the game to 60 frames per second
  arduboy.setFrameRate(60);
  arduboy.clear();
}

void loop() {

  //Frame Capping
  if(!arduboy.nextFrame()) {
    return;
  }
  arduboy.clear();

  //Game code here
  switch( gamestate ) {

    case 0:
      //Title screen
      arduboy.setCursor(42, 25);
      arduboy.setTextSize(2);
      arduboy.print("PONG");
      arduboy.setTextSize(1);
      delay(200);
      //Change the gamestate
      if (arduboy.pressed(A_BUTTON)) {
        gamestate = 4;
        //delay(50);
      }
      break;

    case 1:
      //Gameplay screen
      //Display the player's score
      arduboy.setCursor(20, 0);
      arduboy.print(player1score);
      //Display the player2's score
      arduboy.setCursor(101, 0);
      arduboy.print(player2score);
      //Draw the ball
      arduboy.fillRect(ballx, bally, ballsize, ballsize, WHITE);
      //Move the ball right
      if(ballright >= 1) {
        ballx = ballx + ballright;
      }
      //Move the ball left
      if(ballright <= -1) {
        ballx = ballx + ballright;
      }

      //Move the ball down
      if(balldown == 1) {
        bally = bally + 1;
      }
      //Move the ball up
      if(balldown == -1) {
        bally = bally - 1;
      }
      //Reflect the ball off of the top and bottom of the screen
      if(bally == 0 || bally + ballsize == 63) {
        balldown = -balldown;
      }
    
      //Draw the player's paddle
      arduboy.fillRect(player1x, player1y, paddlewidth, paddleheight, WHITE);
      //If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
      if(arduboy.pressed(UP_BUTTON) && player1y > 0) {
        player1y = player1y - 2;
      }
      //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
      if(arduboy.pressed(DOWN_BUTTON) && player1y + paddleheight < 63) {
        player1y = player1y + 2;
      }
      //Draw the player2's paddle
      arduboy.fillRect(player2x, player2y, paddlewidth, paddleheight, WHITE);

      if (singleplayer == true) {
        if (ballx > 96) {
          if (bally < player2y) {
            player2y = player2y - computerspeed;
          }
          //If the bottom of the ball is lower than the bottom of the computer's paddle, move the comptuer's paddle down
          if (bally + ballsize > player2y + paddleheight) {
            player2y = player2y + computerspeed;
          }
        }
      }
      else {
        if(arduboy.pressed(LEFT_BUTTON) && player2y > 0) {
          player2y = player2y - 2;
        }
        //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
        if(arduboy.pressed(RIGHT_BUTTON) && player2y + paddleheight < 63) {
          player2y = player2y + 2;
        }
      }
      //If the ball moves off of the screen to the left...
      if(ballx < -10) {
        //Move the ball back to the middle of the screen
        ballx = 63;
        ballright = 1;
        //Give the player2 a point
        player2score = player2score + 1;
        computerspeed = random(1, 2);
      }
      //If the ball moves off of the screen to the right....
      if(ballx > 130) {
        //Move the ball back to the middle of the screen
        ballx = 63;
        ballright = -1;
        //Give the player a point
        player1score = player1score + 1;
        computerspeed = random(1, 2);
      }

      //Check if the player wins
      if(player1score == 5) {
        gamestate = 2;
      }
      //Check if the player2 wins
      if(player2score == 5) {
        gamestate = 3;
      }

      //If the ball makes contact with the player's paddle, bounce it back to the right
      if (ballx == player1x + paddlewidth && player1y < bally + ballsize && player1y + paddleheight > bally) {
        ballright = -ballright;
        ballright += (ballright < 3) ? 0.5 : 0;
      }
      //If the ball makes contact with the player2's paddle, bounce it back to the left
      if (ballx + ballsize == player2x && player2y < bally + ballsize && player2y + paddleheight > bally) {
        ballright = -ballright;
        ballright -= (ballright > -3) ? 0.5 : 0;
      }
      
      break;
 
    case 2:
      //Win screen
      arduboy.setCursor(20, 20);
      arduboy.setTextSize(2);
      if (singleplayer) {
        arduboy.print("YOU WIN"); 
      }
      else {
        arduboy.print("PLAYER 1 \r\n   WINS");  
      }
      arduboy.setTextSize(1);
      //Change the gamestate
      if (arduboy.pressed(A_BUTTON)) {
        resetGame();
        gamestate = 0;
      }
      break;

    case 3:
      //Game over screen
      arduboy.setCursor(20, 20);
      arduboy.setTextSize(2);
      if (singleplayer) {
        arduboy.print("YOU LOSE"); 
      }
      else {
        arduboy.print("PLAYER 2 \r\n   WINS");  
      }
      arduboy.setTextSize(1);
      //Change the gamestate
      if (arduboy.pressed(A_BUTTON)) {
        resetGame();
        gamestate = 0;
      }
      break;

    case 4:
      //mode select screen 
      arduboy.setCursor(10, 10);
      arduboy.setTextSize(1);
      arduboy.print("1 PLAYER");
      arduboy.setCursor(10, 20);
      arduboy.print("2 PLAYERS");
      arduboy.fillCircle(80, selectory, 2, WHITE);
      delay(150);
      if (arduboy.pressed(DOWN_BUTTON) && selectory == 12) {
        selectory = 22;
      }
      if (arduboy.pressed(UP_BUTTON) && selectory == 22) {
        selectory = 12;
      }
      if (arduboy.pressed(A_BUTTON)) {
        singleplayer = (selectory == 12) ? true : false;
        gamestate = 1;
      }
  }

  arduboy.display();
}
