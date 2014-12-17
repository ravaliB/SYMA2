#include "ContactComponent.h"

#include <mlv/simulation/components/KnowledgeComponent.h>

#include <mlv/simulation/SimulationFramework.h>

#include <mlv/behavior/EntityProperties.h>
#include <mlv/behavior/KnowledgeFramework.h>

#include <mlv/utils/StringTable.h>

#include <algorithm>
#include <iterator>

namespace templates
{
	namespace impl_lib
	{
		ContactComponent::ContactComponent(mlv::simulation::SimulationFramework& simulation)
			: entities_()
			, knowledge_(0)
		{
		}

		ContactComponent::~ContactComponent()
		{
		}

		void ContactComponent::reserveEntityCount(std::size_t /*size*/)
		{
			// NOTHING
		}

		void ContactComponent::enableDebugger(bool state)
		{
			// NOTHING
		}

		bool ContactComponent::isDebuggerEnable() const
		{
			return false;
		}

		bool ContactComponent::areEntitiesRegistered(const mlv::utils::EntityIndex entities[], std::size_t entitiesSize) const
		{
			for (std::size_t i(0) ; i < entitiesSize ; ++i)
			{
				if (catchers_.find(entities[i]) == catchers_.end())
					return false;
			}
			
			return true;
		}

		//================================================ PRIVATE METHOD ==================================================

		bool ContactComponent::doInitializeContext(mlv::utils::AbstractLogger* logger)
		{
			knowledge_ = simulation_->getComponent<mlv::simulation::components::KnowledgeComponent>();
			
			if (!knowledge_)
			{
				MLV_LOG_ERROR(logger, mlv::utils::error::invalidConfiguration,"No knowledge component.");
			
				return false;
			}
			
			return true;
		}

		void ContactComponent::doTerminateContext()
		{
			knowledge_ = 0;
		}

		bool ContactComponent::doRegisterEntities(const mlv::utils::EntityIndex entities[], std::size_t entitiesSize, mlv::utils::AbstractLogger*)
		{
			catchers_.insert(entities, entities + entitiesSize);
		
			return true;
		}

		bool ContactComponent::doUnregisterEntities(const mlv::utils::EntityIndex entities[], std::size_t entitiesSize, mlv::utils::AbstractLogger*)
		{
			for(std::size_t i(0) ; i < entitiesSize ; ++i)
				catchers_.erase(entities[i]);
			
			return true;
		}

		bool ContactComponent::doInitializeEntities(const mlv::utils::EntityIndex*, std::size_t, mlv::utils::AbstractLogger*)
		{
			return true;
		}

		void ContactComponent::doTerminateEntities(const mlv::utils::EntityIndex*, std::size_t)
		{
			// NOTHING
		}

		bool ContactComponent::doUpdateEntities( const mlv::utils::Time& dt, const mlv::utils::EntityIndex entities[], std::size_t entitiesSize, mlv::utils::AbstractLogger* logger)
		{
			for (std::size_t i(0) ; i < entitiesSize ; ++i)
			{
				mlv::utils::EntityIndex catcher = entities[i];
				if (catchers_.find(catcher) != catchers_.end() &&  knowledge_->getFramework().getEntityProperties(catcher).isPositionSet())
				{
					// Accessing the working knowledge of the catcher is safe as we are in its update loop, where,
					// by convention only thread is at a time.
					mlv::utils::DynamicObject& catcherKnowledge = knowledge_->getFramework().getEntityWorkingKnowledge(catcher);
					if(catcherKnowledge.getStringHash("target.name") == catcherKnowledge.getStringTable().hash("none"))
					{
						const mlv::behavior::EntityProperties& catcherProperties = knowledge_->getFramework().getEntityWorkingProperties(catcher);
						mlv::utils::Vector3r catcherPosition;
						catcherProperties.getPosition(catcherPosition);

						// Iterate through the other non-catcher entities to find the closest one
						mlv::utils::EntityIndex target = mlv::utils::invalidIndex;
						mlv::utils::Real targetDistanceSqr = std::numeric_limits<mlv::utils::Real>::max();

						mlv::utils::EntityIndex candidate;
						mlv::utils::Vector3r candidatePosition;
						mlv::utils::Real candidateDistanceSqr;

						for (std::size_t j(0), size(knowledge_->getFramework().countEntities()) ; j < size ; ++j)
						{
							candidate = knowledge_->getFramework().getEntities()[j];
							// We are accessing the other entities, to do that safely we're using their commited knolwedge.
							const mlv::utils::DynamicObject& candidateKnowledge = knowledge_->getFramework().getEntityKnowledge(candidate);
							const mlv::behavior::EntityProperties& candidateProperties = knowledge_->getFramework().getEntityProperties(candidate);

							if (catchers_.find(candidate) == catchers_.end() && candidateProperties.isPositionSet() && candidateProperties.isNameSet())
							{
								if (!candidateKnowledge.getBool("hasBeenCaught") && (candidateKnowledge.getString("entityChannel", (char*)0) != 0))
								{
									candidateProperties.getPosition(candidatePosition);

									candidateDistanceSqr = mlv::utils::sqrDistance(candidatePosition, catcherPosition);
									if (candidateDistanceSqr < targetDistanceSqr)
									{
										targetDistanceSqr = candidateDistanceSqr;
										target = candidate;
									}
								}
							}
						}

						if (target != mlv::utils::invalidIndex)
						{
							catcherKnowledge.setString("target.name",knowledge_->getFramework().getEntityProperties(target).getName());
							catcherKnowledge.setString("target.channel",knowledge_->getFramework().getEntityKnowledge(target).getString("entityChannel"));
						}
						else
						{
							catcherKnowledge.setString("target.name","none");
						}
					}
				}
			}
			return true;
		}
	}
}