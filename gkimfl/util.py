
def struct_init(struct, *args, **kwargs):
  '''Init method for simple types using __slots__'''
  for k in struct.__slots__:
    struct.__setattr__(k, None)
  for i,v in enumerate(args):
    struct.__setattr__(struct.__slots__[i], v)
  for k,v in kwargs.items():
    struct.__setattr__(k, v)

def struct(cls):
  '''Decorate a class by adding struct_init method'''
  cls.__init__ = struct_init
  return cls

def is_none(obj):
  return obj is None

def is_not_none(obj):
  return obj is not None

def apply(fun, obj):
  return fun(obj)

def apply_to(obj, fun):
  return fun(obj)

def map_with(fun):
  return lambda objs: map(fun, objs)

def filter_with(fun):
  return lambda objs: filter(fun, objs)

