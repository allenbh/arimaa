
import unittest
from gkimfl import arimaa
from gkimfl import arimaa_pretty
from gkimfl import arimaa_value

class test_material(unittest.TestCase):

  def test(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    v = arimaa_value.material_value(s)
    print('material:', v)

class test_threats(unittest.TestCase):

  def test(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    v = arimaa_value.threats_value(s)
    print('threats:', v)

class test_efficiency(unittest.TestCase):

  def test(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    v = arimaa_value.efficiency_value(s)
    print('efficiency:', v)

class test_evaluate(unittest.TestCase):

  def test(self):
    p = map(arimaa_pretty.sym_piece,
        ' rc  c  r r rr      H d      MHr'+
        'DdRr   h    e  rR C   ER  RRRRm ')
    s = arimaa.state(p)
    v = arimaa_value.evaluate(s)
    print('evaluate:', v)

if __name__ == '__main__':
  unittest.main()
