#ifndef INCREMENT_ACTION_H
#define INCREMENT_ACTION_H

#include <mlv/behavior/AbstractTypedAction.h>

namespace fightinbar
{
	/* Parameters of the action */

	class IncrementParams : public mlv::utils::StaticObject
	{
		MLV_DEFINE_STATIC_OBJECT(fightinbar::IncrementParams, "1e74c1ca-c3cc-4739-8dcc-49744428864f", setMetadata);
	public:
		IncrementParams();
		mlv::utils::String key;
		int value;

	private:
		static bool setMetadata( mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger);
	};

	/* Action */

	class IncrementAction: public mlv::behavior::AbstractTypedAction<IncrementParams>
	{
	public:
		IncrementAction();
		~IncrementAction();

	private:
		virtual mlv::behavior::action::Status doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params);
		virtual mlv::behavior::action::Status doUpdate(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params, const mlv::utils::Time& dt);
		virtual mlv::behavior::action::Status doCancel(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const IncrementParams& params, const mlv::utils::Time& dt);
	};
}

#endif
