#include "ContactComponent.h"

#include <mlv/simulation/components/KnowledgeComponent.h>

#include <mlv/simulation/SimulationFramework.h>

#include <mlv/behavior/EntityProperties.h>
#include <mlv/behavior/KnowledgeFramework.h>

#include <mlv/utils/StringTable.h>

#include <algorithm>
#include <iterator>

namespace barman
{
	namespace impl_lib
	{
		ContactComponent::ContactComponent(mlv::simulation::SimulationFramework& simulation)
			: mlv::simulation::AbstractComponent(simulation)
			,entities_()
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
				if (entities_.find(entities[i]) == entities_.end())
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
			entities_.insert(entities, entities + entitiesSize);
		
			return true;
		}

		bool ContactComponent::doUnregisterEntities(const mlv::utils::EntityIndex entities[], std::size_t entitiesSize, mlv::utils::AbstractLogger*)
		{
			for(std::size_t i(0) ; i < entitiesSize ; ++i)
				entities_.erase(entities[i]);
			
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
				mlv::utils::EntityIndex barman = entities[i];
		
				if (entities_.find(barman) != entities_.end() && knowledge_->getFramework().getEntityProperties(barman).isPositionSet())
				{
					// Accessing the working knowledge of the barman is safe as we are in its update loop, where,
					// by convention only thread is at a time.
					mlv::utils::DynamicObject& barmanKnowledge = knowledge_->getFramework().getEntityWorkingKnowledge(barman);
			
					if (barmanKnowledge.getStringHash("client.name") == barmanKnowledge.getStringTable().hash("none"))
					{
						const mlv::behavior::EntityProperties& barmanProperties = knowledge_->getFramework().getEntityWorkingProperties(barman);
						//mlv::utils::Vector3r catcherPosition;
						//barmanProperties.getPosition(catcherPosition);

						// Iterate through the other non-barman entities to find the closest one
						mlv::utils::EntityIndex client = mlv::utils::invalidIndex;
						//mlv::utils::Real targetDistanceSqr = std::numeric_limits<mlv::utils::Real>::max();

						mlv::utils::EntityIndex candidate;
						mlv::utils::Vector3r candidatePosition;
						//mlv::utils::Real candidateDistanceSqr;

						for (std::size_t j(0), size(knowledge_->getFramework().countEntities()) ; j < size ; ++j)
						{
							candidate = knowledge_->getFramework().getEntities()[j];
							// We are accessing the other entities, to do that safely we're using their commited knolwedge.
							const mlv::utils::DynamicObject& candidateKnowledge = knowledge_->getFramework().getEntityKnowledge(candidate);
							const mlv::behavior::EntityProperties& candidateProperties = knowledge_->getFramework().getEntityProperties(candidate);

							if (entities_.find(candidate) == entities_.end() && candidateProperties.isPositionSet() && candidateProperties.isNameSet())
							{
								if (candidateKnowledge.getBool("isAtBar") && candidateKnowledge.getBool("isDrunk") && (candidateKnowledge.getString("wantDrink", (char*)0) != 0))
								{
								//	candidateProperties.getPosition(candidatePosition);

								//	candidateDistanceSqr = mlv::utils::sqrDistance(candidatePosition, catcherPosition);
								//	if (candidateDistanceSqr < targetDistanceSqr)
								//	{
								//		targetDistanceSqr = candidateDistanceSqr;
										client = candidate;
								//	}
								}
							}
						}

						if (client != mlv::utils::invalidIndex)
						{
							barmanKnowledge.setString("client.name",knowledge_->getFramework().getEntityProperties(client).getName());
							barmanKnowledge.setString("client.channel",knowledge_->getFramework().getEntityKnowledge(client).getString("wantDrink"));
						}
						else
						{
							barmanKnowledge.setString("client.name","none");
						}
					}
				}
			}
			return true;
		}
	}
}