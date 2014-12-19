#include "ImplementationLibrary.h"

#include <mlv/simulation/configuration/ImplementationLibraryPluginMacros.h>
#include <mlv/simulation/SimulationFramework.h>


namespace fightinbar
{
	ImplementationLibrary::ImplementationLibrary()
		: AbstractImplementationLibrary("fightinbar")
		, setEntityBoolean_()
		, increment_ ()
		, component_ (NULL)
	{
		// NOTHING
	}

	ImplementationLibrary::~ImplementationLibrary()
	{
		if (isLoaded())
			forceUnload();
	}

	bool ImplementationLibrary::doLoad(mlv::utils::AbstractLogger* logger)
	{
		bool success = true;
		
		//Components
		if (getSimulationFramework() == 0)
		{
			MLV_LOG_WARNING(logger, "Unable to load the component from implementation library.");
			success = true;
		}
		else
		{
			//create components
			component_ = MLV_NEW ContactComponent(*getSimulationFramework());
			success = addComponent(*component_, logger);
		}

			//Actions
		success = success && registerType<SetEntityBooleanParams>(logger);
		success = success && registerAction(setEntityBoolean_, logger);		
		
		success = success && registerType<IncrementParams>(logger);
		success = success && registerAction(increment_, logger);	
		
		return success;
	}

	bool ImplementationLibrary::doUnload(mlv::utils::AbstractLogger* logger)
	{
		bool success = true;
		
		//Actions
		success = success && unregisterAction(setEntityBoolean_, logger);
		success = success && unregisterAction(increment_, logger);
		
		if (success)
		{
			unregisterType<SetEntityBooleanParams>();
			unregisterType<IncrementParams>();
		}	

		//Components
		success = success && removeComponent(*component_, logger);
		
		if (success)
		{
			MLV_DELETE component_;
			component_ = 0;
		}

		return success;
	}
}

MLV_IMPLEMENTATION_LIBRARY_PLUGIN_EXPORT(fightinbar::ImplementationLibrary);