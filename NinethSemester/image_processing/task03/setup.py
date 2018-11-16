#!/bin/env python
# -*- coding: utf-8 -*-

"""Task 03."""

from setuptools import setup, Extension, find_packages

PACKAGE_NAME = "lutovtask03"

with open("{package_name}/__init__.py".format(package_name=PACKAGE_NAME), "r") as fin:
    VERSION = [line.split('"')[1] for line in fin if line.startswith("__version__ = ")][0]

setup(name=PACKAGE_NAME,
      version=VERSION,
      description=__doc__,
      maintainer='vslutov',
      maintainer_email='vladimir.lutov@graphics.cs.msu.ru',
      # url='',
      license='WTFPL',
      platforms=['any'],
      classifiers=["Development Status :: 2 - Pre-Alpha",
                   "Environment :: Console"],
      install_requires=['numpy>=1.13.3',
                        'scikit-image>=0.13.1',
                       ],
      setup_requires=['cython>=0.27.3',
                      'setuptools>=36.7.2',
                     ],
      packages=find_packages(),
      include_package_data=True,
      entry_points={'console_scripts': ['{package_name} = lutovtask03.__main__:main'.format(package_name=PACKAGE_NAME)]},
      ext_modules = [
          # Extension(
          #     '{package_name}.hysteresis'.format(package_name=PACKAGE_NAME),
          #     sources=['lutovtask02/hysteresis.py'],
          # ),
      ]
     )
