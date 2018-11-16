#pragma once

#include "model/color.h"
#include "model/messages.h"
#include "model/iobject.h"

#include <string>

void
save_image(const Image &, const std::string &);

std::vector<Image>
load_task(const std::string &);

std::string
result_text(const std::vector<IObject> &, ssize_t);
