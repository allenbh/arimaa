
import unittest
from gkimfl import arimaa
from gkimfl import arimaa_pretty

class test_expand(unittest.TestCase):

  def test_official_two(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    e = list(arimaa.moves(s))
    self.assertEqual(len(e), 15419)

  def test_official_two_all(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    e = list(arimaa.moves_all(s))
    self.assertEqual(len(e), 204779)

  def test_official_eight(self):
    p = map(arimaa_pretty.sym_piece,
        '                                '+
        '         R       eE e     ee    ')
    s = arimaa.state(p)
    e = list(arimaa.moves(s))
    self.assertEqual(len(e), 24)

  def test_official_eight_all(self):
    p = map(arimaa_pretty.sym_piece,
        '                                '+
        '         R       eE e     ee    ')
    s = arimaa.state(p)
    e = list(arimaa.moves_all(s))
    self.assertEqual(len(e), 100)

if __name__ == '__main__':
  unittest.main();
