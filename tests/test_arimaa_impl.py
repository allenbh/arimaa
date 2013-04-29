
import unittest
from gkimfl import arimaa_impl

class test_state(unittest.TestCase):
  def test_setattr_hash(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    s[4] = 4
    self.assertNotEqual(hash(s), 0)
    s[4] = None
    self.assertEqual(hash(s), 0)

  def test_setattr_replace_hash(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    s[4] = 4
    self.assertNotEqual(hash(s), 0)
    val = hash(s)
    s[4] = 6
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    s[4] = None
    self.assertEqual(hash(s), 0)

  def test_special_hash(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    s.special = (4, 4)
    self.assertNotEqual(hash(s), 0)
    s.special = None
    self.assertEqual(hash(s), 0)

  def test_special_replace_hash(self):
    s = arimaa_impl.state()
    self.assertEqual(hash(s), 0)
    s.special = (4, 4)
    self.assertNotEqual(hash(s), 0)
    val = hash(s)
    s.special = (6, 6)
    self.assertNotEqual(hash(s), 0)
    self.assertNotEqual(hash(s), val)
    s.special = None
    self.assertEqual(hash(s), 0)

if __name__ == '__main__':
  unittest.main();
