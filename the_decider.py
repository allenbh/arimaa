from gkimfl import arimaa
from gkimfl import arimaa_pretty
from gkimfl import arimaa_value
from gkimfl import search_quota

p = map(arimaa_pretty.sym_piece,
    ' rc  c  r r rr      H d      MHr'+
    'DdRr   h    e  rR C   ER  RRRRm ')
s = arimaa.state(p)

search = search_quota.search(
    start=s,
    expand=arimaa.moves,
    evaluate=arimaa_value.evaluate,
    quota=100)

while True:
  search.expand_best()

