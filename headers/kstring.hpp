#ifndef __KSTRING_HPP__
#define __KSTRING_HPP__

#include <QString>
/**
 * @brief The KString class
 * Solves the interface problem with FlatBuffers, which seems designed to work best with std::string
 */

class KString : public QString {
 public:
  const char* c_str() const {
    return toUtf8().constData();
  }
};

#endif // __KSTRING_HPP__
