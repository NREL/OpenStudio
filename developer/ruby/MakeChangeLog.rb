https://github.com/NREL/EnergyPlus/blob/develop/doc/tools/create_changelog.py

You have to pass these args:
 - Path to the local clone of the repository
 - Path to the output markdown file (that goes up on GitHub)
 - Path to the output HTML file (that goes out with release for users machines)
 - Path to the local git executable (because of trouble Jason had with Cmake, git, path, etc.)
 - A Github token to do authenticated API hits because at one point we were getting API limit failures for anonymous queries
 - The commit SHA for the last major release

We call it here: https://github.com/NREL/EnergyPlus/blob/develop/cmake/Install.cmake#L57
