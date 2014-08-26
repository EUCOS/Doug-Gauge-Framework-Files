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
 * @file include/Survey.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 12.05.2012
 *
 * @brief The Gauge::Survey namespace provides several functions to run both
 * parallel and serial surveys.
 */

#include <GeometryFactory.h>
#include <InputFactory.h>
#include <ProcessorList.h>

namespace Gauge {
  namespace Survey {
    void Parallel(
        int &argc, char **argv,
        Gauge::ProcessorList &&processors,
        Gauge::GeometryFactory *geometry_factory,
        Gauge::InputFactory::Generic *inputs,
        std::string log_file
      );

    void Serial(
        Gauge::ProcessorList &&processors,
        Gauge::GeometryFactory *geometry_factory,
        Gauge::InputFactory::Generic *inputs,
        std::string log_file
      );
  }
}
