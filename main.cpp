#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct cell {
    char colour;	//r - czerwony,	b - niebieski,	n - brak koloru
    int value;		//wartość komórki
    bool fullness;	//czy komórka jest wypełniona
};
int score_red = 0, score_blue = 0;

void printcell(struct cell cell);   //komórka;     wyswietla komórkę
void printboard(struct cell board[4][4]);       //plansza       wyświetla planszę
void board_ini(struct cell board[4][4]);        //plansza       inicjalizuje plansze {'n',0,false}
bool new_cell(struct cell board[4][4],char colour);        //plansza, aktywny gracz        losuje pustą komórkę i nadaje jej wartości
char swap_player(char colour);      //dla 'r' zwraca 'b'        dla 'b' zwraca 'r'
bool direction_valid(char direction);   //zwraca 1 dla w, s, a, d lub x
char valid_move(struct cell board[4][4]);  //pyta uzytkownika o kierunek az nie zostanie podany poprawny

bool merge_up(struct cell board[4][4]);     //funkcje realizujące łączenie się komórek zależnie od zadanego kierunku
bool merge_down(struct cell board[4][4]);
bool merge_left(struct cell board[4][4]);
bool merge_right(struct cell board[4][4]);
//funkcja zwierająca powyższe funkcje i wybierająca odpowiednią w zależności od zmiennej direction('w' , 's' , 'a' , 'd') jeśli dokona zmian zwraca 1
bool merge(struct cell board[4][4], char direction);
void merge_cell(struct cell board[4][4], int i1, int j1, int i2, int j2);    //łączy komórkę nr2 do komórki nr1 (i oraz j są indeksami komórki)
void move_cell(struct cell board[4][4], int i1, int j1, int i2, int j2);    //w miejsce [i2][j2] wstawia [i1][j1] pozostawiając pierwotne położenie puste

void make_simboard(struct cell board[4][4],struct cell simboard[4][4]);    //tworzy kopie planszy wykożystywaną na potrzeby przeciwnika komputera
char ai_easy(struct cell board[4][4]);      //komputer zwraca znak w,s,a,d w zależności od poziomu trudności
char ai_medium(struct cell board[4][4],char colour);
char ai_hard(struct cell board[4][4],int depth,char colour, int dilation);
float score_evaluation(struct cell board[4][4], char colour); //liczy sredni wynik oponenta (zakładając że jest to ai_hard() )
void testsfml(){
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}    //testowanie bilbioteki SFML

int main(){
    srand(time(NULL));
    //testsfml();
    cell board[4][4]{};     //plansza
    char active_player = 'r';       //pierwszy kolor
    char direction = 0;     //zadany kierunek ruchu
    bool loop = 0;
    board_ini(board);   // inicjoweanie pustej planszy
    char difficulty = 0;
    int red = 0; int blue = 0;

    

    cout << "1 - solitatire\n"
        << "2 - player vs player\n"
        << "3 - player(red) vs computer(easy)\n"
        << "4 - player(red) vs computer(medium)\n"
        << "5 - player(red) vs computer(hard)\n"
        << "6 - computer(medium)(red) vs computer(hard)\n";
    cin >> difficulty;
    
    switch (difficulty)
    {
    case '1':
        new_cell(board, active_player);
        while (direction != 'x') {      //dla x zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            direction = valid_move(board);
            cout <<"score = " <<score_blue + score_red << "\n";
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
        }
        break;
    case '2':
        new_cell(board, active_player);
        while (direction != 'x') {      //dla x zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            direction = valid_move(board);
            cout <<"blue score = " << score_blue << " red score = " << score_red << "\n";
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
        }
        break;
    case '3':
        new_cell(board, active_player);
        while (direction != 'x') {      //dla x zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            direction = valid_move(board);
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            merge(board, ai_easy(board));
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
        }
        break;
    case '4':
        new_cell(board, active_player);
        while (direction != 'x') {      //dla x zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            direction = valid_move(board);
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            merge(board, ai_medium(board,'b'));
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
        }
        break;
    case '5':
        new_cell(board, active_player);
        while (direction != 'x') {      //dla x zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            direction = valid_move(board);
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            printboard(board);
            active_player = swap_player(active_player);     //zmień aktywny kolor
            merge(board, ai_hard(board, 4 ,'b', 50));
            if (!new_cell(board, active_player)) {
                direction = 'x';
            };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
            cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
        }
        break;
    case '6':
        new_cell(board, active_player);
        for (int i = 0; i < 1; i++) {
            board_ini(board);
            direction = 0;
            active_player = 'r';
            new_cell(board, active_player);
            while (direction != 'x') {      //dla x zakończ program
                printboard(board);
                active_player = swap_player(active_player);     //zmień aktywny kolor
                merge(board, ai_medium(board,'r'));
                cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
                if (!new_cell(board, active_player)) {
                    direction = 'x';
                };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
                printboard(board);
                active_player = swap_player(active_player);     //zmień aktywny kolor
                merge(board, ai_hard(board,5,'b',50));
                if (!new_cell(board, active_player)) {
                    direction = 'x';
                };     //jesli nie można dodać nowej komókrki (plansza pełna) zakończ program
                cout << "blue score = " << score_blue << " red score = " << score_red << "\n";
            }
            if (score_blue > score_red)  blue++;
            else if (score_blue < score_red)  red++;
            cout << "========red wins: " << red << " blue wins: " << blue << "=====\n\n";
        }
        cout << "red wins: " << red << " blue wins: " << blue << "\n";
        break;
    default:
        cout << "nieprawidłowy znak";
        break;
    }
    return 0;
}

void printcell(struct cell cell) {
    switch (cell.colour)
    {
    case 'n':
        cout << cell.value;
        break;
    case 'r':
        cout << 'r' << cell.value;
        break;
    case 'b':
        cout << 'b' << cell.value;
        break;
    default:
        cout << "ERROR";
        break;
    }
    cout << "\t";
}
void printboard(struct cell board[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printcell(board[i][j]);
        }
        cout << "\n";
    }
    cout << "\n\n";
}
void board_ini(struct cell board[4][4]) {
    score_blue = 2; score_red = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = { 'n' , 0 , false};
        }
    }
}
bool new_cell(struct cell board[4][4], char colour) {
    int free_spots = 16; //ile jest pustych pól?
    int rolled_spot;    //losowy numer pola
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j].fullness)    free_spots--;
        }
    }
    if (!free_spots) return false;
    rolled_spot = rand() % free_spots + 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!board[i][j].fullness)    rolled_spot--;
            if (rolled_spot == 0) {
                board[i][j] = { colour, 2 , true };
                i = 4; j = 4;
            }
        }
    }
    return true;
}
char swap_player(char colour) {
    switch (colour){
    case 'r':
        return 'b';
        break;
    case 'b':
        return 'r';
        break;
    default:
        cout << "ERROR\n";
        return -1;
        break;
    }
}
bool direction_valid(char direction) {
    switch (direction){
    case 'w':
        return 1;
        break;
    case 's':
        return 1;
        break;
    case 'a':
        return 1;
        break;
    case 'd':
        return 1;
        break;
    case 'x':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}
char valid_move(struct cell board[4][4]) {
    bool loop = 0;
    char direction = 0;
    do {       //nie pozwól wprowadzić nieprawidłowego znaku ani takiego któy nie zmienia stanu planszy
        loop = 0;
        cin >> direction;
        if (!direction_valid(direction)) {
            cout << "nieprawidlowy znak\n";
            loop = 1;
        }
        if (!merge(board, direction)) {
            cout << "nie mozna wykonac ruchu\n";
            loop = 1;
        }
    } while (loop);
    return direction;
}

bool merge_up(struct cell board[4][4]) {
    bool made_diffrance = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 1; j < 4; j++) {
            if (board[j][i].fullness) {
                if (!board[j - 1][i].fullness) {
                    move_cell(board, j, i, j - 1, i);
                    made_diffrance = 1;
                    j-=2;
                    if (j < 0) j = 0;
                }
                else if (board[j - 1][i].colour == board[j][i].colour && board[j - 1][i].value == board[j][i].value) {
                    merge_cell(board, j-1, i, j, i);
                    made_diffrance = 1;
                    j-=2;
                    if (j < 0) j = 0;
                }
            }
        }
    }
    return made_diffrance;
}
bool merge_down(struct cell board[4][4]) {
    bool made_diffrance = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 2; j > -1; j--) {
            if (board[j][i].fullness) {
                if (!board[j + 1][i].fullness) {
                    move_cell(board, j, i, j + 1, i);
                    made_diffrance = 1;
                    j += 2;
                    if (j > 3) j = 3;
                }
                else if (board[j + 1][i].colour == board[j][i].colour && board[j + 1][i].value == board[j][i].value) {
                    merge_cell(board, j + 1, i, j, i);
                    made_diffrance = 1;
                    j += 2;
                    if (j > 3) j = 3;
                }
            }
        }
    }
    return made_diffrance;
}
bool merge_left(struct cell board[4][4]) {
    bool made_diffrance = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            if (board[i][j].fullness) {
                if (!board[i][j - 1].fullness) {
                    move_cell(board, i, j, i, j - 1);
                    made_diffrance = 1;
                    j -= 2;
                    if (j < 0) j = 0;
                }
                else if (board[i][j - 1].colour == board[i][j].colour && board[i][j - 1].value == board[i][j].value) {
                    merge_cell(board, i, j - 1, i, j);
                    made_diffrance = 1;
                    j -= 2;
                    if (j < 0) j = 0;
                }
            }
        }
    }
    return made_diffrance;
}
bool merge_right(struct cell board[4][4]) {
    bool made_diffrance = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 2; j > -1; j--) {
            if (board[i][j].fullness) {
                if (!board[i][j + 1].fullness) {
                    move_cell(board, i, j, i, j + 1);
                    made_diffrance = 1;
                    j += 2;
                    if (j > 3) j = 3;
                }
                else if (board[i][j + 1].colour == board[i][j].colour && board[i][j + 1].value == board[i][j].value) {
                    merge_cell(board, i, j + 1, i, j);
                    made_diffrance = 1;
                    j += 2;
                    if (j > 3)  j = 3;
                }
            }
        }
    }
    return made_diffrance;
}
bool merge(struct cell board[4][4], char direction) {
    switch (direction) {
    case 'x':
        return 1;
        break;
    case 'w':
        return merge_up(board);
        break;
    case 's':
        return merge_down(board);
        break;
    case 'a':
        return merge_left(board);
        break;
    case 'd':
        return merge_right(board);
        break;
    default:
        cout << "ERROR\n";
        return -1;
        break;
    }
}
void merge_cell(struct cell board[4][4], int i1, int j1, int i2, int j2) {
    board[i1][j1].value *= 2;
    board[i2][j2] = { 'n' , 0 , false };
    if (board[i1][j1].colour == 'r')    score_red += board[i1][j1].value;
        if (board[i1][j1].colour == 'b')    score_blue += board[i1][j1].value;
}
void move_cell(struct cell board[4][4], int i1, int j1, int i2, int j2) {
    board[i2][j2] = board[i1][j1];
    board[i1][j1] = { 'n' , 0 , false };
}

void make_simboard(struct cell board[4][4],struct cell simboard[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            simboard[i][j] = board[i][j];
        }
    }
}
char ai_easy(struct cell board[4][4]) {
    int blue_score_before = score_blue;
    int red_score_before = score_red;
    cell simboard[4][4];
    make_simboard(board,simboard);
    char possible_choices[4] = { 'w','s','a','d' };
    char number_possible_choices = 4;
    int rolled_number = rand() % 4;

    while (!merge(simboard, possible_choices[rolled_number])) {  //jesli wybrany kierunek nie wykona zmian
        for (int i = rolled_number; i < 3; i++) {
            possible_choices[i] = possible_choices[i + 1];  //przesuniecie reszty mozliwych kierunków w lewo
        }
        number_possible_choices--;
        if (number_possible_choices == 0) {
            score_blue = blue_score_before;     score_red = red_score_before;
            return 'x';
        }

        rolled_number = rand() % number_possible_choices;    //ponowne losowanie 
    }
    score_blue = blue_score_before;     score_red = red_score_before;
    return possible_choices[rolled_number];
}
char ai_medium(struct cell board[4][4],char colour) {
    int blue_score_before = score_blue;
    int red_score_before = score_red;
    char moves[4] = { 'w' , 's' , 'a' , 'd' };
    char chosen_move = ai_easy(board);
    if (chosen_move == 'x')   return 'x';
    int best_delta_score = -100000;
    int delta_score = 0;
    cell simboard[4][4];
    for (int i = 0; i < 4; i++) {
        make_simboard(board,simboard);
        if (merge(simboard, moves[i])) {
            if(colour == 'b')   delta_score = score_blue - blue_score_before - score_red + red_score_before;
            else                delta_score = -score_blue + blue_score_before + score_red - red_score_before;

            if (delta_score > best_delta_score) {
                best_delta_score = delta_score;
                chosen_move = moves[i];
            }
            score_blue = blue_score_before;     score_red = red_score_before;
        }
    }
    return chosen_move;
}
char ai_hard(struct cell board[4][4],int depth,char colour,int dilation) {
    int blue_score_before = score_blue;
    int red_score_before = score_red;
    char chosen_move = ai_easy(board);
    if (rand() % 100 < dilation) return chosen_move;
    char moves[4] = { 'w' , 's' , 'a' , 'd' };
    float mean_score = 0;
    int best_delta = -100000;
    cell simboard[4][4];
    if (depth == 0) {
        for (int i = 0; i < 4; i++) {
            make_simboard(board, simboard);
            if(merge(simboard, moves[i])){
                if(colour == 'b')   mean_score =  score_blue - blue_score_before - score_red + red_score_before;
                if(colour == 'r')   mean_score = -score_blue + blue_score_before + score_red - red_score_before;
                score_blue = blue_score_before;     score_red = red_score_before;
                mean_score -= score_evaluation(simboard, swap_player(colour));
                if (mean_score > best_delta) {
                    best_delta = mean_score;
                    chosen_move = moves[i];
                }
            }
        }
    }
    else {
        int score = 0;
        int checks = 0;
        for (int k = 0; k < 4; k++) {
            make_simboard(board, simboard);
            if (merge(simboard, moves[k])) {
                if (colour == 'b')   score = score_blue - blue_score_before - score_red + red_score_before;
                if (colour == 'r')   score = -score_blue + blue_score_before + score_red - red_score_before;
                score_blue = blue_score_before;     score_red = red_score_before;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (!board[i][j].fullness) {
                            checks++;
                            simboard[i][j] = { swap_player(colour) , 2 , true };
                            merge(simboard, ai_hard(simboard, depth - 1, swap_player(colour), dilation/2));
                            if (colour == 'b')   mean_score = score_blue - blue_score_before - score_red + red_score_before;
                            if (colour == 'r')   mean_score = -score_blue + blue_score_before + score_red - red_score_before;
                            score_blue = blue_score_before;     score_red = red_score_before;
                            simboard[i][j] = { 'n' , 0 , true };
                        }
                    }
                }
                score += mean_score;
                if (score > best_delta) {
                    best_delta = score;
                    chosen_move = moves[k];
                }
            }
        }
    }
    score_blue = blue_score_before;     score_red = red_score_before;
    return chosen_move;
}
float score_evaluation(struct cell board[4][4], char colour) {
    float score = 0;
    int blue_score_before = score_blue;
    int red_score_before = score_red;
    int checks = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            if (!board[i][j].fullness) {
                checks++;
                board[i][j] = { colour , 2 , true };
                merge(board, ai_medium(board, colour));
                if (colour == 'b')   score = score_blue - blue_score_before - score_red + red_score_before;
                else                score = -score_blue + blue_score_before + score_red - red_score_before;
                score_blue = blue_score_before;     score_red = red_score_before;
                board[i][j] = { 'n' , 0 , true };
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!board[i][j].value)    board[i][j].fullness = false;
        }
    }
    return score / checks;
}