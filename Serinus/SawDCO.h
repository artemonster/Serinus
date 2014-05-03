#ifndef SAWDCO_H_
#define SAWDCO_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class SawDCO : public PatchModule  {
public:
    static const CreatorImpl<SawDCO> creator;
	SawDCO(); 
	~SawDCO() {};
	void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
private:
    Sample phase;
    Sample freq;
    Sample amplitude;
};

#endif /* SAWDCO_H_ */
