
#include "gkimfl_arimaa_wrappers.hpp"

using namespace std;
using namespace boost::python;
using namespace gkimfl::arimaa;
using namespace gkimfl::arimaa_wrappers;

BOOST_PYTHON_MODULE(arimaa_impl) {

  class_<wrap_state>("state", init<>())
    //.def(init<const wrap_state&>()) // init<const object&>() matches first
    .def(init<const object&>())
    .def(init<const object&, const object&>())
    .add_property("color",
        &wrap_state::get_color,
        &wrap_state::set_color)
    .add_property("special",
        &wrap_state::get_special,
        &wrap_state::set_special)
    .add_property("pieces",
        &wrap_state::self_get_pieces,
        &wrap_state::set_pieces)
    .def("__eq__", &wrap_state::is_eq)
    .def("__len__", &wrap_state::get_len)
    .def("__hash__", &wrap_state::get_hash)
    .def("__getitem__", &wrap_state::get_piece)
    .def("__setitem__", &wrap_state::set_piece)
    .def("get_steps", &wrap_state::get_steps)
    .def("get_steps", &wrap_state::get_steps)
    .def("turn_over", &wrap_state::turn_over)
    .def("is_forced", &wrap_state::is_forced)
    ;

  class_<wrap_step>("step", init<>())
    .def(init<const wrap_step&>())
    .def_readwrite("piece", &wrap_step::piece)
    .def_readwrite("pos", &wrap_step::pos)
    .def_readwrite("direction", &wrap_step::direction)
    .def_readwrite("is_special", &wrap_step::special)
    .def_readwrite("is_capture", &wrap_step::capture)
    .def_readwrite("capture_piece", &wrap_step::capture_piece)
    .def_readwrite("capture_pos", &wrap_step::capture_pos)
    ;

  class_<wrap_step_list>("step_list", init<const wrap_state&>())
    .def(init<const wrap_step_list&>())
    .def("__iter__", &wrap_step_list::self_iterator)
    .def("next", &wrap_step_list::next)
    ;

}

