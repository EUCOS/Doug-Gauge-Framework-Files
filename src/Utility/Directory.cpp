#include <Utility/Directory.h>

#include <sys/stat.h>

#include <cassert>
#include <functional>
#include <iostream>

#include <iterator>

#include <cstring>

bool Utility::Dir::Create(std::string dir) {
  std::function<bool(std::string)> createDir = [](std::string dir) {
    const char *path = dir.c_str();
    struct stat status;
    return (stat(path, &status) == 0 || mkdir(path, 0777) == 0);
  };

  if (*(--dir.end()) != '/') dir += "/";
  std::string path;
  size_t pos = (dir[0] == '/') ? dir.find('/', 1) : dir.find('/', 0);
  while (pos != std::string::npos) {
    path = dir.substr(0, pos);
    if (!createDir(path)) return false;
    pos = dir.find('/', pos + 1);
  }
  return true;
}

std::string Utility::Dir::Temporary(std::string root) {
  if (root.back() != '/') root += "/";
  root += "XXXXXX";
  char *directory = new char[root.size()];
  strcpy(directory, root.c_str());
  assert(mkdtemp(directory) != NULL);
  root = std::string(directory) + "/";
  delete directory;
  return root;
}
