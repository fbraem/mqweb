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
#include "MQ/Web/AuthenticationInformationCopy.h"

namespace MQ {
namespace Web {

AuthenticationInformationCopy::AuthenticationInformationCopy(CommandServer& commandServer, Poco::JSON::Object::Ptr input, bool replace)
: PCFReplaceCommand(commandServer, MQCMD_CREATE_AUTH_INFO, "AuthenticationInformation", input, replace)
{
	// Required parameters
	addParameter<std::string>(MQCACF_FROM_AUTH_INFO_NAME, "FromAuthInfoName");
	addParameter<std::string>(MQCACF_TO_AUTH_INFO_NAME, "ToAuthInfoName");
	addParameterNumFromString(MQIA_AUTH_INFO_TYPE, "AuthInfoType");

	fillPCF();
}

AuthenticationInformationCopy::~AuthenticationInformationCopy()
{
}

}} //  Namespace MQ::Web
