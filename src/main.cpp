/*
 *  Project:    moba-environment
 *
 *  Copyright (C) 2025 Stefan Paproth <pappi-@gmx.de>
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

#include <memory>

#include <moba-common/daemon.h>
#include <moba-common/ini.h>
#include <moba-common/version.h>
#include <moba-common/helper.h>
#include <moba-common/ipc.h>

#include "../config.h"
#include "msghandler.h"

#include "moba/socket.h"
#include "moba/endpoint.h"
#include "monitor.h"

namespace {
    moba::AppData appData = {
        PACKAGE_NAME,
        moba::Version{PACKAGE_VERSION},
        __DATE__,
        __TIME__,
        "::1",
        7000
    };
}

int main(const int argc, char *argv[]) {
    if(argc == 2) {
        appData.host = std::string(argv[1]);
    }

	unsigned int deviceId = 0;

//    moba::Daemon daemon{appData.appName};

//    daemon.daemonize();

    auto ini = std::make_shared<moba::Ini>(std::string{"./config/" /*SYSCONFDIR*/} + "/" + PACKAGE_NAME + ".conf");

    appData.port = ini->getInt("settings", "port", appData.port);
    appData.host = ini->getString("settings", "host", appData.host);
	deviceId = ini->getInt("settings", "device_id", deviceId);

    const auto socket = std::make_shared<Socket>(appData.host, appData.port);
    const auto endpoint = EndpointPtr{new Endpoint{
        socket,
        appData.appName,
        std::string{"environment #"} + std::to_string(deviceId),
        appData.version,
        {Message::SYSTEM, Message::TIMER, Message::ENVIRONMENT}
    }};

    const auto monitor = std::make_shared<Monitor>();

    MsgHandler handler{deviceId, endpoint, monitor};
    handler();

    return EXIT_SUCCESS;
}
