from setuptools import setup, Extension

setup (
    name = 'GkIMFL Arimaa',
    description = 'Fast arimaa move generator',
    author = 'Allen Hubbe',
    version = '0.1',
    ext_modules = [
      Extension (
        'gkimfl_arimaa',
        sources = ['gkimfl_arimaa_module.c'])
      ]
    )

