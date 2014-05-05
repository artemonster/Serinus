#ifndef KNOB_H_
#define KNOB_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class Knob : public PatchModule {
public:
    static const CreatorImpl<Knob> creator;
    Knob();
    ~Knob() {};
    inline void Tick();
    inline void setValue(Sample val);
	ModuleTypes getParameterTypes() {
		ModuleTypes map{
			std::make_pair(0, Types::INT),
		};
		return map;
	};
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
private:
    Sample value;
};

#endif /* KNOB_H_ */