namespace openstudio {
namespace energyplus {

  /** \mainpage OpenStudio EnergyPlus Translator
 *
 *  The EnergyPlus Translator module provides the capability to convert OpenStudio models
 *  (OSMs) to EnergyPlus IDF format, and the capability to convert EnergyPlus IDFs to OSMs.
 *  Converting from an OSM to an IDF is referred to as forward translation; converting an
 *  IDF to OSM format is called reverse translation.
 *
 *  \li ForwardTranslator (OSM to IDF)
 *  \li ReverseTranslator (IDF to OSM)
 *
 *  \section forward_translator OpenStudio Model to EnergyPlus IDF
 *
 *  The ForwardTranslator includes two significant functions:
 *  ForwardTranslator::translateModelObject and ForwardTranslator::translateModel. These
 *  functions allow translation of a complete OpenStudio model or only part of a model.
 *  It is important to understand the depth of the work done by the forward translator.
 *  When ForwardTranslator::translateModelObject is called, the specified model object, and
 *  possibly also some other related objects, will be translated. Exactly which related model
 *  objects get translated is defined by the ModelObject::clone method for a particular object.
 *  Behind the scenes, translateModelObject first clones the model object to be translated into
 *  a blank Model. This entire model is then translated to IDF. This design is intended to
 *  establish consistency of model object boundaries.  It can loosely be thought that translating
 *  a model object to EnergyPlus IDF is like cloning a model object into an EnergyPlus Workspace.
 *  This is only partially correct, however, because the translator is free to change model
 *  objects and add new IDF objects as required to make valid IDF snippets.
 *
 *  Translating a complete model using ForwardTranslator::translateModel converts an entire
 *  OpenStudio model to EnergyPlus IDF format. The intent is to retain all relationships between
 *  model objects, and as a result translateModel does not clone or sever relationships before
 *  translating.
 *
 *  \section reverse_translator EnergyPlus IDF to OpenStudio Model
 *
 *  The ReverseTranslator has one primary function: ReverseTranslator::translateWorkspace. It
 *  translates a Workspace following the (current version of the) EnergyPlus IDD into OSM format.
 *  At this time, the process can be fairly lossy, especially with regards to HVAC systems.
 *  ReverseTranslator::warnings, ReverseTranslator::errors and
 *  ReverseTranslator::untranslatedIdfObjects can be used to ascertain which objects were not
 *  translated or otherwise lost information.
 */

}  // namespace energyplus
}  // namespace openstudio
