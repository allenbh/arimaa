
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

static PyObject *
Gk_State_expand(Gk_State * self);

static Py_hash_t
Gk_State_hash(Gk_State * self);

static PyMethodDef
Gk_State_methods[] = {
  {"expand", (PyCFunction)Gk_State_expand, METH_NOARGS,
    "Return an iterator of one step transitions" },
  {0}
};

static PyMemberDef
Gk_State_members[] = {
  {"player_color", T_INT,
    offsetof(Gk_State, ob_state.player_color),
    0, "Player color" },
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
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  (hashfunc)Gk_State_hash,             /* tp_hash  */
  0,                                   /* tp_call */
  0,                                   /* tp_str */
  0,                                   /* tp_getattro */
  0,                                   /* tp_setattro */
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
  0,                                   /* tp_init */
  0,                                   /* tp_alloc */
  0,                                   /* tp_new */
};

/* ------------------------------------------------------------------ */
/* State methods                                                      */
/* ------------------------------------------------------------------ */

static PyObject *
Gk_State_expand(Gk_State * self) {
  Gk_MoveIter * iter;
  
  iter = PyObject_New(Gk_MoveIter, &Gk_MoveIter_type);

  if(iter) {
    state_moves(&self->ob_state, &iter->ob_moves);
  }
    
  return (PyObject *)iter;
}

static Py_hash_t
Gk_State_hash(Gk_State * self) {
  return _Py_HashBytes(
      (unsigned char *)&self->ob_state,
      sizeof(struct state));
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

