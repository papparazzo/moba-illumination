/*
 *  Project:    moba-connector
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
 *  along with this program. If not, see <https://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "monitor.h"

#include <iomanip>
#include <string>

void Monitor::appendAction(const std::string &action, const std::string &message) {
    std::lock_guard l{m};

    std::cerr << moba::LogLevel::NOTICE << "[" << action << "] " << message << std::endl;
}

void Monitor::appendAction(const moba::LogLevel level, const std::string &action) {
    std::lock_guard l{m};

    std::cerr << level << action << std::endl;
}

void Monitor::printException(const std::string &where, const std::string &what) {
    std::lock_guard l{m};
    std::cerr << moba::LogLevel::CRITICAL << where << " " << what << std::endl;
}

void Monitor::printStatus(const std::string &status) {
    std::cerr << moba::LogLevel::NOTICE << "Status switched to <" << status << ">" << std::endl;
}
