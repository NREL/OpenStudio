namespace openstudio {
namespace gltf {

  /** \mainpage OpenStudio Gltf
 *
 * \section overview_gltf Overview
 *
 * glTF (derivative short form of Graphics Language Transmission Format or GL Transmission Format)
 * is a standard file format for three-dimensional scenes and models.
 *
 * An OpenStudio model can be ForwardTranslated to a glTF file for external viewing such as in-browser via WebGL.
 * OpenStudio uses the glTF standard 2.0. Internally it uses the `tinygltf` library to write the glTF file.
 *
 * The following document aims to explains briefly how glTF and supporting OpenStudio class are laid out.
 *
 * The following classes form the foundation of the OpenStudio Gltf functionality:
 *
 * \li GltfForwardTranslator
 * \li GltfUserData
 * \li GltfMetaData
 * \li GltfModelObjectMetaData
 * \li GltfMaterialData
 *
 * The `GltfForwardTranslator` class is the orchestrator. The other classes are **interface** classes between the OpenStudio Model and the gltf Model.
 * Their data members are there to represent the data that will eventually be stored in the gltf Model.
 *
 * \li They have one or several constructors that take in OpenStudio Model or ModelObject to populate the data members.
 * \li They have a function that will export to a `tinygltf::Value::Object`
 * \li They also have a constructor that does the reverse: populating the data members by reading a `tinygltf::Value` for inspection for eg
 *
 *
 * \subsection gltf_geometry Geometry: PlanarSurfaces <=> glTF Node
 *
 * The geometry part of the glTF represents the surfaces of the OpenStudio Model.
 *
 * OpenStudio Model `PlanarSurface`s (such as `Surface`, `SubSurface`, `ShadingSurface`, etc) are glTF `Node`s. The Surface Vertices are packed in a
 * base64 encoded buffer that the glTF `Node` will reference.
 *
 * The class `GltfUserData` is used to create node `extras`: we place some attribute on these nodes, such as the surface type, its construction,
 * the Space and SpaceType and ThermalZone it belongs to. External viewers can use that information for displaying attributes or filtering.
 *
 * A glTF `Material` is also attached to these gltf `Node`s, with the help of the `GltfMaterialData` class, which interfaces between an OpenStudio
 * RenderingColor and a `tinygltf::Material`
 *
 * \subsection gltf_metadata Scene MetaData
 *
 * A glTF `Scene` can also bear `extras`. We use the class `GltfMetaData` to do so. GltfMetaData holds:
 *
 * \li An array of `GltfModelObjectMetaData`
 * \li a `GltfBoundingBox`
 *
 * GltfMetadata has a constructor that takes an OpenStudio Model. For there, it sets a few members like the northAxis, and will dispatch ModelObjects
 * to its supporting classes:
 *
 * \li PlanarSurfaceGroups are dispatched to the GltfBoundingBox to calculate the glTF boundingbox
 * \li ThermalZone, SpaceType, AirLoopHVAC, BuildingStory, BuildingUnit, DefaultConstructionSet objects are sent to the GltfModelObjectMetaData class
 *     to store attributes such as the name, handle, iddObjectType, multiplier, etc
 *
 * It has a function that exports to a `tinygltf::Value::Object` and it has a constructor that can do the reverse: read from a `tinygltf::Value`.
 * It dispatches to its supporting classes which are able to the do the same thing.
 **/

}  // namespace gltf
}  // namespace openstudio
