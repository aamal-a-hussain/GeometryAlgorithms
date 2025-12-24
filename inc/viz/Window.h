#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <highfive/highfive.hpp>

namespace geom {
namespace viz {
    class Renderer {
    public:
        Renderer() : convex_hull_points_(-1) {
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

        void Clear_() {
            shapes_.clear();
        }

        void LoadData_(const char path[64]) {
            const auto p = path + std::string(".h5");
            const HighFive::File file (p, HighFive::File::ReadOnly);
            const auto dataset = file.getDataSet("points");
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

        void SetupImGui_() {
            bool loadData = false;
            ImGui::SFML::Update(window_, clock_.restart());
            ImGui::Begin("Convex Hull Points");
            const auto preview = (convex_hull_points_ >=0) ? convex_hull_data_files[convex_hull_points_] : "Choose an option";

            if (ImGui::BeginCombo("Filename", preview.data())) {
                for (int n = 0; n < IM_ARRAYSIZE(convex_hull_data_files); ++n) {
                    if (const auto filename = convex_hull_data_files[n].data(); ImGui::Selectable(filename, convex_hull_points_ == n)) {
                        convex_hull_points_ = n;
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Generate points")) loadData = true;
            ImGui::SameLine();
            if (ImGui::Button("Clear")) Clear_();
            if (loadData && convex_hull_points_ >= 0) LoadData_(convex_hull_data_files[convex_hull_points_].data());
            ImGui::End();
        }

        void Update() {
            SetupImGui_();
            window_.clear();
            for (const auto& shape : shapes_) window_.draw(*shape);

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
            static constexpr std::string_view convex_hull_data_files[] = {"res/points"};
        private:
            std::vector<std::unique_ptr<sf::Shape>> shapes_;
            sf::RenderWindow                        window_;
            sf::Clock                               clock_;
            int                                     convex_hull_points_;
    };
}
} // namespace geom
