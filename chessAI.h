#include <vector>
#include <stdlib.h>
#include <fstream>

/*
	The computer chess AI takes in a 2-d char vector array and computes a move. It then returns
	a 2-d char vector array with the new board state after the computers move.
*/

class chessAI {
   public:
	 	std::vector<std::vector<char> > board;
	 	std::vector<std::vector<char> > best_board;
	 	int best_board_eval;
	 	bool terminal_found;
	 	int pawn_val, knight_val, rook_val, bishop_val, queen_val, king_val;
	 	std::vector<std::vector<int> > 	pawn_mat, knight_mat, rook_mat, 
	 									bishop_mat, queen_mat, king_mat,
	 									pawn_mat_human, knight_mat_human, bishop_mat_human,
	 									rook_mat_human, queen_mat_human, king_mat_human;

	 	void setBoard(std::vector<std::vector<char> > in) {
	 		board = in;
	 		best_board_eval = 0;
	 		best_board.clear();
	 		if (pawn_val != 100) {	//if it hasnt been set yet
	 			pawn_val = 100;
		 		knight_val = 320;
		 		bishop_val = 330;
		 		rook_val = 500;
		 		queen_val = 900;
		 		king_val = 20000;
		 		setAdjacencyMatrices();
	 		}
	 		
	 	}

		std::vector<std::vector<char> > getMove(int ply){
			std::vector<std::vector<std::vector<char> > > valid_moves;
			terminal_found = false;
			int heuristic = -1000;
			int temp;
			valid_moves = getValidMoves();
			for (int i = 0; i < valid_moves.size(); i++) {
				temp = minimax(valid_moves[i], 4, -10000, 10000, true);
				std::cout << "heur: " << temp << std::endl;
				if (temp > heuristic) {
					heuristic = temp;
					best_board = valid_moves[i];
				}
			}
			std::cout << "heuristic: " << heuristic << std::endl;
			printBoard(best_board);
			return best_board;
		}	 	
	private:

		void printBoard(std::vector<std::vector<char> > print) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					std::cout << print[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		//returns the larger of two integers as an integer
		int max (int a, int b) {
			if (a > b) return a;
			else return b;
		}
		//returns the smaller of two integers as an integer
		int min (int a, int b) {
			if (a < b) return a;
			else return b;
		}

		int minimax(std::vector<std::vector<char> > node, int depth, int a, int b, bool maxing){
			if (depth == 0){
				int node_eval = evaluate(node);
				return node_eval;
			}
			if (maxing){
				int v = -50000;
				std::vector<std::vector<std::vector<char> > > valid_moves;
				valid_moves = getValidMoves();
				for (int i = 0; i < valid_moves.size(); i++) {
					v = max(v, minimax(valid_moves[i], depth-1, a, b, false));
					a = max(a, v);
					if (b <= a) break; //prune
				}
				return v;
			} else {
				int v = 50000;
				std::vector<std::vector<std::vector<char> > > valid_human_moves;
				valid_human_moves = getValidHumanMoves();
				for (int i = 0; i < valid_human_moves.size(); i++) {
					v = min(v, minimax(valid_human_moves[i], depth-1, a, b, true));
					b = min(b, v);
					if (b <= a) break;
				}
				return v;
			}
		}

		int evaluate(std::vector<std::vector<char> > state) {
			int hum_result = 0;
			int comp_result = 0;
			for (int i = 0; i < state.size(); i++) {
				for (int j = 0; j < state[i].size(); j++) {
					if (isupper(state[i][j])) {
						if (state[i][j] == 'P'){
							hum_result += pawn_val + pawn_mat_human[i][j];
						}
						else if (state[i][j] == 'N'){
							hum_result += knight_val + knight_mat_human[i][j];
						}
						else if (state[i][j] == 'B'){
							hum_result += bishop_val + bishop_mat_human[i][j];
						}
						else if (state[i][j] == 'R'){
							hum_result += rook_val + rook_mat_human[i][j];
						}
						else if (state[i][j] == 'Q'){
							hum_result += queen_val + queen_mat_human[i][j];
						}
						else if (state[i][j] == 'K'){
							hum_result += king_val + king_mat_human[i][j];
						}
					}
					if (islower(state[i][j])) {
						if (state[i][j] == 'p'){
							comp_result += pawn_val + pawn_mat[i][j];
						}
						else if (state[i][j] == 'n'){
							comp_result += knight_val + knight_mat[i][j];
						}
						else if (state[i][j] == 'b'){
							comp_result += bishop_val + bishop_mat[i][j];
						}
						else if (state[i][j] == 'r'){
							comp_result += rook_val + rook_mat[i][j];
						}
						else if (state[i][j] == 'q'){
							comp_result += queen_val + queen_mat[i][j];
						}
						else if (state[i][j] == 'k'){
							comp_result += king_val + king_mat[i][j];
						}
					}
				}
			}
			//std::cout << "heur= " << result << std::endl;
			return comp_result - hum_result;
		}



		std::vector<std::vector<std::vector<char> > > getValidMoves(){
			std::vector<std::vector<std::vector<char> > > valid_moves;
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					if (board[x][y] == 'p' && board[x+1][y] == ' ') {
						char temp = board[x+1][y];
						board[x+1][y] = 'p';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x+1][y] = temp;
						board[x][y] = 'p';
						
					}
					if (board[x][y] == 'p' && x == 1 && board[x+1][y] == ' ' && board[x+2][y] == ' ') {
						char temp = board[x+2][y];
							board[x+2][y] = 'p';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y] = temp;
							board[x][y] = 'p';
						}
					if (board[x][y] == 'p' && isupper(board[x+1][y-1])){
						char temp = board[x+1][y-1];
						board[x+1][y-1] = 'p';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x+1][y-1] = temp;
						board[x][y] = 'p';
					}
					if (board[x][y] == 'p' && isupper(board[x+1][y+1])){
						char temp = board[x+1][y+1];
						board[x+1][y+1] = 'p';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x+1][y+1] = temp;
						board[x][y] = 'p';
					}
					if (board[x][y] == 'h') {
						if (x-1 >= 0 && y-2 >= 0 && (board[x-1][y-2] == ' ' || isupper(board[x-1][y-2]))){ 
							char temp = board[x-1][y-2];
							board[x-1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (x-1 >= 0 && y+2 < 8 && (board[x-1][y+2] == ' ' || isupper(board[x-1][y+2]))){ 
							char temp = board[x-1][y+2];
							board[x-1][y+2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y+2] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (x+1 < 8 && y+2 < 8 && (board[x+1][y+2] == ' ' || isupper(board[x+1][y+2]))){
							char temp = board[x+1][y+2];
							board[x+1][y+2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y+2] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (x+1 < 8 && y-2 >= 0 && (board[x+1][y-2] == ' ' || isupper(board[x+1][y-2]))){
							char temp = board[x+1][y-2];
							board[x+1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y-2] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (y-2 >= 0 && x-1 >= 0 && (board[x-1][y-2] == ' ' || isupper(board[x-1][y-2]))){
							char temp = board[x-1][y-2];
							board[x-1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = temp;
							board[x][y] = 'h';
						}
					}////////////////////////////
					if (board[x][y] == 'h') {
						if (y-1 >= 0 && x-2 >= 0 && (board[x-2][y-1] == ' ' || isupper(board[x-2][y-1]))){
							char temp = board[x-2][y-1];
							board[x-2][y-1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y-1] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (y+1 < 8 && x-2 >= 0 && (board[x-2][y+1] == ' ' || isupper(board[x-2][y+1]))){
							char temp = board[x-2][y+1];
							board[x-2][y+1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y+1] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (y+1 < 8 && x+2 < 8 && (board[x+2][y+1] == ' ' || isupper(board[x+2][y+1]))){
							char temp = board[x+2][y+1];
							board[x+2][y+1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y+1] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'h') {
						if (y-1 >= 0 && x+2 < 8 && (board[x+2][y-1] == ' ' || isupper(board[x+2][y-1]))){
							char temp = board[x+2][y-1];
							board[x+2][y-1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y-1] = temp;
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'b') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'b';
							}
							else if (x+i < 8 && y+i < 8 && !islower(board[x+i][y+i]) && board[x+i][y+i] != ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'b';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'b') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'b';
							}
							else if (x+i < 8 && y-i >= 0 && !islower(board[x+i][y-i]) && board[x+i][y-i] != ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'b';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'b') {
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'b';
							}
							else if (x-i >= 0 && y+i < 8 && !islower(board[x-i][y+i]) && board[x-i][y+i] != ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'b';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'b') {
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'b';
							}
							else if (x-i >= 0 && y-i >= 0 && !islower(board[x-i][y-i]) && board[x-i][y-i] != ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'b';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'r') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								char temp = board[x+i][y];
								board[x+i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'r';
							}
							else if (x+i < 8 && isupper(board[x+i][y])) {
								char temp = board[x+i][y];
								board[x+i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								char temp = board[x-i][y];
								board[x-i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'r';
							}
							else if (x-i >= 0 && isupper(board[x-i][y])) {
								char temp = board[x-i][y];
								board[x-i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								char temp = board[x][y+i];
								board[x][y+i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'r';
							}
							else if (y+i < 8 && isupper(board[x][y+i])) {
								char temp = board[x][y+i];
								board[x][y+i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								char temp = board[x][y-i];
								board[x][y-i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'r';
							}
							else if (y-i >= 0 && isupper(board[x][y-i])) {
								char temp = board[x][y-i];
								board[x][y-i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'r';
								break;
							} else break;
						}/////////////////////////////////
					}
					if (board[x][y] == 'q') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								char temp = board[x+i][y];
								board[x+i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'q';
							}
							else if (x+i < 8 && isupper(board[x+i][y])) {
								char temp = board[x+i][y];
								board[x+i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								char temp = board[x-i][y];
								board[x-i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && isupper(board[x-i][y])) {
								char temp = board[x-i][y];
								board[x-i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								char temp = board[x][y+i];
								board[x][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'q';
							}
							else if (y+i < 8 && isupper(board[x][y+i])) {
								char temp = board[x][y+i];
								board[x][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								char temp = board[x][y-i];
								board[x][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'q';
							}
							else if (y-i >= 0 && isupper(board[x][y-i])) {
								char temp = board[x][y-i];
								board[x][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'q';
							}
							else if (x+i < 8 && y+i < 8 && isupper(board[x+i][y+i])) {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'q';
							}
							else if (x+i < 8 && y-i >= 0 && isupper(board[x+i][y-i])) {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && y+i < 8 && isupper(board[x-i][y+i])) {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && y-i >= 0 && isupper(board[x-i][y-i])) {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'q';
								break;
							} else break;
						}
					}
				}
			}
			return valid_moves;
		}

		std::vector<std::vector<std::vector<char> > > getValidHumanMoves(){
			std::vector<std::vector<std::vector<char> > > valid_moves;
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					if (board[x][y] == 'P' && board[x-1][y] == ' ') {
						char temp = board[x-1][y];
						board[x-1][y] = 'P';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x-1][y] = temp;
						board[x][y] = 'P';
						
					}
					if (board[x][y] == 'P' && x == 6 && board[x-1][y] == ' ' && board[x-2][y] == ' ') {
						char temp = board[x-2][y];
							board[x-2][y] = 'P';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y] = temp;
							board[x][y] = 'P';
						}////////////
					if (board[x][y] == 'P' && islower(board[x-1][y-1])){
						char temp = board[x-1][y-1];
						board[x-1][y-1] = 'P';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x-1][y-1] = temp;
						board[x][y] = 'P';
					}
					if (board[x][y] == 'P' && islower(board[x-1][y+1])){
						char temp = board[x-1][y+1];
						board[x-1][y+1] = 'P';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x-1][y+1] = temp;
						board[x][y] = 'P';
					}
					if (board[x][y] == 'H') {
						if (x-1 >= 0 && y-2 >= 0 && (board[x-1][y-2] == ' ' || islower(board[x-1][y-2]))){ 
							char temp = board[x-1][y-2];
							board[x-1][y-2] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (x-1 >= 0 && y+2 < 8 && (board[x-1][y+2] == ' ' || islower(board[x-1][y+2]))){ 
							char temp = board[x-1][y+2];
							board[x-1][y+2] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y+2] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (x+1 < 8 && y+2 < 8 && (board[x+1][y+2] == ' ' || islower(board[x+1][y+2]))){
							char temp = board[x+1][y+2];
							board[x+1][y+2] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y+2] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (x+1 < 8 && y-2 >= 0 && (board[x+1][y-2] == ' ' || islower(board[x+1][y-2]))){
							char temp = board[x+1][y-2];
							board[x+1][y-2] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y-2] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (y-2 >= 0 && x-1 >= 0 && (board[x-1][y-2] == ' ' || islower(board[x-1][y-2]))){
							char temp = board[x-1][y-2];
							board[x-1][y-2] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = temp;
							board[x][y] = 'H';
						}
					}////////////////////////////
					if (board[x][y] == 'H') {
						if (y-1 >= 0 && x-2 >= 0 && (board[x-2][y-1] == ' ' || islower(board[x-2][y-1]))){
							char temp = board[x-2][y-1];
							board[x-2][y-1] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y-1] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (y+1 < 8 && x-2 >= 0 && (board[x-2][y+1] == ' ' || islower(board[x-2][y+1]))){
							char temp = board[x-2][y+1];
							board[x-2][y+1] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y+1] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (y+1 < 8 && x+2 < 8 && (board[x+2][y+1] == ' ' || islower(board[x+2][y+1]))){
							char temp = board[x+2][y+1];
							board[x+2][y+1] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y+1] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'H') {
						if (y-1 >= 0 && x+2 < 8 && (board[x+2][y-1] == ' ' || islower(board[x+2][y-1]))){
							char temp = board[x+2][y-1];
							board[x+2][y-1] = 'H';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y-1] = temp;
							board[x][y] = 'H';
						}
					}
					if (board[x][y] == 'B') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'B';
							}
							else if (x+i < 8 && y+i < 8 && !isupper(board[x+i][y+i]) && board[x+i][y+i] != ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'B';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'B') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'B';
							}
							else if (x+i < 8 && y-i >= 0 && !isupper(board[x+i][y-i]) && board[x+i][y-i] != ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'B';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'B') {
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'B';
							}
							else if (x-i >= 0 && y+i < 8 && !isupper(board[x-i][y+i]) && board[x-i][y+i] != ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'B';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'B') {
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'B';
							}
							else if (x-i >= 0 && y-i >= 0 && !isupper(board[x-i][y-i]) && board[x-i][y-i] != ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'B';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'B';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'R') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								char temp = board[x+i][y];
								board[x+i][y] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'R';
							}
							else if (x+i < 8 && islower(board[x+i][y])) {
								char temp = board[x+i][y];
								board[x+i][y] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'R';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								char temp = board[x-i][y];
								board[x-i][y] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'R';
							}
							else if (x-i >= 0 && islower(board[x-i][y])) {
								char temp = board[x-i][y];
								board[x-i][y] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'R';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								char temp = board[x][y+i];
								board[x][y+i] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'R';
							}
							else if (y+i < 8 && islower(board[x][y+i])) {
								char temp = board[x][y+i];
								board[x][y+i] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'R';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								char temp = board[x][y-i];
								board[x][y-i] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'R';
							}
							else if (y-i >= 0 && islower(board[x][y-i])) {
								char temp = board[x][y-i];
								board[x][y-i] = 'R';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'R';
								break;
							} else break;
						}/////////////////////////////////
					}
					if (board[x][y] == 'Q') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								char temp = board[x+i][y];
								board[x+i][y] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'Q';
							}
							else if (x+i < 8 && islower(board[x+i][y])) {
								char temp = board[x+i][y];
								board[x+i][y] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								char temp = board[x-i][y];
								board[x-i][y] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'Q';
							}
							else if (x-i >= 0 && islower(board[x-i][y])) {
								char temp = board[x-i][y];
								board[x-i][y] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								char temp = board[x][y+i];
								board[x][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'Q';
							}
							else if (y+i < 8 && islower(board[x][y+i])) {
								char temp = board[x][y+i];
								board[x][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								char temp = board[x][y-i];
								board[x][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'Q';
							}
							else if (y-i >= 0 && islower(board[x][y-i])) {
								char temp = board[x][y-i];
								board[x][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'Q';
							}
							else if (x+i < 8 && y+i < 8 && islower(board[x+i][y+i])) {
								char temp = board[x+i][y+i];
								board[x+i][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'Q';
							}
							else if (x+i < 8 && y-i >= 0 && islower(board[x+i][y-i])) {
								char temp = board[x+i][y-i];
								board[x+i][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'Q';
							}
							else if (x-i >= 0 && y+i < 8 && islower(board[x-i][y+i])) {
								char temp = board[x-i][y+i];
								board[x-i][y+i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'Q';
							}
							else if (x-i >= 0 && y-i >= 0 && islower(board[x-i][y-i])) {
								char temp = board[x-i][y-i];
								board[x-i][y-i] = 'Q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = temp;
								board[x][y] = 'Q';
								break;
							} else break;
						}
					}
				}
			}
			return valid_moves;
		}

		void setAdjacencyMatrices() {
			pawn_mat = {
				{0,  0,  0,  0,  0,  0,  0,  0},
				{5, 10, 10,-20,-20, 10, 10,  5},
				{5, -5,-10,  0,  0,-10, -5,  5},
				{0,  0,  0, 20, 20,  0,  0,  0},
				{5,  5, 10, 25, 25, 10,  5,  5},
				{10, 10, 20, 30, 30, 20, 10, 10},
				{50, 50, 50, 50, 50, 50, 50, 50},
				{0,  0,  0,  0,  0,  0,  0,  0}
				
			};
			knight_mat = {
				{-50,-40,-30,-30,-30,-30,-40,-50},
				{-40,-20,  0,  5,  5,  0,-20,-40},
				{-30,  5, 10, 15, 15, 10,  5,-30},
				{-30,  0, 15, 20, 20, 15,  0,-30},
				{-30,  5, 15, 20, 20, 15,  5,-30},
				{-30,  0, 10, 15, 15, 10,  0,-30},
				{-40,-20,  0,  0,  0,  0,-20,-40},
				{-50,-40,-30,-30,-30,-30,-40,-50}
				
			};
			bishop_mat = {
				{-20,-10,-10,-10,-10,-10,-10,-20},
				{-10,  5,  0,  0,  0,  0,  5,-10},
				{-10, 10, 10, 10, 10, 10, 10,-10},
				{-10,  0, 10, 10, 10, 10,  0,-10},
				{-10,  5,  5, 10, 10,  5,  5,-10},
				{-10,  0,  5, 10, 10,  5,  0,-10},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-20,-10,-10,-10,-10,-10,-10,-20}
			};
			rook_mat = {
				{ 0,  0,  0,  5,  5,  0,  0,  0},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{ 5, 10, 10, 10, 10, 10, 10,  5},
				{ 0,  0,  0,  0,  0,  0,  0,  0}
			};
			queen_mat = {
				{-20,-10,-10, -5, -5,-10,-10,-20},
				{-10,  0,  5,  0,  0,  0,  0,-10},
				{-10,  5,  5,  5,  5,  5,  0,-10},
				{  0,  0,  5,  5,  5,  5,  0, -5},
				{ -5,  0,  5,  5,  5,  5,  0, -5},
				{-10,  0,  5,  5,  5,  5,  0,-10},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-20,-10,-10, -5, -5,-10,-10,-20}
			};
			king_mat = {
				{ 20, 30, 10,  0,  0, 10, 30, 20},
				{ 20, 20,  0,  0,  0,  0, 20, 20},
				{-10,-20,-20,-20,-20,-20,-20,-10},
				{-20,-30,-30,-40,-40,-30,-30,-20},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30}
			};

			pawn_mat_human = {
				{0,  0,  0,  0,  0,  0,  0,  0},
				{50, 50, 50, 50, 50, 50, 50, 50},
				{10, 10, 20, 30, 30, 20, 10, 10},
				{5,  5, 10, 25, 25, 10,  5,  5},
				{0,  0,  0, 20, 20,  0,  0,  0},
				{5, -5,-10,  0,  0,-10, -5,  5},
				{5, 10, 10,-20,-20, 10, 10,  5},
				{0,  0,  0,  0,  0,  0,  0,  0}
			};
			knight_mat_human = {
				{-50,-40,-30,-30,-30,-30,-40,-50},
				{-40,-20,  0,  0,  0,  0,-20,-40},
				{-30,  0, 10, 15, 15, 10,  0,-30},
				{-30,  5, 15, 20, 20, 15,  5,-30},
				{-30,  0, 15, 20, 20, 15,  0,-30},
				{-30,  5, 10, 15, 15, 10,  5,-30},
				{-40,-20,  0,  5,  5,  0,-20,-40},
				{-50,-40,-30,-30,-30,-30,-40,-50}
			};
			bishop_mat_human = {
				{-20,-10,-10,-10,-10,-10,-10,-20},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-10,  0,  5, 10, 10,  5,  0,-10},
				{-10,  5,  5, 10, 10,  5,  5,-10},
				{-10,  0, 10, 10, 10, 10,  0,-10},
				{-10, 10, 10, 10, 10, 10, 10,-10},
				{-10,  5,  0,  0,  0,  0,  5,-10},
				{-20,-10,-10,-10,-10,-10,-10,-20}
			};	
			rook_mat_human = {
				{ 0,  0,  0,  0,  0,  0,  0,  0},
				{ 5, 10, 10, 10, 10, 10, 10,  5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{-5,  0,  0,  0,  0,  0,  0, -5},
				{ 0,  0,  0,  5,  5,  0,  0,  0}
			};
			queen_mat_human = {
				{-20,-10,-10, -5, -5,-10,-10,-20},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-10,  0,  5,  5,  5,  5,  0,-10},
				{ -5,  0,  5,  5,  5,  5,  0, -5},
				{  0,  0,  5,  5,  5,  5,  0, -5},
				{-10,  5,  5,  5,  5,  5,  0,-10},
				{-10,  0,  5,  0,  0,  0,  0,-10},
				{-20,-10,-10, -5, -5,-10,-10,-20}
			};
			king_mat_human = {
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-20,-30,-30,-40,-40,-30,-30,-20},
				{-10,-20,-20,-20,-20,-20,-20,-10},
				{ 20, 20,  0,  0,  0,  0, 20, 20},
				{ 20, 30, 10,  0,  0, 10, 30, 20}
			};
		}
};