/*
Copyright (c) 2011, Stanislaw Adaszewski
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Stanislaw Adaszewski nor the
      names of other contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "QDomNodeModel"

#include <QDomNode>
#include <QDomDocument>
#include <QUrl>
#include <QVector>
#include <QSourceLocation>
#include <QVariant>

class MyDomNode: public QDomNode
{
public:
	MyDomNode(const QDomNode& other):
		QDomNode(other)
	{
	}

	MyDomNode(QDomNodePrivate *otherImpl):
		QDomNode(otherImpl)
	{
	}

	QDomNodePrivate* getImpl()
	{
		return impl;
	}
};

QDomNodeModel::QDomNodeModel(QXmlNamePool pool, QDomDocument doc):
	m_Pool(pool), m_Doc(doc)
{

}

QUrl QDomNodeModel::baseUri (const QXmlNodeModelIndex &) const
{
	// TODO: Not implemented.
	return QUrl();
}

QXmlNodeModelIndex::DocumentOrder QDomNodeModel::compareOrder (
	const QXmlNodeModelIndex & ni1,
	const QXmlNodeModelIndex & ni2 ) const
{
	QDomNode n1 = toDomNode(ni1);
	QDomNode n2 = toDomNode(ni2);

	if (n1 == n2)
		return QXmlNodeModelIndex::Is;

	Path p1 = path(n1);
	Path p2 = path(n2);

	for (int i = 1; i < p1.size(); i++)
		if (p1[i] == n2)
			return QXmlNodeModelIndex::Follows;

	for (int i = 1; i < p2.size(); i++)
		if (p2[i] == n1)
			return QXmlNodeModelIndex::Precedes;

	for (int i = 1; i < p1.size(); i++)
		for (int j = 1; j < p2.size(); j++)
		{
			if (p1[i] == p2[j]) // Common ancestor
			{
				int ci1 = childIndex(p1[i-1]);
				int ci2 = childIndex(p2[j-1]);

				if (ci1 < ci2)
					return QXmlNodeModelIndex::Precedes;
				else
					return QXmlNodeModelIndex::Follows;
			}
		}

	return QXmlNodeModelIndex::Precedes; // Should be impossible!
}

QUrl QDomNodeModel::documentUri (const QXmlNodeModelIndex&) const
{
	// TODO: Not implemented.
	return QUrl();
}

QXmlNodeModelIndex QDomNodeModel::elementById ( const QXmlName & id ) const
{
	return fromDomNode(m_Doc.elementById(id.toClarkName(m_Pool)));
}

QXmlNodeModelIndex::NodeKind QDomNodeModel::kind ( const QXmlNodeModelIndex & ni ) const
{
	QDomNode n = toDomNode(ni);
	if (n.isAttr())
		return QXmlNodeModelIndex::Attribute;
	else if (n.isText())
		return QXmlNodeModelIndex::Text;
	else if (n.isComment())
		return QXmlNodeModelIndex::Comment;
	else if (n.isDocument())
		return QXmlNodeModelIndex::Document;
	else if (n.isElement())
		return QXmlNodeModelIndex::Element;
	else if (n.isProcessingInstruction())
		return QXmlNodeModelIndex::ProcessingInstruction;

	return (QXmlNodeModelIndex::NodeKind) 0;
}

QXmlName QDomNodeModel::name ( const QXmlNodeModelIndex & ni ) const
{
	QDomNode n = toDomNode(ni);

  if (n.isAttr() || n.isElement() || n.isProcessingInstruction()){
    if (n.localName().isNull()){
      //std::cout << n.nodeName().toStdString() << std::endl;
      //std::cout << n.localName().toStdString() << std::endl;
      //std::cout << n.namespaceURI().toStdString() << std::endl;
      //std::cout << n.prefix().toStdString() << std::endl;
      return QXmlName(m_Pool, n.nodeName(), QString(), QString());
    }else{
      //std::cout << n.nodeName().toStdString() << std::endl;
      //std::cout << n.localName().toStdString() << std::endl;
      //std::cout << n.namespaceURI().toStdString() << std::endl;
      //std::cout << n.prefix().toStdString() << std::endl;
      return QXmlName(m_Pool, n.localName(), n.namespaceURI(), n.prefix());
    }

  }

	return QXmlName(m_Pool, QString(), QString(), QString());
}

QVector<QXmlName> QDomNodeModel::namespaceBindings(const QXmlNodeModelIndex&) const
{
	// TODO: Not implemented.
	return QVector<QXmlName>();
}

QVector<QXmlNodeModelIndex> QDomNodeModel::nodesByIdref(const QXmlName&) const
{
	// TODO: Not implemented.
	return QVector<QXmlNodeModelIndex>();
}

QXmlNodeModelIndex QDomNodeModel::root ( const QXmlNodeModelIndex & ni ) const
{
	QDomNode n = toDomNode(ni);
	while (!n.parentNode().isNull())
		n = n.parentNode();

	return fromDomNode(n);
}

QSourceLocation	QDomNodeModel::sourceLocation(const QXmlNodeModelIndex&) const
{
	// TODO: Not implemented.
	return QSourceLocation();
}

QString	QDomNodeModel::stringValue ( const QXmlNodeModelIndex & ni ) const
{
	QDomNode n = toDomNode(ni);

	if (n.isProcessingInstruction())
		return n.toProcessingInstruction().data();
	else if (n.isText())
		return n.toText().data();
	else if (n.isComment())
		return n.toComment().data();
	else if (n.isElement())
		return n.toElement().text();
	else if (n.isDocument())
		return n.toDocument().documentElement().text();
	else if (n.isAttr())
		return n.toAttr().value();

	return QString();
}

QVariant QDomNodeModel::typedValue ( const QXmlNodeModelIndex & ni ) const
{
	return qVariantFromValue(stringValue(ni));
}

QXmlNodeModelIndex QDomNodeModel::fromDomNode(const QDomNode &n) const
{
	if (n.isNull())
		return QXmlNodeModelIndex();

	return createIndex(MyDomNode(n).getImpl(), 0);
}

QDomNode QDomNodeModel::toDomNode(const QXmlNodeModelIndex &ni) const
{
	return MyDomNode((QDomNodePrivate*) ni.data());
}

QDomNodeModel::Path QDomNodeModel::path(const QDomNode &n) const
{
	Path res;
	QDomNode cur = n;
	while (!cur.isNull())
	{
		res.push_back(cur);
		cur = cur.parentNode();
	}
	return res;
}

int QDomNodeModel::childIndex(const QDomNode &n) const
{
	QDomNodeList children = n.parentNode().childNodes();
	for (int i = 0; i < children.size(); i++)
		if (children.at(i) == n)
			return i;

	return -1;
}

QVector<QXmlNodeModelIndex> QDomNodeModel::attributes ( const QXmlNodeModelIndex & ni ) const
{
	QDomElement n = toDomNode(ni).toElement();
	QDomNamedNodeMap attrs = n.attributes();
	QVector<QXmlNodeModelIndex> res;
	for (int i = 0; i < attrs.size(); i++)
	{
		res.push_back(fromDomNode(attrs.item(i)));
	}
	return res;
}

QXmlNodeModelIndex QDomNodeModel::nextFromSimpleAxis ( SimpleAxis axis, const QXmlNodeModelIndex & ni) const
{
	QDomNode n = toDomNode(ni);
	switch(axis)
	{
	case Parent:
		return fromDomNode(n.parentNode());

	case FirstChild:
		return fromDomNode(n.firstChild());

	case PreviousSibling:
		return fromDomNode(n.previousSibling());

	case NextSibling:
		return fromDomNode(n.nextSibling());
	}

	return QXmlNodeModelIndex();
}
