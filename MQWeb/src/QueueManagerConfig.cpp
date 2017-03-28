/*
 * Copyright 2017 MQWeb - Franky Braem
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

#include "MQ/Web/QueueManagerConfig.h"

namespace MQ {
namespace Web {

QueueManagerConfig::QueueManagerConfig(const std::string& qmgrName) : _qmgrName(qmgrName)
{
}

QueueManagerConfig::~QueueManagerConfig()
{
}

} } // Namespace MQ::Web
