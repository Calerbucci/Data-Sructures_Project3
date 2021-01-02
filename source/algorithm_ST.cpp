#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
/*------class vector------*/
template <class T>
class  Vector
{
public:

    typedef T * iterator;

    Vector(){        
        buffer = 0;
        my_capacity = 0;
        my_size = 0;
    }     
    ~Vector(){
        delete[ ] buffer;
    }  
    bool empty() const;
    unsigned int size() const{return my_size;}
    void push_back(const T & v){
        if (my_size >= my_capacity){
            eval_my_capacity = my_capacity +5;
            reserve(eval_my_capacity);
        }
            
        buffer [my_size++] = v;
    }
    void pop_back(){my_size = my_size-1;};

    T & operator[](unsigned int index){return buffer[index];}
    void clear(){
        my_capacity = 0;
        my_size = 0;
        buffer = 0;
    }
    bool empty(){
        if(my_size==0) return 1;
        else return 0;
    }
private:
    unsigned int my_capacity;
    unsigned int eval_my_capacity;
    unsigned int my_size;
        
    T * buffer;
    void reserve(unsigned int capacity){
        if(!buffer)
        {
            my_size = 0;
            my_capacity = 0;
        }    
        T * Newbuffer = new T [capacity];
        unsigned int l_Size;
        if(capacity<my_size) l_Size = capacity;
        else l_Size = my_size;

        unsigned int i=0;
            while (i<l_Size)
            {
                Newbuffer[i] = buffer[i];
                i++;
            }
            
        my_capacity = capacity;
        delete[] buffer;
        buffer = Newbuffer;
    }
};
/*------class vector------*/

struct Pair{ 
    int second;
    int first;
    Pair():first(0),second(0){};
    Pair(int a, int b):first(a),second(b){};
    
};

int max(double a, double b){
    if(a>b) return a;
    else return b;
}
int min(double a, double b){
    if(a<b) return a;
    else return b;
    
}

double evaluation(Board &board, int color){
    double eval_enemy = 0;
    double eval = 0;
    double ret_val=0;
    int enemy_color ;
    if(color == 'r'){
        enemy_color = 'b';
    }else
    {
        enemy_color = 'r';
    } 
    int i=0;
    while (i<5)
    {   int j =0;
        while (j<6)
        {
            if(board.get_cell_color(i, j) == color){
                eval = eval+1;
                }
            else if(board.get_cell_color(i, j) == enemy_color){
                eval_enemy = eval_enemy+1;
                }
            j++;
        }
        i++;       
    }

     ret_val = (eval == 30)?100:(eval == 0)?-100:eval-eval_enemy; 
     return ret_val;
    
}
bool throw_error()
    {
        cout<<"Something went wrong";
        return 12345;
    }
int miniMax(double pre_eval, Board board, int row, int col, int depth, int alpha, int beta, bool maximizingPlayer, char me, char enemy){
    int color;
    if(!maximizingPlayer){
        color = me;
    }
    else{
        color = enemy;
    }   
    //Board update  
    Player red_player(RED);
    Board board_next = board;
    Player blue_player(BLUE);
    (color== 'r')?board_next.place_orb(row, col, &red_player):(color=='b')?board_next.place_orb(row, col, &blue_player):throw_error();  
    double evaluation_of_position = evaluation(board_next, me);
    double eval_after=0;
    if (depth == 0 || evaluation_of_position==100 ||evaluation_of_position==0) //eval==100 means game over
    {
        eval_after = evaluation_of_position-pre_eval;
        return eval_after;
    }        
    int flag=0;
    if (maximizingPlayer){
        double maxEval = -1000000;//very small number
        //for each position
        int i=0;
        while (i<5)
        {
            int j= 0;
            while (j<6)
            {
                if(board.get_cell_color(row, col) != me && board.get_cell_color(row, col) != 'w'){
                    continue;
                }
                maxEval = max(maxEval, miniMax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, false, me, enemy));
                alpha = alpha;
                alpha = max(alpha, miniMax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, false, me, enemy));
                if(beta <= alpha){
                    flag=1;
                    break;
                } 
                j++;
            }
            if(flag){
                    break;
                }
            i++;
        }
        return maxEval;

    }
    else{
        double minEval = +100000;//very large number
        //for each position
         int i=0;
        while (i<5)
        {
            int j =0;
            while (j<6)
            {
                if(board.get_cell_color(row, col) != enemy && board.get_cell_color(row, col) != 'w'){
                    continue;
                }
                minEval = min(minEval, miniMax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, true, me, enemy));
                beta = beta;
                beta = min(beta, miniMax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, true, me, enemy));
                if(beta <= alpha){
                    flag = 1;
                    break;
                }
                j++;
            }
            if(flag==1){
                break;
            }
            i++;
        }
        return minEval;
    }   
}

void algorithm_A(Board board, Player player, int index[]){ 
    Vector<Pair> bad_pos;
    Vector<Pair> best_pos;
    int color = player.get_color();
    //check 4 corner
    board.get_cell_color(0, 0) == 'w'?best_pos.push_back(Pair(0,0)):bad_pos.push_back(Pair(0,0));
    board.get_cell_color(0, 5) == 'w'?best_pos.push_back(Pair(0,5)):bad_pos.push_back(Pair(0,5));
    board.get_cell_color(4, 0) == 'w'?best_pos.push_back(Pair(4,0)):bad_pos.push_back(Pair(4,0));
    board.get_cell_color(4, 5) == 'w'?best_pos.push_back(Pair(4,5)):bad_pos.push_back(Pair(4,5));
    if(best_pos.empty()!=1){
        srand(time(NULL)*time(NULL));
        int pos=rand()%best_pos.size();
        int row_ = best_pos[pos].first;
        int col_ = best_pos[pos].second;
        index[0]= row_;
        index[1]= col_;
        return;
    }
    //alpha-beta pruning algorithm
    char enemy ;
    char me = player.get_color();   
    if(me == 'r') {
        enemy = 'b';
    }
    else
    {
        enemy = 'r';
    }
    
   
    double Max_eval=-10000;
    int row=0;
    while (row<5)
    {   int col = 0;
        while (col < 6)
        {
            if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w'){
                if(Max_eval<miniMax(evaluation(board, me),board, row, col, 0, -10000, 10000, false, me, enemy)){
                    best_pos.clear();
                    Max_eval = miniMax(evaluation(board, me),board, row, col, 0, -10000, 10000, false, me, enemy);                  
                    best_pos.push_back(Pair(row,col));
                }else if(miniMax(evaluation(board, me),board, row, col, 0, -10000, 10000, false, me, enemy)==Max_eval){
                    best_pos.push_back(Pair(row,col));
                }
            }
            col++;
        }
        row++;

    }
    

    srand(time(NULL)*time(NULL));
    int pos = rand()%best_pos.size();
    int _row = best_pos[pos].first;
    int _col = best_pos[pos].second;
    index[0] = _row;
    index[1] = _col;
}