# Copyright 2013 by Allen Hubbe. All Rights Reserved.

'''A priority queue implemented as a heap.'''

from . import heap_alg
from operator import le, ge

__all__ = [ 'heap' ]

def _no_get_pos(item):
  raise LookupError('no get_pos')

_no_set_pos = heap_alg._no_set_pos

def _identity(item):
  return item

class heap(object):

  def __init__(self, items=None, key=None,
      comp=le, get_pos=_no_get_pos, set_pos=_no_set_pos):
    '''Initialize a heap.

    Parameters:
      items - Initial contents of the heap
      key - Key function for determining order (defualt: identity)
      comp - Compare function (default: le for min heap)
      get_pos - Get position of item (default: raise LookupError)
      set_pos - Set position of item (defualt: pass)
    Complexity:
      O(len(items))
    '''

    if get_pos is not _no_get_pos and set_pos is _no_get_pos:
      raise ValueError('get_pos is invalid without set_pos')

    self._comp = comp
    self._get_pos = get_pos

    if key is None:
      self._wrap = _identity
      self._unwrap = _identity
      self._set_pos = set_pos
    else:
      self._wrap = lambda item: (key(item), item)
      self._unwrap = lambda wrapped: wrapped[1]
      if set_pos is _no_set_pos:
        self._set_pos = _no_set_pos
      else:
        self._set_pos = lambda wrapped, pos: set_pos(wrapped[1], pos)

    if items is None:
      self._heap = []
    else:
      self._heap = list(map(self._wrap, items))
      heap_alg.heapify(self._heap,
          comp=self._comp, set_pos=self._set_pos)

  def __len__(self):
    '''Return the number of items in the heap.'''
    return len(self._heap)

  def insert(self, x):
    '''Insert an item into the heap.'''
    pos = len(self._heap)
    self._heap.append(self._wrap(x))
    heap_alg.pull_up(self._heap, pos,
        comp=self._comp, set_pos=self._set_pos)

  def first(self):
    '''Return the first item from the heap.'''
    return self._unwrap(self._heap[0])

  def remove_first(self):
    '''Remove and return the first item from the heap.'''
    x = self._heap[0]
    self._set_pos(x, -1)
    x = self._unwrap(x)
    if len(self._heap) == 1:
      self._heap.pop()
    else:
      self._heap[0] = self._heap.pop()
      heap_alg.push_down(self._heap, 0,
          comp=self._comp, set_pos=self._set_pos)
    return x

  def remove(self, x):
    '''Remove and return the specified item.'''
    pos = self._get_pos(x)
    x = self._heap[pos]
    self._set_pos(x, -1)
    x = self._unwrap(x)
    if len(self._heap) == 1 + pos:
      self._heap.pop()
    else:
      try:
        self._heap[pos] = self._heap.pop()
      except IndexError:
        print(pos)
        raise IndexError
      pos = heap_alg.pull_up(self._heap, pos,
          comp=self._comp, set_pos=self._set_pos)
      pos = heap_alg.push_down(self._heap, pos,
          comp=self._comp, set_pos=self._set_pos)
    return x

  def bump(self, x):
    '''Bump the specified item because it's value may have changed.'''
    pos = self._get_pos(x)
    self._heap[pos] = self._wrap(x)
    pos = heap_alg.pull_up(self._heap, pos,
        comp=self._comp, set_pos=self._set_pos)
    pos = heap_alg.push_down(self._heap, pos,
        comp=self._comp, set_pos=self._set_pos)

