#pragma once

#include <string>

namespace Utility {
  namespace Dir {
    bool Create(std::string path);
    std::string Temporary(std::string root);
  }
}
