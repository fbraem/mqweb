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
#include "MQ/Web/JSONPView.h"

namespace MQ {
namespace Web {

JSONPView::JSONPView(const std::string& callback) : JSONView(), _callback(callback)
{
}


JSONPView::~JSONPView()
{
}

bool JSONPView::render(Poco::JSON::Object::Ptr data, std::ostream& os)
{
	os << _callback;
	os << '(';
	JSONView::render(data, os);
	os << ");";
	return true;
}


}} // Namespace MQ::Web
