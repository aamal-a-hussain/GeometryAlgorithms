#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace geom {
namespace viz {
    class Renderer {
    public:
        Renderer() {
            window_ = sf::RenderWindow(sf::VideoMode({640, 480}), "Geometry Renderer");
            window_.setFramerateLimit(60);
            std::ignore = ImGui::SFML::Init(window_);
            shapes_.push_back(std::make_unique<sf::CircleShape>(100.0f));
        }

        void HandleInputs() {
            while (const auto event = window_.pollEvent()) {
                ImGui::SFML::ProcessEvent(window_, *event);
                if (event -> is<sf::Event::Closed>()) window_.close();
            }
        }

        void Update() {
            ImGui::SFML::Update(window_, clock_.restart());
            ImGui::Begin("Hwllo World");
            ImGui::Button("Look at this button");
            ImGui::End();

            window_.clear();
            for (const auto& shape : shapes_) {
                window_.draw(*shape);
            }

            ImGui::SFML::Render(window_);
            window_.display();
        }

        [[nodiscard]] inline bool IsRunning() {
            return window_.isOpen();
        }

        ~Renderer() {
            ImGui::SFML::Shutdown();
        }

        private:
            std::vector<std::unique_ptr<sf::Shape>> shapes_;
            sf::RenderWindow window_;
            sf::Clock clock_;
    };
}
} // namespace geom
