
#include <iostream>
#include "gkimfl_arimaa.cpp"

using namespace std;
using namespace gkimfl::arimaa;

namespace {
static const state empty_state;
static const step empty_step;
static const step_list empty_step_list;
static const step_list::next_type empty_next;
};

/* ------------------------------------------------------------------ */
/* Simple value tests                                                 */
/* ------------------------------------------------------------------ */

int test_piece() {
  if( 0 != (int)piece(GOLD, RBT) ) return 1;
  if( 1 != (int)piece(SILV, RBT) ) return 2;
  if( 2 != (int)piece(GOLD, CAT) ) return 3;
  if( 3 != (int)piece(SILV, CAT) ) return 4;
  if( 10 != (int)piece(GOLD, ELF) ) return 5;
  if( 11 != (int)piece(SILV, ELF) ) return 6;
  return 0;
}

int test_piece_color() {
  if( GOLD != piece_color((piece_t)0) ) return 1;
  if( SILV != piece_color((piece_t)1) ) return 2;
  if( GOLD != piece_color((piece_t)2) ) return 3;
  if( SILV != piece_color((piece_t)3) ) return 4;
  if( GOLD != piece_color((piece_t)10) ) return 5;
  if( SILV != piece_color((piece_t)11) ) return 6;
  return 0;
}

int test_piece_rank() {
  if( RBT != piece_rank((piece_t)0) ) return 1;
  if( RBT != piece_rank((piece_t)1) ) return 2;
  if( CAT != piece_rank((piece_t)2) ) return 3;
  if( CAT != piece_rank((piece_t)3) ) return 4;
  if( ELF != piece_rank((piece_t)10) ) return 5;
  if( ELF != piece_rank((piece_t)11) ) return 6;
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
  bitboard_t bit = 0x8100240000240081ull;
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

int test_state_put_bit_piece() {
  state state(empty_state);

  state.put_bit_piece((bitboard_t)1, piece(SILV, CML));
  state.put_bit_piece((bitboard_t)2, piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)4, piece(GOLD, ELF));

  if( state.bit_present != 7 ) return 1;
  if( state.bit_color[SILV] != 1 ) return 2;
  if( state.bit_color[GOLD] != 6 ) return 3;
  if( state.bit_rank[HRS] != 0 ) return 4;
  if( state.bit_rank[CML] != 3 ) return 5;
  if( state.bit_rank[ELF] != 4 ) return 6;
  return 0;
}

int test_state_bit_color() {
  state state(empty_state);

  state.put_bit_piece((bitboard_t)1, piece(SILV, CML));
  state.put_bit_piece((bitboard_t)2, piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)4, piece(GOLD, ELF));

  if( SILV != state.get_bit_color((bitboard_t)1) ) return 1;
  if( GOLD != state.get_bit_color((bitboard_t)2) ) return 2;
  if( GOLD != state.get_bit_color((bitboard_t)4) ) return 3;
  return 0;
}

int test_state_bit_rank() {
  state state(empty_state);

  state.put_bit_piece((bitboard_t)1, piece(SILV, CML));
  state.put_bit_piece((bitboard_t)2, piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)4, piece(GOLD, ELF));

  if( CML != state.get_bit_rank((bitboard_t)1) ) return 1;
  if( CML != state.get_bit_rank((bitboard_t)2) ) return 2;
  if( ELF != state.get_bit_rank((bitboard_t)4) ) return 3;
  return 0;
}

int test_state_bit_piece() {
  state state(empty_state);

  state.put_bit_piece((bitboard_t)1, piece(SILV, CML));
  state.put_bit_piece((bitboard_t)2, piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)4, piece(GOLD, ELF));

  if( piece(SILV, CML) != state.get_bit_piece((bitboard_t)1) ) return 1;
  if( piece(GOLD, CML) != state.get_bit_piece((bitboard_t)2) ) return 2;
  if( piece(GOLD, ELF) != state.get_bit_piece((bitboard_t)4) ) return 3;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Transition Function Tests                                          */
/* ------------------------------------------------------------------ */

int test_state_capt__empty() {
  state state(empty_state);
  step move(empty_step);

  state_capt(state, move, (index_t)42);

  if( 0 != move.capture ) return 1;
  return 0;
}

int test_state_capt__alone() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(42), piece(GOLD, CML));

  state_capt(state, move, (index_t)42);

  if( 1 != move.capture ) return 1;
  if( piece(GOLD, CML) != move.capture_piece ) return 2;
  if( 42 != move.capture_pos ) return 3;
  return 0;
}

int test_state_capt__enemy_nearby() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(42), piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)pos_bit(41), piece(SILV, ELF));

  state_capt(state, move, (index_t)42);

  if( 1 != move.capture ) return 1;
  if( piece(GOLD, CML) != move.capture_piece ) return 2;
  if( 42 != move.capture_pos ) return 3;
  return 0;
}

int test_state_capt__friend_nearby() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(42), piece(GOLD, CML));
  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, RBT));

  state_capt(state, move, (index_t)42);

  if( 0 != move.capture ) return 1;
  return 0;
}

int test_state_trans__simple() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = NORTH;

  state_trans(state, move);

  if( pos_bit(41) & state.bit_present ) return 1;
  if( pos_bit(33) & ~state.bit_present ) return 2;
  if( piece(GOLD,HRS) !=
      state.get_bit_piece((bitboard_t)pos_bit(33)) ) return 3;
  if( 0 != move.capture ) return 4;
  if( 0 != state.bit_special ) return 5;
  return 0;
}

int test_state_trans__capture() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = EAST;

  state_trans(state, move);

  if( state.bit_present ) return 1;
  if( 1 != move.capture ) return 2;
  if( piece(GOLD, HRS) != move.capture_piece ) return 3;
  if( 42 != move.capture_pos ) return 4;
  return 0;
}

int test_state_trans__special() {
  state state(empty_state);
  step move(empty_step);

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, HRS));

  move.piece = piece(GOLD, HRS);
  move.pos = 41;
  move.direction = NORTH;
  move.special = 1;

  state_trans(state, move);

  if( pos_bit(41) != state.bit_special ) return 1;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Expand Function Tests                                              */
/* ------------------------------------------------------------------ */

int test_state_moves__empty() {
  state state(empty_state);

  state.player_color = GOLD;

  step_list moves(state);

  if( 0 != moves.direction ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__simple() {
  state state(empty_state);

  state.player_color = GOLD;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, CAT));

  step_list moves(state);

  if( pos_bit(41) != moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__gold_rabbit() {
  state state(empty_state);

  state.player_color = GOLD;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, RBT));

  step_list moves(state);

  if( 0 != moves.bits[SOUTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[NORTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__silv_rabbit() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(SILV, RBT));

  step_list moves(state);

  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) != moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__frozen() {
  state state(empty_state);

  state.player_color = GOLD;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, CAT));
  state.put_bit_piece((bitboard_t)pos_bit(40), piece(SILV, DOG));

  step_list moves(state);

  if( 0 != moves.bits[NORTH].bit_mobile ) return 1;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__protected() {
  state state(empty_state);

  state.player_color = GOLD;

  state.put_bit_piece((bitboard_t)pos_bit(42), piece(GOLD, CAT));
  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, DOG));
  state.put_bit_piece((bitboard_t)pos_bit(40), piece(SILV, HRS));

  step_list moves(state);

  if( (pos_bit(42)|pos_bit(41)) !=
      moves.bits[NORTH].bit_mobile ) return 1;
  if( (pos_bit(42)|pos_bit(41)) !=
      moves.bits[SOUTH].bit_mobile ) return 2;
  if( 0 != moves.bits[WEST].bit_mobile ) return 3;
  if( pos_bit(42) != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__special() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, DOG));
  state.put_bit_piece((bitboard_t)pos_bit(49), piece(SILV, HRS));

  step_list moves(state);

  if( pos_bit(41) != moves.bits[NORTH].bit_special ) return 4;
  if( pos_bit(49) != moves.bits[SOUTH].bit_special ) return 4;
  if( (pos_bit(41)|pos_bit(49)) !=
      moves.bits[WEST].bit_special ) return 3;
  if( (pos_bit(41)|pos_bit(49)) !=
      moves.bits[EAST].bit_special ) return 4;
  return 0;
}

int test_state_moves__notspecial() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(49), piece(SILV, HRS));

  step_list moves(state);

  if( 0 != moves.bits[NORTH].bit_special ) return 1;
  if( 0 != moves.bits[SOUTH].bit_special ) return 2;
  if( 0 != moves.bits[WEST].bit_special ) return 3;
  if( 0 != moves.bits[EAST].bit_special ) return 4;
  return 0;
}

int test_state_moves__push() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, CAT));
  state.put_bit_piece((bitboard_t)pos_bit(40), piece(SILV, DOG));

  step_list moves(state);

  if( pos_bit(41) & ~moves.bits[NORTH].bit_mobile ) return 1;
  if( pos_bit(41) & ~moves.bits[SOUTH].bit_mobile ) return 2;
  if( pos_bit(41) != moves.bits[EAST].bit_mobile ) return 3;
  return 0;
}

int test_state_moves__push_complete() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, CAT));
  state.put_bit_piece((bitboard_t)pos_bit(40), piece(SILV, DOG));
  state.put_bit_piece((bitboard_t)pos_bit(33), piece(SILV, CAT));

  state.bit_special ^= pos_bit(41);
  state.bit_present ^= pos_bit(41);

  step_list moves(state);

  if( pos_bit(40) != moves.bits[EAST].bit_mobile ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 2;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 3;
  if( 0 != moves.bits[WEST].bit_mobile ) return 4;
  return 0;
}

int test_state_moves__pull_complete() {
  state state(empty_state);

  state.player_color = SILV;

  state.put_bit_piece((bitboard_t)pos_bit(41), piece(GOLD, CAT));
  state.put_bit_piece((bitboard_t)pos_bit(40), piece(SILV, DOG));

  state.bit_special ^= pos_bit(40);
  state.bit_present ^= pos_bit(40);

  step_list moves(state);

  if( pos_bit(41) != moves.bits[WEST].bit_mobile ) return 1;
  if( 0 != moves.bits[NORTH].bit_mobile ) return 2;
  if( 0 != moves.bits[SOUTH].bit_mobile ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  if( 0 != moves.bits[WEST].bit_special ) return 5;
  return 0;
}

int test_state_moves__parent() {
  state state(empty_state);

  state.player_color = (color_t)1;
  state.bit_present = BITBOARD_C(2);
  state.bit_special = BITBOARD_C(3);
  state.bit_color[GOLD] = BITBOARD_C(4);
  state.bit_color[SILV] = BITBOARD_C(5);
  state.bit_rank[RBT] = BITBOARD_C(6);
  state.bit_rank[ELF] = BITBOARD_C(7);

  step_list moves(state);

  if( (color_t)1 != moves.parent.player_color ) return 1;
  if( BITBOARD_C(2) != moves.parent.bit_present ) return 2;
  if( BITBOARD_C(3) != moves.parent.bit_special ) return 3;
  if( BITBOARD_C(4) != moves.parent.bit_color[GOLD] ) return 4;
  if( BITBOARD_C(5) != moves.parent.bit_color[SILV] ) return 5;
  if( BITBOARD_C(6) != moves.parent.bit_rank[RBT] ) return 6;
  if( BITBOARD_C(7) != moves.parent.bit_rank[ELF] ) return 7;
  return 0;
}

/* ------------------------------------------------------------------ */
/* Iterator Tests                                                     */
/* ------------------------------------------------------------------ */

int test_moves_next_move__same_direction() {
  step_list moves(empty_step_list);
  step_list::next_type move(empty_next);

  moves.parent.put_bit_piece(pos_bit(42), piece(GOLD, CML));
  moves.direction = EAST;
  moves.bits[EAST].bit_mobile = pos_bit(42);

  if( ! moves.get_next(move) ) return 1;
  if( move.first.pos != 42 ) return 2;
  if( move.first.direction != EAST ) return 3;
  return 0;
}

int test_moves_next_move__next_direction() {
  step_list moves(empty_step_list);
  step_list::next_type move(empty_next);

  moves.parent.put_bit_piece(pos_bit(42), piece(GOLD, CML));
  moves.direction = NORTH;
  moves.bits[EAST].bit_mobile = pos_bit(42);

  if( ! moves.get_next(move) ) return 1;
  if( 42 != move.first.pos ) return 2;
  if( EAST != move.first.direction ) return 3;
  if( 0 != moves.bits[EAST].bit_mobile ) return 4;
  return 0;
}

int test_moves_next__stop_iteration() {
  step_list moves(empty_step_list);
  step_list::next_type move(empty_next);

  if( moves.get_next(move) ) return 1;
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
 * 3 | c D H     x M   |
 * 2 |   d             |
 * 1 |                 |
 *    -----------------
 *     a b c d e f g h */
void setup_smoke_A(state& state) {
  state.put_bit_piece(pos_bit(2), piece(GOLD, DOG));
  state.put_bit_piece(pos_bit(9), piece(SILV, HRS));
  state.put_bit_piece(pos_bit(10), piece(SILV, DOG));
  state.put_bit_piece(pos_bit(11), piece(GOLD, CAT));
  state.put_bit_piece(pos_bit(17), piece(GOLD, ELF));
  state.put_bit_piece(pos_bit(24), piece(SILV, RBT));
  state.put_bit_piece(pos_bit(25), piece(GOLD, CAT));
  state.put_bit_piece(pos_bit(26), piece(SILV, CAT));
  state.put_bit_piece(pos_bit(27), piece(GOLD, RBT));
  state.put_bit_piece(pos_bit(34), piece(SILV, ELF));
  state.put_bit_piece(pos_bit(40), piece(SILV, CAT));
  state.put_bit_piece(pos_bit(41), piece(GOLD, DOG));
  state.put_bit_piece(pos_bit(42), piece(GOLD, HRS));
  state.put_bit_piece(pos_bit(46), piece(GOLD, CML));
  state.put_bit_piece(pos_bit(49), piece(SILV, DOG));
}

int test_smoke_A__gold_move_data() {
  state state(empty_state);
  int count;

  setup_smoke_A(state);
  state.player_color = GOLD;

  step_list moves(state);

  if( 0x0000430001000200ull != moves.bits[0].bit_mobile ) return 1;
  if( 0x0000030001000200ull != moves.bits[0].bit_special ) return 2;
  if( 0x0000450003000000ull != moves.bits[1].bit_mobile ) return 3;
  if( 0x0000010003000000ull != moves.bits[1].bit_special ) return 4;
  if( 0x0000400000020204ull != moves.bits[2].bit_mobile ) return 5;
  if( 0x0000000000020200ull != moves.bits[2].bit_special ) return 6;
  if( 0x0000440000020004ull != moves.bits[3].bit_mobile ) return 7;
  if( 0x0000000000020000ull != moves.bits[3].bit_special ) return 8;

  for(count = 0; count < 1000; ++count) {
    step_list::next_type move(empty_next);
    if(!moves.get_next(move)) {
      break;
    }
  }

  if( 18 != count ) return 9;

  return 0;
}

int test_smoke_A__silv_move_data() {
  state state(empty_state);
  int count;

  setup_smoke_A(state);
  state.player_color = SILV;

  step_list moves(state);

  if( 0x000000000c000a00ull != moves.bits[0].bit_mobile ) return 1;
  if( 0x000000000c000800ull != moves.bits[0].bit_special ) return 2;
  if( 0x0002040008000c00ull != moves.bits[1].bit_mobile ) return 3;
  if( 0x0000040008000c00ull != moves.bits[1].bit_special ) return 4;
  if( 0x0002000400000200ull != moves.bits[2].bit_mobile ) return 5;
  if( 0x0000000400000000ull != moves.bits[2].bit_special ) return 6;
  if( 0x0002040408000800ull != moves.bits[3].bit_mobile ) return 7;
  if( 0x0000040408000800ull != moves.bits[3].bit_special ) return 8;

  for(count = 0; count < 1000; ++count) {
    step_list::next_type move(empty_next);
    if(!moves.get_next(move)) {
      break;
    }
  }

  if( 17 != count ) return 9;

  return 0;
}

int test_smoke_A__capt_gold_elf() {
  step_list moves(empty_step_list);
  step_list::next_type move(empty_next);

  setup_smoke_A(moves.parent);
  moves.parent.player_color = GOLD;

  moves.bits[0].bit_mobile  = 0;
  moves.bits[0].bit_special = 0;
  moves.bits[1].bit_mobile  = 0;
  moves.bits[1].bit_special = 0;
  moves.bits[2].bit_mobile  = 0;
  moves.bits[2].bit_special = 0;
  moves.bits[3].bit_mobile  = 0x0000000000020000ull;
  moves.bits[3].bit_special = 0x0000000000020000ull;

  if( !moves.get_next(move) ) return 1;
  if( move.first.piece != piece(GOLD,ELF) ) return 2;
  if( move.first.pos != 17 ) return 3;
  if( move.first.direction != EAST ) return 4;
  if( move.first.capture != 1 ) return 5;
  if( move.first.capture_piece != piece(GOLD,ELF) ) return 6;
  if( move.first.capture_pos != 18 ) return 7;

  return 0;
}

int test_smoke_A__capt_gold_hrs() {
  step_list moves(empty_step_list);
  step_list::next_type move(empty_next);

  setup_smoke_A(moves.parent);
  moves.parent.player_color = GOLD;

  moves.bits[0].bit_mobile  = 0x0000020000000000ull;
  moves.bits[0].bit_special = 0x0000020000000000ull;
  moves.bits[1].bit_mobile  = 0;
  moves.bits[1].bit_special = 0;
  moves.bits[2].bit_mobile  = 0;
  moves.bits[2].bit_special = 0;
  moves.bits[3].bit_mobile  = 0;
  moves.bits[3].bit_special = 0;

  if( !moves.get_next(move) ) return 1;
  if( move.first.piece != piece(GOLD,DOG) ) return 2;
  if( move.first.pos != 41 ) return 3;
  if( move.first.direction != NORTH ) return 4;
  if( move.first.capture != 1 ) return 5;
  if( move.first.capture_piece != piece(GOLD,HRS) ) return 6;
  if( move.first.capture_pos != 42 ) return 7;

  return 0;
}

/* ------------------------------------------------------------------ */
/* Test Harness                                                       */
/* ------------------------------------------------------------------ */

#define TEST(test_name) \
  result = test_name();                \
  if(result) {                         \
    ++fail;                            \
    cerr << "Failed " << #test_name    \
         << ": " << result << endl;    \
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
  TEST(test_state_put_bit_piece);
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
  TEST(test_state_moves__parent);
  TEST(test_moves_next_move__same_direction);
  TEST(test_moves_next_move__next_direction);
  TEST(test_moves_next__stop_iteration);
  TEST(test_smoke_A__gold_move_data);
  TEST(test_smoke_A__silv_move_data);
  TEST(test_smoke_A__capt_gold_elf);
  TEST(test_smoke_A__capt_gold_hrs);

  if(fail) {
    cerr << "Failed " << fail << " of " << count << " tests." << endl;
  } else {
    cerr << "Passed " << count << " tests." << endl;
  }

  return fail;
}


