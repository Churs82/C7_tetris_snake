#include "model.h"

namespace s21::snake {

       void model::TogglePause() { game_info_->pause = !game_info_->pause; }
       void model::Exit() { TransitionTo<Exit_state>(); }

    
};  // namespace s21::snake
