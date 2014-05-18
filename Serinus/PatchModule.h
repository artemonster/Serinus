#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_

#include "Serinus.h"
#include "Util\Factory.h"
/**
This is a base class for every patch module, used in the engine.
Each module impementation should implement all virtual methods according to their specifications.
Each module has their special enum-namespaces, which should be maintained in PatchModuleConfigs.h
These namespaces serve many purposes, but main one is to name IO and parameters of the module.
Each module has it's own set of inputs (pointers to other module's outputs) and outputs (Samples)
Inputs can be treated as sample inputs, as well as modulating inputs for the parameters.
All parameters internal parameters pointers should be mapped to their respective members,
and do not forget to map all types in getParameterTypes() method.

Authored: AK
Last revision: 05.05.2014
*/
class PatchModule {
public:
    /**This method updates all outputs of the module by internal algorithm.*/
    virtual void Tick() = 0;
    /**This method updates all outputs and internal states of the module by an external command.*/
    virtual void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) = 0;
    /**
    This method is called to deliver a map of all parameter types in the derived class.
    This mapping is mandatory for the parameters, which should be serialized for storing\loading the configuration.
    */ 
    virtual ModuleTypes getParameterTypes() = 0;
    /**This method loads up the configuration map (internal state) for the parameters.*/
    inline void LoadConfiguration(const ModuleTypes &types, const ModuleValues &values) {
        if (types.empty() || values.empty()) return;
        ModuleTypes::const_iterator typesIt;
        ModuleValues::const_iterator valuesIt;
        for (valuesIt = values.begin(); valuesIt != values.end(); valuesIt++) {
            int parameter = valuesIt->first;
            std::string value = valuesIt->second;
            typesIt = types.find(parameter);
            int type = typesIt->second;
            switch (type) {
                case Types::INT: {
                    int *ptr = static_cast<int*>( parameters[parameter] );
                    *ptr = atoi(value.c_str());
                    break;
                }
                case Types::FLOAT: {
                    float *ptr = static_cast<float*>( parameters[parameter] );
                    *ptr = static_cast<float>( atof(value.c_str()) );
                    break;
                }
                case Types::BOOL: {
                    bool *ptr = static_cast<bool*>( parameters[parameter] );
                    if (value[0] == 'T' || value[0] == 't' || value[0] == '1') {
                        *ptr = true;
                    } else {
                        *ptr = false;
                    }
                    break;
                }
                default: break;
            }
        }
    };
    /**This method returns the address of the output sample of a given index.*/
    inline Sample* getOutput(const int index) {
        return &(output[index]);
    }
    /**This method is used to link input with the output of other module.*/
    inline void setLink(const int index, Sample* link) {
        input[index] = link;
    }
    /**This method checks, whether or not the module has some inputs.*/
    inline bool hasNoInputs() {
        if (input == NULL) {
            return true;
        } else {
            return false;
        }
    }
    /**Virtual desctructor, so we can delete derived by deleting base class.*/
    virtual ~PatchModule() {
        delete[] output;
        delete[] input;
    };
protected:
    /**Pointer to an array of output samples.*/
    Sample *output;
    /**Pointer to an array of pointers to output samples of input modules.*/
    Sample **input;
    /**Pointer to an array of pointers to internal parameters of the module.*/
    void **parameters;
};

#endif /* PATCHMODULE_H_ */
