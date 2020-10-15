from setuptools import setup, find_packages
from setuptools.dist import Distribution
from setuptools.command.install import install
import platform

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
            print("Original: (python, abi, plat) = ({}, {}, {})".format(python, abi, plat))
            if platform.system() != 'Windows':
                # There is no ABI incompatibility on Unix
                # On windows, there is... since we need to link to Python37.dll for eg

                # Edit: I later discovered that building on python3.9 for eg will
                # not be compatible with installing on 3.8.6:
                #    ImportError: /home/julien/Virtualenvs/py38/lib/python3.8/site-packages/openstudio/_openstudioairflow.so: undefined symbol: PyCMethod_New
                # Commit that added it, which was in 3.9.0 but not in 3.8.x (3.8.6 included): https://github.com/python/cpython/commit/e1becf46b4e3ba6d7d32ebf4bbd3e0804766a423
                # I think there is a chance that building on 3.8 will work for install on 3.9 though
                python = 'py3'
                if abi != 'cp39':
                    abi = 'none'

            # Our bindings won't be compatible with all distributions,
            # BUT pypi will refuse the upload if we do not replace
            # Binary wheel 'openstudio-3.1.0rc3-py3-none-linux_x86_64.whl' has an unsupported platform tag 'linux_x86_64'
            plat = plat.lower().replace('linux', 'manylinux1')
            plat = plat.lower().replace('darwin_x86_64', 'macosx_10_6_intel')
            if plat[:3] == 'mac':
                # We don't use a fat binary ('intel' = both i386 and x86_64)
                # but we set the platform to old one in the hope that it'll
                # work for all
                plat = 'macosx_10_9_x86_64'
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


dev_release = ""
# TODO: TEMP
dev_release = ".dev1"

setup(
    name='openstudio',
    # eg 3.1.0rc3
    version='@OpenStudio_VERSION@'.replace('-', '') + dev_release,
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
    package_data={
        "openstudio": [
            "*.so",
            "*.pyd", "*.dll",
            "*.dylib", "*.bundle",
            # "*.lib", "*.exp",
        ],
    },
    install_requires=[],
    cmdclass={
        'bdist_wheel': bdist_wheel,
        'install': InstallPlatlib
    },
    distclass=BinaryDistribution,
)
