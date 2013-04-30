
#include "gkimfl_arimaa.hpp"

using namespace gkimfl::arimaa;

// Single trap capture function
static inline void state_capt(
    state& state,
    step& step,
    index_t pos) {

  bitboard_t bit = pos_bit(pos);

  if(bit & is_trap(state.bit_present)) {

    color_t color;
    rank_t rank;

    color = state.get_bit_color(bit);

    if(bit & bit_neighbors_all(
          state.bit_present & state.bit_color[color])) {
      return; // friendly piece nearby
    }

    rank = state.get_bit_rank(bit);

    state.bit_present ^= bit;
    state.bit_color[color] ^= bit;
    state.bit_rank[rank] ^= bit;

    step.capture = 1;
    step.capture_piece = piece(color, rank);
    step.capture_pos = pos;
  }
}

// Transition function
static inline void state_trans(
    state & state,
    step & step) {

  bitboard_t bit = pos_bit(step.pos);
  bitboard_t bit_moved = bit_neighbors(bit, step.direction);
  bitboard_t bit_flip = bit | bit_moved;
  bitboard_t bit_special = step.special ? bit : 0;

  color_t color = piece_color(step.piece);
  rank_t rank = piece_rank(step.piece);

  if(state.bit_special) {
    state.clear_bits(state.bit_special);
  }

  state.bit_special = bit_special;
  state.bit_present ^= bit_flip;
  bit_flip ^= bit_special;
  state.bit_color[color] ^= bit_flip;
  state.bit_rank[rank] ^= bit_flip;

  if(is_trap_neighbor(bit)) {
    state_capt(state, step, trap_pos(step.pos));
  }
}

// ------------------------------------------------------------------
// Arimaa Expand Functions                                           
// ------------------------------------------------------------------

// Expand steps that complete a push
static inline void state_steps_push_complete(
    const state& state,
    step_list& steps) {

  // get presence information
  bitboard_t present = state.bit_present;
  bitboard_t special = state.bit_special;

  // get color information
  color_t friend_color = state.player_color;
  color_t enemy_color = color_opposite(friend_color);

  // get pieces of each color
  bitboard_t friend_bits = present & state.bit_color[friend_color];
  bitboard_t enemy_bits = present & state.bit_color[enemy_color];

  // get friendly bits that can be moved freely
  friend_bits = bits_not_frozen(
      friend_bits, enemy_bits, state.bit_rank);

  // and only bits that threaten the special bit
  friend_bits = bits_threat_to(
      friend_bits, special, state.bit_rank);

  for (int dir = 0; dir < DIRECTION_COUNT; ++dir) {

    bitboard_t bit = friend_bits;

    // filter in the opposite direction of the special space
    bit &= bit_neighbors(special, dir_opposite((direction_t)dir));

    steps.bits[dir].bit_mobile = bit;
    steps.bits[dir].bit_special = 0;
  }
}

// Expand steps when not forced to complete a push
static inline void state_steps_normal(
    const state& state,
    step_list& steps) {

  // get presence information
  bitboard_t present = state.bit_present;
  bitboard_t special = state.bit_special;
  bitboard_t empty = ~present;

  // get color information
  color_t friend_color = state.player_color;
  color_t enemy_color = color_opposite(friend_color);

  // get pieces of each color
  bitboard_t friend_bits = present & state.bit_color[friend_color];
  bitboard_t enemy_bits = present & state.bit_color[enemy_color];

  // get friendly bits that can be moved freely
  bitboard_t friend_mobile_bits = bits_not_frozen(
      friend_bits, enemy_bits, state.bit_rank);

  // get enemy bits that can be moved freely
  bitboard_t enemy_mobile_bits = bits_threatened(
      enemy_bits, friend_bits, state.bit_rank);

  // pieces that can be moved in any direction
  bitboard_t mobile_bits = friend_mobile_bits | enemy_mobile_bits;

  // move pieces in each direction
  for(int dir = 0; dir < DIRECTION_COUNT; ++dir) {

    direction_t opposite_dir = dir_opposite((direction_t)dir);

    bitboard_t dir_mobile = mobile_bits;

    // enemy pieces threatened by a pull in this direction
    bitboard_t special_mobile = bits_dir_threatened(
        enemy_bits, special, state.bit_rank, opposite_dir);

    // remove friendly rabbits north or south
    if(dir_axis((direction_t)dir) == NORTH_SOUTH &&
        dir_sign((direction_t)dir) != friend_color) {
      bitboard_t rabbits = state.bit_rank[RBT];
      rabbits &= state.bit_color[friend_color];
      dir_mobile &= ~rabbits;
    }

    // add special pieces
    dir_mobile |= special_mobile;

    // filter in the opposite direction of an empty space
    dir_mobile &= bit_neighbors(empty, opposite_dir);

    // to reduce the size of the state space,
    // ignore non-threatening friendly special
    bitboard_t next_special = dir_mobile & ~special_mobile;
    bitboard_t special_friends = next_special & friend_bits;
    bitboard_t special_enemies = next_special & enemy_bits;

    special_friends &= bits_threat_to(
      special_friends, enemy_bits, state.bit_rank);

    next_special = special_friends | special_enemies;

    // ready to make some moves
    steps.bits[dir].bit_mobile = dir_mobile;
    steps.bits[dir].bit_special = next_special;
  }
}

// Compute steps from a parent state
static inline void state_steps(
    const state& state,
    step_list& steps) {

  steps.parent = state;
  steps.direction = (direction_t)0;

  if ( state.force_push_complete() ) {
    state_steps_push_complete(state, steps);
  }
  else {
    state_steps_normal(state, steps);
  }
}

// Compute the next step in steps
static inline bool steps_next_step(
    step_list& steps,
    step& step) {

  while(steps.direction < DIRECTION_COUNT) {

    step_list::bits_type & bits = steps.bits[steps.direction];

    if(bits.bit_mobile) {
      index_t pos = bit_pos(bits.bit_mobile);
      bitboard_t bit = pos_bit(pos);

      step.piece = steps.parent.get_bit_piece(bit);
      step.pos = pos;
      step.direction = (direction_t)steps.direction;

      step.special = bit & bits.bit_special ? 1 : 0;

      // capture will be set by state_capt
      step.capture = false;
      step.capture_piece = (piece_t)0;
      step.capture_pos = 0;

      bits.bit_mobile ^= bit;

      return true;
    }

    ++steps.direction;
  }

  return false;
}

// Compute the next child state transition in steps
static inline bool steps_next(
    step_list& steps,
    step& step,
    state& child) {

  if(steps_next_step(steps,step)) {

    child = steps.parent;
    state_trans(child, step); 

    return true;
  }

  return false;
}

step_list::step_list(const state& parent) {
  state_steps(parent, *this);
}

bool step_list::get_next(next_type & next) {
  return steps_next(*this, next.first, next.second);
}

