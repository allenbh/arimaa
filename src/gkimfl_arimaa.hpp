#ifndef gkimfl_arimaa_hpp
#define gkimfl_arimaa_hpp

#include <assert.h>

#include <utility>

#include <boost/cstdint.hpp>

#ifdef BOOST_NO_INT64_T
#error "This implementation requires a 64 bit integer type"
#endif

namespace gkimfl { namespace arimaa {

enum color_t { GOLD, SILV, COLOR_COUNT };
enum rank_t { RBT, CAT, DOG, HRS, CML, ELF, RANK_COUNT };

enum piece_t { GOLD_RBT, SILV_RBT, GOLD_CAT, SILV_CAT,
  GOLD_DOG, SILV_DOG, GOLD_HRS, SILV_HRS, GOLD_CML,
  SILV_CML, GOLD_ELF, SILV_ELF, PIECE_COUNT };

enum direction_t { NORTH, SOUTH, WEST, EAST, DIRECTION_COUNT };
enum axis_t { NORTH_SOUTH, WEST_EAST, AXIS_COUNT };

typedef int index_t;
typedef uint64_t bitboard_t;
#define BITBOARD_C(val) UINT64_C(val)

static inline color_t color_opposite(color_t color) {
  assert(color >= 0);
  assert(color < COLOR_COUNT);
  return (color_t)(color ^ 1);
}

static inline piece_t piece(color_t color, rank_t rank) {
  assert(color >= 0);
  assert(color < COLOR_COUNT);
  assert(rank >= 0);
  assert(rank < RANK_COUNT);
  return (piece_t)((rank << 1) | color);
}

static inline color_t piece_color(piece_t piece) {
  assert(piece >= 0);
  assert(piece < PIECE_COUNT);
  return (color_t)(piece & 1);
}

static inline rank_t piece_rank(piece_t piece) {
  assert(piece >= 0);
  assert(piece < PIECE_COUNT);
  return (rank_t)(piece >> 1);
}

static inline color_t dir_sign(direction_t dir) {
  assert(dir >= 0);
  assert(dir < DIRECTION_COUNT);
  return (color_t)(dir & 1);
}

static inline axis_t dir_axis(direction_t dir) {
  assert(dir >= 0);
  assert(dir < DIRECTION_COUNT);
  return (axis_t)(dir >> 1);
}

static inline direction_t dir_opposite(direction_t dir) {
  assert(dir >= 0);
  assert(dir < DIRECTION_COUNT);
  return (direction_t)(dir ^ 1);
}

static inline bitboard_t has_west(bitboard_t bits) {
  return bits & BITBOARD_C(0xfefefefefefefefe);
}

static inline bitboard_t has_east(bitboard_t bits) {
  return bits & BITBOARD_C(0x7f7f7f7f7f7f7f7f);
}

static inline bitboard_t is_trap(bitboard_t bits) {
  return bits & BITBOARD_C(0x0000240000240000);
}

static inline bitboard_t is_trap_neighbor(bitboard_t bits) {
  return bits & BITBOARD_C(0x00245a24245a2400);
}

// Find nearest trap
static inline index_t trap_pos(index_t pos) {
  switch(pos) {
    case 10: case 17: case 19: case 26: return 18;
    case 13: case 20: case 22: case 29: return 21;
    case 34: case 41: case 43: case 50: return 42;
    case 37: case 44: case 46: case 53: return 45;
    default: ;
  }
  return 0;
}

#ifdef __GNUC__
// ffsll declaration compatible with gcc
extern "C" {
__extension__ extern int ffsll (long long int __ll)
     __THROW __attribute__ ((__const__));
}

static inline index_t bit_pos(bitboard_t bits) {
  assert(bits != BITBOARD_C(0));
  return ffsll(bits) - 1;
}
#else
# error "you need to implement bit_pos for your compiler"
#endif

static inline bitboard_t pos_bit(index_t pos) {
  return BITBOARD_C(1) << pos;
}

static inline bool is_exactly_one_bit(bitboard_t bit) {
  return (bit != BITBOARD_C(0)) && (bit == pos_bit(bit_pos(bit)));
}

static inline bitboard_t bit_north(bitboard_t bits) {
  return bits >> 8;
}

static inline bitboard_t bit_south(bitboard_t bits) {
  return bits << 8;
}

static inline bitboard_t bit_west(bitboard_t bits) {
  return has_west(bits) >> 1;
}

static inline bitboard_t bit_east(bitboard_t bits) {
  return has_east(bits) << 1;
}

static inline bitboard_t bit_neighbors(bitboard_t bits, direction_t dir) {
  switch (dir) {
    case NORTH: return bit_north(bits);
    case SOUTH: return bit_south(bits);
    case WEST: return bit_west(bits);
    case EAST: return bit_east(bits);
    default: ;
  }
  assert(false);
  return bits;
}

static inline bitboard_t bit_neighbors_all(bitboard_t bits) {
  return bit_north(bits) | bit_south(bits) | bit_west(bits) | bit_east(bits);
}

// Compute bits that threaten other bits
static inline bitboard_t bits_threat_to(
    bitboard_t bits,
    bitboard_t threatened,
    const bitboard_t ranks[]) {

  bitboard_t rank_threatened = 0;
  bitboard_t threats = 0;

  for (int i = 0; i < RANK_COUNT; ++i) {
    threats |= ranks[i] & bit_neighbors_all(rank_threatened);
    rank_threatened |= ranks[i] & threatened;
  }

  return bits & threats;
}

// Compute bits threatened by other bits
static inline bitboard_t bits_threatened(
    bitboard_t bits,
    bitboard_t threats,
    const bitboard_t ranks[]) {

  bitboard_t rank_threats = 0;
  bitboard_t threatened = 0;

  for (int i = RANK_COUNT - 1; 0 <= i; --i) {
    threatened |= ranks[i] & bit_neighbors_all(rank_threats);
    rank_threats |= ranks[i] & threats;
  }

  return bits & threatened;
}

// Compute bits threatened by other bits in one direction
static inline bitboard_t bits_dir_threatened(
    bitboard_t bits,
    bitboard_t threats,
    const bitboard_t ranks[],
    direction_t dir) {

  bitboard_t rank_threats = 0;
  bitboard_t threatened = 0;

  for (int i = RANK_COUNT - 1; 0 <= i; --i) {
    threatened |= ranks[i] & bit_neighbors(rank_threats, dir);
    rank_threats |= ranks[i] & threats;
  }

  return bits & threatened;
}

// Compute bits that are not frozen by other bits
static inline bitboard_t bits_not_frozen(
    bitboard_t bits,
    bitboard_t threats,
    const bitboard_t ranks[]) {

  return bits &
    (bit_neighbors_all(bits) |
     ~bits_threatened(bits, threats, ranks));
}

// ------------------------------------------------------------------ //
// Arimaa Bit Board                                                   //
// ------------------------------------------------------------------ //

struct state {
  color_t player_color;
  bitboard_t bit_present;
  bitboard_t bit_special;
  bitboard_t bit_color[COLOR_COUNT];
  bitboard_t bit_rank[RANK_COUNT];

  bool operator == (const state& other) const {
    if(player_color != other.player_color) return false;
    if(bit_present != other.bit_present) return false;
    if(bit_special != other.bit_special) return false;
    if(bit_color[GOLD] != other.bit_color[GOLD]) return false;
    if(bit_color[SILV] != other.bit_color[SILV]) return false;
    if(bit_rank[RBT] != other.bit_rank[RBT]) return false;
    if(bit_rank[CAT] != other.bit_rank[CAT]) return false;
    if(bit_rank[DOG] != other.bit_rank[DOG]) return false;
    if(bit_rank[HRS] != other.bit_rank[HRS]) return false;
    if(bit_rank[CML] != other.bit_rank[CML]) return false;
    if(bit_rank[ELF] != other.bit_rank[ELF]) return false;
    return true;
  }

  bool operator != (const state& other) const {
    return !(*this == other);
  }

  // Find the presence of a bit
  bool get_bit_present(bitboard_t bit) const {
    assert(is_exactly_one_bit(bit));
    return bit_present & bit;
  }

  // Find the color of a bit
  color_t get_bit_color(bitboard_t bit) const {
    assert(is_exactly_one_bit(bit));
    if(bit_color[GOLD] & bit) { return GOLD; }
    assert(bit_color[SILV] & bit); return SILV;
  }

  // Find the rank of a bit
  rank_t get_bit_rank(bitboard_t bit) const {
    assert(is_exactly_one_bit(bit));
    if(bit_rank[RBT] & bit) { return RBT; }
    if(bit_rank[CAT] & bit) { return CAT; }
    if(bit_rank[DOG] & bit) { return DOG; }
    if(bit_rank[HRS] & bit) { return HRS; }
    if(bit_rank[CML] & bit) { return CML; }
    assert(bit_rank[ELF] & bit); return ELF;
  }

  // Find the piece of a bit
  piece_t get_bit_piece(bitboard_t bit) const {
    assert(is_exactly_one_bit(bit));
    color_t color = get_bit_color(bit);
    rank_t rank = get_bit_rank(bit);
    return piece(color, rank);
  }

  // Put a piece on the board
  void put_bit_piece(bitboard_t bit, piece_t piece) {
    assert(is_exactly_one_bit(bit));
    color_t color = piece_color(piece);
    rank_t rank = piece_rank(piece);
    bit_present |= bit;
    bit_color[color] |= bit;
    bit_rank[rank] |= bit;
  }

  // Clear pieces from the board
  void clear_bits(bitboard_t bits) {
    bits = ~bits;
    bit_present &= bits;
    bit_special &= bits;
    bit_color[GOLD] &= bits;
    bit_color[SILV] &= bits;
    bit_rank[RBT] &= bits;
    bit_rank[CAT] &= bits;
    bit_rank[DOG] &= bits;
    bit_rank[HRS] &= bits;
    bit_rank[CML] &= bits;
    bit_rank[ELF] &= bits;
  }

  // Put a piece on the board, clearing an already present piece
  void set_bit_piece(bitboard_t bit, piece_t piece) {
    if(get_bit_present(bit)) {
      clear_bits(bit);
    }
    put_bit_piece(bit, piece);
  }

  // Force a push completion if special is the enemy color
  bool force_push_complete() const {
    color_t enemy_color = color_opposite(player_color);
    return bit_color[enemy_color] & bit_special;
  }

};

// ------------------------------------------------------------------ //
// Arimaa Transition                                                  //
// ------------------------------------------------------------------ //

// Single step transition
struct step {
  piece_t piece;
  index_t pos;
  direction_t direction;
  bool special;
  bool capture;
  piece_t capture_piece;
  index_t capture_pos;
};

// Iterable steps from a parent state
struct step_list {
  typedef std::pair<step, state> next_type;

  struct bits_type {
    bitboard_t bit_mobile;
    bitboard_t bit_special;
  } bits[DIRECTION_COUNT];
  
  int direction;
  state parent;

  step_list(const state& parent);
  bool get_next(next_type & next);
};

} } // namespace gkimfl::arimaa

#endif // gkimfl_arimaa_hpp
