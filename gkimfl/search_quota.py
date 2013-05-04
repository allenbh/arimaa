
import time
from gkimfl import heap
from operator import le, ge

class search(object):

  def __init__(self, start, expand, evaluate, quota=100000):
    self.evaluate = evaluate
    self.expand = expand
    self.quota = quota

    class node(object):
      search = self
      __init__ = _node__init__
      expand = _node_expand
      contract = _node_contract

    self.node = node
    self.root = node(start)
    self.population = 1

  def expand_best(self):
    pop = self.population
    tim = time.time()
    self.root.expand()
    tim = time.time() - tim
    pop = self.population - pop
    print('expanded %6d   in %6.4f sec   %6.0f per sec'%(pop, tim, pop/tim))
    pop = self.population
    tim = time.time()
    while self.population > self.quota:
      self.root.contract()
    tim = time.time() - tim
    pop = pop - self.population
    print('contract %6d   in %6.4f sec   %6.0f per sec'%(pop, tim, pop/tim))

  def is_trivial_best(self):
    return len(self.root.best) == 1

  def get_best(self):
    best_node = self.root.best.first()
    return (best_node.trans, best_node.state)


def _get_best_val(node):
  return node.best_val

def _get_best_pos(node):
  return node.best_pos

def _set_best_pos(node, pos):
  node.best_pos = pos

def _get_worst_val(node):
  return node.worst_val

def _get_worst_pos(node):
  return node.worst_pos

def _set_worst_pos(node, pos):
  node.worst_pos = pos

def _comp_best_worst(color):
  if color == 0:
    return (ge, le)
  else:
    return (le, ge)


def _node__init__(self, state, trans=None):
  self.state = state
  self.trans = trans
  self.best_val = self.search.evaluate(state)
  self.worst_val = self.best_val
  self.best_pos = -1
  self.worst_pos = -1
  self.expanded = False

def _node_expand(self):
  if not self.expanded:
    children = []
    for trans, child in self.search.expand(self.state):
      children.append(self.search.node(child, trans))
    self.search.population += len(children)
    comp_best, comp_worst = _comp_best_worst(self.state.color)
    self.best = heap.heap(items=children,
        comp=comp_best, key=_get_best_val,
        get_pos=_get_best_pos, set_pos=_set_best_pos)
    self.worst = heap.heap(items=children,
        comp=comp_worst, key=_get_worst_val,
        get_pos=_get_worst_pos, set_pos=_set_worst_pos)
    self.expanded = True
  else:
    best_node = self.best.first()
    best_node.expand()
    self.best.bump(best_node)
    self.worst.bump(best_node)
  if len(self.best) > 0:
    self.best_val = self.best.first().best_val
    self.worst_val = self.worst.first().worst_val

def _node_contract(self):
  try:
    worst_node = self.worst.first() # may throw IndexError
  except AttributeError:
    raise IndexError # fake empty when no self.worst
  try:
    worst_node.contract()
    self.best.bump(worst_node)
    self.best_val = self.best.first().best_val
    self.worst.bump(worst_node)
    self.worst_val = self.worst.first().worst_val
  except IndexError:
    self.search.population -= 1
    self.best.remove(worst_node)
    self.best_val = self.best.first().best_val # may throw IndexError
    self.worst.remove(worst_node)
    self.worst_val = self.worst.first().worst_val

