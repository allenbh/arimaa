
#include <boost/python.hpp>
#include "gkimfl_arimaa_hash.hpp"
#include "gkimfl_arimaa_wrappers.hpp"

using namespace std;
using namespace boost::python;
using namespace gkimfl::arimaa;
using namespace gkimfl::arimaa_hash;
using namespace gkimfl::arimaa_wrappers;

static const state empty_state = { };
static const step empty_step = { };

static inline void validate_piece(int piece) {
  if(piece < 0 || PIECE_COUNT <= piece) {
    PyErr_SetNone(PyExc_ValueError);
    throw_error_already_set();
  }
}

static inline void validate_pos(int pos) {
  if(pos < 0 || 64 <= pos) {
    PyErr_SetNone(PyExc_IndexError);
    throw_error_already_set();
  }
}

static inline void validate_color(int color) {
  if(color < 0 || COLOR_COUNT <= color) {
    PyErr_SetNone(PyExc_ValueError);
    throw_error_already_set();
  }
}

static inline void validate_pair(const tuple& pair) {
  if(len(pair) != 2) {
    PyErr_SetNone(PyExc_ValueError);
    throw_error_already_set();
  }
}

wrap_step::wrap_step() :
  step(empty_step)
{
}

wrap_step::wrap_step(const step& _step) :
  step(_step)
{
}

long wrap_step::get_hash() const {
  return 0;
}

wrap_step_list::wrap_step_list(const wrap_state& state) :
  step_list(state)
{
  parent_hash = state.hash;
  if(state.bit_special) {
    int piece = (int)state.get_bit_piece(state.bit_special);
    int pos = (int)bit_pos(state.bit_special);
    parent_hash ^= hash_piece(piece, pos);
    parent_hash ^= hash_special(pos);
  }
}

object wrap_step_list::next() {
  next_type next;
  if(!get_next(next)) {
    PyErr_SetNone(PyExc_StopIteration);
    throw_error_already_set();
  }

  wrap_step step(next.first);

  long hash = parent_hash;
  if(step.special) {
    hash ^= hash_special((int)step.pos);
  }
  else {
    hash ^= hash_piece((int)step.piece, (int)step.pos);
  }

  switch(step.direction) {
    case NORTH: hash ^= hash_piece((int)step.piece, (int)step.pos - 8); break;
    case SOUTH: hash ^= hash_piece((int)step.piece, (int)step.pos + 8); break;
    case WEST: hash ^= hash_piece((int)step.piece, (int)step.pos - 1); break;
    case EAST: hash ^= hash_piece((int)step.piece, (int)step.pos + 1); break;
    default: ;
  }

  if(step.capture) {
    hash ^= hash_piece((int)step.capture_piece, (int)step.capture_pos);
  }

  wrap_state state(next.second, hash);

  object state_obj(state);

  object step_obj(step);

  tuple trans(make_tuple(step_obj, state_obj));

  return trans;
}

wrap_state::wrap_state() :
  state(empty_state), hash(0)
{
}

wrap_state::wrap_state(const object& pieces) :
  state(empty_state), hash(0)
{
  extract<wrap_state> get_state(pieces);
  if(get_state.check()) {
    *this = get_state();
  }
  else {
    set_pieces(pieces);
  }
}

wrap_state::wrap_state(const object& pieces, const object& special) :
  state(empty_state), hash(0)
{
  set_pieces(pieces);
  set_special(special);
}

wrap_state::wrap_state(const state& _state, long _hash) :
  state(_state), hash(_hash)
{
}

long wrap_state::get_hash() const {
  return hash;
}

bool wrap_state::is_eq(const wrap_state& other) {
  if(hash != other.hash) return false;
  return *this == other;
}

int wrap_state::get_color() const {
  return player_color;
}

void wrap_state::set_color(int color) {
  validate_color(color);
  if(player_color != (color_t)color) {
    hash = ~hash;
    player_color = (color_t)color;
  }
}

object wrap_state::get_special() const {
  if(bit_special) {
    int piece = (int)get_bit_piece(bit_special);
    int pos = (int)bit_pos(bit_special);
    return make_tuple(piece, pos);
  }
  return object();
}

void wrap_state::set_special(const object& special_obj) {
  if(special_obj.is_none()) {
    if(bit_special) {
      int cur_piece = (int)get_bit_piece(bit_special);
      int cur_pos = (int)bit_pos(bit_special);
      hash ^= hash_piece(cur_piece, cur_pos);
      hash ^= hash_special(cur_pos);
      clear_bit_piece(bit_special, (piece_t)cur_piece);
    }
  }
  else {
    tuple special = extract<tuple>(special_obj);
    validate_pair(special);
    int piece = extract<int>(special[0]);
    validate_piece(piece);
    int pos = extract<int>(special[1]);
    validate_pos(pos);
    bitboard_t bit = pos_bit((index_t)pos);
    if(bit & bit_present) {
      int cur_piece = (int)get_bit_piece(bit);
      hash ^= hash_piece(cur_piece, pos);
      clear_bit_piece(bit, (piece_t)cur_piece);
    }
    if(bit_special) {
      int cur_piece = (int)get_bit_piece(bit_special);
      int cur_pos = (int)bit_pos(bit_special);
      hash ^= hash_piece(cur_piece, cur_pos);
      hash ^= hash_special(cur_pos);
      clear_bit_piece(bit_special, (piece_t)cur_piece);
    }
    hash ^= hash_piece(piece, pos);
    hash ^= hash_special(pos);
    put_bit_piece(bit, (piece_t)piece);
    bit_special = bit;
    bit_present ^= bit;
  }
}

object wrap_state::get_piece(int pos) const {
  validate_pos(pos);
  bitboard_t bit = pos_bit((index_t)pos);
  if(bit & bit_present) {
    return object((int)get_bit_piece(bit));
  }
  return object();
}

void wrap_state::set_piece(int pos, const object& piece_obj) {
  validate_pos(pos);
  bitboard_t bit = pos_bit((index_t)pos);
  if(piece_obj.is_none()) {
    if(bit & bit_present) {
      int piece = (int)get_bit_piece(bit);
      hash ^= hash_piece(piece, pos);
      clear_bit_piece(bit, (piece_t)piece);
    }
  }
  else {
    int piece = extract<int>(piece_obj);
    validate_piece(piece);
    if(bit & (bit_present|bit_special)) {
      int cur_piece = (int)get_bit_piece(bit);
      hash ^= hash_piece(cur_piece, pos);
      if(bit & bit_special) {
        hash ^= hash_special(pos);
      }
      clear_bit_piece(bit, (piece_t)cur_piece);
    }
    hash ^= hash_piece(piece, pos);
    put_bit_piece(bit, (piece_t)piece);
  }
}

void wrap_state::set_pieces(const object& pieces_obj) {
  object iter_obj = object(handle<>(PyObject_GetIter(pieces_obj.ptr())));
  int pos = 0;
  PyObject* piece_ptr;
  while(piece_ptr = PyIter_Next(iter_obj.ptr())) {
    object piece_obj = object(handle<>(piece_ptr));
    set_piece(pos, piece_obj);
    ++pos;
  }
  if(pos != 64) {
    PyErr_SetNone(PyExc_IndexError);
    throw_error_already_set();
  }
}

bool wrap_state::is_forced() const {
  return force_push_complete();
}

void wrap_state::turn_over() {
  player_color = color_opposite(player_color);
  hash = ~hash;
  if(bit_special) {
    int piece = (int)get_bit_piece(bit_special);
    int pos = (int)bit_pos(bit_special);
    hash ^= hash_piece(piece, pos);
    hash ^= hash_special(pos);
    clear_bit_piece(bit_special, (piece_t)piece);
  }
}

