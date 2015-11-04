/** \mainpage OpenStudio ModelEditor
 *
 *  The OpenStudio Editor graphically exposes the OpenStudio Building Model.
 *
 *  \section introduction_modeleditor Introduction
 *
 *  The ModelEditor provides a comprehensive GUI for model definition,
 *  model object browsing, simulation management, simulation verifications,
 *  and results presentation.  A natural workflow is presented to make
 *  usage more intuitive but it is not rigidly enforced, allowing the user
 *  to navigate as desired.  The ModelEditor exposes EnergyPlus files in
 *  both hierarchical tree views and class-based list views to effectively
 *  present model while imposing formatting provided by the BuildingModel,
 *  thereby reducing EnergyPlus file creation time, while improving the
 *  quality of created models.  The ModelEditor utilizes the BuildingModel
 *  API, accessing BuildingModel functionality and imposing its
 *  requirements regarding data presentation (i.e. combo box or text box)
 *  and modifications (ex: can a tree node be added).  The ModelEditor is
 *  designed with reuse in mind, and the expectation of being a sub-widget
 *  of ProjectEditor.  The GUI is based on Qt, an open source,
 *  cross-platform framework with strong internationalization capabilities.
 *
 *  \section model_views Model Views
 *
 *  <b>Hierarchical tree view:</b> all objects are displayed in a hierarchical
 *  tree view with nodes determined by the BuildingModel's parent/child
 *  metadata.  Orphan objects (i.e. an object with no parent) are also
 *  displayed in the tree view.  As an additional note, orphan objects
 *  may have children of their own.
 *
 *  <b>Class view:</b> all objects are displayed in a list view, with no
 *  consideration as to parent/child relationships.  Several sorting
 *  criterion are envisioned, such as 1) IDF order, and 2) alphabetical
 *  order.
 *
 *  <b>Detail view:</b> an object selected in either the tree view or the
 *  class view will have detailed information of its consist displayed
 *  in this pane.
 *
 *  <b>Construction view:</b> construction sets, constructions, and material
 *  relationships will be displayed in three parallel list views,
 *  highlighting related constructions and materials between sets.
 *
 *  <b>Diff view:</b> if two or more models are compared, the resultant view
 *  would highlight model difference.  The view paradigm is similar to
 *  Visual Diff, although the diffs generated would be by object
 *  definition, not by order encountered in the various IDFs.
 *
 *  <b>Error view:</b> to better ensure robust model and IDF creation,
 *  BuildingModel's IDD rule interpretation will be leveraged to
 *  analyze the user created model prior to IDF creation.  Similar
 *  to an XML schema verification tool, the error view will clearly
 *  highlight BuildingModel identified issues, and then provide
 *  additional tools to aid the user in repairing these deficiencies.
 *
 *  \section functionality Functionality
 *
 *  <b>Search:</b> the ModelEditor will provide search functionality,
 *  highlighting an active culling of available object options as the
 *  search string becomes increasingly defined.  Additionally, search
 *  strings will be persisted for future usage.
 *
 *  <b>Cross-referencing:</b> the ModelEditor will provide logical options
 *  (as defined by BuildingModel) in the pull down lists that reference
 *  objects.
 *
 *  <b>Intra-Model / Inter-Model:</b> data modification methods within a model,
 *  or between models, will be similar in the ends, but may differ by user
 *  required workflow and underlying business logic.  Complexities of
 *  simultaneous multiple model modifications will also impose additional
 *  requirements on both users and business logic.
 *
 *  <b>Rules-based modifications:</b> all model field creation / modification
 *  will be subject to evaluation by the BuildingModel.  As additional
 *  functionality is added to the BuildingModel, additional rules will be
 *  imposed on model modification and improved model robustness will result.
 *
 *  <b>Singular / Multiple object selection:</b> the user is provided maximum
 *  flexibility by being able to select single objects, multiple consecutive
 *  objects, or multiple non-consecutive objects.
 *
 *  <b>Add:</b> the user may add one or more objects to the model.  In the tree
 *  view, these objects will be parented to the node to which they are added.
 *  In the class view, all added objects are orphans, as described above.
 *
 *  <b>Delete:</b> the user may delete one or more objects from the model.  In
 *  the tree view, all the deleted object(s) will also have their respective
 *  children deleted too.   In the class view, all objects belonging to a
 *  deleted object will remain but become orphans.
 *
 *  <b>Copy:</b> the user may select single or multiple (contiguous or
 *  non-contiguous) objects to copy to a model.
 *
 *  <b>Paste:</b> the user may choose where to paste the objects held in the
 *  copy buffer.  Similar to the add functionality, tree view modifications
 *  will result in automatic parenting and class view will result in
 *  additional orphans.   Additionally, in the tree view, all copied
 *  objects will have copies of their children too.
 *
 */

/** \namespace modeleditor
 *
 *  Contains the OpenStudio Model Editor
 */
