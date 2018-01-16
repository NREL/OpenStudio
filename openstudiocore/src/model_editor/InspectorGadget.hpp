/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODELEDITOR_INSPECTORGADGET_HPP
#define MODELEDITOR_INSPECTORGADGET_HPP

#include <QComboBox>
#include <QEvent>
#include <QWidget>

#include "../model/AccessPolicyStore.hpp"
#include "../model/ModelObject.hpp"

#include "ModelEditorAPI.hpp"

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

#include "../utilities/idd/IddField.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/Workspace_Impl.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"

#include <string>

class QDoubleSpinBox;
class QErrorMessage;
class QPushButton;
class QScrollArea;
class QSpinBox;
class QVBoxLayout;

class ComboHighlightBridge;

class MODELEDITOR_API IGWidget : public QWidget, public Nano::Observer
{
  public:

  IGWidget(QWidget * parent = nullptr);

  QSize sizeHint() const override;
};

class IGComboBox : public QComboBox
{
  public:

  IGComboBox( QWidget * parent = nullptr )
    : QComboBox( parent )
  {
  }

  protected:

  bool event ( QEvent * e ) override
  {
    if( e->type() == QEvent::Wheel )
    {
      return false;
    }
    else
    {
      return QComboBox::event(e);
    }
  }
};

/*! \brief InspectorGadget will take a ModelObejct (or WorkspaceObject) and display it attributes
 *
 * InsepctorGadget interrogates objects and displays their fields/attributes based on the values
 * contained in the AccessPolicyStore. When a new model is passed into the IG, it will delete all
 * its old QWidgets and create new ones.
 *
 * AccessPolicy::FREE Real and Integers get displayed as spinboxes
 * AccessPolicy::LOCKED Real and Integer and all Alpha get displayed as labels.
 * Choice is displayed as a ComboBox
 *
 */
class MODELEDITOR_API InspectorGadget : public QWidget, public Nano::Observer
{

  Q_OBJECT

public:

  friend class IGLineEdit;
  friend class IGDSpinBox;

  enum FLOAT_DISPLAY { FIXED, SCIENTIFIC, UNFORMATED };

  enum UNIT_SYSTEM { SI, IP };

  /*! constructor
   *
   * \param parent The GUI object that owns IG
   * \param indent The indentation for IGChildFrame
   * \param bridge pass in a bridge here if you need signals to get out of the IG (or the IG's children)
   *
   *
   * \sa IGChildFrame
   * \sa layoutModel
   */
  InspectorGadget(QWidget* parent = nullptr, int indent = 0, ComboHighlightBridge* bridge=nullptr);
  /*! destructor
   *
   * The really shouldn't need to call this, the parent Widget ought to delete the IG.
   *
   */
  virtual ~InspectorGadget();

  /*! \brief lays out the WorkspaceObj
   *
   * \param workObj the current workspace
   * \param force forces a layout even if we just laid out this modelObj
   * \param recursive if true, delete the children and rebuild them
   * \param locked set all the FREE items to LOCKED
   * \param hideChildren shows or hides the children
   *
   * This call lays out a model. It will ask the Model for its AttributeVector and use that
   * to decide what to display. If the ModelObject is a ParentChildObejct, then this
   * function will create an IGChildFrame and that child calls layoutItems for
   * its frame.
   *
   * Calling this method a 2nd time will delete everything that was previously built and
   * regenerate the QWidgets.
   *
   * Recursive only effects anything if you are sending in the same workObj, if you pick a different one, the
   * children are ALWAYS deleted and rebuilt. (because they are different children!)
   *
   * If you are calling this from a place where the user should not be allowed to edit the fields, set the lock
   * parameter to true
   *
   */
  void layoutModelObj(openstudio::WorkspaceObject& workObj,
                      bool force=false,
                      bool recursive=true,
                      bool locked=false,
                      bool hideChildren=false);

  /*! \brief sets the display precision for number fields
   *
   * \param prec the # of significant figures to display
   * \param dispType how to format the floating point number
   *
   * If you pass in a zero to this function, IG will not format the number in anyway,
   * meaning you will see exactly the string that was typed into the IDF/OSM file. (Default behavior)
   * If you pass in a non zero number, all number fields will display that number of significant figures
   * in scientific notation.
   */
  void setPrecision( unsigned int prec, FLOAT_DISPLAY dispType );

  void setUnitSystem(const UNIT_SYSTEM unitSystem);

  void removeWorkspaceObject(const openstudio::Handle &); // Middleman nano slot to emit QT signal to simulate signal chaining

public slots:

    void toggleUnits(bool displayIP);

/*! \brief lays out the last object again.
   *
   *\param recursive if true, apply the change to the children
   * Use this if you change a config parameter that controls how the layout happens.
   *
   */
  void rebuild(bool recursive);

  /*! \brief removes everything from the IG
   *
   *\param recursive if true, deletes the children.
   * Removes all the information from the IG. Call this if the object being inspected
   * gets deleted.
   *
   */
  void clear(bool recursive);

  /*! \brief changes the color of widgets
   *
   * When widget are built, if the Field is required but there is no value and no default value
   *(which in a way, is bad idf) the IG will put 0 or 0.0 in there for you, but color it red
   * to point out that you should address this. Once you set a value, this function is automatically
   * called for you and sets the color back to black.
   *
   */
  void IGdefaultRemoved(const QString&);

  void IGvalueChanged(const QString&);

  void IGcommentChanged(const QString&);

  void IGautosize(bool toggled);

  /*! \brief show/hide comment fields
   *
   * \param showComments if true, show comments, if false, hide them
   *
   * IdfObjects have comments about their fields. (and about themselves)
   * This function allows you to toggle weather or not you wish IG to show those comments.
   * The Default is not to show them.
   */
  void commentConfig(bool showComments);

  void setPrec();

  void addExtensible();

  void removeExtensible();
  /*!
   * Create all fields in the IDD. IDFObjects can be instantiated with some
   * IDD fields null. (they are not 'required' fields. Once you call this function,
   * all fields possible will be created and presented to the user.
   */
  void createAllFields();

  void showAllFields(bool state);

  void setRecursive( bool recursive);

 signals:

  void nameChanged(QString);

  void toggleUnitsClicked(bool);

  /*!
   *This signal is emitted when the IG changes the model. If your program
   *uses IG, then you should connect to this so that you know if the file needs saving or not
   *
   */
  void dirty();

  void workspaceObjectRemoved(const openstudio::Handle &);


protected slots:

  void onWorkspaceObjectChanged();

  void onTimeout();

  void onWorkspaceObjectRemoved(const openstudio::Handle &);

protected:

/*! \brief constructor
    \param workspaceObj the current workspace
    \param indent indentation for the child frame
    \param bridge pass in a bridge here if you need signals to get out of the IG (or the IG's children)
    \param precision sets the number of sigfigs to display
    \param style sets the style of floating point display
    \param showComments show or hide idf comment field
    \param showAllFields show or hide all fields
    \param recursive recursively create and show fields
    \param locked if true, lock all fields the ACCESS_POLICY sets to FREE
    If model has children, those children will get their own InspectorGadget, indent
    will be passed again to that constructor so the frames will nest.
  */
  InspectorGadget( openstudio::WorkspaceObject& workspaceObj,
                   int indent,
                   ComboHighlightBridge* bridge,
                   int precision,
                   FLOAT_DISPLAY style,
                   bool showComments,
                   bool showAllFields,
                   bool recursive,
                   bool locked);
  /*!
    \param layout the layout to attach the items to
    \param parent the widget that owns all the items that will be created.
    \param hideChildren show or hide children (hidden by default)


    This function is a big loop over the attributes or fields*, it does the core of the work for
    deciding what exact widgets we should build. IGChildFrame uses this function to fill
    out its child frame.

    *It depends on weather or not you called layoutModel with a ModelObject or a WorkspaceObj
    */
  virtual void layoutItems( QVBoxLayout* layout,
                            QWidget* parent,
                            bool hideChildren=false);

  void parseItem( QVBoxLayout* layout,
                  QWidget* parent,
                  openstudio::IddField& field,
                  const std::string& name,
                  const std::string& curVal,
                  openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                  int index,
                  const std::string& comment,
                  bool exists);

  void layoutText( QVBoxLayout* layout,
                   QWidget* parent,
                   openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                   const std::string& val,
                   int index,
                   const std::string& comment);

  virtual void layoutText( QVBoxLayout* layout,
                           QWidget* parent,
                           openstudio::IddField& field,
                           openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                           const std::string& name,
                           const std::string& curVal,
                           int index,
                           const std::string& comment,
                           bool exists,
                           bool number,
                           bool real=false);

  void layoutComboBox( QVBoxLayout* layout,
                       QWidget* parent,
                       openstudio::IddField& field,
                       openstudio::IddFieldProperties& properties,
                       const std::string& name,
                       const std::string& curVal,
                       int index,
                       const std::string& comment,
                       bool exists);

  void createExtensibleToolBar( QVBoxLayout* layout,
                                QWidget* parent,
                                const openstudio::IddObjectProperties& props );

  void checkRemoveBtn( QPushButton* btn );

  void stripchar(std::string& strip,char c);

  QVBoxLayout * m_layout;
  QScrollArea * m_scroll;
  QWidget *     m_deleteHandle; // we need a parent for everything in IG so we can delete it all.

  int m_indent; // how much we indent each IGChildFrame widget by
  // TODO replace below by m_workspaceObjs[0]
  openstudio::OptionalWorkspaceObject m_workspaceObj;
  //std::vector<openstudio::OptionalWorkspaceObject>& m_workspaceObjs;
  QErrorMessage * m_errorMessage;
  bool m_locked;
  bool m_stretch; // shall we put a stretch in after we layout items?

  ComboHighlightBridge * m_comboBridge;

  REGISTER_LOGGER("InspectorGadget");

  static const char * s_indexSlotName;

  bool m_objectHasName;
  boost::optional<int> m_nameIndex;
  bool m_lastLocked;
  bool m_lastHideChildren;
  bool m_showComments;
  bool m_showAllFields;
  bool m_recursive;
  unsigned int m_precision;
  FLOAT_DISPLAY m_floatDisplayType;
  UNIT_SYSTEM m_unitSystem;
  bool m_workspaceObjectChanged;

  typedef std::map <openstudio::model::ModelObject,InspectorGadget*> MODELMAP;
  MODELMAP m_childMap;

  void connectWorkspaceObjectSignals() const;

  void disconnectWorkspaceObjectSignals() const;

private:

  void connectSignalsAndSlots();
};

#endif // MODELEDITOR_INSPECTORGADGET_HPP
