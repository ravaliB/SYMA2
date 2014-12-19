#ifndef IMPLEMENTATION_LIBRARY_H
#define IMPLEMENTATION_LIBRARY_H

#include "setEntityBooleanAction.h"
#include "ContactComponent.h"
#include "IncrementAction.h"

#include <mlv/simulation/configuration/AbstractImplementationLibrary.h>

namespace fightinbar
{
	class ImplementationLibrary : public mlv::simulation::configuration::AbstractImplementationLibrary
	{
	public:
		ImplementationLibrary();
		virtual ~ImplementationLibrary();
	
	private:
		virtual bool doLoad(mlv::utils::AbstractLogger* logger);
		virtual bool doUnload(mlv::utils::AbstractLogger* logger);

		//Custom Action
		SetEntityBoolean setEntityBoolean_;
		IncrementAction increment_;

		//Component
		ContactComponent* component_;
	}; 
}

#endif
