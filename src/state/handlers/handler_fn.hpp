#ifndef HANDLER_FN_H
#define HANDLER_FN_H

#include <vector>

#include <monome.h>


typedef void (*midi_handler)(double, std::vector<unsigned char> *, void *);
typedef void (*monome_handler)(const monome_event_t *, void *);

#endif
