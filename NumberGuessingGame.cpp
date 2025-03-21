//UnqGit on GitHub A.K.A.Vishesh
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <limits>
#include <ctime>
using namespace std;
using namespace chrono;

void time_buffer(float seconds){//buffers time
  int timeM = seconds*1000;
  this_thread::sleep_for(chrono::milliseconds(timeM));
}

void cin_clear(){
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
}

int  total_guesses, max_num;
string difficulty, play_again;
bool wrong, wrong2;

int random_num_gen(int i){
  return rand()%i+1;
}

void ShowText(const string& str){
  for(const char& ch : str){
  cout << ch << flush; 
  time_buffer(0.008);
  }
}

void string_corrector(string& difficulty){
  difficulty.erase(0, difficulty.find_first_not_of(" \t\n"));
  difficulty.erase(difficulty.find_last_not_of(" \t\n") + 1);
  for(char& ch : difficulty) ch = tolower(ch);
}

void difficulty_checker(){
  string_corrector(difficulty);
  if(difficulty=="hard"){total_guesses = 5, max_num=100; wrong = 0;}
  else if(difficulty=="medium"){ total_guesses = 6, max_num=50; wrong = 0;} 
  else if(difficulty=="easy"){ total_guesses = 7, max_num=20; wrong = 0;}
  else wrong = 1;
}

void replaying_check(){
  string_corrector(play_again);
  if(play_again=="yes"||play_again=="no") wrong2=false;
  else wrong2=true;
}

string time_convertor(long dur){
  string result = "s";
  long seconds = dur%60;
  result=to_string(seconds)+result;
  long minutes = dur/60;
  if(minutes>0)result = to_string(minutes)+"m "+result;
  return result;
}

void GraphIt(vector<int> Time, vector<int> Guess, int rand_number) {
  vector<pair<int, int>> PairEmUp;
  for(int i = 0; i < Guess.size(); i++) PairEmUp.push_back({Guess[i], Time[i]});
  int maxy = *max_element(Time.begin(), Time.end());
  if(maxy == 0) maxy = 1;
  int Height = min(maxy + 1, 20);
  int minx = min(*min_element(Guess.begin(), Guess.end()), rand_number) - 1;
  int maxx = max(*max_element(Guess.begin(), Guess.end()), rand_number) + 1;
  int range = maxx - minx;
  int Width = min(range, 40);
  double x_scale = range > Width ? (double)Width / range : 1.0;
  vector<vector<char>> Graph(Height, vector<char>(Width, ' '));
  int target_x = (int)((rand_number - minx) * x_scale);
  if(target_x >= 0 && target_x < Width) {
    for (int i = 0; i < Height; i++) Graph[i][target_x] = '|';
  }
  for(auto [guess, time] : PairEmUp){
    int x = (int)((guess - minx) * x_scale);
    int y = (int)(time * (Height - 1) / maxy);
    if(x >= 0 && x < Width && y >= 0 && y < Height) {
      Graph[y][x] = (Graph[y][x] == '|' && abs(guess - rand_number) < 2) ? '#' : '*';
    }
  }
  for(int i = Height - 1; i >= 0; i--) {
    for(int j = 0; j < Width; j++) cout << Graph[i][j];
    cout << " | " << (i * maxy / (Height - 1)) << "s\n";
    time_buffer(0.15);
  }
  for(int i = 0; i < Width+2; i++) cout << "—";
  cout << "\nX: " << minx << " to " << maxx << " | Target: " << rand_number << "\n";
  cout << "Y: 0 to " << maxy << "s | Guesses: ";
  for (int g : Guess) cout << g << " ";
  cout << "\n";
}

int main(){
  int streak = 0, lose = 0;
  bool Wanna_play = true;
  srand(time(0));
  ShowText("Hi, this is a random number guessing game!");
  time_buffer(1);
  ShowText("\nAre you up for the challange?\n");
  time_buffer(1);
  ShowText("\nI will not be waiting for your answer >:)");
  time_buffer(1);
  ShowText("\nChoose your difficulty!(hard, medium, easy)\n");
  time_buffer(1);
  getline(cin, difficulty);
  difficulty_checker();
  while(wrong){
    ShowText("\nDo I need to teach the spelling of those when you can clearly see them?!!?\n");
    ShowText("Enter again!\n");
    getline(cin, difficulty);
    difficulty_checker();
  }
while(Wanna_play){
  vector<int> Time, Guess;
  bool won=false;
  int rand_number, prev_guess, guess=0;
  rand_number=random_num_gen(max_num);
  time_buffer(0.75);
  ShowText("You have a total of "); cout << total_guesses; ShowText(" tries!!\nGood luck!\n");
  time_buffer(0.75);
  ShowText("You will be guessing a number from 1 to "); cout << max_num << endl;
  auto start = chrono::high_resolution_clock::now();
  for(int i = 0; i < total_guesses; i++){
    time_buffer(0.55);
    if(i < total_guesses-1){ShowText("Try number "); cout << "#" << i+1 ;time_buffer(0.25);ShowText("\nEnter your number: ");}
    else{ShowText("\nLAST TRY, buckle up!!\nEnter your number: ");}
    prev_guess=guess;
    double temp;
    while (!(cin >> temp) || (temp!=floor(temp)) || temp < 1 || temp > max_num){
      if(cin.fail()) {
        ShowText("\nThat's not a number! Try again:\n");
        cin_clear();
      }
      else {
        ShowText("\nGuess must be a whole number between 1 and "); cout << max_num << "!\n";
      }
    }
    guess = (int)temp;
    auto now = chrono::high_resolution_clock::now();
    auto timetoguess = chrono::duration_cast<seconds>(now-start);
    Time.push_back(timetoguess.count());
    Guess.push_back(guess);
    if(i == 0){
      if(guess==rand_number){
        time_buffer(0.25);
        ShowText("\nNO WAY, on the first try?!?!?!\nYou're a fricking mind reader!!\n");
        won = true;
        break;
      }
      else if(abs(guess-rand_number)<3){time_buffer(0.25);ShowText("\nHOT!!\n");}
      else if(guess<rand_number){time_buffer(0.25);ShowText("\nTry higher!!\n");}
      else {time_buffer(0.25);ShowText("\nTry lower!!\n");}
    }
    else{
      if(guess==rand_number){
        if(i<total_guesses/2){time_buffer(0.25);ShowText("\nWoah that was quick!!\nNice work!\n");}
        else {time_buffer(0.25);ShowText("\nPhew, you got it in time!\n");}
        won = true;
        break;
      }
      if(prev_guess==guess){time_buffer(0.25);ShowText("\nWhy are you guessing the same number you dumbo!\n");}
      else if(abs(guess-rand_number)<3 && i!=total_guesses-1){time_buffer(0.25);ShowText("\nHot!!\n");}
      else if(abs(rand_number - prev_guess)<abs(rand_number - guess)&&i!=total_guesses-1){time_buffer(0.25);ShowText("\nColder!\n");}
      else if(i!=total_guesses-1){time_buffer(0.25);ShowText("\nWarmer!!\n");}
    }
  }
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<seconds>(stop-start);
  time_buffer(1);
  ShowText("\nIt took you a total time of ");cout << time_convertor(duration.count()) << endl;
  time_buffer(1);
  ShowText("\nFeel about that however you want to :P\n");
  time_buffer(1);
  ShowText("\nBefore playing again :) or leaving :( have\na look");
  time_buffer(1);
  ShowText(" at the graph of your play!\n\n");
  time_buffer(1);
  GraphIt(Time, Guess, rand_number);
  if(won){
    streak++;
    lose=0;
    if(streak==3){
      if(difficulty=="easy")ShowText("I mean it's alright, like...\n");
      else if(difficulty=="medium")ShowText("Nice streak!!\n");
      else ShowText("Niceeeeeeeeeeeee!!\n");
    }
    else if(streak==5){
      if(difficulty=="easy") ShowText("I mean it's alright, like...\n");
      else if(difficulty=="medium")ShowText("You are bonkers mate!!!\n");
      else ShowText("WOAHHH, i could never @w@\n");
      if(difficulty!="hard"){ShowText("\nAlright, changing the difficulty to hard mode!\n");
        total_guesses = 5, max_num=100;
        difficulty = "hard";
      }
    }
    else if(streak==10){
      ShowText("Are you kidding me!?!?!\nThat shouldn't be possible!!!\n");
    }
    else if(streak>10){
      ShowText("DON'T CHEAT!!\nCHEATING CAN LEAD TO PERMANENT\nMORALITY LOSS!!\n");
    }
    time_buffer(0.85);
    ShowText("\nNice! would you like to play again? (yes/no)\n");
    time_buffer(0.85);
    ShowText("\nYou have a win streak of ");cout << streak << "!\n";
  }
  else if(streak >= 1){
    int winstreak = streak;
    streak = 0;
    lose++;
    time_buffer(0.85);
    ShowText("You had a win streak of ");cout << winstreak;
    time_buffer(0.85);
    ShowText("\nBut sadly it's back to 0 now!\n(hint: play again?..uwu..(yes/no)?)\n");
  }
  else{lose++; streak = 0;ShowText("Lol you lost, anyways\nwanna gamble again?? >D (yes/no)\n");}
  if(lose==4){ShowText("LOL, you lost 4 times in a row...absolute dog poop lmao!\nChanging the difficulty to easy(if it\nis already that then you should go drink some\n water because what the hell is this performance)");
    total_guesses = 7, max_num=20;
    difficulty = "easy";
  }  
  if(lose > 4 && lose < 10) ShowText("BROOOO, I am dyiiiiing lmaooo\n");
  if(lose>=10) ShowText("You're trying to lose on purpose aren't you XD\n");
  if(lose>0){
    int motivate = rand()%2;
    string motivational_quotes[3] = {
      "Don't give up! Even pros fail sometimes!",
      "You'll get it next time, champ!",
      "Remember, it's all about the journey, not just the wins."};
    if(motivate){time_buffer(0.85);ShowText(motivational_quotes[rand() % 3]);cout << endl;}
  }
  cin_clear();
  cout << "\nEnter(yes/no): ";
  getline(cin, play_again);
  replaying_check();
  while(wrong2){
    ShowText("What did you just write uhhhhh...type again please(yes/no)\n");
    cin_clear();
    getline(cin, play_again);
    replaying_check();
  }
  if(play_again=="yes"){
    cout << "\n————————————————next——game————————————————\n\n";
    time_buffer(0.8);
    continue;
  }
  else Wanna_play=false;
  }
  cout << "\n||:——————Thanks—for—playing—:D —————:||" << endl;
  return 0;
}
