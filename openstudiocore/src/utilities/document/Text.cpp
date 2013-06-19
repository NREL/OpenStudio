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

#include <utilities/document/Text.hpp>
#include <utilities/document/Text_Impl.hpp>
#include <utilities/document/DocumentRegex.hpp>

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/archive/archive_exception.hpp>

namespace openstudio {

namespace detail {

  Text_Impl::Text_Impl() {}

  Text_Impl::Text_Impl(const std::string& paragraph) : m_paragraphs(1u,paragraph) {}

  Text_Impl::Text_Impl(const std::vector<std::string>& paragraphs) : m_paragraphs(paragraphs) {}

  DocumentElement Text_Impl::clone() const {
    boost::shared_ptr<Text_Impl> impl(new Text_Impl(*this));
    return Text(impl).cast<DocumentElement>();
  }

  std::vector<std::string> Text_Impl::paragraphs() const { return m_paragraphs; }

  std::string Text_Impl::getParagraph(unsigned index) const { 
    if (index >= numParagraphs()) {
      LOG_AND_THROW("Cannot return paragraph " << index << ", as there are only "
        << numParagraphs() << " in this Text object.");
    }
    return m_paragraphs[index];
  }

  // SETTERS

  void Text_Impl::clear() {
    m_paragraphs.clear();
  }

  void Text_Impl::replaceAll(const std::string& paragraph) {
    m_paragraphs = StringVector(1u,paragraph);
  }

  void Text_Impl::replaceAll(const std::vector<std::string>& paragraphs) {
    m_paragraphs = paragraphs;
  }

  void Text_Impl::replace(unsigned index, const std::string& paragraph) {
    unsigned n = numParagraphs();
    if (index > n) {
      LOG_AND_THROW("There is no paragraph at index " << index << " to replace.");
    }
    if (index == n) {
      LOG(Warn,"There is no paragraph at index " << index << ", appending new paragraph instead.");
      append(paragraph);
      return;
    }
    m_paragraphs[index] = paragraph;
  }

  void Text_Impl::append(const std::string& paragraph) {
    m_paragraphs.push_back(paragraph);
  }

  void Text_Impl::append(const std::vector<std::string>& paragraphs) {
    m_paragraphs.insert(m_paragraphs.end(),paragraphs.begin(),paragraphs.end());
  }

  void Text_Impl::insert(unsigned index, const std::string& paragraph) {
    unsigned n = numParagraphs();
    if (index > n) {
      LOG_AND_THROW("Cannot insert a paragraph at index " << index << ", because there are "
          << "currently " << n << " paragraphs in this Text object.");
    }
    if (index == n) { 
      append(paragraph); 
      return;
    }
    StringVector::iterator it = m_paragraphs.begin();
    for (unsigned i = 0; i < index; ++i, ++it) {}
    m_paragraphs.insert(it,paragraph);
  }

  void Text_Impl::erase(unsigned index) {
    if (index < numParagraphs()) {
      StringVector::iterator it = m_paragraphs.begin();
      for (unsigned i = 0; i < index; ++i, ++it) {}
      m_paragraphs.erase(it);
    }
  }

  unsigned Text_Impl::numParagraphs() const {
    return m_paragraphs.size();
  }

  std::string Text_Impl::print(DocumentFormat fmt) const {
    if (fmt == DocumentFormat::COUT) { return mf_printCOUT(); }
    if (fmt == DocumentFormat::LaTeX) { return mf_printLaTeX(); }
    if (fmt == DocumentFormat::XHTML) { return mf_printXHTML(); }
    return std::string();
  }

  std::ostream& Text_Impl::printToStream(std::ostream& os, DocumentFormat fmt) const {
    os << print(fmt);
    return os;
  }

  // PRIVATE

  std::string Text_Impl::mf_printCOUT() const {
    std::stringstream ss;
    BOOST_FOREACH(const std::string& str,m_paragraphs) {
      ss << std::endl 
         << str << std::endl;
    }
    return ss.str();
  }

  std::string Text_Impl::mf_printLaTeX() const {
    std::stringstream ss;
    BOOST_FOREACH(const std::string& str,m_paragraphs) {
      ss << std::endl << formatText(str,DocumentFormat(DocumentFormat::LaTeX)) << std::endl;
    }
    return ss.str();
  }

  std::string Text_Impl::mf_printXHTML() const {
    std::stringstream ss;
    BOOST_FOREACH(const std::string& str,m_paragraphs) {
      ss << std::endl 
         << "<p>" << std::endl 
         << formatText(str,DocumentFormat(DocumentFormat::XHTML)) << std::endl 
         << "</p>" << std::endl;
    }
    return ss.str();
  }

} // detail

Text::Text() 
  : SectionElement(boost::shared_ptr<detail::Text_Impl>(new detail::Text_Impl()))
{}

Text::Text(const std::string& paragraph)
  : SectionElement(boost::shared_ptr<detail::Text_Impl>(new detail::Text_Impl(paragraph)))
{}

Text::Text(const std::vector<std::string>& paragraphs)
  : SectionElement(boost::shared_ptr<detail::Text_Impl>(new detail::Text_Impl(paragraphs)))
{}

std::vector<std::string> Text::paragraphs() const {
  return getImpl<detail::Text_Impl>()->paragraphs();
}

std::string Text::getParagraph(unsigned index) const {
  return getImpl<detail::Text_Impl>()->getParagraph(index);
}

void Text::clear() {
  getImpl<detail::Text_Impl>()->clear();
}

void Text::replaceAll(const std::string& paragraph) {
  return getImpl<detail::Text_Impl>()->replaceAll(paragraph);
}

void Text::replaceAll(const std::vector<std::string>& paragraphs) {
  getImpl<detail::Text_Impl>()->replaceAll(paragraphs);
}

void Text::replace(unsigned index, const std::string& paragraph) {
  getImpl<detail::Text_Impl>()->replace(index,paragraph);
}

void Text::append(const std::string& paragraph) {
  getImpl<detail::Text_Impl>()->append(paragraph);
}

void Text::append(const std::vector<std::string>& paragraphs) {
  getImpl<detail::Text_Impl>()->append(paragraphs);
}

void Text::insert(unsigned index, const std::string& paragraph) {
  getImpl<detail::Text_Impl>()->insert(index,paragraph);
}

void Text::erase(unsigned index) {
  getImpl<detail::Text_Impl>()->erase(index);
}

unsigned Text::numParagraphs() const {
  return getImpl<detail::Text_Impl>()->numParagraphs();
}

Text::Text(boost::shared_ptr<detail::Text_Impl> impl)
  : SectionElement(impl)
{}

} // openstudio
