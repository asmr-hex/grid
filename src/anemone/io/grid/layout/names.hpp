/**
 * @file   io/grid/layout/names.hpp
 * @brief  IO Grid Layout Name Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_LAYOUT_NAMES_H
#define IO_GRID_LAYOUT_NAMES_H


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
};


/// @brief Layout names.
enum class
LayoutName {
            /// main sequencer + instrument layout
            SequencerAndInstrument,
            /// parallel sequencer layout
            ParallelSequencer,
};

#endif
