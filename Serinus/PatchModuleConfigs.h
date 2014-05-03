#ifndef PATCHMODULECONFIGS_H_
#define PATCHMODULECONFIGS_H_
namespace I_SawDCO { enum e { PHASE, FREQ, AMP, MAX = AMP }; }
namespace O_SawDCO { enum e { SAMPLE, MAX = SAMPLE }; }

namespace O_Knob { enum e { VALUE, MAX = VALUE }; }

namespace O_WaveTableOsc { enum e { SAMPLE, MAX = SAMPLE }; }
namespace I_WaveTableOsc { enum e { FREQ, INTERPOLATE, MAX = INTERPOLATE }; }
#endif /* PATCHMODULECONFIGS_H_ */