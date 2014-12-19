#include "setEntityBooleanAction.h"

#include <mlv/behavior/EntityKnowledgeFacade.h>
#include <mlv/behavior/EntityProperties.h>

namespace fightinbar
{
	SetEntityBooleanParams::SetEntityBooleanParams()
		: key ("")
		, value (false)
	{
		// Initialization of the parameters
	}

	bool SetEntityBooleanParams::setMetadata(mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger)
	{
		return manager.registerProperty("key", &SetEntityBooleanParams::key, logger)
			&& manager.registerProperty("value", &SetEntityBooleanParams::value, logger);
	}

	SetEntityBoolean::SetEntityBoolean()
		: mlv::behavior::AbstractTypedAction<SetEntityBooleanParams>("SetEntityBoolean") //Name used in the BT editor to create an instance of this action
	{
		// Initialization of the action
	}

	SetEntityBoolean::~SetEntityBoolean()
	{
		// Destruction of the action
	}

	mlv::behavior::action::Status SetEntityBoolean::doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params)
	{			
		// The action begin
		mlv::utils::DynamicObject& entityKnowledge = entity.getKnowledge();
		mlv::utils::DynamicObject::PropertyHandle entityKnowledgeProperty = entityKnowledge.getProperty(params.key.c_str(),params.key.length());

		if (entityKnowledgeProperty == 0)
		{
			entityKnowledgeProperty = entityKnowledge.addProperty(params.key.c_str(),params.key.length());

			if (entityKnowledgeProperty==0)
				return mlv::behavior::action::canceled;
		}

		if (entityKnowledge.isTypeCompatible(entityKnowledgeProperty,mlv::utils::type::boolean))
		{
			entityKnowledge.setBool(entityKnowledgeProperty, params.value);

			return mlv::behavior::action::running;
		}
		else
			return mlv::behavior::action::canceled;

		return mlv::behavior::action::running;
	}

	mlv::behavior::action::Status SetEntityBoolean::doUpdate(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params, const mlv::utils::Time& dt)
	{
		// The action continue

		return mlv::behavior::action::succeeded;
	}

	mlv::behavior::action::Status SetEntityBoolean::doCancel(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params, const mlv::utils::Time& dt)
	{
		//The action is canceled

		return mlv::behavior::action::canceled;
	}
}
