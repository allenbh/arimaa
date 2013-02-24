from setuptools import setup, Extension

setup (
    name = 'GkIMFL Arimaa',
    description = 'Fast arimaa move generator',
    author = 'Allen Hubbe',
    version = '0.1',
    test_suite = 'gkimfl_arimaa_test',
    ext_modules = [
      Extension (
        'gkimfl.arimaa',
        sources = ['gkimfl_arimaa_module.c'])
      ]
    )

