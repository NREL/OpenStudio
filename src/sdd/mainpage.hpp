namespace openstudio {
namespace sdd {
  /** \mainpage OpenStudio SDD Translator
*
* OpenStudio SDD Translator is a conversion utility between SDD data files and
* OpenStudio Model (osm files).  The translation is supported in both
* directions.  The utility that converts from SDD format to the OpenStudio Model
* is called SDD ReverseTranslator.  Similarly, SDD ForwardTransltor is the
* utility that converts from an OpenStudio Model to SDD format.  The conversion
* from SDD to OpenStudio is comprehensive, encompassing all aspects of the
* building which relate to energy modeling, including geometry, loads, and HVAC.
* Translation from OpenStudio to SDD is partial, supporting only the geometric
* aspects of the building.
*
* The implementation of both the forward and reverse translators is a series of
* procedural functions that are called hierarchically.  Functions that translate
* high level building concepts are invoked first, and these functions in turn
* call other functions to translate lower level building concepts.  The process
* begins at the project level, to the building, spaces, zones, space loads, and
* so forth.
*
* ReverseTranslator.hpp defines the complete set of (private) translator
* functions for converting an SDD file to an OpenStudio Model.  The method
* ReverseTranslate::loadModel(), is the public interface for carrying out the
* translation.  The method simply accepts a path to the SDD file that is to be
* translated.
*
* ForwardTransltor.hpp defines the complete set of translator functions for
* converting an OpenStudio Model to an SDD file.  Translation is invoked by
* calling ForwardTranslate::modelToSDD(), passing the model to translate and the
* path to save the SDD file.
*
*/

}  // namespace sdd
}  // namespace openstudio
