# Copyright 2013 by Allen Hubbe. All Rights Reserved.

'''Heap algorithims for sequences'''

from operator import le, ge

__all__ = [ 'is_heap', 'pull_up', 'push_down', 'heapify' ]

def _no_set_pos(item, pos):
  pass

def is_heap(heap, comp=le):
  for i in range(1, len(heap)):
    if not comp(heap[(i-1)>>1], heap[i]):
      return False
  return True

def pull_up(heap, pos, comp=le, set_pos=_no_set_pos):
  '''Restore the heap invariant by pulling toward the root.

  Parameters:
    heap - a sequence that conforms to the heap invariant except at pos
    pos - index where the heap invariant needs to be restored
    comp - compare function (default: le for min heap)
    set_pos - record item position (default: pass)
  Returns:
    out_pos - final index of the element originally at pos
  '''
  x = heap[pos]
  while pos > 0:
    ppos = (pos - 1) >> 1
    if comp(heap[ppos], x):
      break
    heap[pos] = heap[ppos]
    set_pos(heap[pos], pos)
    pos = ppos
  heap[pos] = x
  set_pos(x, pos)
  return pos

def push_down(heap, pos, comp=le, set_pos=_no_set_pos):
  '''Restore the heap invariant by pushing away from the root.

  Parameters:
    heap - a sequence that conforms to the heap invariant except at pos
    pos - index where the heap invariant needs to be restored
    comp - compare function (default: le for min heap)
    set_pos - record item position (default: pass)
  Returns:
    out_pos - final index of the element originally at pos
  '''
  x = heap[pos]
  cpos = (pos << 1) + 1
  while cpos < len(heap):
    if cpos + 1 < len(heap):
      if comp(heap[cpos+1], heap[cpos]):
        cpos = cpos + 1
    if comp(x, heap[cpos]):
      break
    heap[pos] = heap[cpos]
    set_pos(heap[pos], pos)
    pos = cpos
    cpos = (pos << 1) + 1
  heap[pos] = x
  set_pos(x, pos)
  return pos

def heapify(heap, comp=le, set_pos=_no_set_pos):
  '''Establish the heap invariant in a sequence.
  
  Parameters:
    heap - a sequence
    **kwds - see push_down
  '''
  if len(heap) < 2:
    return
  pos = (len(heap) - 2) >> 1
  if set_pos is not _no_set_pos:
    pos = len(heap) - 1 # no shortcut: all elements need pos
  while pos >= 0:
    push_down(heap, pos, comp, set_pos)
    pos -= 1

