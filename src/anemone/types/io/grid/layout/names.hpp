/**
 * @file   types/io/grid/layout/names.hpp
 * @brief  IO Grid Layout Name Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_IO_GRID_LAYOUT_NAMES_H
#define TYPES_IO_GRID_LAYOUT_NAMES_H


// TODO it would be *nice* to eventually create the names of layouts and sections
// within the plugins that they are relevant to (i.e. in anemone/plugins/...), so that
// when someone is making a new plugin, they don't have to edit this file of global
// names...but let's cross that bridge when we get there....

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

                 /// control selection section
                 ControlSelect,
                 
                 /// instrument selection section
                 InstrumentSelect,
                 /// instrument panel section
                 InstrumentPanel,
                 /// instrument control panel
                 InstrumentControls,

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
                 Oscillators,
                 Cymbals,
                 AudioIns,
                 DelayDepth,
                 DelayTime,
                 Level,
                 Pan,
                 LowBoost,
                 Pitch,
                 Decay,
                 ModType,
                 ModDepth,
                 ModSpeed,
                 WaveType,

                 // MicroGRanny Names
                 MicroGrannySamples,
                 MicroGrannyPresets,
                 MicroGrannyBanks,
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
            MicroGranny,
};

#endif
