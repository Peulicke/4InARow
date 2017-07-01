#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

using namespace std;

vector<vector<int> > game(6,vector<int>(7,0));

int win = 0;

void reset(){
    game = vector<vector<int> >(6,vector<int>(7,0));
    win = 0;
}

bool place(int column, int player){
    if(game[0][column] != 0) return false;
    int i;
    for(i = game.size()-1; game[i][column] != 0; --i);
    game[i][column] = player;
    if(i < game.size()-3){
        if(game[i+1][column] == player && game[i+2][column] == player && game[i+3][column] == player) win = player;
    }
    int j, k;

    for(j = -1; column+j >= 0 && game[i][column+j] == player; --j);
    ++j;
    for(k = 0; k < 4 && column+j+k < game[0].size() && game[i][column+j+k] == player; ++k);
    if(k == 4) win = player;

    for(j = -1; column+j >= 0 && i+j >= 0 && game[i+j][column+j] == player; --j);
    ++j;
    for(k = 0; k < 4 && column+j+k < game[0].size() && i+j+k < game.size() && game[i+j+k][column+j+k] == player; ++k);
    if(k == 4) win = player;

    for(j = -1; column+j >= 0 && i-j < game.size() && game[i-j][column+j] == player; --j);
    ++j;
    for(k = 0; k < 4 && column+j+k < game[0].size() && i-j-k >= 0 && game[i-j-k][column+j+k] == player; ++k);
    if(k == 4) win = player;

    return true;
}

bool undo(int column){
    if(game[game.size()-1][column] == 0) return false;
    int i;
    for(i = 0; game[i][column] == 0; ++i);
    game[i][column] = 0;
    return true;
}

bool full(){
    for(int i = 0; i < game[0].size(); ++i){
        if(game[0][i] == 0) return false;
    }
    return true;
}

void printBoard(){
    for(int i = 0; i < game.size(); ++i){
        for(int j = 0; j < game[i].size(); ++j){
            if(game[i][j] == 0) cout << '.';
            if(game[i][j] == 1) cout << 'X';
            if(game[i][j] == -1) cout << 'O';
        }
        cout << endl;
    }
    cout << endl;
}
/*
long long wins = 0;

void check(int player, int level = 0){
    if(level < 27){
        printBoard();
        cout << wins << endl;
    }
    wins += win;
    if(win != 0) return;
    for(int i = 0; i < game[0].size(); ++i){
        if(!place(i,player)) continue;
        check(-player,level+1);
        undo(i);
        win = 0;
    }
}
*/
int randomGame(int playerStart){
    if(win != 0){
        int result = win;
        win = 0;
        return result;
    }
    if(full()) return 0;
    int pos;
    while(1){
        pos = rand()%7;
        if(place(pos,playerStart)) break;
    }
    int result = randomGame(-playerStart);
    undo(pos);
    return result;
}

float checkRandom(int player){
    const int numGames = 4;
    int wins = 0;
    for(int i = 0; i < numGames; ++i){
        wins += randomGame(player);
    }
    return (float)wins/numGames;
}

pair<int,float> turn(int player, int level = 0){
    const int maxLevel = 6;
    int best = -1;
    float bestWinRate;
    for(int i = 0; i < game[0].size(); ++i){
        if(!place(i,player)) continue;
        if(win != 0){
            undo(i);
            int result = win;
            win = 0;
            if(level == 0) cout << endl;
            return make_pair(i,result);
        }
        float winRate;
        if(level == maxLevel) winRate = checkRandom(-player);
        else winRate = turn(-player,level+1).second;
        if(level == 0) cout << winRate << ' ';
        if(best == -1 || (player == 1 && winRate > bestWinRate) || (player == -1 && winRate < bestWinRate)){
            best = i;
            bestWinRate = winRate;
        }
        undo(i);
        win = 0;
    }
    if(level == 0) cout << endl;
    return make_pair(best,bestWinRate);
}

void usage(char * const *argv) {
	cout << "Usage:" << endl;
	cout << argv[0] << " [-c] [-h]" << endl;
}

int main(int argc, char **argv){
    std::cout.setf(std::ios::unitbuf);

    int opt;
    bool computerOnly = false;

    while((opt = getopt(argc, argv, "ch")) != -1) {
        switch(opt) {
            case 'h':
                usage(argv);
                return 0;
            case 'c':
                computerOnly = true;
                break;
            default:
                cout << "Bad option" << endl;
                return 1;
        }
    }

    if(!computerOnly) {
        while(!full() && win == 0){
            int i;
            cin >> i;
            --i;
            if(!place(i,1)){
                cout << "Illegal move" << endl;
                continue;
            }
            printBoard();
            if(win != 0) break;
            place(turn(-1).first,-1);
            printBoard();
        }
    } else {
        for(int player = 1; win == 0 && !full(); player = -player){
            place(turn(player).first,player);
            printBoard();
        }
        cout << "Winner: ";
        if(win == 0) cout << "none";
        if(win == 1) cout << "X";
        if(win == -1) cout << "O";
        cout << endl;
    }
    /*
    int numGames = 100000;
    float turnsAvg = 0;
    for(int a = 0; a < numGames; ++a){
        reset();
        int turns = 0;
        for(int i = 1; win == 0 && !full(); i = -i, ++turns){
            place(rand()%7,i);
        }
        turnsAvg += turns;
    }
    turnsAvg /= numGames;
    cout << turnsAvg << endl << 100*turnsAvg/(game.size()*game[0].size()) << "%" << endl;*/
    return 0;
}

