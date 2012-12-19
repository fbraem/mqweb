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
#ifndef _MQWeb_ChannelMQMapper_H
#define _MQWeb_ChannelMQMapper_H

#include <MQ/Web/MQMapper.h>

namespace MQ {
namespace Web {

class ChannelMQMapper : public MQMapper
	/// Maps channel object to Websphere MQ
{
public:

	ChannelMQMapper(CommandServer& commandServer);


	virtual ~ChannelMQMapper();


	void change(const Poco::JSON::Object::Ptr&obj);


	void create(const Poco::JSON::Object::Ptr& obj, bool replace = false);


	void copy(const Poco::JSON::Object::Ptr& obj, bool replace = false);


	Poco::JSON::Array::Ptr inquire(const Poco::JSON::Object::Ptr& filter);


private:


	ChannelMQMapper(const ChannelMQMapper&);

};

}} // Namespace MQ::Web

#endif // _MQWeb_ChannelMQMapper_H
