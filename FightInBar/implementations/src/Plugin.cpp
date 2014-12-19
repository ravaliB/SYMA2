#include "ImplementationLibrary.h"

#ifdef _MSC_VER
#define PLUGIN_API __declspec( dllexport )
#else
#define PLUGIN_API
#endif

extern "C"
{
    /** Create an implementation library instance */
    PLUGIN_API void* createImplementationLibrary(mlv::utils::AbstractLogger* /*logger*/)
    {
        return MLV_NEW fightinbar::ImplementationLibrary();
    }

    /** Destroy an implementation library instance */
    PLUGIN_API void destroyImplementationLibrary(void* library, mlv::utils::AbstractLogger* /*logger*/)
    {
        MLV_DELETE static_cast<fightinbar::ImplementationLibrary*>(library); 
    }
}