
from . import arimaa_impl

state = arimaa_impl.state

steps = arimaa_impl.step_list

def moves(start):
  return _expand_turn(start, set())

def moves_all(start):
  return _expand_turn(start, _empty_set)

class _empty_set(object):
  def add(self, x): pass
  def __contains__(self, x): return False
_empty_set = _empty_set()

def _expand_turn(start, seen):
  start_over = arimaa_impl.state(start)
  start_over.turn_over()
  seen.add(start)
  seen.add(start_over)
  children = [((), start)]
  for ply in range(4):
    next_ply = []
    for start_steps, start_child in children:
      for step, child in steps(start_child):
        if child in seen:
          continue
        seen.add(child)
        child_steps = start_steps + (step,)
        if not child.is_forced():
          over = arimaa_impl.state(child)
          over.turn_over()
          if over not in seen:
            seen.add(over)
            yield (child_steps, over)
        child_pair = (child_steps, child)
        next_ply.append(child_pair)
    children = next_ply

