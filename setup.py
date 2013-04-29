try: from setuptools import setup, Extension
except ImportError:
  try: from distutils2.core import setup, Extension
  except ImportError:
    from distutils.core import setup, Extension


setup (
    name = 'GkIMFL Arimaa',
    description = 'Fast arimaa move generator',
    author = 'Allen Hubbe',
    version = '0.1',
    test_suite = 'test.gkimfl.test',
    ext_modules = [
      Extension (
        'gkimfl.arimaa_impl',
        sources = [
          'src/gkimfl_arimaa.cpp',
          'src/gkimfl_arimaa_hash.cpp',
          'src/gkimfl_arimaa_module.cpp',
          'src/gkimfl_arimaa_wrappers.cpp',
          ],
        include_dirs = [ 'src' ],
        libraries = [ 'boost_python' ],
        )
      ]
    )

