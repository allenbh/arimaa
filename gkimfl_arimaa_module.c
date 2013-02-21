#include <stddef.h>

#include <Python.h>
#include "structmember.h"

#include "gkimfl_arimaa.h"


static void
gkimfl_dealloc(PyObject * self) {
  fprintf(stderr, "gkimfl_dealloc\n");
  self->ob_type->tp_free(self);
}


/* --------------------------- */
/* State                       */
/* --------------------------- */

typedef struct {
  PyObject_HEAD
  struct state ob_state;
} gkimfl_arimaa_state;

static PyMemberDef
gkimfl_arimaa_state_members[] = {
  {"player_color", T_INT,
    offsetof(gkimfl_arimaa_state, ob_state.player_color),
    0, "Player color" },
  {0}
};

static PyObject *
gkimfl_arimaa_state_expand(PyObject * self) {
  fprintf(stderr, "gkimfl_arimaa_state_expand\n");
  Py_RETURN_NONE;
}
 
static PyMethodDef
gkimfl_arimaa_state_methods[] = {
  {"expand", (PyCFunction)gkimfl_arimaa_state_expand, METH_NOARGS,
    "Return an iterator of one step transitions" },
  {0}
};

static Py_hash_t
gkimfl_arimaa_state_hash(PyObject * self) {
  fprintf(stderr, "gkimfl_arimaa_state_hash\n");
  return 0;
}
 
static PyTypeObject
gkimfl_arimaa_state_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.state",               /* tp_name */
  sizeof(gkimfl_arimaa_state),         /* tp_basicsize */
  0,                                   /* tp_itemsize */
  gkimfl_dealloc,                      /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  gkimfl_arimaa_state_hash,            /* tp_hash  */
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
  gkimfl_arimaa_state_methods,         /* tp_methods */
  gkimfl_arimaa_state_members,         /* tp_members */
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


/* --------------------------- */
/* Move                        */
/* --------------------------- */

typedef struct {
  PyObject_HEAD
  struct move ob_move;
} gkimfl_arimaa_move;

static PyMemberDef
gkimfl_arimaa_move_members[] = {
  {0}
};

static Py_hash_t
gkimfl_arimaa_move_hash(PyObject * self) {
  fprintf(stderr, "gkimfl_arimaa_move_hash\n");
  return 0;
}
 
static PyTypeObject
gkimfl_arimaa_move_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.move",                /* tp_name */
  sizeof(gkimfl_arimaa_move),          /* tp_basicsize */
  0,                                   /* tp_itemsize */
  gkimfl_dealloc,                      /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  gkimfl_arimaa_move_hash,             /* tp_hash  */
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
  gkimfl_arimaa_move_members,          /* tp_members */
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

/* --------------------------- */
/* Moves                       */
/* --------------------------- */

typedef struct {
  PyObject_HEAD
  struct moves ob_moves;
} gkimfl_arimaa_moves;

static Py_hash_t
gkimfl_arimaa_moves_hash(PyObject * self) {
  fprintf(stderr, "gkimfl_arimaa_moves_hash\n");
  return 0;
}
 
static PyObject *
gkimfl_arimaa_moves_next(PyObject * self) {
  fprintf(stderr, "gkimfl_arimaa_moves_next\n");
  return NULL;
}
 
static PyTypeObject
gkimfl_arimaa_moves_type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "gkimfl_arimaa.moves",               /* tp_name */
  sizeof(gkimfl_arimaa_moves),         /* tp_basicsize */
  0,                                   /* tp_itemsize */
  gkimfl_dealloc,                      /* tp_dealloc */
  0,                                   /* tp_print */
  0,                                   /* tp_getattr */
  0,                                   /* tp_setattr */
  0,                                   /* tp_compare */
  0,                                   /* tp_repr */
  0,                                   /* tp_as_number */
  0,                                   /* tp_as_sequence */
  0,                                   /* tp_as_mapping */
  gkimfl_arimaa_moves_hash,            /* tp_hash  */
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
  gkimfl_arimaa_moves_next,            /* tp_iternext */
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


/* --------------------------- */
/* Module Init                 */
/* --------------------------- */

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

    gkimfl_arimaa_state_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&gkimfl_arimaa_state_type) < 0)
        return NULL;

    gkimfl_arimaa_move_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&gkimfl_arimaa_move_type) < 0)
        return NULL;

    gkimfl_arimaa_moves_type.tp_new = PyType_GenericNew;
    gkimfl_arimaa_moves_type.tp_iter = PyObject_SelfIter;
    if (PyType_Ready(&gkimfl_arimaa_moves_type) < 0)
        return NULL;

    m = PyModule_Create(&gkimfl_arimaa);
    if (m == NULL)
        return NULL;

    Py_INCREF(&gkimfl_arimaa_state_type);
    PyModule_AddObject(m, "state",
        (PyObject*)&gkimfl_arimaa_state_type);

    Py_INCREF(&gkimfl_arimaa_move_type);
    PyModule_AddObject(m, "move",
        (PyObject*)&gkimfl_arimaa_move_type);

    Py_INCREF(&gkimfl_arimaa_moves_type);
    PyModule_AddObject(m, "moves",
        (PyObject*)&gkimfl_arimaa_moves_type);

    return m;
}

