namespace openstudio {
namespace osversion {

  /** \mainpage OpenStudio OSVersion
 *
 *  The openstudio::osversion namespace currently contains one class, VersionTranslator, provided
 *  to enable backward compatibility. In particular, OpenStudio models (.osm) and components
 *  (.osc) made with previous versions of OpenStudio Version 0.7.0 or newer can be updated to the
 *  latest version by opening them with VersionTranslator::loadModel or
 *  VersionTranslator::loadComponent, rather than with Model::load or Component::load.
 *
 *  The VersionTranslator is already integrated into the OpenStudio Application, and is also
 *  available through the Ruby and C# bindings.
 */

}  // namespace osversion
}  // namespace openstudio
