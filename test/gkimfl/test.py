
import unittest
import gkimfl.arimaa_impl as g
import gkimfl.marshal as m

print("here I am")

def stateOfficialTwo(player_color):
  '''Official move generator input two
     -----------------
  8 |   r c     c     |
  7 | r   r   r r     |
  6 |         H   d   |
  5 |           M H r |
  4 | D d R r       h |
  3 |         e     r |
  2 | R   C       E R |
  1 |     R R R R m   |
     -----------------
      a b c d e f g h
  '''
  syms = (
      ' rc  c  r r rr      H d      MHr' +
      'DdRr   h    e  rR C   ER  RRRRm ' )
  return g.state(
      pieces = map(m.sym_piece, syms),
      color = player_color,
      )

def stateOfficialEight(player_color):
  '''Official move generator input eight
     -----------------
  8 |                 |
  7 |                 |
  6 |                 |
  5 |                 |
  4 |                 |
  3 |   R             |
  2 |   e E   e       |
  1 |     e e         |
     -----------------
      a b c d e f g h
  '''
  syms = (
      '                                ' +
      '         R       eE e     ee    ' )
  return g.state(
      pieces = map(m.sym_piece, syms),
      color = player_color,
      )
                                  
def stateA(player_color):
  '''State A
     -----------------
  8 |     D           |
  7 |   h d C         |
  6 |   E x     x     |
  5 | r C c R         |
  4 |     e           |
  3 | c D H     x M   |
  2 |   d             |
  1 |                 |
     -----------------
      a b c d e f g h
  '''
  syms = (
      '  D      hdC     Ex  x  rCcR    ' +
      '  e     cDH  xM  d              ' )
  return g.state(
      pieces = map(m.sym_piece, syms),
      color = player_color,
      )

def expand(state, depth=1, move_list=()):
  if depth <= 0:
    return
  depth -= 1
  for (move,child) in g.move_iter(state):
    move = move_list + (move,)
    yield (move,child)
    yield from expand(child, depth, move)

def turn_over(state):
  return g.state(
      pieces = state,
      color = state.color^1)

def test_step(self):
  state = self.state
  count = 0
  for (move,child) in expand(state):
    count += 1
  self.assertEqual(count, self.count)

def test_turn(self):
  state = self.state
  count = 0
  seen = set()
  seen.add(turn_over(state))
  for (move,child) in expand(state, 4):
    if not child.is_forced():
      count += 1
      seen.add(turn_over(child))
  seen.discard(turn_over(state))
  self.assertEqual(len(seen), self.unique)
  self.assertEqual(count, self.count)

class TestOfficialOneStep(unittest.TestCase):
  state = stateOfficialTwo(0)
  test = test_step
  count = 27

class TestOfficialOneTurn(unittest.TestCase):
  state = stateOfficialTwo(0)
  test = test_turn
  count = 204779
  unique = 15419

class TestOfficialEightStep(unittest.TestCase):
  state = stateOfficialEight(0)
  test = test_step
  count = 2

class TestOfficialEightTurn(unittest.TestCase):
  state = stateOfficialEight(0)
  test = test_turn
  count = 100
  unique = 24

class TestStepAGold(unittest.TestCase):
  state = stateA(0)
  test = test_step
  count = 18

class TestStepASilv(unittest.TestCase):
  state = stateA(1)
  test = test_step
  count = 17

if __name__ == '__main__':
  unittest.main()
