/**
 * @file   types/layout/names.hpp
 * @brief  IO Grid Layout Name Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_LAYOUT_NAMES_H
#define TYPES_LAYOUT_NAMES_H


/// @brief Names of grid layout sections.
enum class
GridSectionName {
                 /// play/pause section
                 PlayPause,
                 /// stop section
                 Stop,
                 /// record section
                 Record,
                 /// metronome section
                 Metronome,

                 /// shift section
                 Shift,

                 /// instrument selection section
                 InstrumentSelect,
                 /// instrument panel section
                 InstrumentPanel,

                 /// step sequencer section
                 Steps,
                 /// last step section
                 LastStep,
                 /// pages section
                 Pages,

                 /// parts section
                 Parts,
                 /// banks section
                 Banks,

                 /// pulse-per-quarter-note (ppqn) section
                 PPQN,

                 /// switch layout section
                 SwitchLayout,

                 // TODO seperate this out into a seperate InstrumentSectionName enum
                 // ER1 stuff
                 OSC1,
                 OSC2,
                 OSC3,
                 OSC4,
};


/// @brief Layout names.
enum class
LayoutName {
            /// main sequencer + instrument layout
            SequencerAndInstrument,
            /// parallel sequencer layout
            ParallelSequencer,

            // TODO seperate this into a seperate InstrumentLayoutName enum
            ER1,
};

#endif
