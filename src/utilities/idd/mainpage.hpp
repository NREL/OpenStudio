/** \namespace openstudio
 *
 *  All OpenStudio code is placed in the openstudio namespace. OpenStudio utilities are placed
 *  directly in the openstudio namespace; higher level sub-projects are placed in sub-namespaces
 *  (e.g. openstudio::model). */
namespace openstudio {

/** \mainpage OpenStudio Idd
 *
 * \details OpenStudio provides parsing of and programmatic access to Input Data Dictionary (IDD) files,
 * objects, and fields through the OpenStudio utilities library (in the openstudio namespace). IDD files
 * of central importance to OpenStudio are wrapped into the \link IddFactorySingleton IddFactory \endlink,
 * and used to generate IddFileType, IddObjectType, and field-level enumeration classes (see IddEnums.hpp
 * and IddFieldEnums.hxx).
 *
 * \section quick_links_idd Quick Links
 *
 * Base classes and enums
 *
 * \li IddFile
 * \li IddObject
 * \li IddObjectProperties
 * \li IddField
 * \li IddFieldProperties
 * \li IddFieldType
 * \li IddKey
 * \li IddKeyProperties
 *
 * \link IddFactorySingleton IddFactory\endlink classes and enums
 *
 * \li \link IddFactorySingleton IddFactory\endlink
 * \li IddFileType
 * \li IddObjectType
 * \li IddFieldEnums.hxx
 * \li IddFileAndFactoryWrapper
 *
 * \section background_idd Background
 *
 * OpenStudio consists of middleware and end-user tools for building energy analysis. As such, it
 * needs to be able to model buildings at varying levels of detail, and to communicate with
 * building energy simulation engines.
 *
 * OpenStudio's primary simulation engine target is EnergyPlus, which is freely available and
 * actively developed with support from the U.S. Department of Energy and the California Energy
 * Commission.
 *
 * EnergyPlus operates off of text input files written in its own Input Data File (IDF) format.
 * IDF files are similar to XML files in that they are intended to conform to a data schema
 * written using similar syntax. For XML, the schema format is XSD; for IDF, the schema format
 * is IDD. Although idiosyncratic, IDD and IDF have the advantages of being native to EnergyPlus
 * and more compact than XSD and XML.
 *
 * For each release of EnergyPlus, valid IDF files are defined by the Energy+.idd file shipped
 * with the release. Therefore, by adopting the IDD and IDF data formats directly, OpenStudio can
 * more easily keep up with the EnergyPlus release cycle, support legacy building models
 * written in IDF, and provide detailed modeling capability (by allowing the user lightly filtered
 * access to EnergyPlus IDF objects). However, there is an upfront cost in that there are
 * no third-party tools available for programmatic access to IDF and IDD files, as there are for
 * XML and XSD.
 *
 * This document, along with the documentation for files in openstudiocore/src/utilities/idd/
 * (accessible through links on this page) describes the classes and functions available through
 * OpenStudio for parsing and accessing \link IddFile IddFiles\endlink, and \link IddObject
 * IddObjects\endlink, including the two \link IddFile IddFiles\endlink of central importance to
 * OpenStudio: the current versions of Energy+.idd and OpenStudio.idd, which are best accessed
 * through the \link IddFactorySingleton IddFactory \endlink. (Legacy versions of the
 * OpenStudio.idd should also be loaded from disk using the
 * \link IddFactorySingleton IddFactory\endlink, to avoid duplicate parsing effort.)
 *
 * \section parsing_idd File and Object Parsing
 *
 * Objects of type IddObject and IddFile are typically constructed by parsing IDD text using the
 * methods IddFile::load and IddObject::load. File, object, and field-level slash codes such as
 *
\verbatim
\group
\memo
\min-fields
\autosizable
\endverbatim
 *
 * are captured and made available through methods
 *
 * \code
 * std::string groupName = IddObject::group();
 * \endcode
 *
 * and properties
 *
 * \code
 * std::string objectMemo = IddObject::properties().memo;
 * unsigned minNumFields = IddObject::properties().minFields; // returns 0 if no fields required
 * bool canAutosize = IddField::properties().autosizable;
 * \endcode
 *
 * Individual \link IddObject IddObjects\endlink are uniquely identified by the IddObject::name
 * string, which is the initial token used to identify the object in the IDD file, and to identify
 * IDF objects following that IddObject's schema. \link IddField IddFields\endlink may or may not
 * have a non-empty IddField::name. They are always uniquely identified by their index in
 * IddObject::nonextensibleFields or IddObject::extensibleGroup.
 *
 * In addition to containing objects, IddFiles typically start with a header, at the top of which
 * a version string is specified, prefixed by '!IDD_Version '. This version string should be the
 * default value for the Version Identifier field in the file's version object ('Version' in the
 * EnergyPlus IDD, 'OS:Version' in the OpenStudio IDD). Version information is generally available
 * through the IddFile, \link IddFactorySingleton IddFactory \endlink, IddFileAndFactoryWrapper,
 * IdfFile, and Workspace classes.
 *
 * \section usage_idd Programmatic Access
 *
 * After parsing an IddFile, file-level information such as
 *
 * \code
 * std::string fileHeader = IddFile::header();
 * std::string fileVersion = IddFile::version();
 * \endcode
 *
 * can be accessed, as can its objects. Individual objects can be retrieved by name. Vectors of
 * all objects, or of objects satisfying certain cardinality requirements can also be retrieved.
 *
 * \code
 * boost::optional<IddObject> optionalBuildingIdd = IddFile::getObject("Building");
 * if (optionalBuildingIdd) {
 *   std::cout << "Text of retrieved IddObject named '" << optionalBuildingIdd->name() << "':" << '\n';
 *   std::cout << optionalBuildingIdd.get();
 * }
 * std::vector<IddObject> allIddObjects = IddFile::objects();
 * std::vector<IddObject> onlyTheRequiredIddObjects = IddFile::requiredObjects();
 * \endcode
 *
 * \link IddFile IddFiles\endlink can be saved to file using the IddFile::save method. Note that
 * the default behavior of this (and other OpenStudio save methods) is to not overwrite an existing
 * file. If you want to overwrite the file in the case that it already exists, you will need to pass
 * true in for the overwrite argument. Also, to maintain cross-platform compatibility,
 * you should explicitly construct the openstudio::path argument either directly or with one of the
 * toPath methods in utilities/core/Path.hpp. IDD text can be directed to other destinations using
 * std::ostream, IddFile::print, IddObject::print, and operator<<.
 *
 * \link IddObject IddObjects\endlink provide access to object level properties, and to fields.
 *
 * \code
 * IddObjectProperties objectProperties = IddObject::properties();
 * std::vector<IddField> fieldsBeforeExtensibleMarker = IddObject::nonextensibleFields();
 * std::vector<IddField> fieldsAtAndAfterExtensibleMarker = IddObject::extensibleGroup();
 * \endcode
 *
 * \link IddObject IddObjects\endlink, and the IdfObjects that instantiate them, start with a list
 * of nonextensible fields, that is, with basic data fields that are not repeated. Optionally, the
 * nonextensible fields may be followed by an extensible group, that is, a group of fields that
 * may be repeated indefinitely. For instance, several EnergyPlus objects have an extensible group
 * that defines a vertex. The corresponding IddObject only needs to define one vertex. The
 * IdfObject instantiation can then define multiple vertices, each of which refers to the same
 * IddObject::extensibleGroup.
 *
 * IddObject provides direct support for indexing fields in extensible groups. Any IdfObject field
 * that is in such a group can be accessed with either its overall field index, or with an
 * ExtensibleIndex, which specifies the index of the ExtensibleGroup (ExtensibleIndex::group), and
 * the index of the field within that group (ExtensibleIndex::field). IddObject converts between
 * the two representations using IddObject::numFields and IddObjectProperties::numExtensible.
 * All field and group indices use the C/C++ convention of labeling the first element with index 0.
 *
 * \section factory_idd Energy+.idd and OpenStudio.idd
 *
 * Two particular IddFiles, the current Energy+.idd and OpenStudio.idd, are central to OpenStudio,
 * and so the utilities/idd sub-project provides convenience functionality to support their
 * consistent use throughout the rest of the project via the \link IddFactorySingleton IddFactory
 * \endlink.
 *
 * The IddFactory is generated from ProposedEnergy+.idd (contains the same information as
 * Energy+.idd, but has slightly modified slash codes, and only one copy of each extensible group.)
 * and OpenStudio.idd.
 *
 * All IDD objects in these two files are listed in the IddObjectType enumeration; and all fields
 * of all of these objects are accessible through the enumerations indirectly listed in
 * IddFieldEnums.hxx. (In most integrated developer environments (IDEs) you can right-click on the
 * file names listed there to open and subsequently inspect the individual enumerations.) The
 * field enumerations are split into Fields and ExtensibleFields, as in:
 * \code
 * BuildingSurface_DetailedFields surfaceTypeField("surface type");
 * BuildingSurface_DetailedExtensibleFields yCoordinateField(
 *     BuildingSurface_DetailedExtensibleFields::VertexYcoordinate);
 * \endcode
 * IddFieldEnums.hxx should never be included directly; please use it as a table of contents to find
 * the specific files that should be \#included in your source.
 *
 * The \link IddFactorySingleton IddFactory\endlink API and IddFile API are purposefully similar, as
 * openstudio::IdfFile and openstudio::Workspace can be used with either an explicit IddFile, or with
 * an implicit IDD that is specified by filtering the IddFactory with an IddFileType (typically
 * IddFileType::EnergyPlus or IddFileType::OpenStudio). Instead of each of those classes handling the
 * possibility of using a custom IDD or an IddFactory IDD on their own, this interchangeability is
 * embodied in the IddFileAndFactoryWrapper, which explicitly provides a common API to these two
 * in-memory instantiations of IDD.
 *
 * The \link IddFactorySingleton IddFactory\endlink also provides access to older versions of the
 * OpenStudio.idd. In addition to the convenience of the \link IddFactorySingleton::getIddFile
 * IddFactory::instance().getIddFile \endlink syntax, accessing the legacy files in this way ensures
 * that each file is only parsed once, which is necessary for some applications (for
 * instance, the openstudio::osversion::VersionTranslator) to have acceptable performance.
 */

}  // namespace openstudio
