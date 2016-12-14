function geographiclibinterface(incdir, libdir)
%GEOGRAPHICLIBINTERFACE  Compile interface code (DEPRECATED)
%
%   GEOGRAPHICLIBINTERFACE
%   GEOGRAPHICLIBINTERFACE(installdir)
%   GEOGRAPHICLIBINTERFACE(incdir, libdir)
%
%   This function is DEPRECATED because native Matlab implementations are
%   available for all the functions.  It and the associated C++ wrapper
%   code will be removed at some point in 2016.
%
%   With one argument GEOGRAPHICLIBINTERFACE looks for the compiled library
%   in installdir/lib and the include files in installdir/include.
%
%   With no arguments, installdir is taked to be '/usr/local', on Unix and
%   Linux systems, and 'C:/Program Files/GeographicLib', on Windows systems
%
%   With two arguments, the library is looked for in libdir and the include
%   files in incdir.
%
%   This has been tested with
%
%     Octave 3.2.3 and g++ 4.4.4 under Linux
%     Octave 3.6.4 and g++ 4.8.3 under Linux
%     Matlab 2007a and Visual Studio 2005 under Windows
%     Matlab 2008a and Visual Studio 2005 under Windows
%     Matlab 2008a and Visual Studio 2008 under Windows (*)
%     Matlab 2010b and Visual Studio 2005 under Windows
%     Matlab 2010b and Visual Studio 2008 under Windows (*)
%     Matlab 2010b and Visual Studio 2010 under Windows
%     Matlab 2013b and Visual Studio 2012 under Windows
%     Matlab 2014b and Mac OSX 10.10 (Yosemite)
%
%   Run 'mex -setup' to configure the C++ compiler for Matlab to use.

% Copyright (c) Charles Karney (2010-2014) <charles@karney.com> and
% licensed under the MIT/X11 License.  For more information, see
% http://geographiclib.sf.net/html/other.html#matlab

  funs = {'geodesicdirect', 'geodesicinverse', 'geodesicline', ...
          'geoidheight', ...
          'utmupsforward', 'utmupsreverse', ...
          'mgrsforward', 'mgrsreverse', ...
          'geocentricforward', 'geocentricreverse', ...
          'localcartesianforward', 'localcartesianreverse', ...
          'polygonarea'};
  lib='Geographic';
  if (ispc && ...
      strcmp(mex.getCompilerConfigurations().Manufacturer, 'Microsoft') && ...
      strcmp(mex.getCompilerConfigurations().Version, '9.0'))
    warning(['geoidheight compiled with Visual Studio 2008 ',...
             'causes Matlab to CRASH']);
  end
  if (nargin < 2)
    if (nargin == 0)
      if ispc
        installdir = 'C:/Program Files/GeographicLib';
      else
        installdir = '/usr/local';
      end
    else
      installdir = incdir;
    end
    incdir=[installdir '/include'];
    libdir=[installdir '/lib'];
  end
  testheader = [incdir '/GeographicLib/Constants.hpp'];
  if (~ exist(testheader, 'file'))
    error(['Cannot find ' testheader]);
  end
  fprintf('Compiling Matlab interface to GeographicLib\n');
  fprintf('Include directory: %s\nLibrary directory: %s\n', incdir, libdir);
  for i=1:size(funs,2)
    fprintf('Compiling %s...', funs{i});
    if ispc || ismac
      mex( ['-I' incdir], ['-L' libdir], ['-l' lib], [funs{i} '.cpp'] );
    else
      mex( ['-I' incdir], ['-L' libdir], ['-l' lib], ['-Wl,-rpath=' libdir], ...
           [funs{i} '.cpp'] );
    end
    fprintf(' done.\n');
  end
end
