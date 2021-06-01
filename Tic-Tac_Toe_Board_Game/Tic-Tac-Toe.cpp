#include <iostream>
using namespace std;

char square[10] = {'0','1','2','3','4','5','6','7','8','9'};

int checkwin();
void board();
bool ifNotUsed(int i);
void resetBoard();


int main()
{
	int player = 1,i,choice;
    bool isinvalid = false;
    bool run = true;
    int score1 = 0;
    int score2 = 0;
    char mark;
    do{
        resetBoard();
        do
        {
            board();
            player=(player%2)?1:2;
            if(!isinvalid)
                cout << "Player " << player << ", enter a number:  ";
            else
                cout << "Invalid move ! Player " << player << ", enter a number again:  ";

            cin >> choice;

            mark=(player == 1) ? 'X' : 'O';

            if(ifNotUsed(choice)){
                square[choice] = mark;
                isinvalid = false;
            }
            else
            {
                isinvalid = true;
                player--;
            }
            i=checkwin();

            player++;
        }while(i==-1);
        board();
        if(i==1){
            if(player==1)
                score1++;
            else
                score2++;
            cout<<"==>\aPlayer "<<--player<<" win ";
        }
        else
            cout<<"==>\aGame draw";
        cout<< "\nIf you want to play again press 1 or press any other key to exit: ";
        int key;
        cin>>key;
        if(key==1)
            run=true;
        else
            run = false;
    }while(run);
    cout<< "GAME OVER!!\nScore of Player1 : "<<score1<<"\nScore of Player2 :"<<score2;

    return 0;
}

/********************************************
    This is a function to reset the Board
*********************************************/

void resetBoard(){
    for(int i=0;i<=9;i++){
        square[i] = char(i+'0');
    }
}

/*********************************************
    CHECK IF THE SELECTED SQUARE IS NOT USED
**********************************************/

bool ifNotUsed(int i){
    return (square[i]==char(i+'0'));
}

/*********************************************
    FUNCTION TO RETURN GAME STATUS
    1 FOR GAME IS OVER WITH RESULT
    -1 FOR GAME IS IN PROGRESS
    O GAME IS OVER AND NO RESULT
**********************************************/

int checkwin()
{
    if (square[1] == square[2] && square[2] == square[3])

        return 1;
    else if (square[4] == square[5] && square[5] == square[6])

        return 1;
    else if (square[7] == square[8] && square[8] == square[9])

        return 1;
    else if (square[1] == square[4] && square[4] == square[7])

        return 1;
    else if (square[2] == square[5] && square[5] == square[8])

        return 1;
    else if (square[3] == square[6] && square[6] == square[9])

        return 1;
    else if (square[1] == square[5] && square[5] == square[9])

        return 1;
    else if (square[3] == square[5] && square[5] == square[7])

        return 1;
    else if (square[1] != '1' && square[2] != '2' && square[3] != '3'
                    && square[4] != '4' && square[5] != '5' && square[6] != '6'
                  && square[7] != '7' && square[8] != '8' && square[9] != '9')

        return 0;
    else
        return -1;
}


/*******************************************************************
     FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK
********************************************************************/


void board()
{
    system("cls");
    cout << "\n\n\tTic Tac Toe\n\n";

    cout << "Player 1 token (X)  :  Player 2 token (O)" << endl << endl;
    cout << endl;

    cout << "     |     |     " << endl;
    cout << "  " << square[1] << "  |  " << square[2] << "  |  " << square[3] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[4] << "  |  " << square[5] << "  |  " << square[6] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[7] << "  |  " << square[8] << "  |  " << square[9] << endl;

    cout << "     |     |     " << endl << endl;
}

