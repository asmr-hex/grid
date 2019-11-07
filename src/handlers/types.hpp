#ifndef HANDLER_FN_H
#define HANDLER_FN_H

#include <vector>

#include <monome.h>

#include "../io/io.hpp"
#include "../state/state.hpp"
#include "../config/config.hpp"


typedef void (*midi_handler)(IO *, State*, Config *,std::vector<unsigned char> *);
typedef void (*monome_handler)(IO *, State*, Config *, const monome_event_t *);

#endif
