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
 * @file include/Logger.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.27.2012
 *
 * @brief The Gauge::Logger class is defined.
 */

#pragma once

#include <ctime>
#include <cstring>
#include <fstream>
#include <string>

namespace Gauge {
  class Logger {
    public:
      Logger(std::string log_file);
      ~Logger() {};

      void Log(std::string message);

    private:
      std::ofstream stream;

      char *getTime() const;
  };
}
