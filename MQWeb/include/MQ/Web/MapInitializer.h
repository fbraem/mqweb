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
#ifndef _MQWeb_MapInitializer_H
#define _MQWeb_MapInitializer_H

// In the future when all compilers support c++11
// this can be replaced by an initializer list
// http://www.stroustrup.com/C++11FAQ.html#init-list

namespace MQ {
namespace Web {

template <typename T, typename U>
class MapInitializer
	/// Utility class for initializing a std::map
{
public:
	MapInitializer(const T& key, const U& val)
	{
		_map[key] = val;
	}


	MapInitializer<T, U>& operator()(const T& key, const U& val)
	{
		_map[key] = val;
		return *this;
	}


	operator std::map<T, U>()
	{
		return _map;
	}

private:
	std::map<T, U> _map;
};

}} // Namespace MQ::Web

#endif //_MQWeb_MapInitializer_H
