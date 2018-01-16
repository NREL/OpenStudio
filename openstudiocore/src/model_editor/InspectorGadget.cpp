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

#include "InspectorGadget.hpp"

#include "BridgeClasses.hpp"
#include "IGLineEdit.hpp"
#include "IGPrecisionDialog.hpp"
#include "IGSpinBoxes.hpp"

#include "../model/Model.hpp"
#include "../model/ParentObject.hpp"
#include "../model/ParentObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include "../utilities/idd/IddField.hpp"
#include "../utilities/idd/IddFieldProperties.hpp"
#include "../utilities/idd/IddKey.hpp"
#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddObjectProperties.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/QuantityConverter.hpp"

#include <boost/numeric/conversion/cast.hpp>

#include <float.h>
#include <iostream>
#include <limits.h>
#include <vector>

#include <QDoubleValidator>
#include <QErrorMessage>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>

using namespace openstudio;
using namespace openstudio::model;
using std::cout;
using std::endl;
using std::string;
using std::vector;

const char* InspectorGadget::s_indexSlotName="indexSlot";
//const char* FIELDS_MATCH = "fields match";

struct ModelEditorLibResourceInitializer{
  ModelEditorLibResourceInitializer()
  {
    Q_INIT_RESOURCE(modeleditorlib);
  }
};
static ModelEditorLibResourceInitializer __modelEditorLibResourceInitializer__;

IGWidget::IGWidget(QWidget* parent):
  QWidget(parent)
{
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
}

QSize IGWidget::sizeHint() const
{
  return QSize(200, QWidget::sizeHint().height());
}

InspectorGadget::InspectorGadget(QWidget* parent, int indent,ComboHighlightBridge* bridge):
  QWidget(parent),
  m_layout(nullptr),
  m_deleteHandle(nullptr),
  m_indent(indent),
  m_locked(false),
  m_stretch(true),
  m_comboBridge(bridge),
  m_objectHasName(false),
  m_nameIndex(boost::none),
  m_showComments(false),
  m_showAllFields(true),
  m_recursive(false),
  m_precision(1000),
  m_floatDisplayType( UNFORMATED ),
  m_unitSystem(IP),
  m_workspaceObjectChanged(false)
  //m_workspaceObjs(std::vector<openstudio::OptionalWorkspaceObject>&())
{
  m_layout = new QVBoxLayout(this);
  m_layout->setSpacing(0);
  m_layout->setMargin(0);
  setLayout(m_layout);
  m_scroll = new QScrollArea(this);
  m_scroll->setWidgetResizable(true);
  m_scroll->setFrameStyle( QFrame::NoFrame );
  m_errorMessage = new QErrorMessage(this);
  m_layout->addWidget(m_scroll);
  setContentsMargins(0,0,0,0);
  connectSignalsAndSlots();
}

InspectorGadget::InspectorGadget( WorkspaceObject& workspaceObj,
                                  int indent,
                                  ComboHighlightBridge* bridge,
                                  int precision,
                                  FLOAT_DISPLAY style,
                                  bool showComments,
                                  bool showAllFields,
                                  bool recursive,
                                  bool locked):
  QWidget(nullptr),
  m_layout(nullptr),
  m_deleteHandle(nullptr),
  m_indent(indent),
  m_locked(locked),
  m_stretch(true),
  m_comboBridge(bridge),
  m_objectHasName(false),
  m_nameIndex(boost::none),
  m_showComments(showComments),
  m_showAllFields(showAllFields),
  m_recursive(recursive),
  m_precision(precision),
  m_floatDisplayType( style ),
  m_unitSystem(IP),
  m_workspaceObjectChanged(false)
  //m_workspaceObjs(std::vector<openstudio::OptionalWorkspaceObject>&())
{
  m_layout = new QVBoxLayout(this);
  m_layout->setSpacing(0);
  m_layout->setMargin(0);
  setLayout(m_layout);
  m_scroll = nullptr;
  m_errorMessage = new QErrorMessage(this);
  setContentsMargins(0,0,0,0);
  layoutModelObj(workspaceObj,false,m_recursive,m_locked);
  connectSignalsAndSlots();
}

InspectorGadget::~InspectorGadget()
{
  clear(true);
}

void InspectorGadget::connectSignalsAndSlots()
{
  connect(this, &InspectorGadget::toggleUnitsClicked, this, &InspectorGadget::toggleUnits);
}

void InspectorGadget::rebuild(bool recursive)
{
  if(m_workspaceObj.is_initialized()){
    layoutModelObj(*m_workspaceObj,true,recursive,m_lastLocked,m_lastHideChildren);
  }
  return;
}

void InspectorGadget::layoutModelObj(openstudio::WorkspaceObject& workspaceObj,
                                     bool force,
                                     bool recursive,
                                     bool locked,
                                     bool hideChildren)
{
  disconnectWorkspaceObjectSignals();

  if (m_lastLocked != locked){
    force = true;
    m_lastLocked = locked; //::rebuild will need this, rebuild always sets force to true
  }

  if (m_lastHideChildren != hideChildren){
    force = true;
    m_lastHideChildren = hideChildren;
  }

  if(m_workspaceObj){
    if( workspaceObj.handle() == m_workspaceObj->handle() ){
      if(!force){
        //DUH! we are already displaying it!
        connectWorkspaceObjectSignals();
        return;
      }else{
        //we have the same object, we are forcing a rebuild.... obey the recursive flag
        clear(recursive);
      }
    }else{
      //we have a NEW object, the recursive flag is stupid because its a new object, nuke everything
      clear(true);
    }
  }

  m_workspaceObj = workspaceObj;

  m_objectHasName = workspaceObj.name();
  if (m_objectHasName){
    IddField iddField = workspaceObj.iddObject().getField(0).get();
    std::string fieldName = iddField.name();
    if (istringEqual(fieldName, "Name")){
      m_nameIndex = 0;
    }else{
      m_nameIndex = 1;
    }
  }

  m_deleteHandle = new IGWidget(this);
  m_deleteHandle->setContentsMargins(0,0,0,0);
  m_deleteHandle->setObjectName("IG");

  auto layout = new QVBoxLayout(m_deleteHandle);
  layout->setSpacing(0);
  layout->setMargin(0);
  m_deleteHandle->setLayout( layout );
  m_locked=locked;
  layoutItems(layout,m_deleteHandle,hideChildren);

  if(m_scroll){
    m_scroll->setWidget(m_deleteHandle);
  }else{
    m_layout->addWidget(m_deleteHandle);
  }
  m_deleteHandle->show();

  connectWorkspaceObjectSignals();
}

void InspectorGadget::clear(bool recursive)
{
  disconnectWorkspaceObjectSignals();

  if(m_deleteHandle)
  {
    QLayout* layout = m_deleteHandle->layout();
    if(m_workspaceObj->optionalCast<model::ModelObject>())
    {
      if( !recursive )
      {
        for(const auto & elem : m_childMap)
        {
          layout->removeWidget(elem.second);
          elem.second->setParent(nullptr);
        }
      }
      else
      {
        m_childMap.clear();
      }
    }

    // delete widgets before resetting m_workspaceObj so can handle editingFinished signals from any text boxes
    delete m_deleteHandle;
    m_deleteHandle=nullptr;
  }

  // This line is commented out to prevent a crash when displaying the Inspector Gadget
  // within SketchUp 2016.  We have no idea why this works or what repercussions it may cause
  //m_workspaceObj.reset();
}

void InspectorGadget::setPrecision( unsigned int prec, FLOAT_DISPLAY dispType )
{
  m_floatDisplayType=dispType;
  m_precision=prec;
  rebuild(true);
}

void InspectorGadget::layoutItems(QVBoxLayout* masterLayout,
                                  QWidget* parent,
                                  bool hideChildren)
{
  IddObject iddObj = m_workspaceObj->iddObject();
  string comment = m_workspaceObj->comment();

  if( comment.size() >= 1)
  {
    string::size_type i = comment.find('!');
    if(i!=string::npos)
    {
      comment.erase(0,i+1);
    }
  }

  auto layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setMargin(0);
  auto hlayout = new QHBoxLayout();
  hlayout->setSpacing(0);
  hlayout->setMargin(0);
  masterLayout->addLayout( hlayout );
  hlayout->addLayout(layout );
  layoutText(layout,parent,AccessPolicy::LOCKED,iddObj.type().valueDescription().c_str(),-1,comment);

  AccessPolicy::ACCESS_LEVEL level;
  const AccessPolicy* pAccessPolicy = AccessPolicyStore::Instance().getPolicy( iddObj.type() );
  for(unsigned int i=0,iend=m_workspaceObj->numFields();i<iend;++i)
  {
    openstudio::IddField field( *(iddObj.getField(i)) );
    if(pAccessPolicy)
    {
      level = pAccessPolicy->getAccess(i);
    }
    else
    {
      level=AccessPolicy::FREE;
    }

    if( m_locked && (level==AccessPolicy::FREE) )
    {
      level = AccessPolicy::LOCKED;
    }

    comment = *(m_workspaceObj->fieldComment(i,true));
    // starting at 1 because the "base" obj is at 0
    //for(unsigned it=1; i<m_workspaceObjs.size(); i++)
    //{
    //  if(comment == m_workspaceObjs[it]->fieldComment(i,true)){
    //    // keep track of a field set to the value FIELDS_MATCH,
    //    // if it changed, change the respective elements in any
    //    // other
    //    comment == FIELDS_MATCH;
    //    break;
    //  }
    //}

    //Strip off prefix of "!"
    if( comment.size() >= 1)
    {
      string::size_type i = comment.find('!');
      if(i!=string::npos)
      {
        comment.erase(0,i+1);
      }
    }
    parseItem( layout, parent, field, field.name(), *(m_workspaceObj->getString(i,true)), level, i, comment, true);
  }

  const IddObjectProperties& props = m_workspaceObj->iddObject().properties();
  if(pAccessPolicy){
    unsigned numFields = iddObj.numFields();
    unsigned extensibleGroupSize = iddObj.extensibleGroup().size();
    for (unsigned i = numFields; i < numFields + extensibleGroupSize; ++i){
      level = pAccessPolicy->getAccess(i);
      if (level != AccessPolicy::HIDDEN){
        // only call if any extensible fields are visible
        createExtensibleToolBar( layout, parent, props );
        break;
      }
    }
  }else{
    createExtensibleToolBar( layout, parent, props );
  }


  if( m_showAllFields )
  {
    string empty;
    for(unsigned int existCount = m_workspaceObj->numNonextensibleFields(),possibleCount = iddObj.numFields();existCount!=possibleCount;++existCount)
    {
      openstudio::IddField field( *(iddObj.getField(existCount)) );
      parseItem( layout, parent, field, field.name(),empty,AccessPolicy::LOCKED,existCount,empty,false);
    }
  }
  // model only follows...
  OptionalParentObject p = m_workspaceObj->optionalCast<ParentObject>();
  if( p && (!hideChildren))
  {
    ModelObjectVector cvec=p->children();
    for(auto & elem : cvec)
    {
      auto igChildItr = m_childMap.find(elem);
      if( igChildItr != m_childMap.end() )
      {
        InspectorGadget* igchild = igChildItr->second;
        layout->addWidget( igchild );
      }
      else
      {
        bool comment=false,fields=true;
        if(m_recursive)
        {
          comment = m_showComments;
          fields = m_showAllFields;
        }
        auto igChild = new InspectorGadget(elem,
                                           m_indent,
                                           m_comboBridge,
                                           m_precision,
                                           m_floatDisplayType,
                                           comment,
                                           fields,
                                           m_recursive,
                                           m_locked);

        igChild->setUnitSystem(m_unitSystem);
        layout->addWidget(igChild);
        m_childMap[elem] = igChild;
      }
    }
  } // if(p)

  if( m_stretch )
    masterLayout->addStretch();
}

void InspectorGadget::parseItem(QVBoxLayout* layout,
                                QWidget* parent,
                                openstudio::IddField& field,
                                const std::string& name,
                                const std::string& curVal,
                                openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                                int index,
                                const std::string& comment,
                                bool exists)
{
  IddFieldProperties prop = field.properties();
  if( AccessPolicy::HIDDEN == level )
  {
    return;
  }

  switch( prop.type.value() )
  {

  case IddFieldType::IntegerType:
    {
      layoutText(layout,parent,field,level,name,curVal,index,comment,exists,true,false);
      break;
    }

  case IddFieldType::RealType:
    {
      // curVal is in SI and will be set to the default value if no value is set
      std::string realVal = curVal;
      if (!field.unitsBasedOnOtherField()){
        if (m_unitSystem == IP) {
          OSOptionalQuantity q = m_workspaceObj->getQuantity(index,true,true);
          if (q.isSet()) {
            realVal = boost::lexical_cast<std::string>(q.get().value());
          }
        }
      }
      layoutText(layout,parent,field,level,name,realVal,index,comment,exists,true,true);
      break;
    }
  case IddFieldType::AlphaType:
  case IddFieldType::UnknownType:
  case IddFieldType::ExternalListType:
    {
      layoutText(layout,parent,field,level,name,curVal,index,comment,exists,false);
      break;
    }
  case IddFieldType::ObjectListType:
    {
      if(AccessPolicy::LOCKED==level)
      {
        layoutText(layout,parent,field,level,name,curVal,index,comment,exists,false);
      }
      else
      {
        layoutComboBox( layout,
                        parent,
                        field,
                        prop,
                        name,
                        curVal,
                        index,
                        comment,
                        exists);
      }
      break;
    }
  case IddFieldType::ChoiceType:
    {
      if(AccessPolicy::LOCKED==level)
      {
        layoutText(layout,parent,field,level,name,curVal,index,comment,exists,false);
      }
      else
      {
        layoutComboBox( layout,
                        parent,
                        field,
                        prop,
                        name,
                        curVal,
                        index,
                        comment,
                        exists);
      }
      break;
    }
  case IddFieldType::NodeType:
    {
      break;
    }
  case IddFieldType::URLType:
    {
      break;
    }
  case IddFieldType::HandleType:
    {
      break;
    }
  default:
    {
      LOG(Error, "InspectorGadget::parseItem has failed, unknown IddFieldType " << prop.type.valueDescription());
    }

  }
}

void InspectorGadget::layoutText( QVBoxLayout* layout,
                                  QWidget* parent,
                                  openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                                  const string& val,
                                  int index,
                                  const string& comment)
{
  auto frame = new QFrame(parent);
  frame->setContentsMargins(0,0,0,0);
  auto hbox = new QHBoxLayout();
  frame->setLayout(hbox);
  frame->setObjectName("IGHeader");
  hbox->setSpacing(0);
  hbox->setMargin(0);

  if( level == AccessPolicy::LOCKED )
  {
    string stripped(val);
    //stripchar(stripped,'_');
    QLabel* label = new QLabel( QString(stripped.c_str()), parent  );
    label->setObjectName("IGHeader");
    label->setStyleSheet("font : bold");
    // Qt::Alignment a = Qt::AlignHCenter;
    hbox->addWidget(label);

    hbox->addStretch();
  }
  else
  {
    //QLineEdit* text = new QLineEdit( QString(val.c_str()), parent  );
    IGLineEdit* text = new IGLineEdit( QString(val.c_str()), this, parent  );
    hbox->addWidget( text );
    text->setProperty(s_indexSlotName,index);

    //connect(text, &IGLineEdit::textEdited, this, &InspectorGadget::IGvalueChanged, Qt::QueuedConnection);

    connect(text, &IGLineEdit::editingFinished, text, &IGLineEdit::editDone);
    connect(text, &IGLineEdit::newValue, this, &InspectorGadget::IGvalueChanged);
  }

  QLineEdit* commentText = new QLineEdit( QString(comment.c_str()), parent  );
  commentText->setProperty(s_indexSlotName,index);
  connect(commentText, &QLineEdit::textEdited, this, &InspectorGadget::IGcommentChanged);
  if( !m_showComments )
  {
    commentText->hide();
  }
  hbox->addWidget( commentText );
  commentText->setObjectName("IDFHeaderComment");

  layout->addWidget( frame );
}

void InspectorGadget::layoutText( QVBoxLayout* layout,
                                  QWidget* parent,
                                  openstudio::IddField& field,
                                  openstudio::model::AccessPolicy::ACCESS_LEVEL level,
                                  const std::string& name,
                                  const std::string& curVal,
                                  int index,
                                  const string& comment,
                                  bool exists,
                                  bool number,
                                  bool real)
{
  openstudio::IddFieldProperties prop = field.properties();

  auto frame = new QFrame(parent);
  auto vbox = new QVBoxLayout();
  frame->setLayout(vbox);

  QLabel* label = new QLabel( QString(name.c_str()), parent  );
  label->setWordWrap( true );
  vbox->addWidget( label );

  QString val= QString::fromStdString( curVal );

  auto text = new IGLineEdit( val, this, parent );
  text->setProperty(s_indexSlotName,index);
  QLineEdit* commentText = new QLineEdit( QString(comment.c_str()), parent  );
  commentText->setProperty(s_indexSlotName,index);
  if( level == AccessPolicy::LOCKED )
  {
    text->setEnabled( false );
    commentText->setEnabled( false );
  }
  else
  {
    //connect(text, &IGLineEdit::textEdited, this, &InspectorGadget::IGvalueChanged, Qt::QueuedConnection);

    connect(text, &IGLineEdit::editingFinished, text, &IGLineEdit::editDone);
    connect(text, &IGLineEdit::newValue, this, &InspectorGadget::IGvalueChanged);

    connect(commentText, &QLineEdit::textEdited, this, &InspectorGadget::IGcommentChanged);
  }

  if( number )
  {
    string s;
    if(real)
    {
      Unit u_si;
      Unit u;

      if (!field.unitsBasedOnOtherField()){
        OSOptionalQuantity q;
        OSOptionalQuantity q_si = m_workspaceObj->getQuantity(index,true);
        if (m_unitSystem == SI) {
          q = q_si;
        } else {
          q = m_workspaceObj->getQuantity(index,true,true);
        }
        u_si = q_si.units();
        u = q.units();
      }

      //QDoubleValidator* valid = new QDoubleValidator(text);
      //if( m_floatDisplayType != SCIENTIFIC )
      //valid->setNotation( QDoubleValidator::StandardNotation );
      QRegExpValidator* valid = new QRegExpValidator( QRegExp("-?[0-9]*\\.?[0-9]*([eE][-+]?[0-9]*)?"),text);
      if(prop.minBoundType != IddFieldProperties::Unbounded)
      {
        double d = *(prop.minBoundValue);
        if (m_unitSystem == IP) {
          OptionalQuantity minQ = convert(Quantity(d,u_si),u);
          if(prop.minBoundType == IddFieldProperties::ExclusiveBound) {
            if( minQ ) {
              minQ->setValue(minQ->value() + std::numeric_limits<double>::epsilon());
            }
          }
          if (minQ) {
            text->setMin(minQ->value());
          }
        }
        else {
          if(prop.minBoundType == IddFieldProperties::ExclusiveBound) {
            d = d + std::numeric_limits<double>::epsilon();
          }
          text->setMin( d );
        }
      }
      if(prop.maxBoundType != IddFieldProperties::Unbounded)
      {
        double d = *(prop.maxBoundValue);
        if (m_unitSystem == IP) {
          OptionalQuantity maxQ = convert(Quantity(d,u_si),u);
          if( maxQ ) {
            if(prop.maxBoundType == IddFieldProperties::ExclusiveBound) {
              maxQ->setValue(maxQ->value() - std::numeric_limits<double>::epsilon());
            }
          }
          if (maxQ) {
            text->setMax(maxQ->value());
          }
        }
        else {
          if(prop.maxBoundType == IddFieldProperties::ExclusiveBound) {
            d = d - std::numeric_limits<double>::epsilon();
          }
          text->setMax( d );
        }
      }

      text->setValidator( valid );

      std::stringstream ss;
      ss << u;
      s = ss.str();
      s = formatUnitString(s,DocumentFormat::XHTML);
    }
    else
    {
      auto valid = new QIntValidator(text);

      if(prop.minBoundType != IddFieldProperties::Unbounded)
      {
        valid->setBottom( boost::numeric_cast<int>(*(prop.minBoundValue)));
      }
      if(prop.maxBoundType != IddFieldProperties::Unbounded)
      {
        valid->setTop( boost::numeric_cast<int>(*(prop.maxBoundValue)) );
      }
      text->setValidator( valid );
    }

    if (s == "m(m<sup>2</sup>)") {
      s = "cm<sup>2</sup>";
    }
    QLabel* units = new QLabel( s.c_str(), parent );
    units->setTextFormat(Qt::RichText);

    auto hardSizedLayout = new QHBoxLayout();

    if( prop.autosizable)
    {
      auto autoSizedLayout = new QHBoxLayout();
      QRadioButton* hardSizedRadio = new QRadioButton(tr("Hard Sized"), parent );
      QRadioButton* autosizedRadio = new QRadioButton(tr("Autosized"), parent );
      autosizedRadio->setProperty(s_indexSlotName,index);

      if( level == AccessPolicy::LOCKED )
      {
        hardSizedRadio->setEnabled(false);
        autosizedRadio->setEnabled(false);
      }

      bool isAutosized = istringEqual(curVal, "autosize");
      if (curVal.empty() && prop.stringDefault && istringEqual(prop.stringDefault.get(), "autosize")){
        isAutosized = true;
      }

      if(isAutosized)
      {
        autosizedRadio->setChecked(true);
        hardSizedRadio->setChecked(false);
        text->setText("");
        text->setEnabled(false);
      }
      else
      {
        autosizedRadio->setChecked(false);
        hardSizedRadio->setChecked(true);
        text->setEnabled(true);
      }

      QLabel* autolabel = new QLabel("Autosize", parent );
      autolabel->setEnabled(false);

      autoSizedLayout->addWidget( autosizedRadio );
      autoSizedLayout->addWidget( autolabel );

      hardSizedLayout->addWidget( hardSizedRadio );
      hardSizedLayout->addWidget( text );
      hardSizedLayout->addWidget( units );

      // enable the hard size field
      connect(hardSizedRadio, &QRadioButton::toggled, text, &IGLineEdit::setEnabled);
      // when clicking on the hard size radio set text
      connect(hardSizedRadio, &QRadioButton::clicked, text, &IGLineEdit::hardsizeClicked);
      // set the field to auto size
      connect(autosizedRadio, &QRadioButton::toggled, this, &InspectorGadget::IGautosize);
      vbox->addLayout(hardSizedLayout);
      vbox->addLayout(autoSizedLayout);
    }
    else
    {
      hardSizedLayout->addWidget(text);
      hardSizedLayout->addWidget( units );
      vbox->addLayout(hardSizedLayout);
    }

    if( m_floatDisplayType != UNFORMATED)
    {
      auto btn = new QPushButton(this);
      QIcon ico(":images/switch.png");
      btn->setIcon( ico );
      btn->setStyleSheet(" margin: 0px; border: 0px;" );

      hardSizedLayout->addWidget( btn );

      btn->setProperty(s_indexSlotName,index);
      connect(btn, &QPushButton::clicked, text, &IGLineEdit::togglePrec);
      text->setPrec(m_precision);
      text->setStyle(m_floatDisplayType);
      text->setDisplay(true);
    }
  }
  else//not a number
  {
    vbox->addWidget(text);
  }

  if(!m_showComments)
  {
    commentText->hide();
  }
  commentText->setObjectName("IDFcomment");

  vbox->addWidget(commentText);

  if(exists)
  {
    frame->setObjectName("IGRow");
  }
  else
  {
    frame->setEnabled(false);
    frame->setObjectName("IGRowDisabled");
  }
  layout->addWidget( frame );
}

void InspectorGadget::layoutComboBox( QVBoxLayout* layout,
                                      QWidget* parent,
                                      openstudio::IddField& field,
                                      openstudio::IddFieldProperties& prop,
                                      const std::string& name,
                                      const std::string& curVal,
                                      int index,
                                      const std::string& comment,
                                      bool exists)
{
  auto frame = new QFrame(parent);
  auto vbox = new QVBoxLayout();
  frame->setLayout(vbox);
  QLabel* label = new QLabel( QString(name.c_str()), parent  );
  label->setWordWrap( true );

  QComboBox* combo = new IGComboBox(parent);
  combo->setSizeAdjustPolicy( QComboBox::AdjustToContentsOnFirstShow );

  if (prop.objectLists.size() && m_workspaceObj && !m_workspaceObj->handle().isNull())
  {
    Workspace workspace = m_workspaceObj->workspace();
    std::vector<std::string> names;

    for (const std::string& objectList : prop.objectLists){
      for (const WorkspaceObject& workspaceObject : workspace.getObjectsByReference(objectList)){
        names.push_back(workspaceObject.name().get());
      }
    }

    std::sort(names.begin(), names.end(), IstringCompare());

    if (!prop.required){
      combo->addItem("");
    }

    for (const std::string& name : names){
      combo->addItem(name.c_str());
    }
  }
  else
  {
    if (!prop.required){
      combo->addItem("");
    }

    for (IddKey key : field.keys()){
      combo->addItem(key.name().c_str());
    }
  }

  int idx = combo->findText( curVal.c_str(),Qt::MatchFixedString );

  if( -1 == idx )
  {
    idx=0;
    connect(combo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &InspectorGadget::IGdefaultRemoved);
    //QString errormsg("We have a value:");
    //errormsg += curVal.c_str();
    //errormsg += " that does not match the allowable values in the idd.Name:";
    //errormsg += name.c_str();
    //m_errorMessage->showMessage(errormsg);
    //combo->setEnabled( false );
    //return;
  }
  combo->setProperty(s_indexSlotName,index);

  if( m_comboBridge)
  {
    connect(combo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::highlighted), m_comboBridge, &ComboHighlightBridge::highlighted);
    connect(combo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated), m_comboBridge, &ComboHighlightBridge::activated);
  }
  combo->setCurrentIndex( idx );
  connect(combo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &InspectorGadget::IGvalueChanged);
  vbox->addWidget(label);
  vbox->addWidget(combo);

  QLineEdit* commentText = new QLineEdit( QString(comment.c_str()), parent  );
  commentText->setProperty(s_indexSlotName,index);
  connect(commentText, &QLineEdit::textEdited, this, &InspectorGadget::IGcommentChanged);
  vbox->addWidget(commentText);
  if(!m_showComments)
  {
    commentText->hide();
  }
  commentText->setObjectName("IDFcomment");

  if( exists )
  {
    frame->setObjectName("IGRow");
  }
  else
  {
    frame->setObjectName("IGRowDisabled");
    frame->setEnabled(false);
  }
  layout->addWidget( frame );
}

void InspectorGadget::createExtensibleToolBar( QVBoxLayout* layout,

                                               QWidget* parent,
                                               const IddObjectProperties& props )
{
  if( !props.extensible )
    return;

  if ( m_locked )
    return;

  auto frame = new QFrame(parent);
  frame->setContentsMargins(0,0,0,0);
  auto hbox = new QHBoxLayout();
  frame->setLayout(hbox);
  hbox->setSpacing(0);
  hbox->setMargin(0);

  frame->setObjectName("IGRow");

  layout->addWidget(frame);

  QLabel* label = new QLabel(tr("Add/Remove Extensible Groups"),parent);

  auto addBtn = new QPushButton(frame);
  QIcon ico(":images/edit_add.png");
  addBtn->setIcon( ico );
  addBtn->setStyleSheet(" margin: 0px; border: 0px;" );

  auto subBtn = new QPushButton(frame);
  QIcon ico2(":images/edit_remove.png");
  subBtn->setIcon( ico2 );
  subBtn->setStyleSheet(" margin: 0px; border: 0px;" );

  connect(addBtn, &QPushButton::clicked, this, &InspectorGadget::addExtensible);
  connect(subBtn, &QPushButton::clicked, this, &InspectorGadget::removeExtensible);
  checkRemoveBtn( subBtn );

  hbox->addSpacing(10);//because this row doesn't have a vbox, we need to move the text over some to get it to line up
  hbox->addWidget(label);
  hbox->addWidget(addBtn);
  hbox->addWidget(subBtn);
}

void InspectorGadget::checkRemoveBtn( QPushButton* btn )
{
  unsigned int numFields = m_workspaceObj->numFields();
  unsigned int numNonEx = m_workspaceObj->numNonextensibleFields();

  if((numFields - numNonEx) <=0 ){
    btn->setEnabled( false );
  }
  else{
    btn->setEnabled( true );
  }
}


void InspectorGadget::stripchar(string& strip,char c)
{
  string::size_type i = strip.find(c);
  while( i != string::npos )
  {
    strip.replace(i,1,1,' ');
    ++i;
    i=strip.find(c,i);
  }

}

//SLOTS
void InspectorGadget::IGdefaultRemoved(const QString&)
{
  QObject* source = sender();
  QWidget* w = dynamic_cast<QWidget*>(source);
  w->setStyleSheet("color:black");
  disconnect(source,nullptr,this,SLOT(IGdefaultRemoved(const QString&)));

}

void InspectorGadget::IGvalueChanged( const QString& value )
{
  OS_ASSERT(m_workspaceObj);

  QString qval=value;
  QObject* source = sender();
  IGLineEdit* le = dynamic_cast<IGLineEdit*>(source);
  if(le){
    if( !le->checkValue(qval) ){
      return;
    }
  }

  QVariant v = source->property(InspectorGadget::s_indexSlotName);
  int index = v.toInt();
  string temp(qval.toStdString());

  disconnectWorkspaceObjectSignals();

  bool test = false;

  IddField iddField = m_workspaceObj->iddObject().getField(index).get();
  if (!temp.empty() && (iddField.properties().type == IddFieldType::RealType) && (m_unitSystem == IP)){
    if (OptionalUnit ipUnits = iddField.getUnits(true)) {
      try {
        double val = boost::lexical_cast<double>(temp);
        test = m_workspaceObj->setQuantity(index,Quantity(val,*ipUnits));
      } catch (...) {
      }
    }
  }else{
    test = m_workspaceObj->setString(index, temp);
  }

  connectWorkspaceObjectSignals();

  if (!test){
    // setting the value failed, do not emit dirty
    QObject* sender = this->sender();

    QComboBox* combo = qobject_cast<QComboBox*>(sender);
    if (combo){
      QString curVal("");
      boost::optional<std::string> tempStr = m_workspaceObj->getString(index, true);
      if (tempStr){
        curVal = toQString(*tempStr);
      }

      int idx = combo->findText(curVal, Qt::MatchFixedString);
      if (idx >= 0){
        combo->blockSignals(true);
        combo->setCurrentIndex(idx);
        combo->blockSignals(false);
      }
    }
    return;
  }

  if(m_objectHasName && index==m_nameIndex.get()){
    emit nameChanged(temp.c_str());
  }
  emit dirty();
  return;
}

void InspectorGadget::IGcommentChanged( const QString& value )
{
  QObject* source = sender();
  QVariant v = source->property(InspectorGadget::s_indexSlotName);
  int index = v.toInt();
  string temp(value.toStdString());
  if (index < 0 ){
    disconnectWorkspaceObjectSignals();
    m_workspaceObj->setComment(temp);
    connectWorkspaceObjectSignals();
  }
  else{
    disconnectWorkspaceObjectSignals();
    m_workspaceObj->setFieldComment(index,temp);
    connectWorkspaceObjectSignals();
  }

  emit dirty();
  return;
}

void InspectorGadget::IGautosize(bool toggled)
{
  if(!toggled)
    return;

  QObject* source = sender();
  QVariant v = source->property(InspectorGadget::s_indexSlotName);
  int index = v.toInt();

  disconnectWorkspaceObjectSignals();
  m_workspaceObj->setString(index,"Autosize");
  connectWorkspaceObjectSignals();
}

void InspectorGadget::commentConfig(bool showComments)
{
  bool oldComments = m_showComments;
  m_showComments = showComments;
  if(oldComments != m_showComments)
  {
    rebuild(m_recursive);
  }
}

void InspectorGadget::setPrec()
{
  IGPrecisionDialog* source = dynamic_cast<IGPrecisionDialog*>(sender());
  OS_ASSERT(source);

  if( source->sciBtn->isChecked() )
  {
    bool ok;
    int val = source->lineEdit->text().toInt(&ok);
    if(!ok)
    {
      val=2;
      source->lineEdit->setText("2");
    }
    setPrecision( val, SCIENTIFIC );
  }
  else if( source->fixedBtn->isChecked() )
  {
    bool ok;
    int val = source->lineEdit->text().toInt(&ok);
    if(!ok)
    {
      val=2;
      source->lineEdit->setText("2");
    }
    setPrecision( val, FIXED );
  }
  else
  {
    setPrecision( 1000, UNFORMATED );
  }
}

void InspectorGadget::addExtensible()
{
  disconnectWorkspaceObjectSignals();
  m_workspaceObj->pushExtensibleGroup();
  connectWorkspaceObjectSignals();

  QPushButton* source = dynamic_cast<QPushButton*>(sender());
  OS_ASSERT(source);
  checkRemoveBtn( source );
  emit dirty();
  rebuild(false);
}

void InspectorGadget::removeExtensible()
{
  disconnectWorkspaceObjectSignals();
  m_workspaceObj->popExtensibleGroup();
  connectWorkspaceObjectSignals();

  QPushButton* source = dynamic_cast<QPushButton*>(sender());
  OS_ASSERT(source);
  checkRemoveBtn( source );
  emit dirty();
  rebuild(false);
}

void InspectorGadget::createAllFields()
{
  unsigned int cur = m_workspaceObj->numNonextensibleFields();
  unsigned int max = m_workspaceObj->iddObject().numFields();
  if( max > cur ){
    disconnectWorkspaceObjectSignals();
    bool result = m_workspaceObj->setString(max-1,"");
    connectWorkspaceObjectSignals();
    OS_ASSERT(result);
    rebuild(false);
  }

  if( m_recursive )
  {
    for(const auto & elem : m_childMap)
    {
      elem.second->createAllFields();
    }
  }
}

void InspectorGadget::showAllFields( bool state )
{
  if (state == m_showAllFields)
    return;

  m_showAllFields = state;
  rebuild( false );
  if( m_recursive )
  {
    for(const auto & elem : m_childMap)
    {
      elem.second->showAllFields(state);
    }
  }
}

void InspectorGadget::setRecursive(bool recursive)
{
  m_recursive=recursive;
  for(const auto & elem : m_childMap)
  {
    elem.second->setRecursive(recursive);
  }
}

void InspectorGadget::onWorkspaceObjectChanged()
{
  m_workspaceObjectChanged = true;

  QTimer::singleShot(0,this,SLOT(onTimeout()));
}

void InspectorGadget::onTimeout()
{
  if (m_workspaceObjectChanged && m_workspaceObj && !m_workspaceObj->handle().isNull()){
    if (m_workspaceObj){
      rebuild(false);
    }
    m_workspaceObjectChanged = false;
  }
}

void InspectorGadget::onWorkspaceObjectRemoved(const openstudio::Handle &)
{
  m_workspaceObjectChanged = true;
  clear(true);

  QTimer::singleShot(0,this,SLOT(onTimeout()));
}

void InspectorGadget::removeWorkspaceObject(const openstudio::Handle &handle)
{
  emit workspaceObjectRemoved(handle);
}

void InspectorGadget::connectWorkspaceObjectSignals() const
{
  if (m_workspaceObj){
    openstudio::detail::WorkspaceObject_Impl* impl = m_workspaceObj->getImpl<openstudio::detail::WorkspaceObject_Impl>().get();
    if (impl){
      impl->openstudio::detail::WorkspaceObject_Impl::onChange.connect<InspectorGadget, &InspectorGadget::onWorkspaceObjectChanged>(const_cast<InspectorGadget *>(this));

      impl->openstudio::detail::WorkspaceObject_Impl::onRemoveFromWorkspace.connect<InspectorGadget, &InspectorGadget::removeWorkspaceObject>(const_cast<InspectorGadget *>(this));

      connect(this, &InspectorGadget::workspaceObjectRemoved, this, &InspectorGadget::onWorkspaceObjectRemoved);
    }
  }
}

void InspectorGadget::disconnectWorkspaceObjectSignals() const
{
  if (m_workspaceObj){
    auto* impl = m_workspaceObj->getImpl<openstudio::detail::WorkspaceObject_Impl>().get();
    if (impl){
      // disconnect all signals from m_workspaceObj to this

      impl->openstudio::detail::WorkspaceObject_Impl::onChange.disconnect<InspectorGadget, &InspectorGadget::onWorkspaceObjectChanged>(const_cast<InspectorGadget *>(this));

      impl->openstudio::detail::WorkspaceObject_Impl::onRemoveFromWorkspace.disconnect<InspectorGadget, &InspectorGadget::removeWorkspaceObject>(const_cast<InspectorGadget *>(this));

      disconnect(this, &InspectorGadget::workspaceObjectRemoved, this, &InspectorGadget::onWorkspaceObjectRemoved);

      // impl->disconnect();
    }
  }
}

void InspectorGadget:: toggleUnits(bool displayIP)
{
  if(displayIP){
    setUnitSystem(InspectorGadget::IP);
  }
  else{
    setUnitSystem(InspectorGadget::SI);
  }
}

void InspectorGadget::setUnitSystem(const InspectorGadget::UNIT_SYSTEM unitSystem)
{
  openstudio::OptionalWorkspaceObject currentObject = m_workspaceObj;
  clear(true);
  m_workspaceObj = currentObject;
  m_unitSystem = unitSystem;
  rebuild(true);
}
