#ifndef SET_ENTITY_BOOLEAN_ACTION_H
#define SET_ENTITY_BOOLEAN_ACTION_H

#include <mlv/behavior/AbstractTypedAction.h>

namespace fightinbar
{
	/* Parameters of the action */

	class SetEntityBooleanParams : public mlv::utils::StaticObject
	{
		MLV_DEFINE_STATIC_OBJECT(fightinbar::SetEntityBooleanParams,"1e74c1ca-c3cc-4739-8dcc-49744428864e",setMetadata);
	public:
		SetEntityBooleanParams();
		mlv::utils::String key;
		bool value;

	private:
		static bool setMetadata( mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger );
	};

	/* Action */

	class SetEntityBoolean: public mlv::behavior::AbstractTypedAction<SetEntityBooleanParams>
	{
	public:
		SetEntityBoolean();
		~SetEntityBoolean();

	private:
		virtual mlv::behavior::action::Status doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params);
		virtual mlv::behavior::action::Status doUpdate(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params, const mlv::utils::Time& dt);
		virtual mlv::behavior::action::Status doCancel(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const SetEntityBooleanParams& params, const mlv::utils::Time& dt);

	};
}

#endif
