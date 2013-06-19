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

#include <utilities/document/Section.hpp>
#include <utilities/document/Section_Impl.hpp>

#include <utilities/document/SectionHeading_Impl.hpp>

#include <boost/foreach.hpp>

namespace openstudio {

namespace detail {

  Section_Impl::Section_Impl(const SectionHeading& heading,
                             const std::vector<SectionElement>& contents)
    : m_heading(heading), m_contents(contents)
  {}

  DocumentElement Section_Impl::clone() const {
    Section myClone(m_heading.clone().cast<SectionHeading>());
    BOOST_FOREACH(const SectionElement& element,m_contents) {
      myClone.append(element.clone().cast<SectionElement>());
    }
    return myClone.cast<DocumentElement>();
  }

  SectionHeading Section_Impl::heading() const {
    return m_heading;
  }

  std::vector<SectionElement> Section_Impl::contents() const {
    return m_contents;
  }

  SectionElement Section_Impl::getElement(unsigned index) const {
    if (index >= numElements()) {
      LOG_AND_THROW("Cannot return element " << index << " of section '" << m_heading.text() 
          << "', because it only contains " << numElements() << " elements.");
    }
    return m_contents[index];
  }

  std::vector<Section> Section_Impl::subsections() const {
    SectionVector result;
    BOOST_FOREACH(const SectionElement& element,m_contents) {
      OptionalSection oSection = element.optionalCast<Section>();
      if (oSection) { result.push_back(*oSection); }
    }
    return result;
  }

  void Section_Impl::clear() {
    m_contents.clear();
  }

  void Section_Impl::replaceAll(const std::vector<SectionElement>& contents) {
    m_contents = contents;
  }

  void Section_Impl::replace(unsigned index, const SectionElement& element) {
    if (index > numElements()) {
      LOG_AND_THROW("Cannot replace element " << index << " of section '" << m_heading.text() 
          << "', because such an element does not exist.");
    }
    if (index == numElements()) {
      LOG(Warn,"Asked to replace element " << index << " of section '" << m_heading.text() 
          << "', but appending instead, because the section currently has " << numElements() 
          << " elements.");
      append(element);
      return;
    }
    m_contents[index] = element;
  }

  void Section_Impl::append(const SectionElement& element) {
    m_contents.push_back(element);
  }

  void Section_Impl::append(const std::vector<SectionElement>& elements) {
    m_contents.insert(m_contents.end(),elements.begin(),elements.end());
  }

  Section Section_Impl::appendSubsection(const std::string& headingText) {
    SectionHeading heading(m_heading.level()+1,headingText);
    Section result(heading);
    append(result);
    return result;
  }

  void Section_Impl::insert(unsigned index, const SectionElement& element) {
    unsigned n = numElements();
    if (index > n) {
      LOG_AND_THROW("Cannot insert an element at index " << index << ", because there are "
          << "currently only " << n << " elements in Section '" << m_heading.text() << "'.");
    }
    if (index == n) { 
      append(element); 
      return;
    }
    SectionElementVector::iterator it = m_contents.begin();
    for (unsigned i = 0; i < index; ++i, ++it) {}
    m_contents.insert(it,element);
  }

  Section Section_Impl::insertSubsection(unsigned index, const std::string& headingText) {
    if (index > numElements()) {
      LOG_AND_THROW("Cannot insert a Subsection at index " << index << ", because there are "
          << "currently only " << numElements() << " elements in Section '" << m_heading.text() << "'.");
    }
    SectionHeading heading(m_heading.level()+1,headingText);
    Section result(heading);
    insert(index,result);
    return result;
  }

  void Section_Impl::erase(unsigned index) {
    if (index < numElements()) {
      SectionElementVector::iterator it = m_contents.begin();
      for (unsigned i = 0; i < index; ++i, ++it) {}
      m_contents.erase(it);
    }
    else {
      LOG(Warn,"No element to erase at index " << index << " of Section '" << m_heading.text() 
          << "'.");
    }
  }

  void Section_Impl::makeHeadingLevelsConsistent() {
    unsigned nextLevel = m_heading.level() + 1;
    SectionVector subsections = this->subsections();
    BOOST_FOREACH(Section& subsection,subsections) {
      subsection.heading().setLevel(nextLevel);
      subsection.makeHeadingLevelsConsistent();
    }
  }

  unsigned Section_Impl::numElements() const {
    return m_contents.size();
  }

  unsigned Section_Impl::numSubsections() const {
    return subsections().size();
  }

  std::string Section_Impl::print(DocumentFormat fmt) const {
    std::stringstream ss;
    ss << m_heading.print(fmt);
    BOOST_FOREACH(const SectionElement& element,m_contents) {
      ss << element.print(fmt);
    }
    return ss.str();
  }

  std::ostream& Section_Impl::printToStream(std::ostream& os, DocumentFormat fmt) const {
    m_heading.printToStream(os,fmt);
    BOOST_FOREACH(const SectionElement& element,m_contents) {
      element.printToStream(os,fmt);
    }
    return os;
  }

} // detail

Section::Section(const SectionHeading& heading,const std::vector<SectionElement>& contents)
  : SectionElement(boost::shared_ptr<detail::Section_Impl>(new detail::Section_Impl(heading,contents)))
{}

SectionHeading Section::heading() const {
  return getImpl<detail::Section_Impl>()->heading();
}

std::vector<SectionElement> Section::contents() const {
  return getImpl<detail::Section_Impl>()->contents();
}

SectionElement Section::getElement(unsigned index) const {
  return getImpl<detail::Section_Impl>()->getElement(index);
}

std::vector<Section> Section::subsections() const {
  return getImpl<detail::Section_Impl>()->subsections();
}

void Section::clear() {
  getImpl<detail::Section_Impl>()->clear();
}

void Section::replaceAll(const std::vector<SectionElement>& contents) {
  getImpl<detail::Section_Impl>()->replaceAll(contents);
}

void Section::replace(unsigned index, const SectionElement& element) {
  getImpl<detail::Section_Impl>()->replace(index,element);
}

void Section::append(const SectionElement& element) {
  getImpl<detail::Section_Impl>()->append(element);
}

void Section::append(const std::vector<SectionElement>& elements) {
  getImpl<detail::Section_Impl>()->append(elements);
}

Section Section::appendSubsection(const std::string& headingText) {
  return getImpl<detail::Section_Impl>()->appendSubsection(headingText);
}

void Section::insert(unsigned index, const SectionElement& element) {
  getImpl<detail::Section_Impl>()->insert(index,element);
}

Section Section::insertSubsection(unsigned index,const std::string& headingText) {
  return getImpl<detail::Section_Impl>()->insertSubsection(index,headingText);
}

void Section::erase(unsigned index) {
  getImpl<detail::Section_Impl>()->erase(index);
}

void Section::makeHeadingLevelsConsistent() {
  getImpl<detail::Section_Impl>()->makeHeadingLevelsConsistent();
}

unsigned Section::numElements() const {
  return getImpl<detail::Section_Impl>()->numElements();
}

unsigned Section::numSubsections() const {
  return getImpl<detail::Section_Impl>()->numSubsections();
}

Section::Section(boost::shared_ptr<detail::Section_Impl> impl)
  : SectionElement(impl)
{}

} // openstudio
