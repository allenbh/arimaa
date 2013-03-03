import operator as op
import math
from . import value

class node(object):
  __slots__ = ('state', 'parent', 'trans', 'goal', 'value')

  def __init__(self, state, parent = None, trans = None):
    self.state = state
    self.parent = parent
    self.trans = trans
    self.value = value.get_value(state)

  def update_value(self, val, depth):
    self.value = val
    value.update_value(self.state, val, depth)

  def child(self, trans_child):
    return node(trans_child[1], self, trans_child[0])

  def expand(self):
    return sorted(
        map(self.child, self.state.expand()),
        key = op.attrgetter('value'))

  def is_goal(self):
    return math.isinf(self.value)

  def is_max(self):
    return self.state.impl.color == 0

  def path(self):
    if self.parent != None:
      p = self.parent.path()
      p.append(self)
      return p
    else:
      return [self]

