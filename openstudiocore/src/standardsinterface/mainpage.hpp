namespace openstudio {
namespace standardsinterface {

/** \mainpage OpenStudio StandardsInterface
 *
 *  \details The purpose of the OpenStudio StandardsInterface is to functionally separate building 
 *  energy simulation software from standards software. From a software development perspective, 
 *  code separation enables independent development, and greater interoperability; One should be 
 *  able to swap out the implementation of one of the functionalities with minimal disturbance to 
 *  the other. From a user's perspective, the Standards Interface lets standards developers write 
 *  rules in a semi-natural language, using simulation engine-agnostic controlled vocabulary terms, 
 *  while providing access to state-of-the-art simulation tools on the backend. It lets standards 
 *  developers and building energy modelers use their own languages, which are mappable because of 
 *  their similarities, but differ because of the relative distance between those communities and 
 *  various building energy models (ASHRAE 90.1, Title 24, EnergyPlus, EQuest, etc.).
 *
 *  To these ends, the AgnosticInterface is put forward as the layer for facilitating communications
 *  between standards and modeling software. The intention is for rules engines to build to this 
 *  abstract interface class, only specializing code to particular simulation engines and data 
 *  formats where absolutely necessary. If this suggestion is followed, it should then be 
 *  straightforward to reuse almost all of a rules engine's code when adding support for a new 
 *  simulation engine. (This flexibility is not free, however. It assumes that the AgnosticInterface
 *  has been implemented for the new simulation engine.)
 *
 *  OpenStudio also provides implementations of the AgnosticInterface that map between the 
 *  DataDictionary (which currently contains terms relevant to the California Energy Commission's 
 *  (CEC's) Title 24 Standard) and the OpenStudio building model (model::Model). 
 *  OpenStudioStandardsInterface provides most all of this mapping. Title24Interface initializes 
 *  a few Title 24 specific items on construction, but otherwise is identical to its base class. 
 *  For instance, Title24Interface specifies that the CEC climate zone designations should be used
 *  by default (rather than ASHRAE's). 
 *
 *  \section quick_links Quick Links
 *
 *  \li DataDictionary
 *  \li AgnosticInterface
 *  \li OpenStudioStandardsInterface
 *  \li Title24Interface
 *
 */

} // standardsinterface
} // openstudio
