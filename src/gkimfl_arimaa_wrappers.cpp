
#include "gkimfl_arimaa_wrappers.hpp"

using namespace std;
using namespace boost::python;
using namespace gkimfl::arimaa;
using namespace gkimfl::arimaa_wrappers;

static const state empty_state = { };
static const step empty_step = { };

wrap_step::wrap_step() :
  step(empty_step)
{
}

wrap_step::wrap_step(const step& _step) :
  step(_step)
{
}

wrap_step_list::wrap_step_list(const wrap_state& state) :
  step_list(state)
{
}

object wrap_step_list::next() {
  next_type next;
  if(!get_next(next)) {
    PyErr_SetNone(PyExc_StopIteration);
    throw_error_already_set();
  }
  int hash = 0; // fixme
  wrap_step step(next.first);
  wrap_state state(next.second, hash);
  return make_tuple(step, state);
}

wrap_state::wrap_state() :
  state(empty_state), hash(0)
{
}

wrap_state::wrap_state(const vector<int>& pieces) :
  state(empty_state), hash(0)
{
  // set pieces
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
}

wrap_state::wrap_state(const vector<int>& pieces,
    int special_piece, int special_pos) :
  state(empty_state), hash(0)
{
  // set pieces
  // set special
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
}

wrap_state::wrap_state(const state& _state, int _hash) :
  state(_state), hash(_hash)
{
}

bool wrap_state::is_eq(const wrap_state& other) {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
  return false;
}

int wrap_state::get_color() const {
  return player_color;
}

void wrap_state::set_color(int color) {
  player_color = (color_t)color;
}

object wrap_state::get_special() const {
  if(bit_special) {
    int piece = (int)get_bit_piece(bit_special);
    int pos = (int)bit_pos(bit_special);
    return make_tuple(piece, pos);
  }
  return object();
}

void wrap_state::set_special(const object& special) {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
}

object wrap_state::get_piece(int pos) const {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
  return object();
}

void wrap_state::set_piece(int pos, const object& special) {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
}

bool wrap_state::is_forced() {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
  return false;
}

void wrap_state::turn_over() {
  PyErr_SetNone(PyExc_NotImplementedError);
  throw_error_already_set();
}

