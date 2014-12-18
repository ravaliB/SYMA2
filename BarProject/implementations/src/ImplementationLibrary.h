#ifndef IMPLEMENTATIONLIBRARY_H
#define IMPLEMENTATIONLIBRARY_H

#include "ContactComponent.h"
#include "InterpolateEntityPositionAction.h"

#include <mlv/simulation/configuration/AbstractImplementationLibrary.h>

namespace barman
{
	namespace impl_lib
	{
		class ImplementationLibrary : public mlv::simulation::configuration::AbstractImplementationLibrary
		{
		public:
			ImplementationLibrary();
			virtual ~ImplementationLibrary();

		private:
			virtual bool doLoad(mlv::utils::AbstractLogger* status);
			virtual bool doUnload(mlv::utils::AbstractLogger* status);

			//action
            InterpolateEntityPositionAction interpolateEntityPositionAction_;


			// components
			ContactComponent* contactComponent_;
		}; 
	}
}

#endif
