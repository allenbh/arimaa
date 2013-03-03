from . arimaa_impl import state as state_impl
import sys
class state(object):
  __slots__ = ('impl', 'turn', 'step')

  def __init__(self, impl, turn = 0, step = 0):
    self.impl = impl
    self.turn = turn
    self.step = step

  def turn_over(self):
    child_impl = state_impl(
        pieces = self.impl.pieces,
        color = 1 ^ self.impl.color)
    child_state = state(
        impl = child_impl,
        turn = self.turn + self.impl.color,
        step = 0)
    return (None, child_state)

  def expand(self):
    if self.step != 0 and not self.impl.is_forced():
      yield self.turn_over()
    child_turn = self.turn
    child_step = self.step + 1
    if child_step < 4:
      for move, child_impl in self.impl.expand():
        child_state = state(
            impl = child_impl,
            turn = child_turn,
            step = child_step)
        yield (move, child_state)

