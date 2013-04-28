#ifndef gkimfl_arimaa_wrappers_hpp
#define gkimfl_arimaa_wrappers_hpp

#include <boost/python.hpp>

#include <vector>

#include "gkimfl_arimaa.hpp"

namespace gkimfl { namespace arimaa_wrappers {

struct wrap_state;

struct wrap_step : public arimaa::step {
  wrap_step();
  wrap_step(const arimaa::step& step);
};

struct wrap_step_list : public arimaa::step_list {
  wrap_step_list(const wrap_state& state);

  static boost::python::object
    self_iterator(const boost::python::object& o) { return o; }

  boost::python::object next();
};

struct wrap_state : public arimaa::state {
  int hash;

  wrap_state();
  wrap_state(const boost::python::object& pieces);
  wrap_state(const boost::python::object& pieces,
      const boost::python::object& special);
  wrap_state(const arimaa::state& state, int hash);

  int get_len() const { return 64; }
  int get_hash() const { return hash; }

  bool is_eq(const wrap_state& other);

  int get_color() const;
  void set_color(int color);

  boost::python::object get_special() const;
  void set_special(const boost::python::object& special);

  boost::python::object get_piece(int pos) const;
  void set_piece(int pos, const boost::python::object& special);

  static boost::python::object
    self_get_pieces(const boost::python::object& o) { return o; }
  void set_pieces(const boost::python::object& pieces);

  wrap_step_list get_steps() const { return wrap_step_list(*this); }

  bool is_forced() const;

  void turn_over();
};

} } // namespace gkimfl::arimaa_wrappers

#endif // gkimfl_arimaa_wrappers_hpp
