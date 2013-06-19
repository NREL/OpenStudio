/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <SimXmlGenerator.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QDir>
#include <QFileInfo>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

std::string makeLCC(const std::string& str)
{
  std::string result = str;
  if (!result.empty()){
    result[0] = tolower(result[0]);
  }

  if (result == "operator"){
    result = "operatorSymbol";
  }

  return result;
}

SimXmlGenerator::SimXmlGenerator(const XsdFile& xsdFile)
  : m_xsdFile(xsdFile)
{
}

int SimXmlGenerator::generate(const std::string& outDir) const
{
  std::string defaultPrefix = m_xsdFile.defaultPrefix();
  if (defaultPrefix.empty()){
    std::cout << "Could not determine prefix for target namespace" << std::endl;
    return -1;
  }

  // open outfiles
  bool test = QDir().mkpath( QString::fromStdString(outDir) );
  if (!test){
    std::cout << "Could not create path " << outDir << std::endl;
    return -1;
  }

  std::string baseName = QFileInfo(QString::fromStdString(m_xsdFile.path())).baseName().toStdString();

  std::string hxxPath = outDir + baseName + ".hxx";
  std::cout << "Initializing " << hxxPath << std::endl;
  std::ofstream hxxFile;
  hxxFile.open(hxxPath.c_str());

  std::string cxxPath = outDir + baseName + ".cxx";
  std::cout << "Initializing " << cxxPath << std::endl;
  std::ofstream cxxFile;
  cxxFile.open(cxxPath.c_str());

  std::string ixxPath = outDir + baseName + ".ixx";
  std::cout << "Initializing " << ixxPath << std::endl;
  std::ofstream ixxFile;
  ixxFile.open(ixxPath.c_str());
  
  std::vector<Element> elements = m_xsdFile.elements();

  std::vector<SimpleType> simpleTypes = m_xsdFile.simpleTypes();

  std::vector<ComplexType> tempComplexTypes = m_xsdFile.complexTypes();

  // have to do this to ensure base classes defined in proper order
  std::vector<ComplexType> complexTypes;
  std::set<std::string> mappedComplexTypes;

  for(std::vector<ComplexType>::const_iterator it = tempComplexTypes.begin(), itend = tempComplexTypes.end(); it != itend; ++it){
    
    // first go through bases in reverse order
    std::vector<std::string> bases = it->bases();
    for(std::vector<std::string>::const_reverse_iterator jt = bases.rbegin(), jtend = bases.rend(); jt != jtend; ++jt){
      QSharedPointer<ComplexType> base = m_xsdFile.findComplexType(*jt);
      if (base){
        // check that this type belongs in this file
        if (base->prefix() == m_xsdFile.defaultPrefix()){
          if (mappedComplexTypes.find(base->qualifiedName()) == mappedComplexTypes.end()){
            mappedComplexTypes.insert(base->qualifiedName());
            complexTypes.push_back(*base);
          }
        }
      }
    }
    
    // no do the type
    if (mappedComplexTypes.find(it->qualifiedName()) == mappedComplexTypes.end()){
      mappedComplexTypes.insert(it->qualifiedName());
      complexTypes.push_back(*it);
    }
  }

  ///////////////////////////////////// HXX FILE /////////////////////////////////
  // include guard
  hxxFile << "#ifndef " << defaultPrefix << "_HXX" << std::endl;
  hxxFile << "#define " << defaultPrefix << "_HXX" << std::endl << std::endl;

  // includes
  hxxFile << "#include <SimXmlDoc.hpp>" << std::endl;
  hxxFile << "#include <SimXmlElement.hpp>" << std::endl;
  std::vector<std::string> importNames = m_xsdFile.importNames();
  for (std::vector<std::string>::const_iterator it = importNames.begin(), itend = importNames.end(); it != itend; ++it){
    QStringList importParts = QString::fromStdString(*it).split(".");
    if (!importParts.empty()){
      hxxFile << "#include <" << importParts.at(0).toStdString() << ".hxx>" << std::endl;
    }
  }
  hxxFile << std::endl;

  // open namespace
  hxxFile << "namespace " << defaultPrefix << " {" << std::endl << std::endl;

  // forward declare all types in namespace
  for (std::vector<SimpleType>::const_iterator it = simpleTypes.begin(), itend = simpleTypes.end(); it!=itend; ++it){
    hxxFile << "  class " << it->name() << ";" << std::endl;
  }
  for (std::vector<ComplexType>::const_iterator it = complexTypes.begin(), itend = complexTypes.end(); it!=itend; ++it){
    hxxFile << "  class " << it->name() << ";" << std::endl;
  }
  hxxFile << std::endl;

  // declare all types
  for (std::vector<SimpleType>::const_iterator it = simpleTypes.begin(), itend = simpleTypes.end(); it!=itend; ++it){
    writeClassDeclaration(hxxFile, *it) << std::endl;
  }
  for (std::vector<ComplexType>::const_iterator it = complexTypes.begin(), itend = complexTypes.end(); it!=itend; ++it){
    writeClassDeclaration(hxxFile, *it) << std::endl;
  }

  // close namespace
  hxxFile << "} // " << defaultPrefix << std::endl << std::endl;

  // include guard
  hxxFile << "#endif // " << defaultPrefix << "_HXX" << std::endl;

  ///////////////////////////////////// END HXX FILE /////////////////////////////////

  ///////////////////////////////////// CXX FILE /////////////////////////////////
  
  // add includes
  cxxFile << "#include <" << baseName << ".hxx>" << std::endl;
  cxxFile << std::endl;
  cxxFile << "#include <SimXmlDoc.hpp>" << std::endl;
  cxxFile << "#include <SimXmlElement.hpp>" << std::endl;
  cxxFile << std::endl;
  cxxFile << "#include <QDomElement>" << std::endl;
  cxxFile << std::endl;
  cxxFile << "#include <vector>" << std::endl;
  cxxFile << std::endl;

  // open namespace
  cxxFile << "namespace " << defaultPrefix << " {" << std::endl << std::endl;

  // define all types
  for (std::vector<SimpleType>::const_iterator it = simpleTypes.begin(), itend = simpleTypes.end(); it!=itend; ++it){
    writeClassDefinition(cxxFile, *it) << std::endl;
  }
  for (std::vector<ComplexType>::const_iterator it = complexTypes.begin(), itend = complexTypes.end(); it!=itend; ++it){
    writeClassDefinition(cxxFile, *it) << std::endl;
  }

  // close namespace
  cxxFile << "} // " << defaultPrefix << std::endl << std::endl;

  ///////////////////////////////////// END CXX FILE /////////////////////////////////

  ///////////////////////////////////// IXX FILE /////////////////////////////////
  
  for (std::vector<SimpleType>::const_iterator it = simpleTypes.begin(), itend = simpleTypes.end(); it!=itend; ++it){

  }
  for (std::vector<ComplexType>::const_iterator it = complexTypes.begin(), itend = complexTypes.end(); it!=itend; ++it){
    ixxFile << "%ignore std::vector<" << it->prefix() << "::" << it->name() << ">::vector(size_type);" << std::endl;
    ixxFile << "%ignore std::vector<" << it->prefix() << "::" << it->name() << ">::resize(size_type);" << std::endl;
    ixxFile << "%template(" << it->name() << "Vector) std::vector<" << it->prefix() << "::" << it->name() << ">;" << std::endl;
    ixxFile << std::endl;
  }

  ///////////////////////////////////// END IXX FILE /////////////////////////////////

  // close files
  hxxFile.close();
  cxxFile.close();
  ixxFile.close();

  return 0;
}

std::ostream& SimXmlGenerator::writeClassDeclaration(std::ostream& os, const SimpleType& simpleType) const
{
  return os;
}

std::ostream& SimXmlGenerator::writeClassDeclaration(std::ostream& os, const ComplexType& complexType) const
{
  os << "  class " << complexType.name();
  if (!complexType.base().empty()){
    QStringList baseParts = QString::fromStdString(complexType.base()).split(":");
    if (baseParts.size() == 1){
      os << " : public " << baseParts.at(0).toStdString();
    }else if (baseParts.size() == 2){
      os << " : public " << baseParts.at(0).toStdString() << "::" << baseParts.at(1).toStdString();
    }
  }else{
    os << " : public simxml::SimXmlElement";
  }
  os << " {" << std::endl;
  os << "  public:" << std::endl;

  // static methods
  if (complexType.isAbstract() != "true"){
    os << "    static std::string typePrefix();" << std::endl;
    os << "    static std::string typeName();" << std::endl;
    os << "    static std::vector<" << complexType.name() << "> getAll(const simxml::SimXmlDoc& doc);" << std::endl;
    os << "    static " << complexType.name() << " find(const std::string& refId, const simxml::SimXmlDoc& doc);" << std::endl;
    os << "    static std::vector<" << complexType.name() << "> findWhere(const std::string& childName, const std::string& childValue, const simxml::SimXmlDoc& doc);" << std::endl;
  }

  // constructor/destructor
  if (complexType.isAbstract() != "true"){
    os << "    " << complexType.name() << "(simxml::SimXmlDoc& doc);" << std::endl;
  }
  os << "    " << complexType.name() << "(const simxml::SimXmlElement& element);" << std::endl;
  os << "    virtual~" << complexType.name() << "();" << std::endl;

  // validation
  if (complexType.isAbstract() != "true"){
    os << "    bool isValid() const;" << std::endl;
  }

  // write accessor for each attribute
  std::vector<std::string> attributes = complexType.attributes();
  for(std::vector<std::string>::const_iterator it = attributes.begin(), itend = attributes.end(); it != itend; ++it){
    os << "    std::string " << makeLCC(*it) << "() const;" << std::endl;
  }

  // write accessor for each element
  std::vector<Element> elements = complexType.elements();
  for(std::vector<Element>::const_iterator it = elements.begin(), itend = elements.end(); it != itend; ++it){
    std::string refQualifiedName = it->refPrefix() + ":" + it->refName();

    std::string type;
    QSharedPointer<Element> element = m_xsdFile.findElement(refQualifiedName);
    if (element){
      type = element->type();
    }
    
    if (type == "xs:IDREF"){
      // this is a reference to an object
      os << "    simxml::SimXmlElement " << makeLCC(it->refName()) << "() const;" << std::endl;
      os << "    std::string " << makeLCC(it->refName()) << "Id() const;" << std::endl;
      os << "    bool set" << it->refName() << "(const simxml::SimXmlElement& value);" << std::endl;
      os << "    bool set" << it->refName() << "Id(const std::string& value);" << std::endl;
    }else if (type == "xs:IDREFS"){
      // this is a reference to multiple objects
      os << "    std::vector<simxml::SimXmlElement> " << makeLCC(it->refName()) << "() const;" << std::endl;
      os << "    std::vector<std::string> " << makeLCC(it->refName()) << "Ids() const;" << std::endl;
      os << "    bool set" << it->refName() << "(const std::vector<simxml::SimXmlElement>& value);" << std::endl;
      os << "    bool set" << it->refName() << "Ids(const std::vector<std::string>& value);" << std::endl;
    }else if (type == "xs:boolean"){
      // bool
      os << "    bool " << makeLCC(it->refName()) << "() const;" << std::endl;   
      os << "    bool set" << it->refName() << "(bool value);" << std::endl;
    }else if (type == "xs:int"){
      // int
      os << "    int " << makeLCC(it->refName()) << "() const;" << std::endl; 
      os << "    bool set" << it->refName() << "(int value);" << std::endl;
    }else if (type == "xs:double"){
      // double
      os << "    double " << makeLCC(it->refName()) << "() const;" << std::endl;    
      os << "    bool set" << it->refName() << "(double value);" << std::endl;
    }else if (type == "xs:string"){
      // string
      os << "    std::string " << makeLCC(it->refName()) << "() const;" << std::endl;    
      os << "    bool set" << it->refName() << "(const std::string& value);" << std::endl;
    }else if (type == "simcore:integerList"){
      // std::vector<int>
      os << "    std::vector<int> " << makeLCC(it->refName()) << "() const;" << std::endl;    
      os << "    bool set" << it->refName() << "(const std::vector<int>& value);" << std::endl;
    }else if (type == "simcore:doubleList"){
      // std::vector<double>
      os << "    std::vector<double> " << makeLCC(it->refName()) << "() const;" << std::endl;    
      os << "    bool set" << it->refName() << "(const std::vector<double>& value);" << std::endl;
    }else{
      // do simple for now
      os << "    std::string " << makeLCC(it->refName()) << "() const;" << std::endl;
      os << "    bool set" << it->refName() << "(const std::string& value);" << std::endl;

      std::cout << "Warning: wrapping " << complexType.prefix() << "::" << complexType.name() << "::" << makeLCC(it->refName()) 
                << " of type '" << type << "' with std::string accessor" << std::endl;
    }
  }

  os << "  };" << std::endl;

  return os;
}

std::ostream& SimXmlGenerator::writeClassDefinition(std::ostream& os, const SimpleType& simpleType) const
{
  return os;
}

std::ostream& SimXmlGenerator::writeClassDefinition(std::ostream& os, const ComplexType& complexType) const
{
  // constructor
  if (complexType.isAbstract() != "true"){
    os << complexType.name() << "::" << complexType.name() << "(simxml::SimXmlDoc& doc)" << std::endl;
    if (!complexType.base().empty()){
      QStringList baseParts = QString::fromStdString(complexType.base()).split(":");
      if (baseParts.size() == 1){
        os << " : " << baseParts.at(0).toStdString() << "(doc.addElement(typeName()))" << std::endl;
      }else if (baseParts.size() == 2){
        os << " : " << baseParts.at(0).toStdString() << "::" << baseParts.at(1).toStdString() << "(doc.addElement(typeName()))" << std::endl;
      }
    }else{
      os << " : simxml::SimXmlElement(doc.addElement(typeName()))" << std::endl;
    }
    os << "{}" << std::endl << std::endl;
  }

  os << complexType.name() << "::" << complexType.name() << "(const simxml::SimXmlElement& element)" << std::endl;
  if (!complexType.base().empty()){
    QStringList baseParts = QString::fromStdString(complexType.base()).split(":");
    if (baseParts.size() == 1){
      os << " : " << baseParts.at(0).toStdString() << "(element)" << std::endl;
    }else if (baseParts.size() == 2){
      os << " : " << baseParts.at(0).toStdString() << "::" << baseParts.at(1).toStdString() << "(element)" << std::endl;
    }
  }else{
    os << " : simxml::SimXmlElement(element)" << std::endl;
  }
  os << "{}" << std::endl << std::endl;

  // destructor
  os << complexType.name() << "::~" << complexType.name() << "(){}" << std::endl << std::endl;

  // static methods
  if (complexType.isAbstract() != "true"){
    os << "std::string " << complexType.name() << "::typePrefix() {return \"" << complexType.prefix() << "\";}" << std::endl << std::endl;
    
    os << "std::string " << complexType.name() << "::typeName() {return \"" << complexType.name() << "\";}" << std::endl << std::endl;
   
    os << "std::vector<" << complexType.name() << "> " << complexType.name() << "::getAll(const simxml::SimXmlDoc& doc){" << std::endl;
    os << "  std::vector<" << complexType.name() << "> result;" << std::endl;
    os << "  std::vector<simxml::SimXmlElement> elements = doc.getElements(typeName());" << std::endl;
    os << "  for(std::vector<simxml::SimXmlElement>::const_iterator it = elements.begin(), itend = elements.end(); it != itend; ++it){" << std::endl;
    os << "    result.push_back(" << complexType.name() << "(*it));" << std::endl;
    os << "  }" << std::endl;
    os << "  return result;" << std::endl;
    os << "}" << std::endl << std::endl;

    os << complexType.name() << " " << complexType.name() << "::find(const std::string& refId, const simxml::SimXmlDoc& doc){" << std::endl;
    os << "  return doc.findElement(typeName(), refId);" << std::endl;
    os << "}" << std::endl << std::endl;

    os << "std::vector<" << complexType.name() << "> " + complexType.name() + "::findWhere(const std::string& childName, const std::string& childValue, const simxml::SimXmlDoc& doc){" << std::endl;
    os << "  std::vector<" << complexType.name() << "> result;" << std::endl;
    os << "  std::vector<simxml::SimXmlElement> elements = doc.findElementsWithChildValue(typeName(), childName, childValue);" << std::endl;
    os << "  for(std::vector<simxml::SimXmlElement>::const_iterator it = elements.begin(), itend = elements.end(); it != itend; ++it){" << std::endl;
    os << "    result.push_back(" << complexType.name() << "(*it));" << std::endl;
    os << "  }" << std::endl;
    os << "  return result;" << std::endl; 
    os << "}" << std::endl << std::endl;
  }

  // instance methods
  if (complexType.isAbstract() != "true"){
    os << "bool " << complexType.name() << "::isValid() const {" << std::endl;
    os << "  if (this->isNull()) { return false; }" << std::endl;
    os << "  return (\"" << complexType.name() << "\" == this->elementName());" << std::endl;
    os << "}" << std::endl << std::endl;
  }

  // write accessor for each attribute
  std::vector<std::string> attributes = complexType.attributes();
  for(std::vector<std::string>::const_iterator it = attributes.begin(), itend = attributes.end(); it != itend; ++it){
    os << "std::string " << complexType.name() << "::" << makeLCC(*it) << "() const {" << std::endl;
    os << "  return impl()->attribute(\"" << *it << "\").toStdString();" << std::endl;
    os << "}" << std::endl << std::endl;
  }

  // write accessor for each element
  std::vector<Element> elements = complexType.elements();
  for(std::vector<Element>::const_iterator it = elements.begin(), itend = elements.end(); it != itend; ++it){
    std::string refQualifiedName = it->refPrefix() + ":" + it->refName();

    std::string type;
    QSharedPointer<Element> element = m_xsdFile.findElement(refQualifiedName);
    if (element){
      type = element->type();
    }

    if (type == "xs:IDREF"){
      // this is a reference to an object
      os << "simxml::SimXmlElement " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  std::string refId = getChildText(\"" << refQualifiedName << "\");" << std::endl;
      os << "  return simXmlDoc().findElement(refId);" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "std::string " << complexType.name() << "::" << makeLCC(it->refName()) << "Id() const {" << std::endl;
      os << "  return getChildText(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const simxml::SimXmlElement& value) {" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", value.refId());" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "Id(const std::string& value) {" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "xs:IDREFS"){
      // this is a reference to multiple objects
      os << "std::vector<simxml::SimXmlElement> " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  std::vector<simxml::SimXmlElement> result;" << std::endl;
      os << "  std::vector<std::string> refIds = xmllib::split(getChildText(\"" << refQualifiedName << "\"), ' ');" << std::endl;
      os << "  for (std::vector<std::string>::const_iterator it = refIds.begin(), itend = refIds.end(); it != itend; ++it){" << std::endl;
      os << "    result.push_back(simXmlDoc().findElement(*it));" << std::endl;
      os << "  }" << std::endl;
      os << "  return result;" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "std::vector<std::string> " << complexType.name() << "::" << makeLCC(it->refName()) << "Ids() const {" << std::endl;
      os << "  return xmllib::split(getChildText(\"" << refQualifiedName << "\"), ' ');" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const std::vector<simxml::SimXmlElement>& value) {" << std::endl;
      os << "  std::string childText;" << std::endl;
      os << "  for (std::vector<simxml::SimXmlElement>::const_iterator it = value.begin(), itend = value.end(); it != itend; ++it){" << std::endl;
      os << "    childText += it->refId();" << std::endl;
      os << "    if (it != itend-1){" << std::endl;
      os << "      childText += \" \";" << std::endl;
      os << "    }" << std::endl;
      os << "  }" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", childText);" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "Ids(const std::vector<std::string>& value) {" << std::endl;
      os << "  std::string childText;" << std::endl;
      os << "  for (std::vector<std::string>::const_iterator it = value.begin(), itend = value.end(); it != itend; ++it){" << std::endl;
      os << "    childText += *it;" << std::endl;
      os << "    if (it != itend-1){" << std::endl;
      os << "      childText += \" \";" << std::endl;
      os << "    }" << std::endl;
      os << "  }" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", childText);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "xs:boolean"){
      // bool
      os << "bool " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildBool(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl; 

      os << "bool " << complexType.name() << "::set" << it->refName() << "(bool value) {" << std::endl;
      os << "  return setChildBool(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "xs:int"){
      // int
      os << "int " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildInt(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl; 

      os << "bool " << complexType.name() << "::set" << it->refName() << "(int value) {" << std::endl;
      os << "  return setChildInt(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "xs:double"){
      // double
      os << "double " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildDouble(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl;  

      os << "bool " << complexType.name() << "::set" << it->refName() << "(double value) {" << std::endl;
      os << "  return setChildDouble(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "xs:string"){
      // string
      os << "std::string " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildText(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const std::string& value) {" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "simcore:integerList"){
      // std::vector<int>
      os << "std::vector<int> " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildIntegerList(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl; 

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const std::vector<int>& value) {" << std::endl;
      os << "  return setChildIntegerList(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else if (type == "simcore:doubleList"){
      // std::vector<double>
      os << "std::vector<double> " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildDoubleList(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl;   

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const std::vector<double>& value) {" << std::endl;
      os << "  return setChildDoubleList(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }else{
      // do simple for now, later test type and do list, etc
      os << "std::string " << complexType.name() << "::" << makeLCC(it->refName()) << "() const {" << std::endl;
      os << "  return getChildText(\"" << refQualifiedName << "\");" << std::endl;
      os << "}" << std::endl << std::endl;

      os << "bool " << complexType.name() << "::set" << it->refName() << "(const std::string& value) {" << std::endl;
      os << "  return setChildText(\"" << refQualifiedName << "\", value);" << std::endl;
      os << "}" << std::endl << std::endl;

    }
  }

  return os;
}
