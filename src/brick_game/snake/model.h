#pragma once
#include <memory>
#include <utility>

#include "defines.h"
#include "lib.h"

namespace s21::snake {

    class model;

    class State {
    public:
        virtual ~State() = default;
        void SetFsm(model* fsm) { fsm_ = fsm; }
        virtual void Enter() = 0;
        virtual void Exit() = 0;
    protected:
        // to allow state changes
        model* fsm_{ nullptr };
        GameInfo_t game_info;
    };
    
    
    class model 
    {
        

        public:
            class factory
            {
                public:
                    template <typename T, typename... Args>
                    static std::unique_ptr<model> create(Args&&... args)
                    {
                        auto fsm_ptr = new model(std::make_unique<T>( std::forward<Args>(args)... ));
                        return std::unique_ptr<model>( fsm_ptr );
                    }
            };
            
            friend class factory;
            
        private:
            model(std::unique_ptr<State> state = nullptr) : state_(std::move(state)) {}
            
        public:
            template <class T>
            void TransitionTo() {
                if (state_ != nullptr) {
                    // if switching to same state, do nothing
                    const auto temp = std::make_unique<T>();
                    if (dynamic_cast<T*>(state_.get()) != nullptr) { return; }
                    state_->Exit();
                }
                state_ = std::make_unique<T>();
                state_->SetFsm(this);
                state_->Enter();
            }
        private:
            std::unique_ptr<State> state_;
    };

};