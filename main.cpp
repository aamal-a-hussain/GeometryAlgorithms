#include "inc/viz/Window.h"
using namespace geom::viz;
int main() {
    auto renderer = Renderer();

    while (renderer.IsRunning()) {
        renderer.HandleInputs();
        renderer.Update();
    }
}
