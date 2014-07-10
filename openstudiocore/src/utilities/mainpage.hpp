/** \namespace openstudio
 *  
 *  All OpenStudio code is placed in the openstudio namespace. OpenStudio utilities are placed 
 *  directly in the openstudio namespace; higher level sub-projects are placed in sub-namespaces 
 *  (e.g. openstudio::model). */
namespace openstudio {

/** \mainpage OpenStudio Utilities
 *
 *  The utilities library provides common functionality used throughout OpenStudio.
 *  OpenStudio's utilities live in the top level OpenStudio namespace, but are broken down into 
 *  sub-components. Programmers (C++, Ruby and C#) new to OpenStudio should start by looking at
 *  the items in \ref core_sec, \ref idd_sec, and \ref idf_sec. \ref bcl_sec, \ref data_sec, 
 *  \ref filetypes_sec, \ref sql_sec, \ref time_sec, and \ref units_sec are also quite 
 *  fundamental. For automatically generating tables and reports, see \ref document_sec. The 
 *  code in \ref plot_sec is to be deprecated in favor of plots generated with the JavaScript
 *  charting library D3.
 *
 *  The C++ standard template library (STL) and some boost libraries are used throughout 
 *  OpenStudio. Please familiarize yourself with std::vector, std::set, and iterators in the STL 
 *  library (<a href="http://www.cplusplus.com/reference/stl/">a good online reference</a>), and 
 *  boost::optional (<a href="http://www.boost.org/doc/libs/1_49_0/libs/optional/doc/html/index.html">
 *  library documentation</a>).
 *
 *  Moving on to the rest of the OpenStudio libraries, consider browsing the documentation for 
 *  the model, energyplus, and runmanager subprojects. Once you have that down, the ruleset, 
 *  analysis, project, and analysisdriver libraries can be used to construct and run multi-model 
 *  analyses. 
 *
 *  Ruby programmers will likely want to move back and forth between the C++ and Ruby 
 *  documentation. Our handwritten doxygen documentation is only available for C++; the Ruby 
 *  documentation should mainly be treated as a syntax reference. 
 *
 *  \section bcl_sec BCL
 *
 *  Classes for pushing and pulling \link BCLComponent BCLComponents\endlink and \link BCLMeasure 
 *  BCLMeasures\endlink from the \link RemoteBCL online\endlink Building Component Library (BCL) 
 *  and a \link LocalBCL local\endlink BCL. Includes support for \link OnDemandGenerator 
 *  OnDemandGenerators\endlink. For using components in OpenStudio format (.osc), please see 
 *  model::Component and osversion::VersionTranslator. Infrastructure for using measures is 
 *  available in the ruleset (ruleset::UserScript, ruleset::OSArgument, ruleset::OSRunner, 
 *  ruleset::RubyUserScriptArgumentGetter), runmanager (runmanager::RubyJobBuilder), and analysis
 *  (analysis::RubyPerturbation) sub-projects.
 *
 *  \section core_sec Core
 *
 *  Functionality and typedefs used throughout the OpenStudio project. Please see the associated
 *  header files:
 *
 *  \li Used throughout: Assert.hpp, Compare.hpp, Containers.hpp, Enum.hpp, FileLogSink.hpp, 
 *      Logger.hpp, LogMessage.hpp, Optional.hpp, Path.hpp, PathHelpers.hpp, String.hpp, UUID.hpp
 *  \li Used more sparingly: Application.hpp, ApplicationPathHelpers.hpp, Checksum.hpp, 
 *      CommandLine.hpp, EnumHelpers.hpp, Exception.hpp, FileReference.hpp, Finder.hpp, 
 *      PathWatcher.hpp, Queue.hpp, Singleton.hpp, StaticInitializer.hpp, 
 *      StringHelpers.hpp, StringStreamLogSink.hpp, System.hpp, UnzipFile.hpp, UpdateManager.hpp, 
 *      URLHelpers.hpp, ZipFile.hpp
 *
 *  \section data_sec Data
 *
 *  Structures for storing data such as \link Vector.hpp vectors\endlink, 
 *  \link Matrix.hpp matrices\endlink, and \link TimeSeries time series\endlink. The Attribute
 *  class is used widely throughout OpenStudio. EndUses, Tag, and the enumeration classes in 
 *  DataEnums.hpp are also of interest.
 *
 *  \section document_sec Document
 *
 *  Generic Document and supporting classes. \link Document Documents\endlink and \link 
 *  DocumentElement DocumentElements\endlink can be output in HTML and LaTeX format. Tables
 *  import from and export to csv; export to JSON; store data in the appropriate boolean, numeric
 *  , or string format; and provides cross-document platform printing and formatting functionality.
 *  \link Document Documents\endlink are essentially vectors of \link SectionHeading
 *  SectionHeadings\endlink, Text, and \link Table Tables\endlink.
 *
 *  \section filetypes_sec Filetypes
 *
 *  Classes representing file types commonly used in building energy simulation. 
 *
 *  \li EpwFile
 *  \li TimeDependentValuationFile
 *
 *  \section geometry_sec Geometry
 *
 *  Classes related to geometry, transformations, intersection tests, etc, see especially
 *  Geometry.hpp, Point3d, Vector3d, and Transformation.
 *
 *  \section idd_sec Idd
 *
 *  Classes related to the Input Data Dictionary (IDD) format used by EnergyPlus and OpenStudio.
 *  The documentation for this sub-project is available on a separate 
 *  <a href="../../utilities_idd/html/index.html">page</a>.
 *
 *  \section idf_sec Idf
 *
 *  Classes related to the Input Data File (IDF) format used by EnergyPlus and OpenStudio. See 
 *  \ref idf_page for detailed information.
 *
 *  \section math_sec Math
 *
 *  See FloatCompare.hpp, Permutation.hpp, and Primes.hpp.
 *
 *  \section plot_sec Plot
 *
 *  Methods used in ResultsViewer for plotting data. Long-term plan is to deprecate this
 *  in favor of D3.
 *
 *  \section sql_sec Sql
 *
 *  SqlFile provides methods for querying EnergyPlus SQLite databases.
 *
 *  \section time_sec Time
 *
 *  Time, Date, DateTime, and Calendar utilities.
 *
 *  \section units_sec Units
 *
 *  A runtime library for constructing, performing mathematical operations with, and
 *  converting quantities between different physical units at runtime (modeled after 
 *  the boost compiletime library).
 *
 *  \li Unit
 *  \li Quantity
 *  \li \link UnitFactorySingleton UnitFactory \endlink - Singleton and related non-member 
 *          functions for creating \link Unit Units\endlink from strings.
 *  \li \link QuantityFactorySingleton QuantityFactory \endlink - Singleton and related 
 *          non-member functions for creating \link Quantity Quantities\endlink from strings.
 *  \li \link QuantityConverterSingleton QuantityConverter \endlink - Singleton and related 
 *          non-member functions for converting \link Quantity Quantities\endlink and 
 *          \link OSQuantityVector OSQuantityVectors \endlink to different units.
 *  \li OSOptionalQuantity - Class for always returning a Unit and optionally 
 *                           returning a Quantity.
 *  \li OSQuantityVector - Class for performing mathematical operations and unit 
 *                         conversions on a vector of values all sharing the same units. 
 */

} // openstudio
