#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <highfive/highfive.hpp>

namespace geom {
namespace viz {
    class Renderer {
    public:
        Renderer() {
            window_ = sf::RenderWindow(sf::VideoMode({kWidth, kHeight}), "Geometry Renderer");
            window_.setFramerateLimit(60);
            std::ignore = ImGui::SFML::Init(window_);
        }

        void HandleInputs() {
            while (const auto event = window_.pollEvent()) {
                ImGui::SFML::ProcessEvent(window_, *event);
                if (event -> is<sf::Event::Closed>()) window_.close();
            }
        }

        void LoadData_(const char path[64]) {
            auto p = std::string("res/") + path;
            HighFive::File file (p, HighFive::File::ReadOnly);
            auto dataset = file.getDataSet("points");
            auto data = dataset.read<std::vector<std::vector<float>>>();
            for (const auto& coords : data) {
                auto c = sf::CircleShape(1.0);
                c.setFillColor(sf::Color::White);
                float x = coords[0] * kWidth;
                float y = kHeight - coords[1] * kHeight;
                c.setPosition({x, y});
                shapes_.push_back(std::make_unique<sf::CircleShape>(c));
            }
        }

        void Update() {
            static char point_path[64] = "";
            bool loadData = false;
            ImGui::SFML::Update(window_, clock_.restart());
            ImGui::Begin("Convex Hull Points");
            ImGui::InputText("Filename (H5)", point_path, IM_ARRAYSIZE(point_path));
            if (ImGui::Button("Generate points")) loadData = true;
            if (loadData) LoadData_(point_path);
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

        public:
            static constexpr int kWidth = 640;
            static constexpr int kHeight = 640;

        private:
            std::vector<std::unique_ptr<sf::Shape>> shapes_;
            sf::RenderWindow window_;
            sf::Clock clock_;
    };
}
} // namespace geom
