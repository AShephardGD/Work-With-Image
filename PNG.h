#ifndef PNG_H_INCLUDED
#define PNG_H_INCLUDED

#include "AbstractImage.h"

#include <string>

class PNG : public AbstractImage {
public:
    PNG(const std::string& path, const std::string& name);

    void save() override;
};

#endif // PNG_H_INCLUDED
