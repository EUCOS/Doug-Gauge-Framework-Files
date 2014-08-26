/*
 * This file is part of The Gauge Framework.
 *
 * The Gauge Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Gauge Framework is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file src/Logger.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.27.2012
 */

#include <Logger.h>

Gauge::Logger::Logger(std::string log_file) {
  stream.open(log_file);
  stream << "Log opened at " << getTime() << "." << std::endl;
}

void Gauge::Logger::Log(std::string message) {
  stream << getTime() << " -> " << message << std::endl;
}

char *Gauge::Logger::getTime() const {
  std::time_t result = std::time(NULL);
  char *time = std::asctime(std::localtime(&result));
  time[strlen(time) - 1] = '\0';
  return time;
}
