#ifndef PATCHMODULECONFIGS_H_
#define PATCHMODULECONFIGS_H_
/**
  * This is a header that contains custom enumeration for each module in the engine.
  * Each enum should be enclosed with a special namespace. 
  * Naming for the namespace is following: prefix+class name. Prefixes:
  * "I_" for inputs
  * "O_" for outputs
  * "C_" for commands
  * Internal enum can have any name.
  * All enums should contain MAX element, which is assigned the value from the last usable element.
  *
  * Authored: AK
  * Last revision: 03.05.2014
  *
*/

namespace I_SawDCO { enum e { PHASE, FREQ, AMP, MAX = AMP }; }
namespace O_SawDCO { enum e { SAMPLE, MAX = SAMPLE }; }

namespace O_Knob { enum e { VALUE, MAX = VALUE }; }

namespace O_WaveTableOsc { enum e { SAMPLE, MAX = SAMPLE }; }
namespace I_WaveTableOsc { enum e { FREQ, INTERPOLATE, MAX = INTERPOLATE }; }

#endif /* PATCHMODULECONFIGS_H_ */