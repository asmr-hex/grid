#ifndef HANDLER_FN_H
#define HANDLER_FN_H

#include <vector>

#include <monome.h>

#include "../io/io.hpp"
#include "../state/state.hpp"
#include "../config/config.hpp"
#include "../animation/animator.hpp"


typedef void (*midi_handler_fn)(IO *, State*, Config *,std::vector<unsigned char> *); // TODO add Animator to midi handlers
//typedef void (*grid_handler_fn)(IO *, Animator *, State*, Config *, const monome_event_t *);
typedef void (*grid_handler_fn)(IO *, Animator *, State*, Config *, const grid_event_t *);


#endif
