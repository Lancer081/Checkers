#include <stdio.h>

#define U64 unsigned long long

#define getBit(bb, sq) (bb & (1ULL << sq))
#define setBit(bb, sq) (bb |= (1ULL << sq))
#define popBit(bb, sq) (bb &= ~(1ULL << sq))

const int SQ_NUM = 64;
const int BRD_SIZE = 8;

enum { red, black };

typedef struct {
	int from, to;
} Move;

typedef struct {
	Move moves[256];
	int count;
} MoveList;

U64 board[2];
U64 kingBoard[2];
U64 occ;

int side = black;

int dirOffsets[] = {-7, -9, 7, 9};

void resetBoard()
{
	occ = 0ULL;
	
	for (int i = 0; i < 2; i++)
	{
		board[i] = 0ULL;
		kingBoard[i] = 0ULL;
	}
	
	side = red;
}

void initBoard()
{
	resetBoard();

	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		int rank = sq / BRD_SIZE;
	
		if (rank < 3)
		{
			if (rank == 0)
				if (sq % 2)
					setBit(board[red], sq);
			if (rank == 1) 
				if (!(sq % 2))
					setBit(board[red], sq);
			if (rank == 2)
				if (sq % 2)
					setBit(board[red], sq);
		}
		
		if (rank >= 5)
		{
			if (rank == 5)
				if (!(sq % 2))
					setBit(board[black], sq);
			if (rank == 6)
				if (sq % 2)
					setBit(board[black], sq);
			if (rank == 7)
				if (!(sq % 2))
					setBit(board[black], sq);
		}
	
		occ = board[red] | board[black] | kingBoard[red] | kingBoard[black];
	}
}

void printBitboard(U64 bb)
{
	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		if (sq % BRD_SIZE == 0)
			printf("\n%d ", BRD_SIZE - (sq / BRD_SIZE));
			
		printf("%d ", getBit(bb, sq) ? 1 : 0);
	}
	
	printf("\n  1 2 3 4 5 6 7 8\n\n");
}

void printBoard()
{
	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		if (!(sq % BRD_SIZE))
			printf("\n%d ", BRD_SIZE - (sq / BRD_SIZE));
	
		if (getBit(board[red], sq))
			printf("|R|");
		else if (getBit(board[black], sq))
			printf("|B|");
		else if (getBit(kingBoard[red], sq))
			printf("|K|");
		else if (getBit(kingBoard[black], sq))
			printf("|k|");
		else
			printf("|_|");
	}
	
	printf("\n   1  2  3  4  5  6  7  8\n\n");
}

void addMove(MoveList *moves, Move move)
{
	Move moveListMove = moves->moves[moves->count++];
	
	moveListMove.from = move.from;
	moveListMove.to = move.to;
}

void generateMoves(MoveList *moves)
{
	moves->count = 0;

	Move move;

	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		if (getBit(board[side], sq))
		{
			for (int dir = (side == red ? 0 : 2); dir < (side == red ? 2 : 4); dir++)
			{
				int tSq = sq + dirOffsets[dir];

				if (tSq > 0)
				{
					if (!getBit(occ, tSq))
					{
						move.from = sq;
						move.to = tSq;

						addMove(moves, move);
					}
					else if (!getBit(occ, tSq + dirOffsets[dir]))
					{
						move.from = sq;
						move.to = tSq + dirOffsets[dir];

						addMove(moves, move);
					}
				}
			}
		}
		else if (getBit(kingBoard[side], sq))
		{
			for (int dir = 0; dir < 4; dir++)
			{
				int tSq = sq + dirOffsets[dir];

				if (!getBit(occ, tSq))
				{
					move.from = sq;
					move.to = tSq;

					addMove(moves, move);
				}
				else if (!getBit(occ, tSq + dirOffsets[dir]))
				{
					move.from = sq;
					move.to = tSq + dirOffsets[dir];

					addMove(moves, move);
				}
			}
		}
	}
}

int main()
{
	initBoard();
	
	printBoard();

	MoveList moves[1];
	generateMoves(moves);

	for (int i = 0; i < moves->count; i++)
	{
		printf("(%d, %d)", moves->moves[i].from % BRD_SIZE, moves->moves[i].from / BRD_SIZE);
		printf(" to (%d, %d)\n", moves->moves[i].to % BRD_SIZE, moves->moves[i].to / BRD_SIZE);
	}

	return 0;
}

