from . import util

@util.struct
class search_node(object):
  __slots__ = (
      'value',
      'depth',
      'size',
      'state',
      'parent',
      'trans',
      )

  def __neg__(self):
    neg = search_node(
        value = -self.value,
        depth = self.depth,
        size = self.size,
        state = self.state,
        parent = self.parent,
        trans = self.trans,
        )
    return neg

@util.struct
class search(object):
  __slots__ = (
      'start_state',
      'node_tbl',
      'expand_fn',
      'value_fn',
      'reduce_fn',
      'prune_fn',
      )

  def get_path(self, ev):
    path = []
    while ev is not None:
      path.append(ev)
      ev = ev.parent
    return reversed(path)

  def search(self, depth):
    node = self.get_node(self.start_state)
    return self._search(node, depth)

  def _search(self, node, depth):
    if self.prune_fn(node, depth):
      children = self._search_children(node, depth-1)
      ev = self.reduce_fn(node, children)
      node.value = ev.value
      node.depth = ev.depth
      node.size = ev.size
      node = ev
    return node

  def _search_children(self, node, depth):
    state = node.state
    for trans, child_state in self.expand_fn(state):
      child = self.get_node(child_state,
          parent = node, trans = trans)
      yield self._search(child, depth)

  def get_node(self, state, parent = None, trans = None):
    try:
      return self.node_tbl[state]
    except:
      node = search_node(
          value = self.value_fn(state),
          depth = 0,
          size = 1,
          state = state,
          parent = parent,
          trans = trans,
          )
      if self.node_tbl is None:
        self.node_tbl = {}
      self.node_tbl[state] = node
      return node

  def clean_tbl(self):
    new_tbl = { self.start_state: self.get_node(self.sgart_state) }
    for node in self.node_tbl.values():
      path = []
      prior = node
      while prior is not None and prior.state not in new_tbl:
        path.append(prior)
        prior = prior.parent
      if prior is not None:
        new_tbl.update(((p.state, p) for p in prior))
    self.node_tbl = new_tbl


