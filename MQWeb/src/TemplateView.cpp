/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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
#include "Poco/JSON/TemplateCache.h"

#include "MQ/Web/TemplateView.h"

namespace MQ {
namespace Web {

TemplateView::TemplateView(const std::string &templateFile) :
	_templateFile(templateFile)
{
}


TemplateView::~TemplateView()
{
}

void TemplateView::initializeResponse(Poco::Net::HTTPServerResponse& response)
{
	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	// Don't cache are views
	response.set("Cache-Control", "no-cache,no-store,must-revalidate");
	response.set("Pragma", "no-cache");
	response.set("Expires", "0");
}

bool TemplateView::render(Poco::JSON::Object::Ptr data, std::ostream& os)
{
	bool ok = true;

	Poco::JSON::Template::Ptr tpl;
	try
	{
		tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path(_templateFile));
		tpl->render(data, os);
		os.flush();
	}
	catch(Poco::FileNotFoundException&)
	{
		poco_error_f1(Poco::Logger::get("mq.web"), "Can't render template %s", _templateFile);
		ok = false;
	}
	return ok;
}


}} // Namespace MQ::Web
