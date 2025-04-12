namespace s21::snake {
    

Model::Model() {
    game_info.pause = false;
    game_info.score = 0;
    game_info.high_score = 0;
    game_info.level = 1;
    game_info.speed = 1;
    game_info.field = nullptr;
    game_info.next = nullptr;
    game_info.prev = nullptr;
}


}
