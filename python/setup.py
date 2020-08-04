from setuptools import setup, find_packages

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
)
