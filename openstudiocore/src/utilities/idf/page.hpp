namespace openstudio {
/** \page idf_page OpenStudio Idf
 *
 * \details OpenStudio provides parsing of and programmatic access to Input Data File (IDF) files and
 * objects through the openstudio_utilities library. A second layer of abstraction is provided to
 * programmatically handle IDF "pointers", that is, string references to other objects. IDF objects
 * and collections can be validated against their <a href="../../utilities_idd/html/index.html">IDD</a>.
 *
 * \section quick_links_idf Quick Links
 *
 * Idf and Imf (parsing and printing)
 *
 * \li IdfFile
 * \li ImfFile
 * \li IdfObject
 * \li IdfExtensibleGroup
 *
 * Workspace (editing and sorting)
 *
 * \li Workspace
 * \li WorkspaceObject
 * \li WorkspaceObjectOrder
 * \li WorkspaceExtensibleGroup
 *
 * Consistency with IDD
 *
 * \li ValidityReport
 * \li DataError
 * \li IdfFile::isValid
 * \li IdfFile::validityReport
 * \li Workspace::isValid
 * \li Workspace::validityReport
 *
 * \section background_idf Background
 *
 * OpenStudio consists of middleware and end-user tools for building energy analysis. As such, it
 * needs to be able to model buildings at varying levels of detail, and to communicate with
 * building energy simulation engines.
 *
 * OpenStudio's primary simulation engine target is EnergyPlus, which is freely available and
 * actively developed with support from the U.S. Department of Energy.
 *
 * EnergyPlus operates off of text input files written in its own Input Data File (IDF) format.
 * IDF files are similar to XML files in that they are intended to conform to a data schema
 * written using similar syntax. For XML, the schema format is XSD; for IDF, the schema format
 * is IDD. Although idiosyncratic, IDD and IDF have the advantage of being more compact than
 * XSD and XML.
 *
 * For each release of EnergyPlus, valid IDF files are defined by the Energy+.idd file shipped
 * with the release. Therefore, by adopting the IDD and IDF data formats directly, OpenStudio can
 * more easily keep up with the EnergyPlus release cycle, support legacy building models
 * written in IDF, and provide detailed modeling capability (by allowing the user lightly filtered
 * direct access to EnergyPlus IDF objects). However, there is an upfront cost in that there are
 * no third-party tools available for programmatic access to IDF and IDD files, as there are for
 * XML and XSD.
 *
 * \section overview_idf Overview
 *
 * This document, along with all the documentation associated with files in src/utilities/idf/
 * (most of which should be accessible through links on this page) describes the classes and
 * functions available through OpenStudio for parsing and accessing \link IdfFile IdfFiles\endlink
 * and \link IdfObject IdfObjects\endlink, including the second abstraction layer, Workspace and
 * WorkspaceObject, that programmatically handles IDF name references.
 *
 * The relationships between Workspace, IdfFile, and IDF files on disk is depicted in
 * \ref IdfFileWorkspaceRelationship_fig. The IDF file class generally handles parsing and printing;
 * major modifications of data should be done using a Workspace object. Once the model is ready to
 * simulate, it needs to go back through the IdfFile class, which can write the model to disk as an
 * IDF file.
 *
 * \anchor IdfFileWorkspaceRelationship_fig
 * \image html IdfFile_Workspace_Relationship.jpg "Relationship Between Workspace, IdfFile, and IDF files"
 * \image latex IdfFile_Workspace_Relationship.jpg "Relationship Between Workspace, IdfFile, and IDF files"
 *
 * Both IdfFile and Workspace contain member functions for helping users modify their data so it
 * complies with the underlying IDD. \link IdfFile IdfFiles\endlink only maintain their data to
 * StrictnessLevel::None; users can set the StrictnessLevel of their \link Workspace
 * Workspaces\endlink. IdfFile does provide a method for generating \link ValidityReport
 * ValidityReports\endlink for higher levels of strictness. Most work should be done in Workspace,
 * whose default StrictnessLevel is Draft. IDF should typically be brought up to
 * StrictnessLevel::Final prior to simulation.
 *
 * \section parsing_idf Parsing and Printing
 *
 * \link IdfFile IdfFiles\endlink, \link ImfFile ImfFiles\endlink, \link IdfObject
 * IdfObjects\endlink, and \link Workspace Workspaces\endlink can all be constructed from text
 * input provided as a path or string. Behind the scenes, IdfFile, ImfFile, and IdfObject parse
 * the inputted text, creating headers, objects, CommentOnly objects, fields, and fieldComments.
 * If no IddFile or IddObject is provided explicitly, IdfObject looks for the necessary
 * IddObjects in the IddFactory. If nothing appropriate is found, the default Catchall
 * IddObject, which preserves the object type name and all fields, but places the data under
 * the Catchall type name and sets IddObjectType::Catchall, is used.
 *
 * \link IdfFile IdfFiles\endlink, \link ImfFile ImfFiles\endlink, and \link IdfObject
 * IdfObjects\endlink can all be printed to std::ostream. IdfFile, ImfFile, and Workspace use
 * this facility to save themselves to an openstudio::path.
 *
 * \section modeling Modeling
 *
 * A Workspace is much like an IdfFile, in that it is essentially a collection of IdfObjects.
 * The primary difference is that Workspace actually holds
 * \link WorkspaceObject WorkspaceObjects\endlink, and together, these two classes handle
 * \\reference and \\object-list fields programmatically. The main difference(s) the user should
 * see between working in a Workspace and manually editing IDF files are:
 *
 * \li You only have to change an object's name once--name references to that object are
 * automatically updated.
 *
 * \code
 * WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
 * zones[0].setName("New Zone Name");
 * \endcode
 *
 * \li Name references can be followed in both directions. For instance, you can get the Zone
 * a Lights object points to, and you can get all Lights objects that point to a given Zone.
 *
 * \code
 * // continued from last snippet ...
 * WorkspaceObjectVector lights = zones[0].getSources(IddObjectType::Lights);
 * OptionalString zoneName = lights.getString(LightsFields::ZoneorZoneListName);
 * assert(*zoneName == "New Zone Name");
 * \endcode
 *
 * \li Objects can be ordered by IddObjectType, or with an explicit, user-specified ordering
 *
 * \code
 * // by default, if Workspace is constructed from an IdfFile, the sorted objects will be in the
 * // same order they were in the IdfFile.
 * WorkspaceObjectVector objectsInCurrentOrder = workspace.objects(true);
 * WorkspaceObjectOrder wsOrder = workspace.order();
 * // the next line of code changes the sorting to order objects by the IddObjectType order.
 * wsOrder.setOrderByIddEnum();
 * WorkspaceObjectVector objectsInNewOrder = workspace.objects(true);
 * assert(objectsInCurrentOrder.size() == objectsInNewOrder.size());
 * assert(objectsInCurrentOrder != objectsInNewOrder);
 * \endcode
 *
 * \li Help finding inconsistencies between your Workspace and its IddFile;
 *
 * \code
 * ValidityReport report = workspace.validityReport(StrictnessLevel::Final);
 * if (report.numErrors() > 0) {
 *   OptionalDataError oError = report.nextError();
 *   while (oError) {
 *     if ((oError->scope() == Scope::Collection) &&
 *         (oError->type() == DataErrorType::NullAndRequired))
 *     {
 *       OptionalIddObjectType oType = oError->objectType();
 *       OS_ASSERT(oType);
 *       OptionalWorkspaceObject oNewObject = workspace.addObject(IdfObject(*oType));
 *       OS_ASSERT(oNewObject);
 *     }
 *     oError = report.nextError();
 *   }
 * }
 * \endcode
 *
 * \li Support for \link IdfExtensibleGroup ExtensibleGroups\endlink
 *
 * \code
 * WorkspaceObject::getExtensibleGroup
 * WorkspaceObject::pushExtensibleGroup
 * IdfExtensibleGroup::setString
 * WorkspaceExtensibleGroup::setPointer
 * \endcode
 *
 * And Workspace and WorkspaceObject provide basic editing functionality:
 *
 * \code
 * WorkspaceObject::getString
 * WorkspaceObject::setString
 * WorkspaceObject::setDouble
 * Workspace::addObjects
 * Workspace::removeObject
 * \endcode
 */
} // openstudio
