#include "IncrementAction.h"

#include <mlv/behavior/EntityKnowledgeFacade.h>
#include <mlv/behavior/EntityProperties.h>

namespace fightinbar
{
	IncrementParams::IncrementParams()
		: key ("")
		, value (0)
	{
		// Initialization of the parameters
	}

	bool IncrementParams::setMetadata(mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger)
	{
		return manager.registerProperty("key", &IncrementParams::key, logger)
			&& manager.registerProperty("value", &IncrementParams::value, logger);
	}

	IncrementAction::IncrementAction()
		: mlv::behavior::AbstractTypedAction<IncrementParams>("Increment") //Name used in the BT editor to create an instance of this action
	{
		// Initialization of the action
	}

	IncrementAction::~IncrementAction()
	{
		// Destruction of the action
	}

	mlv::behavior::action::Status IncrementAction::doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params)
	{			
		// The action begin
		mlv::utils::DynamicObject& entityKnowledge = entity.getKnowledge();
		mlv::utils::DynamicObject::PropertyHandle entityKnowledgeProperty = entityKnowledge.getProperty(params.key.c_str(), params.key.length());

		if (entityKnowledgeProperty == 0)
		{
			entityKnowledgeProperty = entityKnowledge.addProperty(params.key.c_str(),params.key.length());

			if (entityKnowledgeProperty == 0)
				return mlv::behavior::action::canceled;
		}

		if (entityKnowledge.isTypeCompatible(entityKnowledgeProperty, mlv::utils::type::integer))
		{
			uint64_t res = entityKnowledge.getInt(entityKnowledgeProperty) + params.value;
			entityKnowledge.setInt(entityKnowledgeProperty, res);

			return mlv::behavior::action::running;
		}
		else
			return mlv::behavior::action::canceled;

		return mlv::behavior::action::running;
	}

	mlv::behavior::action::Status IncrementAction::doUpdate(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params, const mlv::utils::Time& dt)
	{
		// The action continue
		return mlv::behavior::action::succeeded;
	}

	mlv::behavior::action::Status IncrementAction::doCancel(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params, const mlv::utils::Time& dt)
	{
		//The action is canceled
		return mlv::behavior::action::canceled;
	}
}
