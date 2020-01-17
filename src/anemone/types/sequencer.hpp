/**
 * @file   types/sequencer.hpp
 * @brief  Sequencer Related Types
 * @author coco
 * @date   2020-01-17
 *************************************************/


#ifndef TYPES_SEQUENCER_H
#define TYPES_SEQUENCER_H

#include <map>
#include <vector>


/// @brief Pulse Per Quarter Note (ppqn).
enum class PPQN {
                 One       = 1,
                 Two       = 2,
                 Four      = 4,
                 Eight     = 8,
                 Sixteen   = 16,
                 ThirtyTwo = 32,
                 SixtyFour = 64,
                 Max       = 64,
};


namespace types {

  
  /// @brief sequence page types.
  namespace page {
    /// @brief type alias for a page index; used to identify pages.
    typedef unsigned int idx_t;
  }

  
  /// @brief sequence step types.
  namespace step {
    /// @brief type alias for sequence step index; a sequential id.
    ///
    /// @remark
    /// a step index is used to index a step in a sequence. unlike its granular
    /// counter-part, it represents the index that one actually sees on the sequencer
    /// and can be thought of as the sequential index of an event within the sequence.
    typedef unsigned int idx_t;

    /// @brief type alias for granular sequence indices.
    ///
    /// @remark
    /// granular step indices are used to index into sequence steps and are
    /// used as the active step maintained within a Part as it is advanced by
    /// the sequence scheduler.
    /// these indices are "granular" or "fine-grained" because each step represents
    /// a 1/constants::PPQN_MAX step (the shortest step possible) and, given how
    /// sequences are stored/indexed internally, allows for fluid transitions of a
    /// Part's effective ppqn.
    typedef unsigned int granular_idx_t;

    /// @brief page relative step index. provides the page index and page-relative step index.
    ///
    /// @details
    /// 
    struct page_relative_idx_t {
      page::idx_t page;
      idx_t       step;

      bool operator==(const page_relative_idx_t& rhs) {
        return
          page == rhs.page &&
          step == rhs.step;
      };

      idx_t to_absolute_idx(unsigned int page_size) {
        return (page * page_size) + step;
      };
    };

    /// @brief namespace encapsulates types related to step events
    namespace event {
      /// @brief Step event protocol.
      enum class protocol_t {
                             /// MIDI protocol.
                             Midi,
                             /// Non-Registered Parameter Number (NRPN).
                             NRPN,
      };

      /// @brief Step event unique identifier.
      ///
      /// @details
      /// for midi events:
      /// `0x<status byte><data 1 byte>`
      /// where,
      ///   `status byte` is the command + channel
      ///   `data 1 byte` is a 0-127 value (such as pitch)
      ///
      typedef unsigned short id_t;
      
      /// @brief Step event data type.
      typedef std::vector<unsigned char> data_t;
    }

    /// @brief Step event type.
    struct event_t {
      event::protocol_t protocol;
      event::id_t       id;
      event::data_t     data;
    };
  }


  /// @brief namespace encapsulates types associated with `sequence`s.
  namespace sequence {
    /// @brief a sequence layer type.
    ///
    /// @details
    /// The sequence layer data structure stores sequencer step events in a map keyed
    /// by a unique event id modulo certain characteristic of the event. In other words,
    /// each layer is a step event which can be distinguished from other step events in
    /// a particular way. Currently, the unique id is computed from the midi channel from
    /// which the event came from, the "type" of midi event (e.g. cc, note, nrpn, etc...),
    /// and the "value" of the event (e.g. the cc, note, or nrpn value, etc...). Structuring
    /// the data this way allows for (1) polyphony (2) efficiently muting/soloing specific
    /// voices.
    ///
    typedef std::map<step::event::id_t, step::event_t> layer_t;
  }

  /// @brief type alias for sequence storing data structure.
  typedef std::map<step::idx_t, sequence::layer_t> sequence_t;

}

#endif
