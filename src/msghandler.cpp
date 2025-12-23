/*
 *  Project:    moba-environment
 *
 *  Copyright (C) 2022 Stefan Paproth <pappi-@gmx.de>
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
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "moba/registry.h"
#include "moba/timermessages.h"
#include "moba/environmentmessages.h"

#include <moba-common/ipc.h>
#include <thread>
#include <syslog.h>

#include "msghandler.h"
#include "moba/registry.h"
#include "moba/clientmessages.h"

#include <thread>
#include <functional>
#include <utility>

MsgHandler::MsgHandler(
    const unsigned int deviceId,
    EndpointPtr endpoint,
    MonitorPtr monitor
): endpoint{std::move(endpoint)}, monitor{std::move(monitor)}, deviceId{deviceId} {
}

void MsgHandler::operator()() {
    while(!closing) {
        try {
            endpoint->connect();
            Registry registry;
            registry.registerHandler<ClientShutdown>([this]{shutdown();});
			registry.registerHandler<ClientSelfTesting>([this]{selftesting();});
            registry.registerHandler<ClientReset>(std::bind(&MsgHandler::reset, this, std::placeholders::_1));
            registry.registerHandler<EnvironmentSetFunctions>(std::bind(&MsgHandler::setFunctions, this, std::placeholders::_1));

            while(!closing) {
                registry.handleMsg(endpoint->waitForNewMsg());
            }
        } catch(const std::exception &e) {
            monitor->printException("MsgHandler::operator()()", e.what());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
}

void MsgHandler::shutdown() {
    monitor->appendAction(moba::LogLevel::NOTICE, "Shutdown requested by client");
	execl("/sbin/shutdown", "shutdown", "-h", "now", NULL);
    closing = true;
}

void MsgHandler::reset(ClientReset &&data) const {
    monitor->appendAction(moba::LogLevel::NOTICE, "Reset requested by client");
    if (data.hardware) {
        execl("/usr/local/bin/reboot", "reboot", "-f", static_cast<char *>(nullptr));
    }
}

void MsgHandler::selftesting() {

}

void MsgHandler::setFunctions(EnvironmentSetFunctions &&data) const {
    for(const auto& iter : data.functions) {
        if (iter.address.deviceId != deviceId) {
            continue;
        }
        std::cout <<
            "Controller <" << iter.address.address.controller << ":" << iter.address.address.port <<
                "> set to <" << functionStateEnumToString(iter.functionState) << std::endl;
    }
}
