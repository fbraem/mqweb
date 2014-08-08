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

#ifndef _MQWeb_JSONPView_H
#define _MQWeb_JSONPView_H

#include "Poco/Net/HTTPServerResponse.h"
#include "MQ/Web/JSONView.h"

namespace MQ {
namespace Web {

class JSONPView : public JSONView
	/// A special JSON view that implments the JSONP technique
{
public:
	JSONPView(const std::string& callback);
		/// Constructor

	virtual ~JSONPView();
		/// Destructor

	bool render(Poco::JSON::Object::Ptr data, std::ostream& os);
		/// Renders the JSON as JSONP

private:
	std::string _callback;
};

} } // Namespace MQ::Web

#endif //_MQWeb_JSONPView_H
