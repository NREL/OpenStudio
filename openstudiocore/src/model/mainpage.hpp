namespace openstudio {
namespace model {

// ETH@20110621 - \ref is BROKEN with GENERATE_TREEVIEW = ALL or YES in Doxyfile.in. \ref generates
// links to main.html#..., but the actual page is index.html. This bug may be Doxygen version-
// specific. I am using Doxygen 1.7.4. So, may be fixed by using previous version of Doxygen,
// or by find-replacing main.html/index.html at the end of the _doc targets (CMake hacky-fix).
// Is also fixed by GENERATE_TREEVIEW to any other option, but then we lose the Class Hierarchy :(.

// ETH@20110622 - This bug is documented at https://bugzilla.gnome.org/show_bug.cgi?id=642289. The
// suggested workaround doesn't seem to work.

/** \mainpage OpenStudio Model
 *
 *  \section overview_sec Overview
 *  
 *  An OpenStudio Model (\ref introduction_sec) represents a single building energy model, 
 *  either complete (simulatable with EnergyPlus) or partial. It is composed of a collection of 
 *  \link ModelObject ModelObjects \endlink (\ref modelobject), which are polymorphic, that is,
 *  ModelObject is the base class from which more specific object types are derived. Each leaf of 
 *  the inheritance tree wraps a specific data object type that is ultimately defined by the 
 *  OpenStudio.idd file. The following classes form the foundation of the OpenStudio Model:
 *  
 *  \li Model
 *  \li Component
 *  \li ModelObject
 *  \li ParentObject
 *  \li ResourceObject
 *
 *  The specific model object types can largely be broken down into the categories of simulation 
 *  settings; output data; resources; site and location; geometry; building loads; advanced daylighting; 
 *  heating, ventilation and air conditioning (HVAC) systems; 
 *  economics; and standards. We will now list the most important classes in each of these 
 *  categories. The lists are not meant to be exhaustive, but rather to give an intuitive sense 
 *  of what is and is not currently covered by the OpenStudio Model. Throughout, some of the 
 *  classes will be concrete (have a one-to-one mapping with an object in OpenStudio.idd), and
 *  some will be abstract (provide a higher level interface to multiple Idd objects).  
 *
 *  \subsection simulation_settings_classes Simulation Settings
 *
 *  \li Version
 *  \li YearDescription
 *  \li SimulationControl
 *  \li LightingSimulationControl
 *  \li Timestep
 *  \li RunPeriod
 *  \li ConvergenceLimits
 *  \li SizingParameters
 *
 *  \subsection output_data_classes Output Data
 *
 *  \li Meter
 *  \li OutputVariable
 *
 *  \subsection resource_classes Resources
 *
 *  \li Schedule
 *  \li ScheduleTypeLimits
 *  \li DefaultScheduleSet
 *  \li Construction
 *  \li DefaultConstructionSet
 *  \li OpaqueMaterial
 *  \li FenestrationMaterial
 *  \li ModelPartitionMaterial
 *  \li SpaceType
 *  \li ElectricEquipmentDefinition
 *  \li GasEquipmentDefinition
 *  \li InternalMassDefinition
 *  \li LightsDefinition
 *  \li LuminaireDefinition
 *  \li PeopleDefinition
 *  \li ExteriorLightsDefinition
 *
 *  \subsection site_location_classes Site and Location
 *
 *  \li Site
 *  \li DesignDay
 *  \li LightingDesignDay
 *  \li WeatherFile
 *  \li SiteWaterMainsTemperature
 *
 *  \subsection geometry_classes Geometry
 *
 *  \li Facility
 *  \li Building
 *  \li BuildingStory
 *  \li Space
 *  \li Surface
 *  \li SubSurface
 *  \li ShadingSurfaceGroup
 *  \li ShadingSurface
 *  \li InteriorPartitionSurfaceGroup
 *  \li InteriorPartitionSurface
 *
 *  \subsection building_loads_classes Building Loads
 *
 *  \li ElectricEquipment
 *  \li GasEquipment
 *  \li InternalMass
 *  \li Lights
 *  \li Luminaire
 *  \li People
 *  \li SpaceInfiltrationDesignFlowRate
 *  \li DesignSpecificationOutdoorAir
 *  \li ExteriorLights
 *
 *  \subsection advanced_daylighting_classes Advanced Daylighting
 *
 *  \li DaylightingControl
 *  \li GlareSensor
 *  \li IlluminanceMap
 *  \li DaylightingDeviceShelf
 *  \li LightingSimulationZone
 *
 *  \subsection hvac_classes HVAC Systems
 *
 *  \li AirLoopHVAC
 *  \li PlantLoop
 *  \li ThermalZone
 *  \li SizingZone
 *  \li SizingSystem
 *  \li Splitter
 *  \li Mixer
 *  \li Node
 *  \li HVACComponent
 *  \li ZoneHVACComponent
 *  \li StraightComponent
 *  \li WaterToAirComponent
 *  \li Curve
 *  \li ThermostatSetpointDualSetpoint
 *  \li AvailabilityManagerScheduled
 *  \li ControllerMechanicalVentilation
 *  \li ControllerOutdoorAir
 *  \li ControllerWaterCoil
 *
 *  \subsection economics_classes Economics
 *
 *  This subsection of the model is not fully developed. Classes like ComponentCost_LineItem, 
 *  and UtilityCost_Tariff are available, but we expect to do a full refactor of this area of
 *  the model as time allows.
 *
 *  \subsection standards_classes Standards
 *
 *  This subsection of the model is not fully developed. BuildingStandardsInformation,
 *  ConstructionBaseStandardsInformation, and ClimateZones are examples of this type 
 *  of object.
 *
 *  \subsection other_links Other Links
 *
 *  The following links may also be of general interest. Attributes and relationships are a way to 
 *  access simple (no arguments to get, and one argument to set) ModelObject methods using strings, 
 *  rather than directly navigating the inheritance hierarchy.
 *
 *  \li \ref attributes "Attribute"
 *  \li \ref attributes "Relationship"
 *  \li \ref components
 *  \li \link ConcreteModelObjects.hpp Alphabetical List of Concrete ModelObjects \endlink
 *  \li \ref modelobject_hierachy "Concrete ModelObjects in Parent-Child Tree" 
 *
 *  \section introduction_sec What is the Openstudio Model?
 *
 *  The OpenStudio Model is a hierarchical, object-oriented model of a partial or complete building
 *  that exists to support annual energy and other simulations and related analyses. Complete
 *  building models may be simulated with EnergyPlus, which is developed by the Department of Energy
 *  and is our primary simulation target (http://apps1.eere.energy.gov/buildings/energyplus/).
 *  Daylighting simulation with Radiance is also supported, and OpenStudio can conduct some economic
 *  analyses natively. The OpenStudio Model aims to integerate input and output data into a seamless
 *  whole. EnergyPlus output data is accessed through the Model by connecting to an appropriate SqlFile (see the
 *  utilities documentation), which provides a basic interface to the EnergyPlus SQLite output. Once
 *  the basic connection to an output file is established, intuitive interfaces to specific pieces of
 *  output data are provided at the object level. For instance, see Facility, ThermalZone,
 *  TimeDependentValuation, and \ref modelobjectOutputDataAccessMethods.
 *
 *  \subsection underlying_data Foundation: Data Model
 *
 *  The underlying input data for Model is adapted from the EnergyPlus IDD/IDF file formats, where
 *  an IDD (input data definition) file is a data schema, analogous to an XML Schema file, and an
 *  IDF (input data file) is an instantiation of that schema, analogous to an XML file. The
 *  OpenStudio IDD has been developed using a subset of objects in the EnergyPlus IDD as a starting point so
 *  that OpenStudio can directly leverage EnergyPlus's numerous and extensively documented data
 *  objects.  Over time the OpenStudio IDD has grown to include more objects from the EnergyPlus IDD, with a
 *  focus on including the most important types for energy simulation first.  Additionally, new object types
 *  have been added to the OpenStudio IDD to provide functionalities such as:
 *
 *  <ol>
 *    <li> File/run management ('OS:WeatherFile', WeatherFile)</li>
 *    <li> Output data integration ('OS:Facility', Facility)</li>
 *    <li> Radiance integration ('OS:Luminaire', Luminaire)</li>
 *    <li> HVAC abstractions ('OS:Node', Node)</li>
 *    <li> Data sharing ('OS:ComponentData', ComponentData, Component)</li>
 *    <li> Codes and standards support ('OS:ClimateZones', ClimateZones;
 *         'OS:StandardsInforamtion:Building', BuildingStandardsInformation)</li>
 *  </ol>
 *
 *  Each OpenStudio IDD object is wrapped by a corresponding ModelObject which provides an intellegent API for working
 *  with that data object in an OpenStudio Model.
 *
 *  \link Model Models\endlink are serialized to files with the extension '.osm', which are
 *  instance files of the OpenStudio data schema. OSM files are clear text and look very similar
 *  to IDF.
 *
 *  \subsection object_model Raison D'Etre: Object Model
 *
 *  The last thing the building energy modeling community needs is yet another data model. So why
 *  did we create the OpenStudio Model?
 *
 *  <b>To harness object-oriented programming techniques to provide a powerful interface for
 *  building energy modeling at all fidelity levels, for various software development and user
 *  communities.</b>
 *
 *  By formalizing relationships among data objects (\ref modelobjectRelationships); and by placing
 *  high-level, big-knob methods in appropriate data objects (\ref modelobjectCompoundMethods); by
 *  making it possible to access related input and output data from the same location ( \ref
 *  modelobjectOutputDataAccessMethods); and by supporting multiple types of simulation engines,
 *  the OpenStudio Model provides a number of advantages over direct manipulation and analysis of
 *  EnergyPlus input and output formats.
 *
 *  \subsection model_implementation Implementation Details
 *
 *  Model serves as a container for \link ModelObject ModelObjects \endlink and a SqlFile. Model
 *  inherits its functionality for storing and querying objects from openstudio::Workspace, which is
 *  essentially an in-memory, custom database for openstudio::WorkspaceObject. The hierarchy of
 *  object types will be described in full in \ref object_implementation. For now it is important
 *  to note that all of Workspace's methods (listed in the utilities documentation) are also
 *  available to Model. Ideally, the underlying Workspace methods should only be called in model .cpp files; Model and ModelObject provides
 *  methods for adding and removing objects with more built in intelligence. However, the basic
 *  getters (e.g., getObjectByNameAndReference) and query methods (e.g., numObjects,
 *  numObjectsOfType) may be of general interest.
 *
 *  Model is implemented using the pImpl (pointer to implementation) idiom. In this pattern, each
 *  public class (e.g. openstudio::model::Model) holds a pointer to an
 *  implementation class (e.g.
 *  openstudio::model::detail::Model_Impl) that actually holds the object data and implements its
 *  methods. The public classes forward almost all user requests to the implementation class. This
 *  idiom was adopted because it results in data sharing between copies of public class instances,
 *  without the need for exposed pointers; and because it provides consistency of interface across
 *  native C++ and the SWIG bindings. (At this time, the OpenStudio SDK is available in Ruby and
 *  C#, in addition to C++, by way of SWIG.)
 *
 *  Example of data sharing:
 *
 *  \code
 *  openstudio::path modelPath = toPath(pathString);
 *  OptionalModel oModel = Model::load(modelPath);
 *  if (oModel) {
 *    Model model = *oModel;     // model accesses the same data as *oModel or oModel->
 *    Model anotherCopy = model; // anotherCopy accesses the same data as model
 *    Building buildingObject = model.getUniqueModelObject<Building>();
 *    // now all of these asserts will pass, because every copy of the model public object provides
 *    // access to the same Building object
 *    OS_ASSERT(model.numObjects() == 1u);
 *    OS_ASSERT(model.numObjects() == anotherCopy.numObjects());
 *    OS_ASSERT(model.numObjects() == oModel->numObjects());
 *    // the equality operator tests for whether two public objects point to the exact same
 *    // implementation object
 *    OS_ASSERT(buildingObject == anotherCopy.getUniqueModelObject<Building>());
 *  }
 *  \endcode
 *
 *  \section modelobject What is a ModelObject?
 *
 *  At its most basic, ModelObject is the OpenStudio class that represents an IDF object. Just
 *  like a text IDF object, it consists of a number of ordered string fields, some of which may be
 *  in extensible groups, and all of which conform to the corresponding IDD object. Please see the
 *  utilities IDD and IDF documentation (specifically openstudio::IddObject,
 *  openstudio::ExtensibleIndex, and openstudio::IdfObject) for more information. \link ModelObject
 *  ModelObjects \endlink are always owned by/live in a containing Model, which makes it possible
 *  for \link ModelObject ModelObjects \endlink to point to each other in a meaningful way, even
 *  when multiple \link Model Models \endlink are being operated on side-by-side.
 *
 *  Although, strictly speaking, ModelObject provides access to its underlying string fields, a user
 *  of the OpenStudio SDK should not need to resort to that low-level interface in most
 *  circumstances. ModelObject is the base class for a large inheritance hierarchy, the terminating
 *  nodes of which each wrap a specific object in the OpenStudio IDD. Each data field that should
 *  be publically accessible is wrapped into a getter and a setter method (see \ref
 *  modelobjectGetSet), several types of \ref modelobjectCompoundMethods and \ref
 *  modelobjectOutputDataAccessMethods are provided, as are object getters and setters appropriate
 *  to the various \ref modelobjectRelationships in the Model. Methods for creating and deleting
 *  data are also provided and available through a uniform interface, see \ref modelobjectKeyBehaviors.
 *
 *  At this time, \link ModelObject ModelObjects \endlink are uniquely identifiable by their
 *  IddObjectType and name. \link ModelObject ModelObjects \endlink also have a unique handle that
 *  is available and used in memory, but is currently not serialized to file. A number of objects
 *  are unique in a given model. Unique objects may or may not have a name, as a name is not strictly
 *  necessary for identification. For example:
 *
 *  \code
 *  Model model;
 *  Building building = model.getUniqueModelObject<Building>();
 *  OptionalString oName = building.name();
 *  OS_ASSERT(oName); // passes, because Building has a name
 *  Version version = model.getUniqueModelObject<Version>();
 *  oName = version.name();
 *  OS_ASSERT(!oName); // passes, because Version does not have (and does not need) a name
 *  \endcode
 *
 *  ModelObjects of different types can share a name as long as they are not be in the same IDD
 *  reference list; ModelObjects of the same IddObjectType cannot.
 *
 *  \subsection modelobjectGetSet Field Getters and Setters
 *
 *  \todo The OpenStudio development team is currently in the process of fully implementing these
 *  coding standards.
 *
 *  Every data field that the user should be able to directly manipulate (possibly with side effects,
 *  as in Lights::setLightingLevel), is exposed through getter and setter methods tailored to the
 *  specifics of that field as specified by the IDD. For instance, the IDD slash code '\\type real'
 *  results in getters and setters on double or boost::optional<double>. The name of the field
 *  is generated off of the IDD field name, keeping the names as close to the same as possible, but
 *  removing non-word characters, and transforming to lower camel case. Fields that point to other
 *  objects (\\type object-list) are not exposed in quite the same way, see
 *  \ref modelobjectRelationships.
 *
 *  By maintaining the field names defined in the IDD, and by listing the name of the IDD object
 *  wrapped by a given ModelObject in each \link ConcreteModelObjects.hpp concrete object's\endlink
 *  documentation, we maintain a link with the EnergyPlus Input-Output reference for those \link
 *  ModelObject ModelObjects \endlink that directly or essentially wrap a particular EnergyPlus IDD
 *  object. We therefore do not provide detailed documentation for these methods, unless the field
 *  was introduced by the OpenStudio IDD. However, side effects, like multiple fields being touched
 *  by one setter, are documented.
 *
 *  Data fields in extensible groups also have getters and setters, but may be exposed in different
 *  ways, for instance, in a getter that takes the extensible group's index as an argurment
 *  (e.g., GasMixture::getGasType, GasMixture::setGasType), as part of a class that derives from
 *  ModelExtensibleGroup (e.g. ClimateZone), or through the introduction of a specialized data
 *  structure (e.g. PlanarSurface::vertices, PlanarSurface::setVertices). Classes that derive from
 *  ModelExtensibleGroup are generally preferred, as they let the extensible groups be dealt with
 *  on their own terms as individual objects. An exception to this is the case where there are just
 *  a few simple pieces of data in the group, then a small struct (like the one used by
 *  PlanarSurface::vertices) may be preferrable. A ModelObject with a companion ModelExtensibleGroup
 *  should provide a wrapper method for numExtensibleGroups with the name of the ModelExtesibleGroup
 *  derived class replacing "ExtensibleGroups", and similar wrappers for getExtensibleGroup,
 *  extensibleGroups, clearExtensibleGroups and pushExtensibleGroup (perhaps replacing "push" with
 *  "add"), at a minimum.
 *
 *  \todo Streamline the Idf and Workspace-level ExtensibleGroup interface and port any applicable
 *  changes over to Model.
 *
 *  In general, we make the following translations from IDD slash codes to C++ syntax.
 *
 *  \subsubsection allDataFields All Data Fields
 *
 *  IDD fields that are '\\required' are provided in the object constructors, and are
 *  always returned by value (double, int, or std::string, as opposed to boost::optional<double>,
 *  boost::optional<int>, or boost::optional<std::string>, respectively), unless the field can accept
 *  different data types (as is the case with '\\autocalculatable' and '\\autosizable' fields). IDD
 *  fields with '\\default' values are also returned by value if the field can only accept one data
 *  type (e.g. it is not '\\autocalculatable' or '\\autosizable'). Note that some fields in the EnergyPlus
 *  IDD are marked as '\\required' and '\\default', this is logically inconsistent so these fields are treated
 *  as if they were marked only as '\\default' and not as '\\required'.  It is preferable to mark fields as
 *  '\\default' in the OpenStudio IDD if there is a suitable default value instead of providing default
 *  arguments to the ModelObject constructor.
 *
 *  If, the field is not required and does not have a default or the field can contain
 *  multiple data types (e.g. such as '\\autocalculatable' or '\\autosizable' fields), the return value is wrapped in a boost::optional. If such an optional
 *  return value evaluates to false, then there is no underlying data, and the user must not
 *  dereference the optional container. If the return value evaluates to true, the actual returned
 *  data may be accessed by dereferencing with '*' or '->', for example:
 *
 *  \code
 *  Model model;
 *  Lights lights(model); // by default, sets lightingLevel to 0 W
 *  // OptionalDouble is typedef for boost::optional<double> provided in utilities/core/Optional.hpp
 *  OptionalDouble oLightingLevel = lights.lightingLevel();
 *  OS_ASSERT(oLightingLevel);
 *  double lightingLevel = *oLightingLevel; // or oLightingLevel.get()
 *  std::cout << "Default lighting level for a new Lights object is " << lightingLevel << " W.";
 *  \endcode
 *
 *  If the user goes around the Model getter and setter methods by hand editing files or using the
 *  WorkspaceObject interface directly, it is possible to break the return-by-value getters, in
 *  which case a fatal error will be logged, and an openstudio::Exception will be thrown. To make it
 *  clear to a user when a default value is set (the underlying field is empty), fields with default
 *  values are provided with a query bool [fieldName]IsSetToDefault() const.
 *
 *  If it is possible for a setter to fail, the setter will return a boolean to indicate success
 *  (true) or failure (false). Otherwise, the return type will be void, and the user can trust that
 *  the specified action will execute as expected.
 *
 *  \subsubsection numericFields Numeric Fields
 *
 *  Numeric fields are based on the C++ data types double (if '\\type real') or int (if
 *  '\\type integer'). The getters return a value if the field is required or has a numeric default, and is not
 *  '\\autocalculatable' or '\\autosizable'; and return an optional otherwise. The setters accept a
 *  double or int depending on the appropriate type. If the IDD specifies a minimum or maximum value then
 *  setter may fail and thus returns a bool; otherwise the set operation cannot fail and void is returned.
 *  If the field has a default, the user can check if the field is defaulted using [fieldName]IsSetToDefault,
 *  the user can set the field to default value using set[FieldName]ToDefault. If the field has can be autocalculated,
 *  the user can check if the field is autocalculated using [fieldName]IsSetToAutocalculate,
 *  the user can set the field to autocalculate using set[FieldName]ToAutocalculate.
 *
 *  \todo Implement autosize/autocalculate getters and setters.
 *
 *  \subsubsection choiceFields Choice Fields
 *
 *  Fields of '\\type choice' are IDD defined enumerations. Originally, enum classes were used for choice fields.
 *  However, those classes greatly increased the size of the Model library. Therefore, the following rules were
 *  developed.
 *
 *  If the IddKeys associated with
 *  a choice field are (case-insensitive) 'Yes'/'No', 'On'/'Off', or 'True'/'False'; then the OpenStudo getters and setters use bool
 *  values. Two additional methods 'std::string [fieldName]String() const' and 'bool set[FieldName]String(const std::string&)' are provided
 *  if the user needs to work with the string values.  The general rules concerning getter return values, and setter return values apply to
 *  string versions of the field getters and setters. The general rules concerning default fields apply to the bool version of the getter and setter.
 *  However the bool setters cannot fail, and so always return void.  Finally, if there is no default value and the field is not required
 *  the bool getter will return a boost::optional<bool> the user must be careful not to confuse whether this optional is initialized with
 *  the actual value of the field.
 *
 *  Other choice fields operate soley off of std::string. The allowable values are case insensitive,
 *  defined by the IDD and exposed through static methods like Gas::validGasTypes(). The setters can
 *  always fail; whether a boost::optional<std::string> is returned by the getter or accepted by the
 *  setter conforms to the general rules.
 *
 *  \todo Return OptionalString() for unset choice fields (rather than emtpy strings)?
 *
 *  \subsubsection urlFields URL Fields
 *
 *  The OpenStudio IDD introduces the '\\type url' field to help with locating files other than the
 *  OSM on which a simulation depends. (See WeatherFile and TimeDependentValuation, for instance.)
 *  RunManager provides functionality to normalize url fields at job creation time.  RunManager
 *  looks for the file accross all URLSearchPaths (see utilities documentation) passed in to the
 *  job creation. In this process, if the referenced file can be found, RunManager ensures that the
 *  file is copied to the local computer running the job and that the url field is set to a valid
 *  absolute path to that file for all child jobs (the url field is unchanged in the orginal input
 *  Model). If the field immediately following a url field is named 'Checksum' and is of type alpha,
 *  that field contains a checksum (computed by openstudio::checksum()) that RunManager uses to
 *  verify that the file found is the desired file. External files may be referenced using url
 *  fields in several ways:
 *
 *  \li The url field may contain a single file name (e.g. 'Chicago.epw'). URLSearchPaths are used
 *      to look for relative urls and may be specified as relative to the job's working directory
 *      or to the original input Model.
 *
 *  \li The url field may contain a relative file path (e.g. '../../Chicago.epw'). URLSearchPaths
 *      are used to look for relative urls and may be specified as relative to the job's working
 *      directory or to the original input Model.
 *
 *  \li The url field may contain an absolute file path (e.g. 'C:/weatherData/Chicago.epw' or
 *      '/weatherData/Chicago.epw'). URLSearchPaths are not used to look for absolute file paths.
 *
 *  \li The url field may contain an absolute url (e.g. 'file://C:/weatherData/Chicago.epw' or
 *      'http://www.weatherdata.org/Chicago.epw'). URLSearchPaths are not used to look for absolute
 *      urls.
 *
 *  \li The url field may contain the UUID of a file in a Building Component Library (e.g.
 *      '{550e8400-e29b-41d4-a716-446655440000}'). (This is not yet implemented).
 *
 *  Url fields are implemented with three overloaded set methods which each return void; one taking
 *  a string, one taking an openstudio::path, and one taking a QUrl. The get method returns a
 *  string.  Normal rules for required and default fields apply.
 *
 *  \subsubsection otherFields Other Fields
 *
 *  All other types of fields ('\\type alpha' or '\\type external-list') are treated as strings and
 *  conform to the general rules.
 *
 *  \todo Return OptionalString() for unset string fields (rather than emtpy strings)?
 *
 *  An extended example of data field getter and setter usage:
 *
 *  \code
 *  Model model;
 *  ScheduleCompact scheduleCompact(model);
 *
 *  //the curve
 *  CurveBiquadratic ccFofT(model);
 *  CurveBiquadratic eirFofT(model);
 *  CurveQuadratic ccFofF(model);
 *  CurveQuadratic eirFofF(model);
 *  CurveQuadratic plf(model);
 *
 *  ccFofT.setCoefficient1Constant(1.0);
 *  ccFofT.setCoefficient2x(2.0);
 *  ccFofT.setCoefficient3xPOW2(3.0);
 *  ccFofT.setCoefficient4y(4.0);
 *  ccFofT.setCoefficient5yPOW2(5.0);
 *  ccFofT.setCoefficient6xTIMESY(6.0);
 *  ccFofT.setMinimumValueofx(-10.0);
 *  ccFofT.setMaximumValueofx(100.03);
 *  ccFofT.setMinimumValueofy(-99999);
 *  ccFofT.setMaximumValueofy(100232);
 *  ccFofT.setMinimumCurveOutput(-1000);
 *  ccFofT.setMaximumCurveOutput(99999);
 *  ccFofT.setInputUnitTypeforX("Temperature");
 *  ccFofT.setOutputUnitType("MassFlow");
 *
 *  CoilCoolingDXSingleSpeed cool(model,
 *                                scheduleCompact,
 *                                ccFofT,
 *                                ccFofF,
 *                                eirFofT,
 *                                eirFofF,
 *                                plf);
 *
 *  Schedule s = cool.getAvailabilitySchedule();
 *  OS_ASSERT(s == scheduleCompact)
 *  OptionalDouble od;
 *  cool.setRatedTotalCoolingCapacity(od); // set to "AutoCalculate"
 *
 *  // this passes because the constructor set it to "AirCooled"for you.
 *  OS_ASSERT(cool.getCondenserType() == "AiRCoolEd")
 *
 *  OptionalSchedule s2 = cool.getBasinHeaterOperatingSchedule();
 *  OS_ASSERT(!s2)
 *  // s2 evaluates to false because this schedule is not required and not set
 *  \endcode
 *
 *  \subsection modelobjectCompoundMethods Compound Methods
 *
 *  OpenStudio \link ModelObject ModelObjects \endlink provide "big knob" compound methods to enable
 *  high-level modeling and various types of analysis (perturbation, optimization, standards, etc.)
 *  For example, Space::lightingPowerPerFloorArea calculates lighting power in the space by adding up
 *  lighting power from all Lights and Luminaire objects directly assigned to the space or it's space type,
 *  then dividing by the floor area of these space. Furthermore,
 *  that method, and the Space compound "getter" that corresponds to it are wrapped up together in
 *  an Attribute named "lightingPowerDensity" available through the ModelObject::getAttribute
 *  interface.
 *
 *  PlanarSurface::uFactor and PlanarSurface::setUFactor (and the associated Attribute) is another
 *  example that brings a lot of pieces together to expose a high-level piece of data. Depending on
 *  the underlying construction type, these methods may operate directly on a SimpleGlazing object;
 *  may use static film resistance data in conjunction with one or more Material objects, and maybe
 *  a ConstructionBaseStandardsInformation object (the latter is sometimed needed for the u-factor
 *  setter); or may query the output data in the SqlFile. If u-factor is availabe through input and
 *  output data, the output data is returned, but in the meantime the two values are compared and a
 *  message is logged if their difference exceeds some tolerance. Related methods are available
 *  through ConstructionBase.
 *
 *  Several objects include a number of methods (some static) provided to give the user access to
 *  basic physical information used in EnergyPlus or in building energy standards. The film
 *  resistances alluded to above are available through PlanarSurface::filmResistance and
 *  PlanarSurface::stillAirFilmResistance. Physical properties of gases commonly found in
 *  multi-paned windows are available from FenestrationMaterial.
 *
 *  Overall, the compound methods are difficult to characterize as a whole. We encourage users to
 *  peruse the documentation for each individual object in which they have an interest. Compound
 *  methods are generally listed outside of the "Getters" and "Setters" sections, even though some
 *  methods' functionality will be similar to a get or a set. In general, these methods should be
 *  well documented since they are native to OpenStudio.
 *
 *  \subsection modelobjectOutputDataAccessMethods Output Data Access Methods
 *
 *  The model.setSqlFile(const SqlFile& sqlFile) method is used to attach a Model to related EnergyPlus
 *  simulation output.  Because the OpenStudio Model must be translated to EnergyPlus IDF for simulation, the
 *  results of the EnergyPlus simulation will not map exactly to the Model.  However, the Model will perform
 *  some checks to verify that the given SqlFile does relate to the given Model, the setSqlFile operation will
 *  fail if these checks are not satisfied.  If model.sqlFile() evaluates to true, then a given model is hooked
 *  up to its EnergyPlus simulation output data in the form of a SQLite file, and a number of ways to retrieve output data become
 *  active.
 *
 *  \li \link Meter Meters \endlink provide access to related data once the SqlFile is attached.  Meters
 *      may be present at the Facility, Building, ThermalZone, System, and Plant levels.
 *
 *  \li Every concrete ModelObject must implement the method \link ModelObject::outputVariables
 *      outputVariables() \endlink. With the SqlFile attached, the TimeSeries (see the utilities
 *      documentation) associated with those \link OutputVariable OutputVariables \endlink may be
 *      retrieved, given an appropriate environment period string (see SimulationControl).
 *
 *  \li High level simulation results are available through methods of appropriate ModelObjects.
 *
 *  \li When possible, calculation of a result (e.g. floorArea) from the input data only is preferred to
 *      requiring simulation output be attached to the Model.  In these cases, the method always
 *      computes its value from input data alone, the method does not check simulation results.
 *      At least one unit test ensures that the value calculated from input data matches the value in the
 *      simulation results.
 *
 *  \li Results which require simulation to report (e.g. annualNetSiteEnergy) return an uninitialized optional value
 *      unless a valid SqlFile is attached.  Results that are not specific to a particular environment period (e.g. u-factor) may be accessed
 *      without specifying an environment period.  Results that are specific to a particular environment period (e.g. totalLightingEnergy) require
 *      the environment period to report results as an input. Results that presume an annual simulation or some other condition
 *      (e.g. annualNetSiteEnergy presumes the presence of one and only one annual simulation run period) do not require
 *      an environment period as an input but return an uninitialized optional value if that condition is not met.
 *
 *  \li Finally some virtual methods may be implemented by some concrete classes to require output data but by other classes
*       to not require output data. For instance,
 *      PlanarSurface::uFactor may require simulation data in order to return an actual value,
 *      depending on the exact construction type. (And the u-factor may not be available at all if output data is required
 *      and the surface is not adjacent to the outside environment.)  In this case, the operation of any specific class always uses either
 *      input or output data, the user must refer to the documentation for the specific derived types to understand how the virtual method
 *      will operate in all situations.
 *
 *  The connection to the SqlFile can be reset using model.resetSqlFile().  Closing the SqlFile connection is neccesary in order
 *  to resimulate the Model using EnergyPlus.
 *
 *  \subsection modelobjectRelationships Relationships
 *
 *  There are three primary types of relationships among \link ModelObject ModelObjects\endlink:
 *  Parent-Child, Resource-ResourceUser, and (HVAC) Connection.
 *
 *  The Parent-Child relationship is used to represent containment relationships in the building.
 *  For example, Building parents Space, which parents a number of objects including Lights, People, and
 *  ElectricEquipment. If a ParentObject is removed,
 *  its children (and on down the line recursively to grandchildren, etc.) should also
 *  be removed; and if a ParentObject is cloned (copied), its children (recursively) should also
 *  be cloned.
 *
 *  In addition to helping define behaviors, the Parent-Child relationship is useful for model
 *  visualization and navigation. Navigating using ModelObject::parent() and ParentObject::children()
 *  in a Ruby script, for instance, can give the user intuition about how the various aspects of a
 *  Model conceptually fit together in OpenStudio and EnergyPlus.
 *
 *  A ResourceObject is a ModelObject that can be used by multiple other data objects. For instance,
 *  unlike a Lights object which can only be associated with one Space or SpaceType, a Schedule can be used by
 *  any number of objects, so long as the data type of the Schedule (fractional, on/off,
 *  temperature, etc.) is appropriate for the context. Every ModelObject can return the \link
 *  ResourceObject ResourceObjects \endlink that is uses with the method ModelObject::resources().
 *  \link ResourceObject ResourceObjects \endlink can report their use counts, see
 *  ResourceObject::directUseCount and ResourceObject::nonResourceObjectUseCount. Unused resources
 *  can be removed in one fell swoop with Model::purgeUnusedResourceObjects.
 *
 *  Unlike children, resources are meant to be shared, so they are not removed when a user of the
 *  resource is removed, and they are not automatically cloned when one of their users is cloned
 *  (see \ref modelobjectKeyBehaviors).
 *
 *  Parents, children and resources are available through generic and typed interfaces. For
 *  instance, Building::children() will return all of the \link Space Spaces \endlink in a
 *  std::vector<ModelObject>, and Building also has a method \link Building::spaces() spaces()
 *  \endlink that returns the same data as a std::vector<Space>. The Schedule associated with a
 *  Lights object can be found through the std::vector<ResourceObject> returned by
 *  Schedule::resources(), and also through the method Lights::schedule(). Lights also has the 
 *  method \link Lights::setSchedule setSchedule(const Schedule&) \endlink.
 *
 *  Connections join multiple ModelObjects together via designated points called ports. Connections
 *  between ModelObjects are directional and they are created by identifying a source ModelObject and port
 *  as well as a target ModelObject object and port. Once a connection is made, it is easy to access the ModelObject
 *  connected to a particular port using ModelObject::connectedObject.  New Connections are made using the
 *  openstudio::model::Model::connect function.
 *
 *  ModelObject connections are similar to EnergyPlus nodes, however a connection between ports is 
 *  more general than a node connection in Energyplus.  In the OpenStudio model, connections serve 
 *  the same role as nodes in EnergyPlus, but OpenStudio connections can be applied in contexts other 
 *  than the EnergyPlus system nodes. OpenStudio's connections and ports can be used any time one 
 *  ModelObject can be arbitrarily connected to another ModelObject. For example, EnergyPlus EMS 
 *  sensors and actuaters could be thought of as source and target ports, and a connection could be 
 *  made between the two.
 *
 *  \subsection modelobject_hierachy Parent-Child Hierarchy
 *
 *  As described above, objects in the OpenStudio Model can be organized into a tree (actually a
 *  forest) by linking ModelObject types based on their Parent-Child relationships. The following
 *  is a complete listing of the concrete \link ModelObject ModelObjects \endlink displayed in
 *  Parent-Child hierarchical form. The immediate base class of each object is listed in
 *  parentheses. Some object types are listed more than once because they can be the child of
 *  multiple concrete types (in which case the Parent-Child relationship is often defined by an
 *  abstract base class). If the class name is somewhat different from the IddObjectType valueName,
 *  the name of the IDD object is also listed for reference.
 *
 *  <ul>
 *    <li> Version (ModelObject, unique)
 *    <li> Site (ParentObject, unique) – wraps 'Site:Location'
 *      <ul>
 *        <li> ClimateZones (ModelObject, unique)
 *        <li> DesignDay (SizingPeriod) – wraps 'SizingPeriod:DesignDay'
 *          <ul>
 *            <li> SkyTemperature (ModelObject) – wraps 'OS:WeatherProperty:SkyTemperature'
 *          </ul>
 *        <li> LightingDesignDay (ModelObject)
 *        <li> SkyTemperature (ModelObject) – wraps 'OS:WeatherProperty:SkyTemperature'
 *        <li> TimeDependentValuation (ModelObject, unique)
 *        <li> SiteGroundReflectance (ModelObject, unique)
 *        <li> SiteGroundTemperatureBuildingSurface (ModelObject, unique)
 *        <li> SiteWaterMainsTemperature (ModelObject, unique)
 *        <li> WeatherFile (ModelObject, unique)
 *        <li> WeatherFileConditionType (SizingPeriod)
 *          <ul>
 *            <li> SkyTemperature (ModelObject) – wraps 'OS:WeatherProperty:SkyTemperature'
 *          </ul>
 *        <li> WeatherFileDays (SizingPeriod)
 *          <ul>
 *            <li> SkyTemperature (ModelObject) – wraps 'OS:WeatherProperty:SkyTemperature'
 *          </ul>
 *        <li> ShadingSurfaceGroup (PlanarSurfaceGroup)
 *          <ul>
 *            <li>ShadingSurface (PlanarSurface)
 *          </ul>
 *      </ul>
 *    <li> YearDescription (ParentObject, unique) 
 *      <ul>         
 *        <li> RunPeriodControlSpecialDays (ModelObject)
 *        <li> RunPeriodControlDaylightSavingTime (ModelObject, unique)
 *      </ul>
 *    <li> SimulationControl (ParentObject, unique)
 *      <ul>
 *        <li> Timestep (ModelObject, unique)
 *        <li> RunPeriod (ParentObject)
 *          <ul>
 *            <li> SkyTemperature (ModelObject) – wraps 'OS:WeatherProperty:SkyTemperature'
 *          </ul>
 *        <li> ConvergenceLimits (ModelObject, unique)
 *        <li> ShadowCalculation (ModelObject, unique)
 *        <li> HeatBalanceAlgorithm (ModelObject, unique)
 *        <li> InsideSurfaceConvectionAlgorithm (ModelObject, unique)
 *        <li> OutsideSurfaceConvectionAlgorithm (ModelObject, unique)
 *        <li> SizingParameters (ModelObject, unique)
 *        <li> ZoneAirContaminantBalance (ModelObject, unique)
 *        <li> ZoneAirHeatBalanceAlgorithm (ModelObject, unique)
 *        <li> ZoneCapacitanceMultiplierResearchSpecial (ModelObject, unique)
 *      </ul>
 *    <li> LightingSimulationControl (ModelObject)
 *    <li> Facility (ParentObject, unique)
 *      <ul>
 *        <li> Meter (ModelObject)
 *        <li> ExteriorLights (ModelObject)
 *        <li> Building (ParentObject, unique)
 *          <ul>
 *            <li> BuildingStandardsInformation (ModelObject, unique)
 *            <li> Meter (ModelObject)
 *            <li> BuildingStory (ModelObject)
 *            <li> ShadingSurfaceGroup (PlanarSurfaceGroup)
 *              <ul>
 *                <li> ShadingSurface (PlanarSurface)
 *              </ul>
 *            <li> Space (PlanarSurfaceGroup)
 *              <ul>
 *                <li> ShadingSurfaceGroup (PlanarSurfaceGroup)
 *                  <ul>
 *                    <li> ShadingSurface (PlanarSurface)
 *                  </ul>
 *                <li> InteriorPartitionSurfaceGroup (PlanarSurfaceGroup)
 *                  <ul>
 *                    <li> InteriorPartitionSurface (PlanarSurface)
 *                  </ul>
 *                <li> Surface (PlanarSurface)
 *                  <ul>
 *                    <li> SubSurface (PlanarSurface)
 *                      <ul>
 *                        <li> DaylightingDeviceShelf (ModelObject)
 *                      </ul>
 *                  </ul>
 *                <li> ElectricEquipment (SpaceLoadInstance)
 *                <li> GasEquipment (SpaceLoadInstance)
 *                <li> HotWaterEquipment (SpaceLoadInstance)
 *                <li> SteamEquipment (SpaceLoadInstance)
 *                <li> OtherEquipment (SpaceLoadInstance)
 *                <li> InternalMass (SpaceLoadInstance)
 *                <li> Lights (SpaceLoadInstance)
 *                <li> Luminaire (SpaceLoadInstance)
 *                <li> People (SpaceLoadInstance)
 *                <li> SpaceInfiltrationDesignFlowRate (SpaceLoad)
 *                <li> SpaceInfiltrationEffectiveLeakageArea (SpaceLoad)
 *                <li> DaylightingControl (SpaceItem)
 *                <li> IlluminanceMap (SpaceItem)
 *                <li> GlareSensor (SpaceItem)
 *              </ul>
 *            <li> ThermalZone (HVACComponent)
 *          </ul>
 *      </ul>
 *    <li> AirLoopHVAC (Loop)
 *    <li> PlantLoop (Loop)
 *    <li> AirLoopHVACZoneSplitter (Splitter)
 *    <li> AirLoopHVACZoneMixer (Mixer)
 *    <li> ConnectorSplitter (Splitter)
 *    <li> ConnectorMixer (Mixer)
 *    <li> Connection (ModelObject)
 *    <li> SizingZone (ModelObject)
 *    <li> SizingSystem (ModelObject)
 *    <li> ThermostatSetpointDualSetpoint (ModelObject)
 *    <li> AvailabilityManagerScheduled (ParentObject)
 *    <li> ControllerMechanicalVentilation (ParentObject)
 *    <li> AirLoopHVACOutdoorAirSystem (HVACComponent)
 *      <ul>    
 *        <li> ControllerOutdoorAir (ParentObject)
 *      </ul>
 *    <li> ControllerWaterCoil (HVACComponent)
 *    <li> Node (StraightComponent)
 *      <ul>
 *        <li> SetpointManagerSingleZoneReheat (HVACComponent)
 *        <li> SetpointManagerScheduled (HVACComponent)
 *        <li> SetpointManagerMixedAir (HVACComponent)
 *      </ul>
 *    <li> AirLoopHVACUnitaryHeatPumpAirToAir (StraightComponent)
 *      <ul>
 *        <li> (HVACComponent)
 *      </ul>
 *    <li> AirTerminalSingleDuctParallelPIUReheat (StraightComponent)
 *      <ul>
 *        <li> (HVACComponent)
 *      </ul>
 *    <li> AirTerminalSingleDuctUncontrolled (StraightComponent)
 *    <li> AirTerminalSingleDuctVAVReheat (StraightComponent)
 *    <li> FanConstantVolume (StraightComponent)
 *    <li> FanVariableVolume (StraightComponent)
 *    <li> CoilHeatingGas (StraightComponent)
 *    <li> EvaporativeCoolerDirectResearchSpecial (StraightComponent)
 *    <li> CoilHeatingDXSingleSpeed (StraightComponent)
 *      <ul>
 *        <li> (Curve)
 *      </ul>
 *    <li> CoilCoolingDXSingleSpeed (StraightComponent)
 *      <ul>
 *        <li> (Curve)
 *      </ul>
 *    <li> CoilCoolingDXTwoSpeed (StraightComponent)
 *      <ul>
 *        <li> (Curve)
 *      </ul>
 *    <li> CoilHeatingElectric (StraightComponent)
 *    <li> PumpVariableSpeed (StraightComponent)
 *      <ul>
 *        <li> (Curve)
 *      </ul>
 *    <li> BoilerHotWater (StraightComponent)
 *      <ul>
 *        <li> (Curve)
 *      </ul>
 *    <li> CoilCoolingWater (WaterToAirComponent)
 *    <li> CoilHeatingWater (WaterToAirComponent)
 *    <li> ZoneHVACFourPipeFanCoil (ZoneHVACComponent)
 *      <ul>
 *        <li> (HVACComponent)
 *      </ul>
 *    <li> ZoneHVACPackagedTerminalAirConditioner (ZoneHVACComponent)
 *      <ul>
 *        <li> (HVACComponent)
 *      </ul>
 *    <li> ZoneHVACPackagedTerminalHeatPump (ZoneHVACComponent)
 *      <ul>
 *        <li> (HVACComponent)
 *      </ul>
 *    <li> CurveBicubic (Curve)
 *    <li> CurveBiquadratic (Curve)
 *    <li> CurveCubic (Curve)
 *    <li> CurveDoubleExponentialDecay (Curve)
 *    <li> CurveExponent (Curve)
 *    <li> CurveExponentialDecay (Curve)
 *    <li> CurveExponentialSkewNormal (Curve)
 *    <li> CurveFanPressureRise (Curve)
 *    <li> CurveFunctionalPressureDrop (Curve)
 *    <li> CurveLinear (Curve)
 *    <li> CurveQuadratic (Curve)
 *    <li> CurveQuadraticLinear (Curve)
 *    <li> CurveQuartic (Curve)
 *    <li> CurveRectangularHyperbola1 (Curve) 
 *    <li> CurveRectangularHyperbola2 (Curve)
 *    <li> CurveSigmoid (Curve)
 *    <li> CurveTriquadratic (Curve)
 *    <li> LightingSimulationZone (ModelObject)
 *    <li> ScheduleTypeLimits (ResourceObject)
 *    <li> ScheduleConstant (Schedule)
 *    <li> ScheduleCompact (Schedule)
 *    <li> ScheduleRuleset (Schedule)
 *      <ul>
 *        <li> ScheduleDay (ScheduleBase)
 *        <li> ScheduleRule (ParentObject)
 *          <ul>
 *            <li> ScheduleDay (ScheduleBase)
 *          </ul>
 *      </ul>
 *    <li> ScheduleYear (Schedule)
 *    <li> ScheduleWeek (ResourceObject)
 *    <li> ScheduleDay (ScheduleBase)
 *    <li> ScheduleVariableInterval (ScheduleInterval)
 *    <li> ScheduleFixedInterval (ScheduleInterval)
 *    <li> DefaultScheduleSet (ResourceObject)
 *    <li> Construction (LayeredConstruction)
 *      <ul>
 *        <li> ConstructionBaseStandardsInformation (ModelObject) – wraps 'OS:StandardsInformation:%Construction'
 *      </ul>
 *    <li> ConstructionWithInternalSource (LayeredConstruction) – wraps 'OS:%Construction:InternalSource'
 *      <ul>
 *        <li> ConstructionBaseStandardsInformation (ModelObject) – wraps 'OS:StandardsInformation:%Construction'
 *      </ul>
 *    <li> CFactorUndergroundWallConstruction (ConstructionBase) – wraps 'OS:%Construction:CfactorUndergroundWall'
 *      <ul>
 *        <li> ConstructionBaseStandardsInformation (ModelObject) – wraps 'OS:StandardsInformation:%Construction'
 *      </ul>
 *    <li> FFactorGroundFloorConstruction (ConstructionBase) – wraps 'OS:%Construction:FfactorGroundFloor'
 *      <ul>
 *        <li> ConstructionBaseStandardsInformation (ModelObject) – wraps 'OS:StandardsInformation:%Construction'
 *      </ul>
 *    <li> WindowDataFile (ConstructionBase) – wraps 'OS:%Construction:%WindowDataFile'
 *      <ul>
 *        <li> ConstructionBaseStandardsInformation (ModelObject) – wraps 'OS:StandardsInformation:%Construction'
 *      </ul>
 *    <li> DefaultConstructionSet (ResourceObject)
 *    <li> DefaultSubSurfaceConstructions (ResourceObject)
 *    <li> DefaultSurfaceConstructions (ResourceObject)
 *    <li> StandardOpaqueMaterial (OpaqueMaterial) – wraps 'OS:%Material'
 *    <li> MasslessOpaqueMaterial (OpaqueMaterial) – wraps 'OS:%Material:NoMass'
 *    <li> Gas (GasLayer) – wraps 'OS:WindowMaterial:%Gas'
 *    <li> GasMixture (GasLayer) – wraps 'OS:WindowMaterial:%GasMixture'
 *    <li> InfraredTransparentMaterial (ModelPartitionMaterial) – wraps 'OS:%Material:InfraredTransparent'
 *    <li> AirWallMaterial (ModelPartitionMaterial) – wraps 'OS:%Material:AirWall'
 *    <li> Shade (ShadingMaterial) – wraps 'OS:WindowMaterial:%Shade'
 *    <li> Screen (ShadingMaterial) – wraps 'OS:WindowMaterial:%Screen'
 *    <li> Blind (ShadingMaterial) – wraps 'OS:WindowMaterial:%Blind'
 *    <li> AirGap (OpaqueMaterial) – wraps 'OS:%Material:%AirGap'
 *    <li> RoofVegetation (OpaqueMaterial) – wraps 'OS:%Material:%RoofVegetation'
 *    <li> StandardGlazing (Glazing) – wraps 'OS:WindowMaterial:%Glazing'
 *    <li> SimpleGlazing (Glazing) – wraps 'OS:WindowMaterial:SimpleGlazingSystem'
 *    <li> ThermochromicGlazing (Glazing) – wraps 'OS:WindowMaterial:GlazingGroup:Thermochromic'
 *    <li> RefractionExtinctionGlazing (Glazing) – wraps 'OS:WindowMaterial:Glazing:RefractionExtinctionMethod'
 *    <li> SpaceType (ResourceObject)
 *      <ul>
 *        <li> ElectricEquipment (SpaceLoadInstance)
 *        <li> GasEquipment (SpaceLoadInstance)
 *        <li> HotWaterEquipment (SpaceLoadInstance)
 *        <li> SteamEquipment (SpaceLoadInstance)
 *        <li> OtherEquipment (SpaceLoadInstance)
 *        <li> InternalMass (SpaceLoadInstance)
 *        <li> Lights (SpaceLoadInstance)
 *        <li> Luminaire (SpaceLoadInstance)
 *        <li> People (SpaceLoadInstance)
 *        <li> SpaceInfiltrationDesignFlowRate (SpaceLoad)
 *        <li> SpaceInfiltrationEffectiveLeakageArea (SpaceLoad)
 *      </ul>
 *    <li> RenderingColor (ResourceObject)
 *    <li> ElectricEquipmentDefinition (SpaceLoadDefinition)
 *    <li> GasEquipmentDefinition (SpaceLoadDefinition)
 *    <li> HotWaterEquipmentDefinition (SpaceLoadDefinition)
 *    <li> SteamEquipmentDefinition (SpaceLoadDefinition)
 *    <li> OtherEquipmentDefinition (SpaceLoadDefinition)
 *    <li> InternalMassDefinition (SpaceLoadDefinition)
 *    <li> LightsDefinition (SpaceLoadDefinition)
 *    <li> LuminaireDefinition (SpaceLoadDefinition)
 *    <li> PeopleDefinition (SpaceLoadDefinition)
 *    <li> ExteriorLightsDefinition (ResourceObject)
 *    <li> DesignSpecificationOutdoorAir (ResourceObject)
 *    <li> CurrencyType (ParentObject)
 *    <li> LifeCycleCost_UsePriceEscalation (ParentObject)
 *    <li> LifeCycleCost_UseAdjustment (ParentObject)
 *    <li> LifeCycleCost_Parameters (ParentObject)
 *    <li> ComponentCost_Adjustments (ParentObject)
 *    <li> ComponentCost_Reference (ParentObject)
 *    <li> UtilityCost_Tariff (ParentObject)
 *      <ul>
 *        <li> UtilityCost_Qualify (ParentObject)
 *        <li> UtilityCost_Charge_Simple (ParentObject)
 *        <li> UtilityCost_Charge_Block (ParentObject)
 *        <li> UtilityCost_Ratchet (ParentObject)
 *        <li> UtilityCost_Variable (ParentObject)
 *        <li> UtilityCost_Computation (ParentObject)
 *      </ul>
 *  </ul>
 *
 *  Special Cases that can be Parented by many \link ModelObject ModelObjects \endlink
 *
 *  <ul>
 *    <li> OutputVariable (ModelObject)
 *    <li> ComponentCost_LineItem (ModelObject)
 *    <li> LifeCycleCost_RecurringCosts (ParentObject)
 *    <li> LifeCycleCost_NonrecurringCost (ParentObject)
 *  </ul>
 *
 *  Special Cases related to the Building Component Library
 *
 *  <ul>
 *    <li> ComponentData (ParentObject)</li>
 *  </ul>
 *
 *  \subsection modelobjectKeyBehaviors Key Behaviors
 *
 *  \subsubsection newmodel Create a New Model
 *
 *  There are four constructors available for Model. The default constructor creates a new Model
 *  with a single Version object in it. Version objects are not returned by Model::objects or 
 *  counted by Model::numObjects.
 *
 *  \code
 *  Model model;
 *  OS_ASSERT(model.versionObject());
 *  OS_ASSERT(model.numObjects() == 0u);
 *  OS_ASSERT(model.iddFileType() == IddFileType::OpenStudio);
 *  OS_ASSERT(model.strictnessLevel() == StrictnessLevel::Draft);
 *  \endcode
 *
 *  The preferred method for creating a Model from an OSM file is
 *
 *  \code
 *  OptionalModel oModel = Model::load(modelFilePath);
 *  if (!oModel) { LOG_AND_THROW("Unable to open '" << toString(modelFilePath) << "'."); }
 *  Model model = *oModel;
 *  \endcode
 *
 *  UNLESS that OSM file might be from an old version of OpenStudio, in which case the version 
 *  translator should be used:
 *
 *  \code
 *  osversion::VersionTranslator translator;
 *  OptionalModel oModel = translator.loadModel(modelFilePath);
 *  if (!oModel) { 
 *    LOG_AND_THROW("Unable to open and translate '" << toString(modelFilePath) << "'.");
 *  }
 *  Model model = *oModel;
 *  \endcode
 *
 *  The preferred method for creating a Model from IDF file is
 *
 *  \code
 *  #include <energyplus/ReverseTranslator.hpp>
 *
 *  OptionalModel oModel = energyplus::loadAndTranslateIdf(idfFilePath);
 *  \endcode
 *
 *  In addition, constructors are provided from IdfFile and Workspace (see the utilities
 *  documentation). Those constructors will throw if IdfFile or Workspace uses the wrong IDD file.
 *
 *  Because Model is required to support partial (unsimulatable) building models for data
 *  sharing, and to provide a flexible interface for editing, it is ultimately up to the user to
 *  ensure that a Model is ready for simulation. Model::validityReport and similar methods (see
 *  the utilities documentation, especially Workspace::validityReport,
 *  WorkspaceObject::validityReport, and ValidityEnums.hpp) are provided to help users bring their
 *  models up to the standard set by the IDD. (And there are a few places in Model where validity
 *  checking goes beyond the data schema to rules internal to EnergyPlus, see for instance
 *  LayeredConstruction::layersAreValid, which is used by the virtual implementation of
 *  validityReport for \link LayeredConstruction LayeredConstructions \endlink.) Beyond that, users
 *  will sometimes need to go through an iterative process of modeling, trying to run EnergyPlus
 *  (or another simulation engine), addressing simulation errors, etc.
 *
 *  \subsubsection newmodelobject Create a New ModelObject
 *
 *  Every concrete ModelObject has one or more public constructors, unless
 *  it is marked unique by the IDD. In this case, there is no public constructor in order to
 *  prevent the accidental construction of multiple instance of that type in the same model. Instead,
 *  these objects are made available through Model::getUniqueModelObject and
 *  Model::getOptionalUniqueModelObject:
 *
 *  \code
 *  Model model;
 *
 *  // getOptionalUniqueModelObject only returns an object if it is already in the Model.
 *  // It constructs any new objects.
 *  OptionalBuilding oBuilding = model.getOptionalUniqueModelObject<Building>();
 *  OS_ASSERT(!oBuilding);
 *  OS_ASSERT(model.numObjects() == 0u);
 *
 *  // getUniqueModelObject constructs the object if it is not yet present.
 *  Building building = model.getUniqueModelObject<Building>();
 *  OS_ASSERT(model.numObjects() == 1u);
 *
 *  // now getOptionalUniqueModelObject can return the object that was just created
 *  oBuilding = model.getOptionalUniqueModelObject<Building>();
 *  OS_ASSERT(oBuilding);
 *  OS_ASSERT(model.numObjects() == 1u);
 *  OS_ASSERT(oBuilding.get() == building);
 *  \endcode
 *
 *  For non-unique objects, all public constructors will ensure that required fields, children,
 *  and resources are present as a post-condition. Depending on the constructor, the user may be
 *  required to explicitly provide those objects, or they may be default constructed. For example
 *  the LightsDefinition object is a required argument to the Lights object constructor.  Parents
 *  are often not required by the public constructors; as appropriate, convenience constructors
 *  that take the parent as an argument may be provided. (Orphan construction is allowed in order
 *  to better support data sharing. For instance, if someone wants to create a lighting component
 *  for the Building Component Library, requiring Lights to be constructed with a Space or
 *  SpaceType would be inconvenient.)
 *
 *  \subsubsection removemodelobject Delete a ModelObject
 *
 *  ModelObject::remove is used to remove a ModelObject from a Model. When a model object is
 *  removed, all of its children are also removed. Objects related to the removed object through
 *  a resource or connection relationship are not deleted. Resources persist so they can be used
 *  by other objects. Connections are broken by object removal, but the adjancent object is left
 *  behind, unless it happens to also be a child of the removed object. However, when a resource or
 *  child object is removed, its user objects or parents may also be removed, if the relationship
 *  is a required one for the user/parent. For instance, SpaceLoadDefinition::remove also removes 
 *  all of its SpaceLoadDefinition::instances, since a SpaceLoadInstance cannot exist without its
 *  definition.
 *
 *  For all deleted objects, when all the instances of that object go out of scope and stop
 *  referencing the shared implementation pointer, the memory will be automatically freed for you.
 *
 *  \subsubsection clonemodelobject Clone a ModelObject
 *
 *  ModelObject::clone is used to duplicate a ModelObject.  The clone method may be used to copy
 *  an object into another model, or to duplicate an object in the same model. The act of cloning
 *  an object will change the duplicate object's name if an object of the same type and name
 *  already exists in the target model. Therefore, cloning into the same model will always result
 *  in name change for the duplicate so as to avoid a name collision with the original object.
 *  Cloning a ModelObject also clones all of its children (recursively). Resources are cloned as
 *  necessary (when an equivalent object cannot be located, see IdfObject::dataFieldsEqual and
 *  IdfObject::managedObjectListFieldsNonConflicting in the utilities documentation) when the
 *  original and target models are different.
 *
 *  Examples:
 *  \code
 *  LightsVector originalLights = space.lights();
 *  clonedSpace= space.clone(space.model());
 *  LightsVector clonedLights = clonedSpace.lights();
 *  OS_ASSERT(originalLights.size() == clonedLights.size())
 *
 *  if (originalLights.size() > 0) {
 *    LightVector::iterator i = std::find(originalLights.begin(),originalLights.end(),clonedLights[0]);
 *    OS_ASSERT(i == originalLights.end()); // clonedLights[0] not found in originalLights
 *                                             // (std::find uses ==, which for ModelObjects checks
 *                                             // to see if the objects point to the exact same
 *                                             // implementation object)
 *  }
 *  \endcode
 *
 *  The following code would also pass its OS_ASSERTs. Because we cloned into the same model, the
 *  schedule (a ResourceObject) was NOT cloned.
 *
 *  \code
 *  // continuing from above
 *
 *  if (originalLights.size() > 0) {
 *    // originalLights[0] and clonedLights[0] share the exact same schedule ...
 *    // ... == verifies it.
 *    OS_ASSERT(originalLights[0].schedule() == clonedLights[0].schedule());
 *    // ... as does making a change in clonedLights[0].schedule()
 *    clonedLights[0].schedule().setName("Some Cool Name")
 *    // ... and then looking for the change in originalLights[0].schedule().
 *    OS_ASSERT(originalLights[0].schedule().name().get() == "Some Cool Name")
 *  }
 *  \endcode
 *
 *  \subsection object_implementation Implementation Details
 *
 *  Similar to Model inheriting from openstudio::Workspace, ModelObject inherits from
 *  openstudio::WorkspaceObject, which in turn inherits from openstudio::IdfObject. Briefly,
 *  IdfObject corresponds to a single data object in an IDF file. It has an IddObject, which defines
 *  its type, and it otherwise consists of a vector (ordered list) of string fields, accessible by
 *  index and with an optional user comment. (If you never specify an IDF field comment, default
 *  comments are generated for you from the IddField::name()'s.) WorkspaceObject provides the added
 *  functionality of turning the '\\type object-list' fields that in clear text point to other data
 *  objects by name reference into bi-directional, name-independent pointers to particular objects.
 *  Please see the utilities/idf documentation for more information.
 *
 *  In this way, IdfObject, WorkspaceObject, and Workspace form the underlying data model used by
 *  OpenStudio to represent a single building (assuming the OpenStudio, rather than the EnergyPlus
 *  or other, IDD is referenced). Model, ModelObject and everything else in the openstudio::model
 *  namespace exists to turn that data model into a full-featured object model.
 *
 *  Because Model and ModelObject publically inherit from Workspace and WorkspaceObject, methods of
 *  Workspace and WorkspaceObject are directly available for use through Model and ModelObject.
 *  Use of these methods is discouraged where it is not necessary as these data model-level 
 *  modifications bypass the best-practice logic built into classes derived from ModelObject.
 *
 *  \link ModelObject ModelObjects \endlink (and WorkspaceObjects and IdfObjects) also use the
 *  pImpl idiom described \ref model_implementation "above". So basic copying
 *  and assignment results in data sharing, for example:
 *
 *  \code
 *  Space spaceA(model);
 *  spaceA.setName("Space A");
 *  OS_ASSERT(spaceA.name().get() == "Space A");
 *  Space spaceB = spaceA;
 *  spaceB.setName("Space B");
 *  OS_ASSERT(spaceA.name().get() == "Space B"); // may not be the intended behavior ...
 *                                                  // use clone() to get a deep copy
 *
 *  // Sharing also happens just by passing objects around as aruguments.
 *  // For instance, this function takes a non-const copy, it does not pass by reference.
 *  void changeName(ModelObject object) {
 *    object.setName("New Name");
 *  }
 *
 *  Space spaceC(model);
 *  // function looks like all changes are local to it ...
 *  changeName(spaceC);
 *  // ... but they are not because of pImpl.
 *  OS_ASSERT(spaceC.name().get() == "New Name");
 *  \endcode
 *
 *  The pointer to the implementation is managed by a reference counting pointer, so the
 *  implementation object is destroyed when the last wrapper class goes out of scope. Because the
 *  implementation is shared between copies of the public class, Qt signal and slot connections will
 *  remain active after the public object is destroyed provided that another object still references
 *  the same implementation. Note that at the very least this other object is typically the Model
 *  of which the ModelObject is a member, as Model (actually Workspace) maintains a map of all of its
 *  objects' implementation reference counting pointers.
 *
 *  The combined use of template methods such as Model::getModelObjects<T>() with the pImpl idiom
 *  has led to the unfortunate case where users of a class may need to also include the
 *  corresponding implementation class, for example:
 *
 *  \code
 *  // user will have to include Space.hpp and Space_Impl.hpp
 *  std::vector<Space> spaces = model.getModelObjects<Space>();
 *  \endcode
 *
 *  Another important detail about the OpenStudio Model is that ModelObjects are only valid when
 *  they are part of a Model. The benefit of this requirement is that any ModelObject can access the
 *  entire Model to search for related ModelObjects.  For the most part, the Model interface
 *  enforces this requirement. However, there are some cases where it is possible to break this
 *  invariant, for example:
 *
 *  \code
 *
 *  // EXAMPLE OF WHAT NOT TO DO
 *
 *  // vector to hold ModelObjects
 *  std::vector<ModelObject> modelObjects;
 *
 *  { // create new scope
 *
 *    // create new Model
 *    Model model;
 *
 *    // add some objects
 *    model.getUniqueModelObject<Building>();
 *    model.getUniqueModelObject<Facility>();
 *    Space space1(model);
 *    Space space2(model);
 *
 *    // get objects, they are valid here
 *    modelObjects = model.modelObjects();
 *
 *  }  // model goes out of scope
 *
 *  // modelObjects now contains invalid ModelObjects
 *  OS_ASSERT(!modelObjects[0].initialized()); // Model_Impl destructor disconnects objects
 *
 *  \endcode
 *
 *  \section attributes What are Attributes and Relationships?
 *
 *  The concept of Attributes and Relationships have recently been added to ModelObject for generic
 *  access to object methods for user interface development, constructing (energy-efficiency) measures,
 *  and writing codes and standards.  Attributes and Relationships are both implemented using the Qt 
 *  property system.  A Qt property can be read and written using the generic functions without knowing 
 *  anything about the owning class except the property's name.  For example, the Space ModelObject has 
 *  a method named floorArea.  This method can be accessed through the typed interface as:
 *
 *  \code
 *  Space space(model);
 *  double floorArea = space.floorArea();
 *  \endcode

 *  This same method can be accessed through attributes as:
 *
 *  \code
 *  Space space(model);
 *  boost::optional<Attribute> floorAreaAttribute = space.getAttribute(“floorArea”)
 *  OS_ASSERT(floorAreaAttribute);
 *  double floorArea = floorAreaAttribute.valueAsDouble();
 *  \endcode
 *
 *  Attributes can be read only, or allow read and write access (limited to calling a setter
 *  function taking a single argument).  A list of all attribute names for each ModelObject can be
 *  accessed using ModelObject::attributeNames, the user can check if the attribute is settable using
 *  ModelObject::isSettableAttribute(const std::string& name), and the user can check if the 
 *  attribute is optional using isOptionalAttribute(const std::string& name).  Relationships are very 
 *  similar to Attributes except that Attributes deal with simple, plain-old-data (POD) values (e.g. 
 *  bool, int, double, OSOptionalQuantity etc.) while Relationships deal with ModelObject values. 
 *  The Attribute class is documented in the utilities sub-project, the Relationship class is 
 *  documented in the model sub-project.
 *
 *  \section components What is a Component?
 *
 *  A Component is a partial Model that can be created from a ModelObject and then shared between
 *  models or with other modelers. When ModelObject::createComponent is called, ModelObject::clone
 *  is used to copy that object (the primary object) and related objects (often the children and 
 *  resources of the object, evaluated recursively) into a new, blank Component. A Component is a 
 *  Model that satisfies a few additional constraints. 
 *
 *  \link Component Components\endlink always contain one ComponentData object, which
 *  provides a place to list all the objects in the Component, as well as universal identifiers 
 *  and timestamps used to keep track of the Component as it travels to and from local and remote
 *  Building Component Libraries (BCLs). The contents list is always headed by the primary object, 
 *  which induces a parent-child tree plus resource and connection relationships on the rest of 
 *  objects. For instance, a Construction Component would have a Construction object as its
 *  primary object, and would likely also contain a number of Material objects (resources of
 *  the Construction), and a ComponentCost_LineItem object (child of the Construction).
 *
 *  A component creation example follows:
 *  \code
 *  OptionalModel oModel = Model::load(osmFilepath);
 *  OS_ASSERT(oModel);
 *  model = *oModel;
 *  ConstructionVector constructions = model.getModelObjects<Construction>();
 *  if (constructions.size() > 0) {
 *    Component constructionComponent = constructions[0].createComponent();
 *    ModelObject primary = constructionComponent.primaryObject();
 *    OS_ASSERT(primary.optionalCast<Construction>());
 *    OS_ASSERT(primary.cast<Construction>().layers().size() == constructions[0].layers().size());
 *    OS_ASSERT(primary.cast<Construction>() != constructions[0]);
 *    OS_ASSERT(primary.model() != constructions[0].model());
 *  }
 *  \endcode
 *
 *  The Component class disallows some of the non-const operations of Model. In particular, no
 *  objects can be added to or inserted into a Component. Objects can be removed, except for the
 *  primary ModelObject and the ComponentData object. If either of those objects are removed, the
 *  Component is invalidated and throws an exception. Once created, Components can be saved for
 *  future use, or directly used in another Model. To save a Component for future use, it should be
 *  added to a local BCL, and/or uploaded to the online %Building %Component Library. (These 
 *  features are currently under development. Please see the documentation for utilities/bcl and 
 *  http://bcl.nrel.gov/.)
 *
 *  To use a Component (either directly, or deserialized from a building component library) in a
 *  Model, use the method Model::insertComponent(const Component& ). This method first looks to see
 *  if a copy of the Component is already present in the Model; if not, it clones all of the
 *  objects into the Model. In either case, if successful, the method returns the resulting
 *  ComponentData object. After an insertComponent operation, Model watches the contents of the
 *  inserted Component, changing the ComponentData::versionUUID if data is modified, and deleting
 *  the ComponentData object (and discontinuing the watching operation) if it is invalidated (by
 *  removal of the primaryObject, for instance). In this way, Model can be queried to find out what
 *  \link Component Components \endlink it uses, and one can ascertain whether any data has changed
 *  as compared to the original version. Therefore, this is a mechanism that can be used to trace
 *  coherent pieces of input data through a series of related models, between similar research
 *  project models, or to ensure that the appropriate components are used for building energy
 *  standards compliance.
 *  
 *  An example of using the component created above:
 *  \code
 *  if (constructions.size() > 0) {
 *    Model newModel;
 *    OptionalComponentData ocd = newModel.insertComponent(constructionComponent);
 *    OS_ASSERT(ocd);
 *    unsigned n = newModel.numObjects();
 *    ComponentData cd = *ocd;
 *    Construction clonedConstruction = cd.primaryComponentObject().cast<Construction>();
 *    // now I can make some geometry and assign the construction to a planar surface
 *    // ... or I can remove the new construction, thereby "destroying" the component:
 *    clonedConstruction.remove();
 *    OS_ASSERT(clonedConstruction.handle().isNull()); // clonedConstruction removed from model
 *    OS_ASSERT(cd.handle().isNull());                 // ComponentData object also removed
 *    OS_ASSERT(newModel.numObjects() < n);
 *    OS_ASSERT(newModel.getModelObjects<Construction>().empty());
 *    // there may be some orphaned resources (Materials) still hanging around …
 *    newModel.purgeUnusedResources();
 *    OS_ASSERT(newModel.numObjects() == 0u);
 *  }
 *  \endcode
 */
 } // model
 } // openstudio
