#ifndef JPG_H_INCLUDED
#define JPG_H_INCLUDED

#include <string>

#include "AbstractImage.h"

class JPG : public AbstractImage {
public:
    JPG(const std::string& path, const std::string& name);

    void save() override;
};

#endif // JPG_H_INCLUDED
