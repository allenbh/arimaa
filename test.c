
#include <stdio.h>

#include "gkimfl_arimaa.h"

/* ------------------------------------------------------------------ */
/* Simple value tests                                                 */
/* ------------------------------------------------------------------ */

int test_piece() {
  if( 0 != piece(GOLD, RBT) ) return 1;
  if( 1 != piece(SILV, RBT) ) return 2;
  if( 2 != piece(GOLD, CAT) ) return 3;
  if( 3 != piece(SILV, CAT) ) return 4;
  if( 10 != piece(GOLD, ELF) ) return 5;
  if( 11 != piece(SILV, ELF) ) return 6;
  return 0;
}

int test_piece_color() {
  if( GOLD != piece_color(0) ) return 1;
  if( SILV != piece_color(1) ) return 2;
  if( GOLD != piece_color(2) ) return 3;
  if( SILV != piece_color(3) ) return 4;
  if( GOLD != piece_color(10) ) return 5;
  if( SILV != piece_color(11) ) return 6;
  return 0;
}

int test_piece_rank() {
  if( RBT != piece_rank(0) ) return 1;
  if( RBT != piece_rank(1) ) return 2;
  if( CAT != piece_rank(2) ) return 3;
  if( CAT != piece_rank(3) ) return 4;
  if( ELF != piece_rank(10) ) return 5;
  if( ELF != piece_rank(11) ) return 6;
  return 0;
}

int test_dir_sign() {
  if( GOLD != dir_sign(NORTH) ) return 1;
  if( SILV != dir_sign(SOUTH) ) return 2;
  return 0;
}

int test_dir_axis() {
  if( NORTH_SOUTH != dir_axis(NORTH) ) return 1;
  if( NORTH_SOUTH != dir_axis(SOUTH) ) return 2;
  if( WEST_EAST != dir_axis(WEST) ) return 3;
  if( WEST_EAST != dir_axis(EAST) ) return 4;
  return 0;
}

int test_bit_pos() {
  if( 0 != bit_pos(0x0000000000000001ull) ) return 1;
  if( 1 != bit_pos(0x0000000000000002ull) ) return 2;
  if( 8 != bit_pos(0x0000000000000100ull) ) return 3;
  if( 8 != bit_pos(0x00000f0f0f0f0f00ull) ) return 4;
  if( 63 != bit_pos(0x8000000000000000ull) ) return 5;
  return 0;
}

int test_pos_bit() {
  if( 0x0000000000000001ull != pos_bit(0) ) return 1;
  if( 0x0000000000000002ull != pos_bit(1) ) return 2;
  if( 0x0000000000000100ull != pos_bit(8) ) return 3;
  if( 0x8000000000000000ull != pos_bit(63) ) return 4;
  return 0;
}

int test_bit_north() {
  if( 0x0081002400002400ull !=
      bit_north(0x8100240000240081ull) ) return 1;
  return 0;
}

int test_bit_south() {
  if( 0x0024000024008100ull !=
      bit_south(0x8100240000240081ull) ) return 1;
  return 0;
}

int test_bit_west() {
  if( 0x4000120000120040ull !=
      bit_west(0x8100240000240081ull) ) return 1;
  return 0;
}

int test_bit_east() {
  if( 0x0200480000480002ull !=
      bit_east(0x8100240000240081ull) ) return 1;
  return 0;
}

int test_bit_neighbors() {
  uint64_t bit = 0x8100240000240081ull;
  if( bit_north(bit) != bit_neighbors(bit, NORTH) ) return 1;
  if( bit_south(bit) != bit_neighbors(bit, SOUTH) ) return 2;
  if( bit_west(bit) != bit_neighbors(bit, WEST) ) return 3;
  if( bit_east(bit) != bit_neighbors(bit, EAST) ) return 4;
  return 0;
}

int test_bit_neighbors_all() {
  if( 0x42a55a24245aa542ull !=
      bit_neighbors_all(0x8100240000240081ull) ) return 1;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Simple board tests                                                 */
/* ------------------------------------------------------------------ */

int test_state_put() {
  struct state state = {0};

  state_put(&state, 1, piece(SILV, CML));
  state_put(&state, 2, piece(GOLD, CML));
  state_put(&state, 4, piece(GOLD, ELF));

  if( state.bit_present != 7 ) return 1;
  if( state.bit_color[SILV] != 1 ) return 2;
  if( state.bit_color[GOLD] != 6 ) return 3;
  if( state.bit_rank[HRS] != 0 ) return 4;
  if( state.bit_rank[CML] != 3 ) return 5;
  if( state.bit_rank[ELF] != 4 ) return 6;
  return 0;
}

int test_state_bit_color() {
  struct state state = {0};

  state_put(&state, 1, piece(SILV, CML));
  state_put(&state, 2, piece(GOLD, CML));
  state_put(&state, 4, piece(GOLD, ELF));

  if( SILV != state_bit_color(&state, 1) ) return 1;
  if( GOLD != state_bit_color(&state, 2) ) return 2;
  if( GOLD != state_bit_color(&state, 4) ) return 3;
  return 0;
}

int test_state_bit_rank() {
  struct state state = {0};

  state_put(&state, 1, piece(SILV, CML));
  state_put(&state, 2, piece(GOLD, CML));
  state_put(&state, 4, piece(GOLD, ELF));

  if( CML != state_bit_rank(&state, 1) ) return 1;
  if( CML != state_bit_rank(&state, 2) ) return 2;
  if( ELF != state_bit_rank(&state, 4) ) return 3;
  return 0;
}

int test_state_bit_piece() {
  struct state state = {0};

  state_put(&state, 1, piece(SILV, CML));
  state_put(&state, 2, piece(GOLD, CML));
  state_put(&state, 4, piece(GOLD, ELF));

  if( piece(SILV, CML) != state_bit_piece(&state, 1) ) return 1;
  if( piece(GOLD, CML) != state_bit_piece(&state, 2) ) return 2;
  if( piece(GOLD, ELF) != state_bit_piece(&state, 4) ) return 3;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Transition Function Tests                                          */
/* ------------------------------------------------------------------ */

int test_state_capt__empty() {
  struct state state = {0};
  struct move move = {0};

  state_capt(&state, &move, 42);

  if( 0 != move.capture ) return 1;
  return 0;
}

int test_state_capt__alone() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(42), piece(GOLD, CML));

  state_capt(&state, &move, 42);

  if( 1 != move.capture ) return 1;
  if( piece(GOLD, CML) != move.capture_piece ) return 2;
  if( 42 != move.capture_pos ) return 3;
  return 0;
}

int test_state_capt__enemy_nearby() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(42), piece(GOLD, CML));
  state_put(&state, pos_bit(41), piece(SILV, ELF));

  state_capt(&state, &move, 42);

  if( 1 != move.capture ) return 1;
  if( piece(GOLD, CML) != move.capture_piece ) return 2;
  if( 42 != move.capture_pos ) return 3;
  return 0;
}

int test_state_capt__friend_nearby() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(42), piece(GOLD, CML));
  state_put(&state, pos_bit(41), piece(GOLD, RBT));

  state_capt(&state, &move, 42);

  if( 0 != move.capture ) return 1;
  return 0;
}

int test_state_trans__simple() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = NORTH;

  state_trans(&state, &move);

  if( pos_bit(41) & state.bit_present ) return 1;
  if( pos_bit(33) & ~state.bit_present ) return 2;
  if( piece(GOLD,HRS) !=
      state_bit_piece(&state, pos_bit(33)) ) return 3;
  if( 0 != move.capture ) return 4;
  if( 0 != state.bit_special ) return 5;
  return 0;
}

int test_state_trans__capture() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = EAST;

  state_trans(&state, &move);

  if( state.bit_present ) return 1;
  if( 1 != move.capture ) return 2;
  if( piece(GOLD, HRS) != move.capture_piece ) return 3;
  if( 42 != move.capture_pos ) return 4;
  return 0;
}

int test_state_trans__special() {
  struct state state = {0};
  struct move move = {0};

  state_put(&state, pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = NORTH;
  move.special = 1;

  state_trans(&state, &move);

  if( pos_bit(41) != state.bit_special ) return 1;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Expand Function Tests                                              */
/* ------------------------------------------------------------------ */

int test_state_moves__empty() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_moves(&state, &moves);

  if( 0 != moves.direction ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__simple() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(41), piece(GOLD, CAT));

  state_moves(&state, &moves);

  if( pos_bit(41) != moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__gold_rabbit() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(41), piece(GOLD, RBT));

  state_moves(&state, &moves);

  if( 0 != moves.bits[SOUTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[NORTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__silv_rabbit() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = SILV;

  state_put(&state, pos_bit(41), piece(SILV, RBT));

  state_moves(&state, &moves);

  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__frozen() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(41), piece(GOLD, CAT));
  state_put(&state, pos_bit(40), piece(SILV, DOG));

  state_moves(&state, &moves);

  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__protected() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(42), piece(GOLD, RBT));
  state_put(&state, pos_bit(41), piece(GOLD, CAT));
  state_put(&state, pos_bit(40), piece(SILV, DOG));

  state_moves(&state, &moves);

  if( (pos_bit(42)|pos_bit(41)) !=
      moves.bits[NORTH].bit_mobile ) return 1;
  if( (pos_bit(42)|pos_bit(41)) !=
      moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(42) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__special() {
  return -1;
}

int test_state_moves__notspecial() {
  return -1;
}

int test_state_moves__push() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(41), piece(GOLD, CAT));
  state_put(&state, pos_bit(40), piece(SILV, DOG));

  state_moves(&state, &moves);

  if( pos_bit(41) & ~moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) & ~moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 3;
  return 0;
}

int test_state_moves__push_complete() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = SILV;

  state_put(&state, pos_bit(41), piece(GOLD, CAT));
  state_put(&state, pos_bit(40), piece(SILV, DOG));
  state_put(&state, pos_bit(33), piece(SILV, CAT));

  state.bit_special ^= pos_bit(41);
  state.bit_present ^= pos_bit(41);

  state_moves(&state, &moves);

  if( pos_bit(40) != moves.bits[EAST].bit_mobile ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 2;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 3;
  if( 0 != moves.bits[WEST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__pull_complete() {
  struct state state = {0};
  struct moves moves = {0};

  state.player_color = GOLD;

  state_put(&state, pos_bit(41), piece(GOLD, CAT));
  state_put(&state, pos_bit(40), piece(SILV, DOG));

  state.bit_special ^= pos_bit(40);
  state.bit_present ^= pos_bit(40);

  state_moves(&state, &moves);

  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 2;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Iterator Tests                                                     */
/* ------------------------------------------------------------------ */

int test_moves_next_move__same_direction() {
  struct moves moves = {0};
  struct move move = {0};

  moves.direction = EAST;
  moves.bits[EAST].bit_mobile = pos_bit(42);

  if( ! moves_next_move(&moves, &move) ) return 1;
  if( move.pos != 42 ) return 2;
  if( move.direction != EAST ) return 3;
  return 0;
}

int test_moves_next_move__next_direction() {
  struct moves moves = {0};
  struct move move = {0};

  moves.direction = NORTH;
  moves.bits[EAST].bit_mobile = pos_bit(42);

  if( ! moves_next_move(&moves, &move) ) return 1;
  if( 42 != move.pos ) return 2;
  if( EAST != move.direction ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_moves_next_move__stop_iteration() {
  struct moves moves = {0};
  struct move move = {0};

  if( moves_next_move(&moves, &move) ) return 1;
  return 0;
}

int test_moves_next__transition() {
  struct state state = {0};
  struct moves moves = {0};
  struct move move = {0};

  state_put(&moves.parent, pos_bit(33), piece(GOLD, CML));

  moves.bits[SOUTH].bit_mobile = pos_bit(33);

  if( ! moves_next(&moves, &move, &state) ) return 1;
  if( pos_bit(33) == state.bit_present ) return 2;
  if( pos_bit(41) != state.bit_present ) return 3;

  return 0;
}

int test_moves_next__stop_iteration() {
  struct state state = {0};
  struct moves moves = {0};
  struct move move = {0};

  if( moves_next(&moves, &move, &state) ) return 1;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Smoke Tests                                                        */
/* ------------------------------------------------------------------ */

/* State A
 *    -----------------
 * 8 |     D           |
 * 7 |   h d C         |
 * 6 |   E x     x     |
 * 5 | r C c R         |
 * 4 |     e           |
 * 3 | c D H     M     |
 * 2 |   d             |
 * 1 |                 |
 *    -----------------
 *     a b c d e f g h */
void setup_state_A(struct state * state) {
  state_put(state, pos_bit(2), piece(GOLD, DOG));
  state_put(state, pos_bit(9), piece(SILV, HRS));
  state_put(state, pos_bit(10), piece(SILV, DOG));
  state_put(state, pos_bit(11), piece(GOLD, CAT));
  state_put(state, pos_bit(17), piece(GOLD, ELF));
  state_put(state, pos_bit(24), piece(SILV, RBT));
  state_put(state, pos_bit(25), piece(GOLD, CAT));
  state_put(state, pos_bit(26), piece(SILV, CAT));
  state_put(state, pos_bit(27), piece(GOLD, RBT));
  state_put(state, pos_bit(34), piece(SILV, ELF));
  state_put(state, pos_bit(40), piece(SILV, CAT));
  state_put(state, pos_bit(41), piece(GOLD, DOG));
  state_put(state, pos_bit(42), piece(GOLD, HRS));
  state_put(state, pos_bit(45), piece(GOLD, CML));
  state_put(state, pos_bit(49), piece(SILV, DOG));
}

/* ------------------------------------------------------------------ */
/* Test Harness                                                       */
/* ------------------------------------------------------------------ */

#define TEST(test_name) \
  result = test_name();                \
  if(result) {                         \
    ++fail;                            \
    fprintf(stderr, "Failed %s: %d\n", \
        #test_name, result);           \
  } ++count

int main() {
  int count = 0, fail = 0, result;

  TEST(test_piece);
  TEST(test_piece_color);
  TEST(test_piece_rank);
  TEST(test_dir_sign);
  TEST(test_dir_axis);
  TEST(test_bit_pos);
  TEST(test_pos_bit);
  TEST(test_bit_north);
  TEST(test_bit_south);
  TEST(test_bit_west);
  TEST(test_bit_east);
  TEST(test_bit_neighbors);
  TEST(test_bit_neighbors_all);
  TEST(test_state_put);
  TEST(test_state_bit_color);
  TEST(test_state_bit_rank);
  TEST(test_state_bit_piece);
  TEST(test_state_capt__empty);
  TEST(test_state_capt__alone);
  TEST(test_state_capt__enemy_nearby);
  TEST(test_state_capt__friend_nearby);
  TEST(test_state_trans__simple);
  TEST(test_state_trans__capture);
  TEST(test_state_trans__special);
  TEST(test_state_moves__empty);
  TEST(test_state_moves__simple);
  TEST(test_state_moves__gold_rabbit);
  TEST(test_state_moves__silv_rabbit);
  TEST(test_state_moves__frozen);
  TEST(test_state_moves__protected);
  TEST(test_state_moves__special);
  TEST(test_state_moves__notspecial);
  TEST(test_state_moves__push);
  TEST(test_state_moves__push_complete);
  TEST(test_state_moves__pull_complete);
  TEST(test_moves_next_move__same_direction);
  TEST(test_moves_next_move__next_direction);
  TEST(test_moves_next_move__stop_iteration);
  TEST(test_moves_next__transition);
  TEST(test_moves_next__stop_iteration);

  if(fail) {
    fprintf(stderr, "Failed %d of %d tests.\n", fail, count);
  } else {
    fprintf(stderr, "Passed %d tests.\n", count);
  }

  return fail;
}


