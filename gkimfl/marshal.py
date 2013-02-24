
def _default(fun, val):
  def _fun(arg):
    try:
      return fun(arg)
    except:
      return val
  return _fun

def _val_to_sym(syms, mod):
  return _default(syms[:mod].__getitem__, ' ')

def _sym_to_val(syms, mod):
  _vals = { sym: val%mod for val,sym in enumerate(syms) }
  return _default(_vals.__getitem__, None)

_piece_sym = 'RrCcDdHhMmEe'
piece_sym = _val_to_sym(_piece_sym, 12)
sym_piece = _sym_to_val(_piece_sym, 12)

_color_sym = 'gsGSwbWB'
color_sym = _val_to_sym(_color_sym, 2)
sym_color = _sym_to_val(_color_sym, 2)

_row_sym = '87654321'
row_sym = _val_to_sym(_row_sym, 8)
sym_row = _sym_to_val(_row_sym, 8)

_col_sym = 'abcdefghABCDEFGH'
col_sym = _val_to_sym(_col_sym, 8)
sym_col = _sym_to_val(_col_sym, 8)

