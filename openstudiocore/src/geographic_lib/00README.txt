A library for geographic projections.

Written by Charles Karney <charles@karney.com> and licensed under
the MIT/X11 License.  For more information, see

    http://geographiclib.sourceforge.net/

Files

    00README.txt  -- this file
    AUTHORS -- the authors of the library
    LICENSE.txt -- the MIT/X11 License
    INSTALL -- brief installation instructions
    NEWS -- a history of changes

    include/GeographicLib/ and src/
      Config.h.in, Config.h -- system dependent configuration
      Constants.hpp -- WGS84 constants
      Math.[ch]pp -- math routines
      Utility.[ch]pp -- I/O and date routines
      Accumulator.[ch]pp -- quad precision adder
      PolarStereographic.[ch]pp -- polar stereographic projection
      TransverseMercator.[ch]pp -- transverse Mercator projection
      UTMUPS.[ch]pp -- UTM and UPS
      MGRS.[ch]pp -- MGRS
      TransverseMercatorExact.[ch]pp -- exact TM projection
      EllipticFunction.[ch]pp -- elliptic functions
      GeoCoords.[ch]pp -- hold geographic location
      DMS.[ch]pp -- handle degrees minutes seconds
      Geocentric.[ch]pp -- geocentric coordinates
      LocalCartesian.[ch]pp -- local cartesian coordinates
      Geodesic.[ch]pp -- geodesic calculations
      GeodesicLine.[ch]pp -- calculations on a single geodesic
      PolygonArea.[ch]pp -- polygon area
      AzimuthalEquidistant.[ch]pp -- azimuthal equidistant projection
      Gnomonic.[ch]pp -- gnomonic projection
      CassiniSoldner.[ch]pp -- Cassini-Soldner equidistant projection
      Geoid.[ch]pp -- geoid heights above the ellipsoid
      Gravity{Model,Circle}.[ch]pp -- gravity models
      Magnetic{Model,Circle}.[ch]pp -- geomagentic models
      {Spherical,Circular}Engine.[ch]pp -- spherical harmonic sums
      SphericalHarmonic{,1,2}.hpp -- frontend for spherical harmonics
      LambertConformalConic.[ch]pp -- Lambert conformal conic projection
      AlbersEqualArea.[ch]pp -- Albers equal area projection
      Gnomonic.[ch]pp -- Ellipsoidal gnomonic projection
      OSGB.[ch]pp -- Ordnance Survey grid system
      Geohash.[ch]pp -- conversions for geohashes
      GARS.[ch]pp -- conversions for GARS
      Georef.[ch]pp -- conversions for georefs
      Ellipsoid.[ch]pp -- ellipsoid properties
      Rhumb.[ch]pp -- rhumb line calculations

    examples/
      example-*.cpp -- simple usage examples for all the classes
      GeoidToGTX.cpp -- a parallelization example
      JacobiConformal.[ch]pp -- mapping a triaxial ellipsoid

    tools/
      GeoConvert.cpp -- geographic conversion utility
      TransverseMercatorTest.cpp -- TM tester
      GeodSolve.cpp -- geodesic utility
      CartConvert.cpp -- convert to geocentric and local cartesian
      EquidistantTest.cpp -- exercise AzimuthalEquidistant and CassiniSoldner
      GeoidEval.cpp -- evaluate geoid heights
      Gravity.cpp -- evaluate gravity
      MagneticField.cpp -- evaluate magnetic field
      Planimeter.cpp -- computer polygon areas
      RhumbSolve.cpp -- calculate rhumb lines
      geographiclib-get-geoids -- download geoid datasets
      geographiclib-get-gravity -- download gravity models
      geographiclib-get-magnetic -- download geomagnetic models

    windows/
      GeographicLib-vc9.sln -- MS Studio 2008 solution
      Geographic-vc9.vcproj -- project for library
      GeoConvert-vc9.vcproj -- project for GeoConvert
      TransverseMercatorTest-vc9.vcproj -- project for TransverseMercatorTest
      Geod-vc9.vcproj -- project for Geod
      Planimeter-vc9.vcproj -- project for Planimeter
      CartConvert-vc9.vcproj -- project for CartConvert
      EquidistantTest-vc9.vcproj -- project for EquidistantTest
      GeoidEval-vc9.vcproj -- project for GeoidEval
      Gravity-vc9.vcproj -- project for Gravity
      MagneticField-vc9.vcproj -- project for MagneticField
      also files for MS Studio 2010 (with vc10)
      NETGeographic-vc10.vcxproj -- project for .NET wrapper

    maxima/
      tm.mac -- Maxima code for high precision TM
      ellint.mac -- Maxima code for elliptic functions needed by tm.mac
      tmseries.mac -- Maxima code for series approximations for TM
      geod.mac -- Maxima code for series approximations for Geodesic
      geodesic.mac -- Maxima code for geodesic problems

    matlab/geographiclib
      *.m, private/*.m -- Matlab implementation of some classes

    matlab/geographiclib-legacy
      *.{m,cpp} -- legacy Matlab routines

    doc/
      doxyfile.in -- Doxygen config file
      GeographicLib.dox -- main page of Doxygen documentation
      geodseries30.html -- geodesic series to 30th order
      tmseries30.html -- transverse Mercator series to 30th order
      html/* -- directory with built documentation
      scripts/*.html -- demonstrations of the JavaScript interface
      scripts/GeographicLib/*.js -- JavaScript implementation of geodesics

    man/
      *.pod -- plain old documentation
      *.1 -- man pages in nroff format
      *.1.html -- man pages in html format
      *.usage -- documentation for incorporation into executables

    python/GeographicLib/*.py -- Python implementation of geodesic routines

    java/.../*.java -- Java implementation of geodesic routines

    dotnet/NETGeographicLib/*.{cpp,h} -- .NET wrapper for GeographicLib
    dotnet/examples/CS/*.cs -- simple C# examples for each class
    dotnet/examples/ManagedCPP/*.cpp -- Managed C++ examples for each class
    dotnet/examples/VB/*.vb -- simple Visual Basic examples for each class
    dotnet/Projection/* -- a more complex C# application

    legacy/C/* -- C implementation of geodesic routines
    legacy/Fortran/* -- Fortran implementation of geodesic routines

    Makefile.mk -- Unix/Linux makefiles
    configure -- autoconf configuration script
    CMakeLists.txt -- cmake configuration files
    cmake/
      FindGeographicLib.cmake -- cmake find script
      *.cmake.in -- cmake config templates
