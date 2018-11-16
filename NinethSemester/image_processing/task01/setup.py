#!/bin/env python
# -*- coding: utf-8 -*-

"""Task 01."""

from setuptools import setup, find_packages

PACKAGE_NAME = "lutovtask01"

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
      install_requires=['numpy>=1.12.1',
                        'scikit-image>=0.13',
                       ],
      packages=find_packages(),
      include_package_data=True,
      entry_points={'console_scripts': ['lutovtask01 = lutovtask01.__main__:main']})
