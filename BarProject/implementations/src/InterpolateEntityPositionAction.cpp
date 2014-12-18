#include "InterpolateEntityPositionAction.h"

#include <mlv/behavior/EntityKnowledgeFacade.h>
#include <mlv/behavior/EntityProperties.h>

namespace barman
{
    namespace impl_lib
    {
        InterpolateEntityPositionInput::InterpolateEntityPositionInput()
            : name("")
            , time(2.0)
        {
            // NOTHING
        }

        bool InterpolateEntityPositionInput::setMetadata(mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger)
        {
            return manager.registerProperty("name", &InterpolateEntityPositionInput::name, logger)
                && manager.registerProperty("time", &InterpolateEntityPositionInput::time, logger);
        }

        InterpolateEntityPositionOutput::InterpolateEntityPositionOutput()
            : wantToDrink(false)
        {
            // NOTHING
        }

        bool InterpolateEntityPositionOutput::setMetadata(mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger)
        {
            return manager.registerProperty("interpolatedPosition", &InterpolateEntityPositionOutput::wantToDrink, logger);
        }

        InterpolateEntityPositionAction::InterpolateEntityPositionAction()
            : mlv::behavior::AbstractTypedAction<InterpolateEntityPositionInput, InterpolateEntityPositionOutput>("interpolateEntityPosition")
        {
            // NOTHING
        }

        InterpolateEntityPositionAction::~InterpolateEntityPositionAction()
        {
            // NOTHING
        }

        mlv::utils::EntityIndex InterpolateEntityPositionAction::findEntity(const mlv::behavior::EntityKnowledgeFacade& facade, const char* entityName) const
        {
            if (!entityName)
                return mlv::utils::invalidIndex;

            for(std::size_t i(0), size(facade.countEntities()) ; i < size ; ++i)
            {
                mlv::utils::EntityIndex entity = facade.getEntities()[i];
              
				if(strcmp(facade.getEntityProperties(entity).getName(),entityName) == 0)
                    return entity;
            }

            return mlv::utils::invalidIndex;
        }

        mlv::behavior::action::Status InterpolateEntityPositionAction::doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const InterpolateEntityPositionInput& input, InterpolateEntityPositionOutput& output)
        {
            mlv::utils::DynamicObject& knowledge = entity.getKnowledge();
            mlv::utils::EntityIndex index = findEntity(entity, input.name.c_str());
            
			if (index == mlv::utils::invalidIndex)
                return mlv::behavior::action::failed;
            
			//entity.getEntityProperties(index).getPosition(output.interpolatedPosition);

			//mlv::utils::Vector3r targetVelocity;
            const mlv::utils::DynamicObject& clientKnowledge = entity.getEntityKnowledge(index);
            
			//targetKnowledge.retrieve("velocity", targetVelocity, entity.getTypeManager()); // If the property is not defined, the target velocity will be 0;
			
			if(clientKnowledge.getBool("wantToDrink"))
            {
                output.wantToDrink = clientKnowledge.getBool("wantToDrink");
            }
                
            return mlv::behavior::action::succeeded;
        }

        mlv::behavior::action::Status InterpolateEntityPositionAction::doUpdate(mlv::behavior::EntityKnowledgeFacade& /*entity*/, mlv::behavior::ActionRequestHandle /*requestId*/, const InterpolateEntityPositionInput& /*input*/, InterpolateEntityPositionOutput& /*output*/, const mlv::utils::Time& /*dt*/)
        {
            MLV_FAIL("Unexpected call to update: " << getUri() << " is an 'instant' action");
            return mlv::behavior::action::failed;
        }

        mlv::behavior::action::Status InterpolateEntityPositionAction::doCancel(mlv::behavior::EntityKnowledgeFacade& /*entity*/, mlv::behavior::ActionRequestHandle /*requestId*/, const InterpolateEntityPositionInput& /*input*/, const mlv::utils::Time& /*dt*/)
        {
            MLV_FAIL("Unexpected call to cancel: " << getUri() << " is an 'instant' action");
            return mlv::behavior::action::canceled;
        }
    }
}
