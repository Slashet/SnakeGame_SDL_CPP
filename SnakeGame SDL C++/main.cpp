#include <iostream>         // c++ input output lib 
#include <ctime>            // to make more random rand function 
#include <cstdlib>          // to use rand function

#include <SDL2/SDL.h>       // SDL lib
#include <SDL2/SDL_image.h> // SDL image lib

// window size
const int WIDTH = 720, HEIGHT = 720;
// game box size
const int sizeOfGame = 40;

class SnakeGame{
    public:
        // snake variables
        int snakeSpeed = sizeOfGame;
        int snakeLenght = 0;
        int snakePos[2] = {9 * 40, 9 * 40};
        
        int tailPos[323][2];
        int tailNextPos[324][2];

        // north - 0 | east - 1 | west - 2 | south - 3
        int direction = 1;

        //food variables
        int foodPos[2] = {sizeOfGame * (rand() % (WIDTH / sizeOfGame)), sizeOfGame * (rand() % (HEIGHT / sizeOfGame))};
        
        // game variables
        int gameSpeed = 3 * sizeOfGame;  // Standart gameSpeed
        int gameSpeedd = gameSpeed;      // Actual gameSpeed 
        bool isPause = false;            // is game paused
        bool isRunning = true;           // is game working

        // making food and snake object
        SDL_Rect player{snakePos[0], snakePos[1], sizeOfGame, sizeOfGame};
        SDL_Rect food{foodPos[0], foodPos[1], sizeOfGame, sizeOfGame};

        // creating window and renderer
        SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        // creating Event variable
        SDL_Event windowEvent;
    void start(){
            //Check SDL
            if (SDL_Init(SDL_INIT_VIDEO) < 0){std::cout << std::endl << "SDL could not be initialized: " << SDL_GetError();}
            else {std::cout << std::endl << "SDL VIDEO - OK";}
            //Check SDL TTF
            std::cout << std::endl << "-----------------------------------------" << std::endl;
    }
    void gameLoop(){
        while(isRunning){  // Crating infint loop till we exit
            gameEvent();

            if (!isPause){
                snakeMovement();
                tailsMovement();
            }
            drawBackGround();
            draw();


            SDL_RenderPresent(renderer);
            SDL_Delay(gameSpeed);   
        }

    }
    void gameEvent(){
        //SDL events
        while(SDL_PollEvent(&windowEvent)){
            // checking is user close to window
            if(windowEvent.type == SDL_QUIT){
                SDL_Quit();
                exit(0);
            }
            // the events when keyboard button down
            if (SDL_KEYDOWN){ }
            // the events when keyboard button up
            if (SDL_KEYUP){
                if(windowEvent.key.keysym.sym == SDLK_SPACE && isPause == false) {isPause = true;}
                else if(windowEvent.key.keysym.sym == SDLK_SPACE && isPause == true) {isPause = false;}
                
                if(windowEvent.key.keysym.sym == SDLK_UP && direction != 3) {direction = 0;}
                if(windowEvent.key.keysym.sym == SDLK_DOWN && direction != 0) {direction = 3;}
                if(windowEvent.key.keysym.sym == SDLK_LEFT && direction != 1) {direction = 2;}
                if(windowEvent.key.keysym.sym == SDLK_RIGHT && direction != 2) {direction = 1;}
    
            }
        }
            
        //Eat food
        if (snakePos[0] == foodPos[0] && snakePos[1] == foodPos[1]){
            snakeLenght += 1;
            gameSpeed -= gameSpeedd / 10;
            foodPos[0] = sizeOfGame * (rand() % (WIDTH / sizeOfGame));
            foodPos[1] = sizeOfGame * (rand() % (HEIGHT / sizeOfGame));
        }

        //tail check
        for (int i = 0; i < snakeLenght; i++){
            // if food spawns snake tail
            if (tailPos[i][0] == foodPos[0] && tailPos[i][1] == foodPos[1]){
                foodPos[0] = sizeOfGame * (rand() % (WIDTH / sizeOfGame));
                foodPos[1] = sizeOfGame * (rand() % (HEIGHT / sizeOfGame));
            }
            // if snake hit its own tail
            if (tailPos[i][0] == snakePos[0] && tailPos[i][1] == snakePos[1]){gameLose();}
        }
        // set food x, y to foodPos array
        food.x = foodPos[0];
        food.y = foodPos[1];
    }
    void drawBackGround(){
        SDL_SetRenderDrawColor(renderer, 160, 250, 160, 255); // <- set the color darkGreen
        SDL_RenderClear(renderer); // <- making all surface to SDL renderer current color

        SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255); // <- set the color lightGreen
        for (int i = 0; i<((WIDTH / sizeOfGame) * (HEIGHT / sizeOfGame)); i++){ // to make background cuby
            // check to location to draw
            if (((i % (WIDTH / sizeOfGame)) % 2 == 0 && (i / (HEIGHT / sizeOfGame)) % 2 == 0) || ((i % (WIDTH / sizeOfGame)) % 2 == 1 && (i / (HEIGHT / sizeOfGame)) % 2 == 1)){
                // make background cube
                SDL_Rect bgCube{(i % (WIDTH / sizeOfGame)) * sizeOfGame, (i / (HEIGHT / sizeOfGame)) * sizeOfGame, sizeOfGame, sizeOfGame};
                SDL_RenderFillRect(renderer, &bgCube); // set background cube color
            }
        }
    }
    void snakeMovement(){
        // snake moves to its direction
        if (direction == 0 && snakePos[1] > 0){ snakePos[1] -= snakeSpeed; }
        if (direction == 1 && snakePos[0] < WIDTH - sizeOfGame){ snakePos[0] += snakeSpeed; }
        if (direction == 3 && snakePos[1] < HEIGHT - sizeOfGame){ snakePos[1] += snakeSpeed; }
        if (direction == 2 && snakePos[0] > 0){ snakePos[0] -= snakeSpeed; }
        // set snake x, y to snakePos array
        player.x = snakePos[0];
        player.y = snakePos[1];
    }
    void draw(){
        for (int i=0; i<snakeLenght; i++){
            // making snake tail
            SDL_Rect tail{tailPos[i][0], tailPos[i][1], 40, 40};
            // set snake tails color
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &tail);
            
        }

        // set snake head color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        // set food color
        SDL_SetRenderDrawColor(renderer, 255, 130, 130, 255);
        SDL_RenderFillRect(renderer, &food);
    }
    void tailsMovement(){
        for (int i = 0; i < snakeLenght; i++){
            //setting tails position
            tailPos[i][0] = tailNextPos[i][0];
            tailPos[i][1] = tailNextPos[i][1];
        }
        // setting first tail next position
        tailNextPos[0][0] = snakePos[0];
        tailNextPos[0][1] = snakePos[1];
        for (int i = 0; i < snakeLenght; i++){
            // setting other tails next position
            tailNextPos[i+1][0] = tailPos[i][0];
            tailNextPos[i+1][1] = tailPos[i][1];
        }
    }
    void gameLose(){
        snakeLenght = 0;
        snakePos[0] = 9 * 40;
        snakePos[1] = 9 * 40;
        gameSpeed = gameSpeedd;
        isPause = true;
        foodPos[0] = sizeOfGame * (rand() % (WIDTH / sizeOfGame));
        foodPos[1] = sizeOfGame * (rand() % (HEIGHT / sizeOfGame)); 
    }

    private:

};


int main(int argc, char *argv[]){
    // prepare to rand
    srand(static_cast<unsigned int>(time(nullptr)));

    //  initialized the SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // calling class
    SnakeGame snakeGame;
    // start the checking function
    snakeGame.start();
    // start the game
    snakeGame.gameLoop();

    // destroy window
    SDL_DestroyWindow(snakeGame.window);
    // destroy font
    // quit
    SDL_Quit();
    return 0;   
}