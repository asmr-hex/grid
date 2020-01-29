/**
 * @file   state/selectors/midi_events.hpp
 * @brief  Selectors for Midi Events
 * @author coco
 * @date   2020-01-29
 *************************************************/


#ifndef STATE_SELECTORS_MIDI_EVENTS_H
#define STATE_SELECTORS_MIDI_EVENTS_H

#include "anemone/types.hpp"
#include "anemone/state/root.hpp"


/// @brief selects all midi events on the current step for a given instrument
void get_midi_events_for(State::instrument_t, State::root_t, std::vector<types::step::event_t> *results);

void get_midi_on_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results);
void get_midi_off_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results);
void get_midi_cc_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results);
void get_midi_nrpn_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results);

#endif
