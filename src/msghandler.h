/*
 *  Project:    moba-environment
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/agpl.txt>.
 *
 */


#pragma once

#include <memory>

#include "monitor.h"
#include "moba/endpoint.h"
#include "moba/environmentmessages.h"
#include "moba/systemmessages.h"

#include "moba/clientmessages.h"

class MsgHandler final {
public:
    MsgHandler(unsigned int deviceId, EndpointPtr endpoint, MonitorPtr monitor);

    ~MsgHandler() noexcept = default;

    MsgHandler(const MsgHandler&) = delete;
    MsgHandler(MsgHandler&&) = default;
    MsgHandler& operator=(const MsgHandler&) = delete;
    
    void operator()();

private:
    void setHardwareState(SystemHardwareStateChanged &&data) const;
    void setFunctions(EnvironmentSetFunctions &&data) const;

    void shutdown();
	void selftesting();
    void reset(ClientReset &&data) const;

    bool closing{false};

    EndpointPtr  endpoint;
    MonitorPtr   monitor;
    unsigned int deviceId;
};
