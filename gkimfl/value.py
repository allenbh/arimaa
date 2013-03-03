from . import value_alg

transpos_tbl = {}

evaluate = value_alg.weighted_linear((
  (1.0, value_alg.rabbit_count),
  (0.01, value_alg.special_bump),
  (0.8, value_alg.threat_count),
  (0.4, value_alg.threat_efficiency),
  ))

def get_value(state):
  impl = state.impl
  try:
    return transpos_tbl[impl][0]
  except:
    value = evaluate(impl)
    transpos_tbl[impl] = (value, 0)
    return value

def update_value(state, value, depth):
  impl = state.impl
  try:
    cur_depth = transpos_tbl[impl][1]
    if depth > cur_depth:
      transpos_tbl[impl] = (value, depth)
  except KeyError:
    transpos_tbl[impl] = (value, depth)

