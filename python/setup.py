from setuptools import setup, find_packages
from setuptools.dist import Distribution
from setuptools.command.install import install

try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel

    class bdist_wheel(_bdist_wheel):

        def finalize_options(self):
            _bdist_wheel.finalize_options(self)
            # Mark us as not a pure python package, so that the wheel is marked
            # with the platform name
            self.root_is_pure = False

        def get_tag(self):
            # Setting it up to build generic wheels.
            python, abi, plat = _bdist_wheel.get_tag(self)
            # There is no ABI incompatibility
            python, abi = 'py3', 'none'
            # Our bindings won't be compatible with all, so don't bother
            # plat = plat.lower().replace('linux', 'manylinux1')
            # plat = plat.lower().replace('darwin_x86_64', 'macosx_10_6_intel')
            return python, abi, plat
except ImportError:
    bdist_wheel = None


class BinaryDistribution(Distribution):
    def is_pure(self):
        return False

    def has_ext_modules(self):
        return True


class InstallPlatlib(install):
    def finalize_options(self):
        install.finalize_options(self)
        self.install_lib = self.install_platlib


setup(
    name='openstudio',
    version='@OPENSTUDIO_LONG_VERSION@',
    description='OpenStudio python bindings.',
    long_description="""OpenStudio python bindings.
    """,
    url='https://github.com/NREL/OpenStudio',
    author='Julien Marrec',
    author_email='julien@effibem.com',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Science/Research',
        'Topic :: Scientific/Engineering',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ],
    keywords='openstudio py3 setuptools development',
    packages=find_packages(include=['openstudio']),
    package_data={"openstudio": ["*.so"]},
    install_requires=[
    ],
    cmdclass={
        'bdist_wheel': bdist_wheel,
        'install': InstallPlatlib
    },
    distclass=BinaryDistribution,
)
