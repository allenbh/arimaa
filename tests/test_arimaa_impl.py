
import unittest
from gkimfl import arimaa_impl
from gkimfl import pretty as arimaa_pretty

class test_state(unittest.TestCase):

  def test_color(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.color, 0)
    s.color = 1
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s.color, 1)
    s.color = 0
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.color, 0)

  def test_piece(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s[4], None)
    s[4] = 4
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s[4], 4)
    s[4] = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s[4], None)

  def test_setattr_replace(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s[4], None)
    s[4] = 4
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s[4], 4)
    val = hash(s)
    s[4] = 6
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    self.assertEqual(s[4], 6)
    s[4] = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s[4], None)

  def test_special(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    s.special = (4, 4)
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s.special, (4,4))
    s.special = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)

  def test_special_replace(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    s.special = (4, 4)
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s.special, (4,4))
    val = hash(s)
    s.special = (6, 6)
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    self.assertEqual(s.special, (6,6))
    s.special = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)

  def test_special_replace_setattr(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], None)
    s[4] = 4
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], 4)
    val = hash(s)
    s.special = (6, 4)
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    self.assertEqual(s.special, (6,4))
    self.assertEqual(s[4], None)
    s.special = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], None)

  def test_setattr_replace_special(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], None)
    s.special = (4, 4)
    self.assertNotEqual(hash(s), 0)
    self.assertEqual(s.special, (4,4))
    self.assertEqual(s[4], None)
    val = hash(s)
    s[4] = 6
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], 6)
    s[4] = None
    self.assertEqual(hash(s), 0)
    self.assertEqual(s.special, None)
    self.assertEqual(s[4], None)

class test_expand(unittest.TestCase):

  def test_official_two(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa_impl.state(p)
    e = list(s.expand())
    self.assertEqual(len(e), 27)

  def test_official_eight(self):
    p = map(arimaa_pretty.sym_piece,
        '                                '+
        '         R       eE e     ee    ')
    s = arimaa_impl.state(p)
    e = list(s.expand())
    self.assertEqual(len(e), 2)

if __name__ == '__main__':
  unittest.main();
