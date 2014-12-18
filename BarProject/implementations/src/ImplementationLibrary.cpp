#include "ImplementationLibrary.h"

#include <mlv/simulation/configuration/ImplementationLibraryPluginMacros.h>

#include <mlv/simulation/SimulationFramework.h>

namespace barman
{
	namespace impl_lib
	{
		ImplementationLibrary::ImplementationLibrary()
			: AbstractImplementationLibrary("barman")
			, contactComponent_(NULL)
			, interpolateEntityPositionAction_()
		{
			// NOTHING
		}

		ImplementationLibrary::~ImplementationLibrary()
		{
			if (isLoaded())
			{
				forceUnload();
			}
		}

		bool ImplementationLibrary::doLoad(mlv::utils::AbstractLogger* logger)
		{
			bool success = true;

			// Components.
			if (getSimulationFramework() == 0)
			{
				MLV_LOG_WARNING(logger, "Unable to load the component from implementation library.");
				success = true;
			}
			else
			{
				//create components
				contactComponent_ = MLV_NEW ContactComponent(*getSimulationFramework());
				success = addComponent(*contactComponent_, logger);
			}

			// actions
			success = success && registerType<InterpolateEntityPositionInput>(logger);
			success = success && registerType<InterpolateEntityPositionOutput>(logger);
			success = success && registerAction(interpolateEntityPositionAction_, logger);    

			return success;
		}

		bool ImplementationLibrary::doUnload(mlv::utils::AbstractLogger* logger)
		{
			bool success = true;

			success = success && removeComponent(*contactComponent_, logger);
			success = success && unregisterAction(interpolateEntityPositionAction_, logger);

			if (success)
			{
				unregisterType<InterpolateEntityPositionInput>();
				unregisterType<InterpolateEntityPositionOutput>();
			}

			if (success)
			{
				MLV_DELETE contactComponent_;
				contactComponent_ = 0;
			}

			return success;
		}
	}
}

MLV_IMPLEMENTATION_LIBRARY_PLUGIN_EXPORT(barman::impl_lib::ImplementationLibrary);

