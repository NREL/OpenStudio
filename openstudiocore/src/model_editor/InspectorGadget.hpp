/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
#ifndef MODELEDITOR_INSPECTORGADGET_HPP
#define MODELEDITOR_INSPECTORGADGET_HPP

#include <QWidget>
#include <QComboBox>
#include <QEvent>
#include "../model/AccessPolicyStore.hpp"
#include "../model/ModelObject.hpp"
#include "ModelEditorAPI.hpp"
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

class IGWidget : public QWidget
{
  public:
  
  IGWidget( QWidget * parent = nullptr )
    : QWidget(parent)
  {
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
  }

  QSize sizeHint() const 
  {
    return QSize(200,QWidget::sizeHint().height());
  }
};

class IGComboBox : public QComboBox
{
  public:

  IGComboBox( QWidget * parent = nullptr )
    : QComboBox( parent )
  {
  }

  protected:

  bool event ( QEvent * e )
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
class MODELEDITOR_API InspectorGadget : public QWidget
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
   * \param modelObj the modelObj to layout
   * \param force forces a layout even if we just laid out this modelObj
   * \param if true, delete the children and rebuild them
   * \param locked set all the FREE items to LOCKED
   *
   * This call lays out a model. It will ask the Model for its AttributeVector and use that
   * to decide what to display. If the ModelObject is a ParentChildObejct, then this
   * function will create an IGChildFrame and that child calls layoutItems for
   * its frame.
   *
   * Calling this method a 2nd time will delete everything that was previously built and
   * regenerate the QWidgets.
   *
   * Recursive only effects anything if you are sending in the same modelObj, if you pick a different one, the
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

protected slots:

  void onWorkspaceObjectChanged();

  void onTimeout();

  void onWorkspaceObjectRemoved();

protected:

/*! \brief constructor
    \param parent the owner of all the stuff we just made.
    \param model the model to get data from
    \param indent indentation for the child frame
    \param bridge pass in a bridge here if you need signals to get out of the IG (or the IG's children)
    \param precision sets the number of sigfigs to display
    \param style sets the style of floating point display
    \param showComments show or hide idf comment field
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
