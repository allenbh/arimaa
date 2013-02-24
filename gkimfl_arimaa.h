#ifndef gkimfl_arimaa_h
#define gkimfl_arimaa_h

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Arimaa Piece                                                       */
/* ------------------------------------------------------------------ */

/* Color values */
#define GOLD 0
#define SILV 1
#define COLOR_COUNT 2

/* Rank values */
#define RBT 0
#define CAT 1
#define DOG 2
#define HRS 3
#define CML 4
#define ELF 5
#define RANK_COUNT 6

/* Pieces */
#define PIECE_COUNT (COLOR_COUNT * RANK_COUNT)

static inline int piece(int color, int rank) {
  return (rank << 1) | color;
}

static inline int piece_color(int piece) {
  return piece & 1;
}

static inline int piece_rank(int piece) {
  return piece >> 1;
}

/* ------------------------------------------------------------------ */
/* Arimaa Bits and Positions                                          */
/* ------------------------------------------------------------------ */

/* Direction values */
#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3
#define DIRECTION_COUNT 4

/* Direction axes */
#define NORTH_SOUTH 0
#define WEST_EAST 1

/* Neighbor bit masks */
#define HAS_WEST ((uint64_t)0xfefefefefefefefeull)
#define HAS_EAST ((uint64_t)0x7f7f7f7f7f7f7f7full)

static inline int dir_sign(int dir) {
  return dir & 1;
}

static inline int dir_axis(int dir) {
  return dir >> 1;
}

#ifdef __GNUC__
/* ffsll function declaration copied from <string.h> */
__extension__ extern int ffsll (long long int __ll)
     __THROW __attribute__ ((__const__));

static inline int bit_pos(uint64_t bit) {
  return ffsll(bit) - 1;
}
#else
# error "missing bit_pos support for your compiler"
#endif

static inline uint64_t pos_bit(int pos) {
  return ((uint64_t)1) << pos;
}

static inline uint64_t bit_north(uint64_t bit) {
  return bit >> 8;
}

static inline uint64_t bit_south(uint64_t bit) {
  return bit << 8;
}

static inline uint64_t bit_west(uint64_t bit) {
  return (bit & HAS_WEST) >> 1;
}

static inline uint64_t bit_east(uint64_t bit) {
  return (bit & HAS_EAST) << 1;
}

static inline uint64_t bit_neighbors(uint64_t bit, int dir) {
  switch (dir) {
    case NORTH: return bit_north(bit);
    case SOUTH: return bit_south(bit);
    case WEST: return bit_west(bit);
    case EAST: return bit_east(bit);
  }
  return bit;
}

static inline uint64_t bit_neighbors_all(uint64_t bit) {
  return bit_north(bit) | bit_south(bit) | bit_west(bit) | bit_east(bit);
}

/* ------------------------------------------------------------------ */
/* Arimaa Bit Board                                                   */
/* ------------------------------------------------------------------ */

/* Arimaa bit board */
struct state {
  int player_color;
  uint64_t bit_present;
  uint64_t bit_special;
  uint64_t bit_color[COLOR_COUNT];
  uint64_t bit_rank[RANK_COUNT];
};

/* Find the color of a bit */
static inline int state_bit_color(
    struct state * state,
    uint64_t bit) {

  int color;

  for(color = 0; color < COLOR_COUNT; ++color) {
    if(state->bit_color[color] & bit) {
      return color;
    }
  }

  return 0;
}

/* Find the rank of a bit */
static inline int state_bit_rank(
    struct state * state,
    uint64_t bit) {

  int rank;

  for(rank = 0; rank < RANK_COUNT; ++rank) {
    if(state->bit_rank[rank] & bit) {
      return rank;
    }
  }

  return 0;
}

/* Find the piece of a bit */
static inline int state_bit_piece(
    struct state * state,
    uint64_t bit) {

  int color = state_bit_color(state, bit);
  int rank = state_bit_rank(state, bit);

  return piece(color, rank);
}

/* Put a piece on the board */
static inline void state_bit_put(
    struct state * state,
    uint64_t bit,
    int piece) {

  int color = piece_color(piece);
  int rank = piece_rank(piece);

  state->bit_present |= bit;
  state->bit_color[color] |= bit;
  state->bit_rank[rank] |= bit;
}

/* Clear a piece from the board */
static inline void state_bit_clear(
    struct state * state,
    uint64_t bit) {

  int i;

  bit = ~bit;

  state->bit_present &= bit;
  state->bit_special &= bit;
  for(i = 0; i < COLOR_COUNT; ++i) {
    state->bit_color[i] &= bit;
  }
  for(i = 0; i < RANK_COUNT; ++i) {
    state->bit_rank[i] &= bit;
  }
}

/* ------------------------------------------------------------------ */
/* Arimaa Traps                                                       */
/* ------------------------------------------------------------------ */

/* Traps */
#define TRAP_BITS ((uint64_t)0x0000240000240000ull)
#define TRAP_NEIGHBORS ((uint64_t)0x00245a24245a2400ull)

/* Find nearest trap */
static inline int trap_pos(int pos) {
  switch(pos) {
    case 10: case 17: case 19: case 26: return 18;
    case 13: case 20: case 22: case 29: return 21;
    case 34: case 41: case 43: case 50: return 42;
    case 37: case 44: case 46: case 53: return 45;
  }
  return 0;
}

/* ------------------------------------------------------------------ */
/* Arimaa Transition Functions                                        */
/* ------------------------------------------------------------------ */

/* Data for transition function */
struct move {
  int piece;
  int pos;
  int direction;
  int special;
  int capture;
  int capture_piece;
  int capture_pos;
};

/* Single trap capture function */
static inline void state_capt(
    struct state * state,
    struct move * move,
    int pos) {

  uint64_t bit = pos_bit(pos);

  if(bit & state->bit_present & TRAP_BITS) {

    int color;
    int rank;

    color = state_bit_color(state, bit);

    if(bit & bit_neighbors_all(
          state->bit_present & state->bit_color[color])) {
      return; /* friendly piece nearby */
    }

    rank = state_bit_rank(state, bit);

    state->bit_present ^= bit;
    state->bit_color[color] ^= bit;
    state->bit_rank[rank] ^= bit;

    move->capture = 1;
    move->capture_piece = piece(color, rank);
    move->capture_pos = pos;
  }
}

/* Transition function */
static inline void state_trans(
    struct state * state,
    struct move * move) {

  uint64_t bit = pos_bit(move->pos);
  uint64_t bit_moved = bit_neighbors(bit, move->direction);
  uint64_t bit_flip = bit | bit_moved;
  uint64_t bit_special = move->special ? bit : 0;

  int color = piece_color(move->piece);
  int rank = piece_rank(move->piece);

  state->bit_special = bit_special;
  state->bit_present ^= bit_flip;
  bit_flip ^= bit_special;
  state->bit_color[color] ^= bit_flip;
  state->bit_rank[rank] ^= bit_flip;

  if(bit & TRAP_NEIGHBORS) {
    state_capt(state, move, trap_pos(move->pos));
  }
}

/* ------------------------------------------------------------------ */
/* Arimaa Expand Functions                                            */
/* ------------------------------------------------------------------ */

/* Data for iterating bit moves in one direction */
struct moves_bits {
  uint64_t bit_mobile;
  uint64_t bit_special;
};

/* Data for iterating moves of a parent state */
struct moves {
  int direction;
  struct state parent;
  struct moves_bits bits[DIRECTION_COUNT];
};

/* Compute bits that threaten other bits */
static inline uint64_t bits_threat_to(
    uint64_t bits,
    uint64_t threatened,
    uint64_t ranks[]) {

  uint64_t rank_threatened = 0;
  uint64_t threats = 0;

  int i;
  for (i = 0; i < RANK_COUNT; ++i) {
    threats |= ranks[i] & bit_neighbors_all(rank_threatened);
    rank_threatened |= ranks[i] & threatened;
  }

  return bits & threats;
}

/* Compute bits threatened by other bits */
static inline uint64_t bits_threatened(
    uint64_t bits,
    uint64_t threats,
    uint64_t ranks[]) {

  uint64_t rank_threats = 0;
  uint64_t threatened = 0;

  int i;
  for (i = RANK_COUNT - 1; 0 <= i; --i) {
    threatened |= ranks[i] & bit_neighbors_all(rank_threats);
    rank_threats |= ranks[i] & threats;
  }

  return bits & threatened;
}

/* Compute bits threatened by other bits in one direction */
uint64_t bits_dir_threatened(
    uint64_t bits,
    uint64_t threats,
    uint64_t ranks[],
    int dir) {

  uint64_t rank_threats = 0;
  uint64_t threatened = 0;

  int i;
  for (i = RANK_COUNT - 1; 0 <= i; --i) {
    threatened |= ranks[i] & bit_neighbors(rank_threats, dir);
    rank_threats |= ranks[i] & threats;
  }

  return bits & threatened;
}

/* Compute bits that are not frozen by other bits */
uint64_t bits_not_frozen(
    uint64_t bits,
    uint64_t threats,
    uint64_t ranks[]) {
  return bits &
    (bit_neighbors_all(bits) |
     ~bits_threatened(bits, threats, ranks));
}

/* Expand one step moves that complete a push */
static inline void state_moves_push_complete(
    struct state * state,
    struct moves * moves) {

  /* get presence information */
  uint64_t present = state->bit_present;
  uint64_t special = state->bit_special;

  /* get color information */
  int friend_color = state->player_color;
  int enemy_color = friend_color ^ 1;

  /* get pieces of each color */
  uint64_t friend_bits = present & state->bit_color[friend_color];
  uint64_t enemy_bits = present & state->bit_color[enemy_color];

  /* get friendly bits that can be moved freely */
  friend_bits = bits_not_frozen(
      friend_bits, enemy_bits, state->bit_rank);

  /* and only bits that threaten the special bit */
  friend_bits = bits_threat_to(
      friend_bits, special, state->bit_rank);

  int dir;
  for (dir = 0; dir < DIRECTION_COUNT; ++dir) {

    uint64_t bit = friend_bits;

    /* filter in the opposite direction of the special space */
    bit &= bit_neighbors(special, dir ^ 1);

    moves->bits[dir].bit_mobile = bit;
    moves->bits[dir].bit_special = 0;
  }
}

/* Expand one step moves when not forced to complete a push */
static inline void state_moves_normal(
    struct state * state,
    struct moves * moves) {

  /* get presence information */
  uint64_t present = state->bit_present;
  uint64_t special = state->bit_special;
  uint64_t empty = ~present;

  /* get color information */
  int friend_color = state->player_color;
  int enemy_color = friend_color ^ 1;

  /* get pieces of each color */
  uint64_t friend_bits = present & state->bit_color[friend_color];
  uint64_t enemy_bits = present & state->bit_color[enemy_color];

  /* get friendly bits that can be moved freely */
  uint64_t friend_mobile_bits = bits_not_frozen(
      friend_bits, enemy_bits, state->bit_rank);

  /* get enemy bits that can be moved freely */
  uint64_t enemy_mobile_bits = bits_threatened(
      enemy_bits, friend_bits, state->bit_rank);

  /* pieces that can be moved in any direction */
  uint64_t mobile_bits = friend_mobile_bits | enemy_mobile_bits;

  /* move pieces in each direction */
  int dir;
  for(dir = 0; dir < DIRECTION_COUNT; ++dir) {

    int opposite_dir = dir ^ 1;

    uint64_t dir_mobile = mobile_bits;

    /* enemy pieces threatened by a pull in this direction */
    uint64_t special_mobile = bits_dir_threatened(
        enemy_bits, special, state->bit_rank, opposite_dir);

    /* remove friendly rabbits north or south */
    if(dir_axis(dir) == NORTH_SOUTH &&
        dir_sign(dir) != friend_color) {
      uint64_t rabbits = state->bit_rank[RBT];
      rabbits &= state->bit_color[friend_color];
      dir_mobile &= ~rabbits;
    }

    /* add special pieces */
    dir_mobile |= special_mobile;

    /* filter in the opposite direction of an empty space */
    dir_mobile &= bit_neighbors(empty, opposite_dir);

    /* to reduce the size of the state space,
     * ignore non-threatening friendly special */
    uint64_t next_special = dir_mobile & ~special_mobile;
    uint64_t special_friends = next_special & friend_bits;
    uint64_t special_enemies = next_special & enemy_bits;

    special_friends &= bits_threat_to(
      special_friends, enemy_bits, state->bit_rank);

    next_special = special_friends | special_enemies;

    /* ready to make some moves */
    moves->bits[dir].bit_mobile = dir_mobile;
    moves->bits[dir].bit_special = next_special;
  }
}

/* Force a push completion if special is the enemy color */
static inline int state_force_push_complete(
    struct state * state) {

  int enemy_color = state->player_color ^ 1;

  return ((uint64_t)0) !=
    (state->bit_color[enemy_color] & state->bit_special);
}

/* Compute one step moves from a state */
static inline void state_moves(
    struct state * state,
    struct moves * moves) {

  moves->parent = *state;

  if ( state_force_push_complete(state) ) {
    state_moves_push_complete(state, moves);
  }
  else {
    state_moves_normal(state, moves);
  }
}

/* ------------------------------------------------------------------ */
/* Arimaa Move Iterators                                              */
/* ------------------------------------------------------------------ */

/* Compute the next move in moves */
static inline int moves_next_move(
    struct moves * moves,
    struct move * move) {

  while(moves->direction < DIRECTION_COUNT) {

    struct moves_bits * bits = & moves->bits[moves->direction];

    if(bits->bit_mobile) {
      int pos = bit_pos(bits->bit_mobile);
      uint64_t bit = pos_bit(pos);

      move->piece = state_bit_piece(& moves->parent, bit);
      move->pos = pos;
      move->direction = moves->direction;

      move->special = bit & bits->bit_special ? 1 : 0;

      /* capture will be set by state_capt */
      move->capture = 0;
      move->capture_piece = 0;
      move->capture_pos = 0;

      bits->bit_mobile ^= bit;

      return 1;
    }

    ++moves->direction;
  }

  return 0;
}

/* Compute the next child state transition in moves */
static inline int moves_next(
    struct moves * moves,
    struct move * move,
    struct state * child) {

  if(moves_next_move(moves,move)) {

    *child = moves->parent;
    state_trans(child, move); 

    return 1;
  }

  return 0;
}








#endif /* gkimfl_arimaa_h */
