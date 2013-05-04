from itertools import product
from .arimaa_consts import *

def _piece_counts(pieces):
  counts = [0]*PIECE_COUNT
  for piece in pieces:
    if piece is not None:
      counts[piece] += 1
  return counts

def _rank_counts(counts, rank):
  return [counts[i] for i in range(rank, PIECE_COUNT, 2)]

def _rank_value(gold_count, silv_count):
  return 2.0 * (gold_count - silv_count) / (1 + gold_count + silv_count)
  #return gold_count - silv_count

def _rank_values(gold_counts, silv_counts):
  return [_rank_value(gold_counts[i], silv_counts[i])
      for i in range(RANK_COUNT)]

def _ranks_value(rank_values):
  value = 0
  for i in range(RANK_COUNT):
    value += (i+1) * rank_values[i]
  return value

def material_value(state):
  counts = _piece_counts(state)
  gold_counts = _rank_counts(counts, GOLD)
  silv_counts = _rank_counts(counts, SILV)
  rank_values = _rank_values(gold_counts, silv_counts)
  return _ranks_value(rank_values)

def rabbit_value(state):
  counts = _piece_counts(state)
  gold_counts = _rank_counts(counts, GOLD)
  silv_counts = _rank_counts(counts, SILV)
  return _rank_value(gold_counts[RBT], silv_counts[RBT])

def _threats(threat_counts, threatened_counts):
  value = 0
  count = 0
  for rank in range(RANK_COUNT):
    value += threat_counts[rank] * count
    count += threatened_counts[rank]
  return value

def threats_value(state):
  counts = _piece_counts(state)
  gold_counts = _rank_counts(counts, GOLD)
  silv_counts = _rank_counts(counts, SILV)
  gold_threats = _threats(gold_counts, silv_counts)
  silv_threats = _threats(silv_counts, gold_counts)
  return _rank_value(gold_threats, silv_threats)

def _col(pos):
  return pos & 7

def _row(pos):
  return pos >> 3

def _dist(pos_a, pos_b):
  dist_col = abs(_col(pos_a) - _col(pos_b))
  dist_row = abs(_row(pos_a) - _row(pos_b))
  return dist_col + dist_row

def _rank(piece):
  return piece >> 1

def _color(piece):
  return piece & 1

def efficiency_value(state):
  locs = ([], [])
  for pos, piece in enumerate(state.pieces):
    if piece != None:
      locs[_color(piece)].append((pos, _rank(piece)))
  value = 0
  for gold_loc, silv_loc in product(locs[GOLD], locs[SILV]):
    gold_pos, gold_rank = gold_loc
    silv_pos, silv_rank = silv_loc
    if gold_rank != silv_rank:
      value += 1.0 / (gold_rank - silv_rank) / _dist(gold_pos, silv_pos)
  return value

def _weighted_linear(*weighted_evals):
  def evaluate(state):
    value = 0
    for wt, ev in weighted_evals:
      value += wt * ev(state)
    return value
  return evaluate

evaluate = _weighted_linear(
    (1.0, rabbit_value),
    #(1.0, material_value),
    #(1.0, mobility_value),
    (0.7, threats_value),
    #(0.3, efficiency_value), # this one is expensive
    )

