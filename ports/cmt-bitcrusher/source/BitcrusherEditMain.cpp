/*-----------------------------------------------------------------------------

 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved

-----------------------------------------------------------------------------*/

#include "Bitcrusher.hpp"
//#include "BitcrusherEdit.cpp"
//#include "BCEditor.cpp"
//#include "c_radio_group.cpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new Bitcrusher (audioMaster);
}
