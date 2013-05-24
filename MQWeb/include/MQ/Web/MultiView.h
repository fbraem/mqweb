/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 * will be approved by the European Commission - subsequent
 * versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 * Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://joinup.ec.europa.eu/software/page/eupl
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the Licence is
 * distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */

#ifndef _MQWeb_MultiView_H
#define _MQWeb_MultiView_H

#include <map>

#include "Poco/Net/HTTPServerResponse.h"
#include "MQ/Web/TemplateView.h"

namespace MQ {
namespace Web {

class MultiView : public TemplateView
	/// A template view that can have child views
{
public:

	typedef std::multimap<std::string, Poco::SharedPtr<View> > ViewMap;

	MultiView(const std::string& templateFile);
		/// Constructor

	virtual ~MultiView();
		/// Destructor

	void add(const std::string& name, Poco::SharedPtr<View> view);
		/// Add a view

	bool render(Poco::JSON::Object::Ptr data, std::ostream& os);
		/// Renders the JSON object

private:
	ViewMap _views;
};


inline void MultiView::add(const std::string& name, Poco::SharedPtr<View> view)
{
	_views.insert(std::pair<std::string, Poco::SharedPtr<View> >(name, view));
}

} } // Namespace MQ::Web

#endif //_MQWeb_MultiView_H
