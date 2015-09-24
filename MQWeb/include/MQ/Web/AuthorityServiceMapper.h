/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or ? as soon they
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
#ifndef _MQWeb_AuthorityServiceMapper_H
#define _MQWeb_AuthorityServiceMapper_H

#include "MQ/Web/MQMapper.h"

namespace MQ {
namespace Web {

class AuthorityServiceMapper : public MQMapper
	/// Maps queue object to Websphere MQ
{
public:

	AuthorityServiceMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input);
		/// Constructor

	virtual ~AuthorityServiceMapper();
		/// Destructor

	void change();
		/// Implements the change authority service command. Not implemented yet.

	void create(bool replace = false);
		/// Implements the create authority service command. Not implemented yet.

	void copy(bool replace = false);
		/// Implements the copy authority service command. Not implemented yet.

	Poco::JSON::Array::Ptr inquire();
		/// Implements the inquire authority service command.

private:

	AuthorityServiceMapper(const AuthorityServiceMapper&);
};


}} // Namespace MQ::Web

#endif // _MQWeb_AuthorityServiceMapper_H
