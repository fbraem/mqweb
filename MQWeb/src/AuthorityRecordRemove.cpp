/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "MQ/Web/AuthorityRecordRemove.h"

namespace MQ {
namespace Web {

AuthorityRecordRemove::AuthorityRecordRemove(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFSimpleCommand(commandServer, MQCMD_DELETE_AUTH_REC, "AuthorityRecord", input)
{
	addParameter<std::string>(MQCACF_AUTH_PROFILE_NAME, "ProfileName");
	addParameterNumFromString(MQIACF_OBJECT_TYPE, "ObjectType");

	// Optional parameters
	Poco::JSON::Array::Ptr jsonPrincipals = input->getArray("PrincipalNames");
	if (!jsonPrincipals.isNull())
	{
		std::vector<std::string> principals;
		for(Poco::JSON::Array::ConstIterator it = jsonPrincipals->begin(); it != jsonPrincipals->end(); ++it)
		{
			principals.push_back(*it);
		}
		if (principals.size() > 0)
		{
			pcf()->addStringList(MQCACF_PRINCIPAL_ENTITY_NAMES, principals);
		}
	}

	Poco::JSON::Array::Ptr jsonGroups = input->getArray("GroupNames");
	if (!jsonGroups.isNull())
	{
		std::vector<std::string> groups;
		for(Poco::JSON::Array::ConstIterator it = jsonGroups->begin(); it != jsonGroups->end(); ++it)
		{
			groups.push_back(*it);
		}
		if (groups.size() > 0)
		{
			pcf()->addStringList(MQCACF_GROUP_ENTITY_NAMES, groups);
		}
	}
}

AuthorityRecordRemove::~AuthorityRecordRemove()
{
}

}} //  Namespace MQ::Web
