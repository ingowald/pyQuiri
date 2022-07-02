from setuptools import setup, dist
import wheel 
import os
# required to geneerate a platlib folder required by audittools
from setuptools.command.install import install
# for generating a wheel version from git tag
from setuptools_scm import get_version

class InstallPlatlib(install):
    def finalize_options(self):
        install.finalize_options(self)
        if self.distribution.has_ext_modules():
            self.install_lib = self.install_platlib

# force setuptools to recognize that this is
# actually a binary distribution
class BinaryDistribution(dist.Distribution):
    def is_pure(self):
        return False
    def has_ext_modules(foo):
        return True

# This gets the version from the most recent git tag, potentially concatinating 
# a commit hash at the end.
# current_version = get_version(
#    root = "..", 
#    relative_to = __file__,
#    fallback_version='0.0.0-dev0'
#)
# print(current_version)

__version__ = "0.1.2"

setup(
    # This package is called pyQuiri
    name='pyQuiri',

    #install_requires = ['numpy>=1.19.5'],
    install_requires = [],

    #packages = ['pyQuiri'],
    packages = ['build/install/'],

    # make sure the shared library is included
    package_data = {'': ("*.dll", "*.pyd", "*.so")},
    include_package_data=True,

    description='',

    # See class BinaryDistribution that was defined earlier
    distclass=BinaryDistribution,

    # version = current_version,
    version = __version__,

    author='Ingo Wald',
    author_email='',
    maintainer='',
    maintainer_email='',
    
    python_requires = ">=3.6",
    cmdclass={'install': InstallPlatlib},
)
