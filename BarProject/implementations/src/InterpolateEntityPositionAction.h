#ifndef BARMAN_IMPL_LIB_INTERPOLATEENTITYPOSITIONACTION_H
#define BARMAN_IMPL_LIB_INTERPOLATEENTITYPOSITIONACTION_H

#include <mlv/behavior/AbstractTypedAction.h>

namespace barman
{
    namespace impl_lib
    {
        class InterpolateEntityPositionInput : public mlv::utils::StaticObject
        {
            MLV_DEFINE_STATIC_OBJECT(barman::impl_lib::InterpolateEntityPositionInput,"79bfc5d7-dc5d-4d8f-ba5f-bbcc5bb52000",setMetadata);
        public:
            InterpolateEntityPositionInput();
            mlv::utils::String name; //!< The name of the target entity.
            mlv::utils::Real time; //!< The interpolation time.

        private:
            static bool setMetadata( mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger );
        };

        class InterpolateEntityPositionOutput : public mlv::utils::StaticObject
        {
            MLV_DEFINE_STATIC_OBJECT(barman::impl_lib::InterpolateEntityPositionOutput,"9B572FF0-0A86-4B2F-B3C8-2F01389299E9",setMetadata);
        public:
            InterpolateEntityPositionOutput();
			bool wantToDrink; //!< The target, interpolated, entity position 
        private:
            static bool setMetadata( mlv::utils::TypeManager& manager, mlv::utils::AbstractLogger* logger );
        };

        /** ApproachEntity action.                
        */
        class InterpolateEntityPositionAction: public mlv::behavior::AbstractTypedAction<InterpolateEntityPositionInput, InterpolateEntityPositionOutput>
        {
        public:
            InterpolateEntityPositionAction();
            ~InterpolateEntityPositionAction();

            double computeLookAhead(double dist, double speed);
        private:
            virtual mlv::behavior::action::Status doStart(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const InterpolateEntityPositionInput& input, InterpolateEntityPositionOutput& output);
            virtual mlv::behavior::action::Status doUpdate(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const InterpolateEntityPositionInput& input, InterpolateEntityPositionOutput& output, const mlv::utils::Time& dt);
            virtual mlv::behavior::action::Status doCancel(mlv::behavior::EntityKnowledgeFacade& entity, mlv::behavior::ActionRequestHandle requestId, const InterpolateEntityPositionInput& input, const mlv::utils::Time& dt);

            mlv::utils::EntityIndex findEntity(const mlv::behavior::EntityKnowledgeFacade& facade, const char* entityName) const;
        };
    }
}

#endif
