#ifndef PATCHMODULECONFIGS_H_
#define PATCHMODULECONFIGS_H_
/**
  * This is a header that contains custom enumeration for each module in the engine.
  * Each enum should be enclosed with a special namespace.
  * Naming for the namespace is following: prefix+class name. Prefixes:
  * "I_" for inputs
  * "O_" for outputs
  * "P_" for internal parameters
  * "C_" for commands
  * Internal enum can have any name.
  * All enums should contain MAX element, which is assigned the value from the last usable element.
  *
  * Authored: AK
  * Last revision: 03.05.2014
  *
  */

namespace I_DirectDCO { enum e { PITCH, PWM, PHASE, AMP, MAX }; }
namespace O_DirectDCO { enum e { SAMPLE, MAX }; }
namespace P_DirectDCO { enum e { TUNE, DETUNE, TRIG, WF, MODE, MAX }; }

namespace I_VCA { enum e { INPUT, GAIN, MAX }; }
namespace O_VCA { enum e { SAMPLE, MAX }; }
namespace P_VCA { enum e { MODE, MAX }; }

namespace I_ADSR { enum e { GATE, MAX }; }
namespace O_ADSR { enum e { SAMPLE, MAX }; }
namespace P_ADSR { enum e { MODE, ATTACK, DECAY, SUSTAIN, RELEASE, MAX }; }

namespace O_Knob { enum e { VALUE, MAX }; }
namespace P_Knob { enum e { VALUE, MAX }; }

namespace O_WaveTableOsc { enum e { SAMPLE, MAX }; }
namespace I_WaveTableOsc { enum e { FREQ, INTERPOLATE, MAX }; }

#endif /* PATCHMODULECONFIGS_H_ */