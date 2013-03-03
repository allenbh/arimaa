import itertools as it

def weighted_linear(weighted_evals):
  def _evaluate(state):
    value = 0
    for wt, ev in weighted_evals:
      value += wt * ev(state)
    return value
  return _evaluate

def rabbit_count(state):
  gold_rbt = 0
  silv_rbt = 0
  for piece in state.pieces:
    if piece is not None:
      if piece == 0:
        gold_rbt += 1
      elif piece == 1:
        silv_rbt += 1
  value = gold_rbt - silv_rbt
  return value / 8

def special_bump(state):
  if state.special is None:
    return 0
  elif state.color == 0:
    return 1
  else:
    return -1

def threat_count(state):
  counts = {}
  for piece in state.pieces:
    if piece is not None:
      try:
        counts[piece] += 1
      except KeyError:
        counts[piece] = 1
  gold_pieces = (0,2,4,6,8,10)
  silv_pieces = (1,3,5,7,9,11)
  value = 0
  for gold_rank, gold_piece in enumerate(gold_pieces):
    for silv_rank, silv_piece in enumerate(silv_pieces):
      try:
        if gold_rank > silv_rank:
          value += counts[gold_piece] * counts[silv_piece]
        if gold_rank < silv_rank:
          value -= counts[gold_piece] * counts[silv_piece]
      except KeyError:
        pass
  return value / 80

def threat_efficiency(state):
  locations = []
  for pos, piece in enumerate(state.pieces):
    if piece != None:
      locations.append((pos, piece))
  value = 0
  for loc_a, loc_b in it.combinations(locations, 2):
    color_a = loc_a[1] & 1
    color_b = loc_b[1] & 1
    if color_a != color_b:
      rank_a = loc_a[1] // 2
      rank_b = loc_b[1] // 2
      if rank_a != rank_b:
        col_a = loc_a[0] & 7
        row_a = loc_a[0] // 8
        col_b = loc_b[0] & 7
        row_b = loc_b[0] // 8
        dist = abs(col_a - col_b) + abs(row_a - row_b)
        value += (color_a - color_b) / (rank_a - rank_b) / dist
  return value / 30

