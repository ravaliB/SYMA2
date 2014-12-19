#include "ContactComponent.h"

#include <mlv/simulation/components/KnowledgeComponent.h>

#include <mlv/simulation/SimulationFramework.h>

#include <mlv/behavior/EntityProperties.h>

#include <algorithm>
#include <iterator>

namespace fightinbar
{
	ContactComponent::ContactComponent(mlv::simulation::SimulationFramework& simulation)
		: mlv::simulation::AbstractComponent(simulation)
		, entities_ ()
		, knowledge_(0)   
	{
		// NOTHING
	}

	ContactComponent::~ContactComponent()
	{
		// NOTHING
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
		return true;
	}

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
		entities_.erase(entities, entities + entitiesSize);
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
		mlv::utils::DynamicObject& globalKnowledge = knowledge_->getFramework().getGlobalWorkingKnowledge();

		// Bar interaction handling :
		bool order = false;

		for (std::size_t i = 0; i < simulation_->countEntities() ; ++i)
		{
			const mlv::utils::DynamicObject& entityKnowledge = knowledge_->getFramework().getEntityKnowledge(simulation_->getEntities()[i]);

			if (entityKnowledge.hasProperty ("buyDrink"))
				order |= entityKnowledge.getBool ("buyDrink");
		}

		globalKnowledge.setBool("order", order);

		// Battle handling :
		for (std::size_t i = 0; i < simulation_->countEntities() ; ++i)
		{
			const mlv::utils::DynamicObject& entityKnowledge = knowledge_->getFramework().getEntityKnowledge(simulation_->getEntities()[i]);

			if (entityKnowledge.getBool("wantFight") && entityKnowledge.getBool ("isFighting"))
			{
				knowledge_->getFramework().getEntityWorkingKnowledge(simulation_->getEntities()[i]).setBool ("isFighting", false);

				if (entityKnowledge.getBool("wantFight"))
					globalKnowledge.setInt ("nbBattle", globalKnowledge.getInt ("nbBattle") + 1);
			}

			if (entityKnowledge.hasProperty("place") && entityKnowledge.getBool("place"))
			{
				bool place = entityKnowledge.getBool("place");
				knowledge_->getFramework().getEntityWorkingKnowledge(simulation_->getEntities()[i]).setBool("place", false);

				for (std::size_t j = 0; j < simulation_->countEntities() ; ++j)
				{
					const mlv::utils::DynamicObject& target = knowledge_->getFramework().getEntityKnowledge(simulation_->getEntities()[j]);

					if (target.hasProperty("wantFight") && target.getBool("wantFight") == place)
					{
						knowledge_->getFramework().getEntityWorkingKnowledge(simulation_->getEntities()[j]).setBool ("isNotKO", false);

						if (place)
							globalKnowledge.setInt ("nbBattle", 0);
					}
				}
			}
		}

		return true;
	}

}
