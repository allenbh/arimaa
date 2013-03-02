
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

static Py_ssize_t
Gk_State_length(Gk_State * self);

static PyObject *
Gk_State_item(Gk_State * self, Py_ssize_t pos);

static int
Gk_State_assign(Gk_State * self, Py_ssize_t pos, PyObject * val);

static Py_hash_t
Gk_State_hash(Gk_State * self);

static PyObject *
Gk_State_getattro(Gk_State * self, PyObject * attr_obj);

static int
Gk_State_setattro(Gk_State * self, PyObject * attr_obj,
    PyObject * val_obj);

static PyObject *
Gk_State_richcmp(Gk_State * self, Gk_State * other, int op);

static PyObject *
Gk_State_is_forced(Gk_State * self);

static PyObject *
Gk_State_expand(Gk_State * self);

static int
Gk_State_init(Gk_State * self, PyObject * args, PyObject * kwds);

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
  { "expand", (PyCFunction)Gk_State_expand, METH_NOARGS,
    "Return a move iterator on this state" },
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
  "arimaa_impl.state",                      /* tp_name */
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
  (richcmpfunc)Gk_State_richcmp,       /* tp_richcompare */
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
  (initproc)Gk_State_init,             /* tp_init */
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

static PyObject *
Gk_Move_richcmp(Gk_Move * self, Gk_Move * other, int op);

static int
Gk_Move_init(Gk_Move * self, PyObject * args, PyObject * kwds);
  
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
  "arimaa_impl.move",                       /* tp_name */
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
  (richcmpfunc)Gk_Move_richcmp,        /* tp_richcompare */
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
  (initproc)Gk_Move_init,              /* tp_init */
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
  "arimaa_impl.move_iter",                  /* tp_name */
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

  uint64_t bit = pos_bit(pos);
  int piece;

  if(val_obj == Py_None) {
    state_bit_clear(&self->ob_state, bit);
    return 0;
  }

  if(!PyLong_Check(val_obj)) {
    PyErr_SetNone(PyExc_TypeError);
    return -1;
  }

  piece = PyLong_AsLong(val_obj);

  if(piece < 0 || piece >= PIECE_COUNT) {
    PyErr_SetNone(PyExc_ValueError);
    return -1;
  }

  state_bit_clear(&self->ob_state, bit);
  state_bit_put(&self->ob_state, bit, piece);

  return 0;
}

static Py_hash_t
Gk_State_hash(Gk_State * self) {

  return _Py_HashBytes(
      (unsigned char *)&self->ob_state,
      sizeof(struct state));
}

static PyObject *
_Gk_State_getspecial(Gk_State * self) {

  uint64_t bit = self->ob_state.bit_special;
  int pos;
  int piece;

  if(!bit) {
    Py_RETURN_NONE;
  }

  pos = bit_pos(bit);
  piece = state_bit_piece(&self->ob_state, bit);

  return Py_BuildValue("ii", pos, piece);
}

static int
_Gk_State_setspecial(Gk_State * self, PyObject * val_obj) {

  uint64_t bit;
  int pos;
  int piece;

  if(val_obj == Py_None) {
    bit = self->ob_state.bit_special;
    if(bit) {
      state_bit_clear(&self->ob_state, bit);
    }
    return 0;
  }

  if(!PyArg_ParseTuple(val_obj, "ii", &pos, &piece)) {
    PyErr_SetNone(PyExc_TypeError);
    return -1;
  }

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

static int
_Gk_State_setpieces(Gk_State * self, PyObject * val_obj) {
  PyObject * iter;
  PyObject * piece;
  int pos = 0;

  iter = PyObject_GetIter(val_obj);

  if (!iter) {
    return -1;
  }

  while((piece = PyIter_Next(iter))) {

    if(pos >= 64) {
      Py_DECREF(piece);
      Py_DECREF(iter);
      PyErr_SetNone(PyExc_ValueError);
      return -1;
    }

    if(Gk_State_assign(self, pos, piece)) {
      Py_DECREF(piece);
      Py_DECREF(iter);
      return -1;
    }

    Py_DECREF(piece);
    ++pos;
  }
  Py_DECREF(iter);

  if(PyErr_Occurred()) {
    return -1;
  }

  if(pos != 64) {
    PyErr_SetNone(PyExc_ValueError);
    return -1;
  }

  return 0;
}

static PyObject *
Gk_State_getattro(Gk_State * self, PyObject * attr_obj) {

  char * attr_str;
  PyObject * attr_val;

  attr_val = PyObject_GenericGetAttr((PyObject*)self, attr_obj);
  if(attr_val) {
    return attr_val;
  }
  PyErr_Clear();

  attr_str = PyUnicode_AsUTF8(attr_obj);
  if(!attr_str) {
    PyErr_SetNone(PyExc_AttributeError);
    return NULL;
  }

  if(!strcmp(attr_str, "special")) {
    return _Gk_State_getspecial(self);
  }

  if(!strcmp(attr_str, "pieces")) {
    return (PyObject*)self;
  }

  PyErr_SetNone(PyExc_AttributeError);
  return NULL;
}

static int
Gk_State_setattro(Gk_State * self, PyObject * attr_obj,
    PyObject * val_obj) {

  char * attr_str;

  if(!PyObject_GenericSetAttr((PyObject*)self, attr_obj, val_obj)) {
    return 0;
  }
  PyErr_Clear();

  attr_str = PyUnicode_AsUTF8(attr_obj);
  if(!attr_str) {
    PyErr_SetNone(PyExc_AttributeError);
    return -1;
  }

  if(!strcmp(attr_str, "special")) {
    return _Gk_State_setspecial(self, val_obj);
  }

  if(!strcmp(attr_str, "pieces")) {
    return _Gk_State_setpieces(self, val_obj);
  }

  PyErr_SetNone(PyExc_AttributeError);
  return -1;
}

static int
_Gk_State_equal(Gk_State * self, Gk_State *other) {
  int i;
  if(self->ob_state.player_color !=
      other->ob_state.player_color) {
    return 0;
  }
  if(self->ob_state.bit_present !=
      other->ob_state.bit_present) {
    return 0;
  }
  for(i = 0; i < COLOR_COUNT; ++i) {
    if(self->ob_state.bit_color[i] !=
        other->ob_state.bit_color[i]) {
      return 0;
    }
  }
  for(i = 0; i < RANK_COUNT; ++i) {
    if(self->ob_state.bit_rank[i] !=
        other->ob_state.bit_rank[i]) {
      return 0;
    }
  }
  return 1;
}

static PyObject *
Gk_State_richcmp(Gk_State * self, Gk_State * other, int op) {

  if(!PyObject_TypeCheck((PyObject*)other, &Gk_State_type)) {
    goto err_type;
  }

  switch(op) {

    case Py_EQ:
      if (_Gk_State_equal(self, other)) {
        Py_RETURN_TRUE;
      }
      else {
        Py_RETURN_FALSE;
      }

    case Py_NE:
      if (_Gk_State_equal(self, other)) {
        Py_RETURN_FALSE;
      }
      else {
        Py_RETURN_TRUE;
      }
  }

err_type:
  PyErr_SetNone(PyExc_TypeError);
  return NULL;
}

static PyObject *
Gk_State_is_forced(Gk_State * self) {
  if(state_force_push_complete(&self->ob_state)) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

static PyObject *
Gk_State_expand(Gk_State * self) {
  return PyObject_CallFunction(
      (PyObject*)&Gk_MoveIter_type,
      "O", (PyObject*)self);
}

static int
Gk_State_init(Gk_State * self, PyObject * args, PyObject * kwds) {
  
  static char * kwd_names[] = {
    "pieces", "color", "special", NULL };

  PyObject * pieces = NULL;
  PyObject * color = NULL;
  PyObject * special = NULL;

  if(!PyArg_ParseTupleAndKeywords(
        args, kwds, "|OOO", kwd_names,
        &pieces, &color, &special)) {
    return -1;
  }

  if(pieces) {
    if(PyObject_SetAttrString((PyObject*)self,
          "pieces", pieces)) {
      return -1;
    }
  }
    
  if(color) {
    if(PyObject_SetAttrString((PyObject*)self,
          "color", color)) {
      return -1;
    }
  }

  if(special) {
    if(PyObject_SetAttrString((PyObject*)self,
          "special", special)) {
      return -1;
    }
  }

  return 0;
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

static int
_Gk_Move_equal(Gk_Move * self, Gk_Move *other) {
  if(self->ob_move.piece !=
      other->ob_move.piece) {
    return 0;
  }
  if(self->ob_move.pos !=
      other->ob_move.pos) {
    return 0;
  }
  if(self->ob_move.direction !=
      other->ob_move.direction) {
    return 0;
  }
  if(self->ob_move.capture !=
      other->ob_move.capture) {
    return 0;
  }
  if(self->ob_move.capture_piece !=
      other->ob_move.capture_piece) {
    return 0;
  }
  if(self->ob_move.capture_pos !=
      other->ob_move.capture_pos) {
    return 0;
  }
  return 1;
}

static PyObject *
Gk_Move_richcmp(Gk_Move * self, Gk_Move * other, int op) {

  if(!PyObject_TypeCheck((PyObject*)other, &Gk_Move_type)) {
    goto err_type;
  }

  switch(op) {

    case Py_NE:
      if (_Gk_Move_equal(self, other)) {
        Py_RETURN_FALSE;
      }
      else {
        Py_RETURN_TRUE;
      }

    case Py_EQ:
      if (_Gk_Move_equal(self, other)) {
        Py_RETURN_TRUE;
      }
      else {
        Py_RETURN_FALSE;
      }
  }

err_type:
  PyErr_SetNone(PyExc_TypeError);
  return NULL;
}

static int
Gk_Move_init(Gk_Move * self, PyObject * args, PyObject * kwds) {
  
  static char * kwd_names[] = {
    "piece", "pos", "direction", "capture",
    "capture_piece", "capture_pos", NULL };

  PyObject * piece = NULL;
  PyObject * pos = NULL;
  PyObject * direction = NULL;
  PyObject * capture = NULL;
  PyObject * capture_piece = NULL;
  PyObject * capture_pos = NULL;

  if(!PyArg_ParseTupleAndKeywords(
        args, kwds, "|OOOOOO", kwd_names,
        &piece, &pos, &direction, &capture,
        &capture_piece, &capture_pos)) {
    return -1;
  }

  if(piece) {
    if(PyObject_SetAttrString((PyObject*)self,
          "piece", piece)) {
      return -1;
    }
  }
    
  if(pos) {
    if(PyObject_SetAttrString((PyObject*)self,
          "pos", pos)) {
      return -1;
    }
  }
    
  if(direction) {
    if(PyObject_SetAttrString((PyObject*)self,
          "direction", direction)) {
      return -1;
    }
  }
    
  if(capture) {
    if(PyObject_SetAttrString((PyObject*)self,
          "capture", capture)) {
      return -1;
    }
  }
    
  if(capture_piece) {
    if(PyObject_SetAttrString((PyObject*)self,
          "capture_piece", capture_piece)) {
      return -1;
    }
  }
    
  if(capture_pos) {
    if(PyObject_SetAttrString((PyObject*)self,
          "capture_pos", capture_pos)) {
      return -1;
    }
  }
    
  return 0;
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

  Gk_State * state = NULL;

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

static PyModuleDef arimaa_impl = {
    PyModuleDef_HEAD_INIT,
    "arimaa_impl",
    "GkIMFL Arimaa fast move generator",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_arimaa_impl(void) 
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

    m = PyModule_Create(&arimaa_impl);
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

