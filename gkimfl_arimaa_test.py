
import unittest
import gkimfl_arimaa as g

piece_sym = 'RrCcDdHhMmEe'

def piece_as_sym(piece, pos=None):
  if piece is None:
    if pos in (18, 21, 42, 45):
      return 'x'
    return ' '
  return piece_sym[piece]

sym_piece = { sym:piece for piece,sym in enumerate(piece_sym) }

def sym_as_piece(sym):
  return sym_piece.get(sym, None)

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
  state = g.state()
  state.color = player_color
  for i,v in enumerate(syms):
    state[i] = sym_as_piece(v)
  return state

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
  state = g.state()
  state.color = player_color
  for i,v in enumerate(syms):
    state[i] = sym_as_piece(v)
  return state

def test_step(self):
  state = self.state
  count = 0
  for (m,s) in g.move_iter(state):
    count += 1
  self.assertEqual(count, self.count)

def test_turn(self):
  state = self.state
  count = 0
  seen = set()
  seen.add(state)
  for (m1,s1) in g.move_iter(state):
    if not s1.is_forced():
      count += 1
      seen.add(s1)
    for (m2,s2) in g.move_iter(s1):
      if not s2.is_forced():
        count += 1
        seen.add(s2)
      for (m3,s3) in g.move_iter(s2):
        if not s3.is_forced():
          count += 1
          seen.add(s3)
        for (m4,s4) in g.move_iter(s3):
          if not s4.is_forced():
            count += 1
            seen.add(s4)
  seen.remove(state)
  self.assertEqual(count, self.count)
  self.assertEqual(len(seen), self.unique)

class TestInputOneStep(unittest.TestCase):
  state = stateOfficialTwo(0)
  test = test_step
  count = 123

class TestInputOneTurn(unittest.TestCase):
  state = stateOfficialTwo(0)
  test = test_turn
  count = 204060
  unique = 15419

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
