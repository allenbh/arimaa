
#include <stddef.h>

#include <Python.h>
#include "structmember.h"

#include "gkimfl_arimaa.h"


/* ------------------------------------------------------------------ */
/* State type                                                         */
/* ------------------------------------------------------------------ */

typedef struct {
  PyObject_HEAD
  struct state ob_state;
} Gk_State;

static Py_hash_t
Gk_State_hash(Gk_State * self);

static Py_ssize_t
Gk_State_length(Gk_State * self);

static PyObject *
Gk_State_item(Gk_State * self, Py_ssize_t pos);

static PyObject *
Gk_State_getattro(Gk_State * self, PyObject * attr_obj);

static int
Gk_State_setattro(Gk_State * self, PyObject * attr_obj,
    PyObject * val_obj);

static int
Gk_State_assign(Gk_State * self, Py_ssize_t pos, PyObject * val);

static PyObject *
Gk_State_is_forced(Gk_State * self);

static PySequenceMethods
Gk_State_sequence = {
  (lenfunc) Gk_State_length,           /* sq_length */
  0,                                   /* sq_concat */
  0,                                   /* sq_repeat */
  (ssizeargfunc) Gk_State_item,        /* sq_item */
  0,                                   /* was_sq_slice */
  (ssizeobjargproc) Gk_State_assign,   /* sq_ass_item */
  0,                                   /* was_sq_ass_slice */
  0,                                   /* sq_contains */
  0,                                   /* sq_inplace_concat */
  0,                                   /* sq_inplace_repeat */
};

static PyMethodDef
Gk_State_methods[] = {
  { "is_forced", (PyCFunction)Gk_State_is_forced, METH_NOARGS,
    "Forced completion of a push move" },
  {0}
};

static PyMemberDef
Gk_State_members[] = {
  { "color", T_INT, offsetof(Gk_State, ob_state.player_color), 0,
    "Player color" },
  {0}
};

static PyTypeObject
Gk_State_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.state",               /* tp_name */
  sizeof(Gk_State),                    /* tp_basicsize */
  0,                                   /* tp_itemsize */
  0,                                   /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  &Gk_State_sequence,                  /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  (hashfunc)Gk_State_hash,             /* tp_hash  */
  0,                                   /* tp_call */
  0,                                   /* tp_str */
  (getattrofunc)Gk_State_getattro,     /* tp_getattro */
  (setattrofunc)Gk_State_setattro,     /* tp_setattro */
  0,                                   /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                  /* tp_flags */
  "GkIMFL Arimaa state",               /* tp_doc */
  0,                                   /* tp_traverse */
  0,                                   /* tp_clear */
  0,                                   /* tp_richcompare */
  0,                                   /* tp_weaklistoffset */
  0,                                   /* tp_iter */
  0,                                   /* tp_iternext */
  Gk_State_methods,                    /* tp_methods */
  Gk_State_members,                    /* tp_members */
  0,                                   /* tp_getset */
  0,                                   /* tp_base */
  0,                                   /* tp_dict */
  0,                                   /* tp_descr_get */
  0,                                   /* tp_descr_set */
  0,                                   /* tp_dictoffset */
  0,                                   /* tp_init */
  0,                                   /* tp_alloc */
  0,                                   /* tp_new */
};

/* ------------------------------------------------------------------ */
/* Move type                                                          */
/* ------------------------------------------------------------------ */

typedef struct {
  PyObject_HEAD
  struct move ob_move;
} Gk_Move;

static Py_hash_t
Gk_Move_hash(Gk_Move * self);

static PyMemberDef
Gk_Move_members[] = {
  { "piece", T_INT, offsetof(Gk_Move, ob_move.piece), 0,
    "Piece moved" },
  { "pos", T_INT, offsetof(Gk_Move, ob_move.pos), 0,
    "Piece position before move" },
  { "direction", T_INT, offsetof(Gk_Move, ob_move.direction), 0,
    "Direction of movement" },
  { "capture", T_INT, offsetof(Gk_Move, ob_move.capture), 0,
    "Nonzero if a piece is captured" },
  { "capture_piece", T_INT, offsetof(Gk_Move, ob_move.capture_piece), 0,
    "Captured piece" },
  { "capture_pos", T_INT, offsetof(Gk_Move, ob_move.capture_pos), 0,
    "Position of captured piece" },
  {0}
};

static PyTypeObject
Gk_Move_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.move",                /* tp_name */
  sizeof(Gk_Move),                     /* tp_basicsize */
  0,                                   /* tp_itemsize */
  0,                                   /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  (hashfunc)Gk_Move_hash,              /* tp_hash  */
  0,                                   /* tp_call */
  0,                                   /* tp_str */
  0,                                   /* tp_getattro */
  0,                                   /* tp_setattro */
  0,                                   /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                  /* tp_flags */
  "GkIMFL Arimaa move",                /* tp_doc */
  0,                                   /* tp_traverse */
  0,                                   /* tp_clear */
  0,                                   /* tp_richcompare */
  0,                                   /* tp_weaklistoffset */
  0,                                   /* tp_iter */
  0,                                   /* tp_iternext */
  0,                                   /* tp_methods */
  Gk_Move_members,                     /* tp_members */
  0,                                   /* tp_getset */
  0,                                   /* tp_base */
  0,                                   /* tp_dict */
  0,                                   /* tp_descr_get */
  0,                                   /* tp_descr_set */
  0,                                   /* tp_dictoffset */
  0,                                   /* tp_init */
  0,                                   /* tp_alloc */
  0,                                   /* tp_new */
};

/* ------------------------------------------------------------------ */
/* Move iterator type                                                 */
/* ------------------------------------------------------------------ */

typedef struct {
  PyObject_HEAD
  struct moves ob_moves;
} Gk_MoveIter;

static PyObject *
Gk_MoveIter_next(Gk_MoveIter * self);
 
static int
Gk_MoveIter_init(Gk_MoveIter * self, PyObject * args, PyObject * kwds);
 
static PyTypeObject
Gk_MoveIter_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.move_iter",           /* tp_name */
  sizeof(Gk_MoveIter),                 /* tp_basicsize */
  0,                                   /* tp_itemsize */
  0,                                   /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  0,                                   /* tp_hash  */
  0,                                   /* tp_call */
  0,                                   /* tp_str */
  0,                                   /* tp_getattro */
  0,                                   /* tp_setattro */
  0,                                   /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                  /* tp_flags */
  "GkIMFL Arimaa move iterator",       /* tp_doc */
  0,                                   /* tp_traverse */
  0,                                   /* tp_clear */
  0,                                   /* tp_richcompare */
  0,                                   /* tp_weaklistoffset */
  0,                                   /* tp_iter */
  (iternextfunc)Gk_MoveIter_next,      /* tp_iternext */
  0,                                   /* tp_methods */
  0,                                   /* tp_members */
  0,                                   /* tp_getset */
  0,                                   /* tp_base */
  0,                                   /* tp_dict */
  0,                                   /* tp_descr_get */
  0,                                   /* tp_descr_set */
  0,                                   /* tp_dictoffset */
  (initproc)Gk_MoveIter_init,          /* tp_init */
  0,                                   /* tp_alloc */
  0,                                   /* tp_new */
};

/* ------------------------------------------------------------------ */
/* State methods                                                      */
/* ------------------------------------------------------------------ */

static Py_hash_t
Gk_State_hash(Gk_State * self) {
  return _Py_HashBytes(
      (unsigned char *)&self->ob_state,
      sizeof(struct state));
}

static Py_ssize_t
Gk_State_length(Gk_State * self) {
  return 64;
}

static PyObject *
Gk_State_item(Gk_State * self, Py_ssize_t pos) {
  if(pos < 0 || pos >= 64) {
    PyErr_SetNone(PyExc_IndexError);
    return NULL;
  }
  if(pos_bit(pos) & ~self->ob_state.bit_present) {
    Py_RETURN_NONE;
  }
  return PyLong_FromLong(
      state_bit_piece(&self->ob_state, pos_bit(pos)));
}

static int
Gk_State_assign(Gk_State * self, Py_ssize_t pos, PyObject * val_obj) {

  int piece;

  if(val_obj == Py_None) {
    state_bit_clear(&self->ob_state, pos_bit(pos));
    return 0;
  }

  piece = PyLong_AsLong(val_obj);
  if(PyErr_Occurred()) {
    return -1;
  }
  state_bit_clear(&self->ob_state, pos_bit(pos));
  state_bit_put(&self->ob_state, pos_bit(pos), piece);

  return 0;
}

static PyObject *
Gk_State_getattro(Gk_State * self, PyObject * attr_obj) {

  int piece;
  int pos;
  uint64_t bit;
  PyObject * attr_val;

  attr_val = PyObject_GenericGetAttr((PyObject*)self, attr_obj);
  if(attr_val) {
    return attr_val;
  }
  PyErr_Clear();

  char * attr_str = PyUnicode_AsUTF8(attr_obj);
  if(!attr_str) {
    PyErr_SetNone(PyExc_AttributeError);
    return NULL;
  }

  if(!strcmp(attr_str, "special")) {
    bit = self->ob_state.bit_special;
    if(!bit) {
      Py_RETURN_NONE;
    }
    pos = bit_pos(bit);
    piece = state_bit_piece(&self->ob_state, bit);
    return Py_BuildValue("ii", pos, piece);
  }

  PyErr_SetNone(PyExc_AttributeError);
  return NULL;
}

static int
Gk_State_setattro(Gk_State * self, PyObject * attr_obj,
    PyObject * val_obj) {

  int piece;
  int pos;
  uint64_t bit;

  if(!PyObject_GenericSetAttr((PyObject*)self, attr_obj, val_obj)) {
    return 0;
  }
  PyErr_Clear();

  char * attr_str = PyUnicode_AsUTF8(attr_obj);
  if(!attr_str) {
    PyErr_SetNone(PyExc_AttributeError);
    return -1;
  }

  if(!strcmp(attr_str, "special")) {
    if(val_obj == Py_None) {
      bit = self->ob_state.bit_special;
      if(bit) {
        state_bit_clear(&self->ob_state, bit);
      }
      return 0;
    }
    if(PyArg_ParseTuple(val_obj, "ii", &pos, &piece)) {
      bit = self->ob_state.bit_special;
      if(bit) {
        state_bit_clear(&self->ob_state, bit);
      }
      bit = pos_bit(pos);
      state_bit_clear(&self->ob_state, bit);
      state_bit_put(&self->ob_state, bit, piece);
      self->ob_state.bit_present ^= bit;
      self->ob_state.bit_special ^= bit;
      return 0;
    }
    PyErr_SetNone(PyExc_TypeError);
    return -1;
  }

  PyErr_SetNone(PyExc_AttributeError);
  return -1;
}

static PyObject *
Gk_State_is_forced(Gk_State * self) {
  if(state_force_push_complete(&self->ob_state)) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

/* ------------------------------------------------------------------ */
/* Move methods                                                       */
/* ------------------------------------------------------------------ */

static Py_hash_t
Gk_Move_hash(Gk_Move * self) {
  return _Py_HashBytes(
      (unsigned char *)&self->ob_move,
      sizeof(struct move));
}

/* ------------------------------------------------------------------ */
/* Move iterator methods                                              */
/* ------------------------------------------------------------------ */

static PyObject *
Gk_MoveIter_next(Gk_MoveIter * self) {
  int has_next;
  Gk_Move * move;
  Gk_State * state;
  PyObject * tuple = NULL;

  move = PyObject_New(Gk_Move, &Gk_Move_type);
  if(!move)
    goto err_move;
  Py_INCREF(move);

  state = PyObject_New(Gk_State, &Gk_State_type);
  if(!state)
    goto err_state;
  Py_INCREF(state);
  
  has_next = moves_next(&self->ob_moves,
      &move->ob_move, &state->ob_state);

  if(has_next) {
    tuple = PyTuple_Pack(2, move, state);
  }
    
  Py_DECREF(state);
err_state:
  Py_DECREF(move);
err_move:
  return tuple;
}
 
static int
Gk_MoveIter_init(Gk_MoveIter * self, PyObject * args, PyObject * kwds) {
  
  static char * kwd_names[] = { "state", NULL };

  Gk_State * state;

  if(!PyArg_ParseTupleAndKeywords(
        args, kwds, "O!", kwd_names,
        &Gk_State_type, (PyObject*)&state)) {
    return -1;
  }
    
  state_moves(&state->ob_state, &self->ob_moves);

  return 0;
}

/* ------------------------------------------------------------------ */
/* Module                                                             */
/* ------------------------------------------------------------------ */

static PyModuleDef gkimfl_arimaa = {
    PyModuleDef_HEAD_INIT,
    "gkimfl_arimaa",
    "GkIMFL Arimaa fast move generator",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_gkimfl_arimaa(void) 
{
    PyObject* m;

    Gk_State_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&Gk_State_type) < 0)
        return NULL;

    Gk_Move_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&Gk_Move_type) < 0)
        return NULL;

    Gk_MoveIter_type.tp_new = PyType_GenericNew;
    Gk_MoveIter_type.tp_iter = PyObject_SelfIter;
    if (PyType_Ready(&Gk_MoveIter_type) < 0)
        return NULL;

    m = PyModule_Create(&gkimfl_arimaa);
    if (m == NULL)
        return NULL;

    Py_INCREF(&Gk_State_type);
    PyModule_AddObject(m, "state", (PyObject*)&Gk_State_type);

    Py_INCREF(&Gk_Move_type);
    PyModule_AddObject(m, "move", (PyObject*)&Gk_Move_type);

    Py_INCREF(&Gk_MoveIter_type);
    PyModule_AddObject(m, "move_iter", (PyObject*)&Gk_MoveIter_type);

    return m;
}

