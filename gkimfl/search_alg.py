import sys

def alpha_beta(node, depth, a = None, b = None):
  '''Minimax search with alpha beta pruning'''

  if depth <= 0 or node.is_goal():
    return node
  child_depth = depth - 1

  if node.is_max():
    fruitful = False
    for child in node.expand():
      child_a = alpha_beta(child, child_depth, a, b)
      if child_a is None:
        continue
      fruitful = True
      if a is None or child_a.value > a.value:
        a = child_a
        if b is not None and a.value >= b.value:
          break
    if fruitful:
      node.update_value(a.value, depth)
      return a

  else:
    fruitful = False
    for child in node.expand():
      child_b = alpha_beta(child, child_depth, a, b)
      if child_b is None:
        continue
      fruitful = True
      if b is None or child_b.value < b.value:
        b = child_b
        if a is not None and a.value >= b.value:
          break
    if fruitful:
      node.update_value(b.value, depth)
      return b

  return None

def deepening(search_fn):
  '''Iterative deepening search decorator'''
  def _deepening(node, depth, *args, **kwds):
    child = node
    for depth_i in range(depth):
      child = search_fn(node, depth_i, *args, **kwds)
      if child is None or child.is_goal():
        break
    return child
  return _deepening

