#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.005;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


double sqr(double n){ // created this function cuz it gives a cleaner code(I dont trust pow function).
    return n*n;}


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void collision(vector<Bubble> &bubbles, vector<Bullet> &bullets, Text &Score, string &score){
    // checl each vector objject for collision
    for (unsigned int i=0; i< bubbles.size(); i++){
        for (unsigned int j=0; j< bullets.size(); j++){
            if ((sqr(bullets[j].get_center_x()-bubbles[i].get_center_x()) + // using pythagorus theorem
                 sqr(bullets[j].get_center_y()-bubbles[i].get_center_y()))
                  <=(sqr(bubbles[i].get_radius()+5))){ // just put 5 since the width of bullet is constant and this will save time
                if (bubbles[i].get_radius() == 2*BUBBLE_DEFAULT_RADIUS){
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bubbles[i].get_radius()/2,BUBBLE_DEFAULT_VX/2,BUBBLE_DEFAULT_VY,COLOR(255,105,180)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bubbles[i].get_radius()/2,-BUBBLE_DEFAULT_VX/2,BUBBLE_DEFAULT_VY,COLOR(255,105,180)));}
                if (bubbles[i].get_radius() == 4*BUBBLE_DEFAULT_RADIUS){
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bubbles[i].get_radius()/2,BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_VY,COLOR(100,255,250)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bubbles[i].get_radius()/2,-BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_VY,COLOR(100,255,250)));}
                bubbles.erase(bubbles.begin()+i);
                bullets.erase(bullets.begin()+j);
                score[score.length()-1]++;
                Score.setMessage(score);
                if (score[score.length()-1]=='9'){
                    score[score.length()-2]++;
                    score[score.length()-1] = '0'-1;}}}}}

bool ifSafeShooter(vector<Bubble> &bubbles, Shooter &shooter, char &life){
    for (unsigned int i=0; i<bubbles.size(); i++){
        if((sqr(shooter.get_head_center_x()-bubbles[i].get_center_x()) //Condition is using pythagorus
            + sqr(shooter.get_head_center_y()-bubbles[i].get_center_y())<=
              sqr(bubbles[i].get_radius()+ 8)) ||
              ((abs(shooter.get_body_center_x()-bubbles[i].get_center_x())<=bubbles[i].get_radius() + 9)&&
              (abs(shooter.get_body_center_y()-bubbles[i].get_center_y())<=bubbles[i].get_radius() + 15))){
                  shooter.changeColor();
                  life--;
                  return false;}}
    return true;}

void check_safety(vector<Bubble> &bubbles, Shooter &shooter, int &binary){
    // Checking if before moving the bubbles and shooter, was it safe
    for (unsigned int i=0; i<bubbles.size(); i++){
        if((sqr(shooter.get_head_center_x()-bubbles[i].get_center_x()) //Condition is using pythagorus
            + sqr(shooter.get_head_center_y()-bubbles[i].get_center_y())<=
              sqr(bubbles[i].get_radius()+ 8)) ||
              ((abs(shooter.get_body_center_x()-bubbles[i].get_center_x())<=bubbles[i].get_radius() + 9)&&
              (abs(shooter.get_body_center_y()-bubbles[i].get_center_y())<=bubbles[i].get_radius() + 15))){
                  binary = 0;}}}






vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(100,255,250)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY/2, COLOR(100,0,250)));
    bubbles.push_back(Bubble(3.0*WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY/5, COLOR(255,105,180)));
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    string lives("Health: 3/3");
    Text health(450,20,lives);
    string score("Score: 00");
    Text Score(450,40,score);
    string timer("Time:00/50");
    Text time(LEFT_MARGIN, 20, timer);
    int t=0;

    // initialize lives
    char life = '3'; // use char since adding this to text is easier
    int binary; // used because we dont want to check for safety of shooter unless it was safe b4 checking.

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        binary = 1;
        check_safety(bubbles,shooter,binary);

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        //Check for collision
        collision(bubbles, bullets, Score, score);

        //Update time spent
        t += 1;
        if(t%40 == 0){
            if (timer[timer.length()-4] == '9'){
                timer[timer.length()-4] = '0';
                timer[timer.length()-5]++;}
            else {
                timer[timer.length()-4]++;}
            time.setMessage(timer);}

        // Conditions for lose
        if (timer[timer.length()-4] == '0' && timer[timer.length()-5] == '5'){
           Text timeup(250,250, "Time's up!");
           timeup.setColor(COLOR(255,0,0));
           wait(3);
           break;}
        if (binary){
            shooter.ogColor();
            if (!ifSafeShooter(bubbles, shooter, life)){
                lives[lives.length()-3]--;
                health.setMessage(lives);
                if (life == '0'){
                    Text finish(250,250, "Game Over!");
                    finish.setColor(COLOR(255,0,0));
                    wait(3);
                    break;}}}


        // Condition for win
        if (bubbles.size()==0){
            Text win(250,250, "You Won!");
            win.setColor(COLOR(0,0,255));
            wait(3);
            break;}



        wait(STEP_TIME);

    }

}
