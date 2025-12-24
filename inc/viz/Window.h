#ifndef WINDOW_H
#define WINDOW_H

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <highfive/highfive.hpp>

#include "../alg/ConvexHull.hpp"
#include "../math/Point.hpp"

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
                    if (event->is<sf::Event::Closed>()) window_.close();
                }
            }

            void Clear_() {
                shapes_.clear();
                lines_.clear();
            }

            void AddPoints_(const std::vector<std::array<float, 2> > &points) {
                for (const auto &coords: points) {
                    auto c = sf::CircleShape(1.0);
                    c.setFillColor(sf::Color::White);
                    float x = coords[0] * kWidth;
                    float y = kHeight - coords[1] * kHeight;
                    c.setPosition({x, y});
                    shapes_.push_back(std::make_unique<sf::CircleShape>(c));
                }
            }

            void AddLines_(const std::vector<math::Point2f> &points) {
                for (int i = 0; i < points.size() - 1; ++i) {
                    lines_.emplace_back(std::array{
                        sf::Vertex({points[i].x(), points[i].y()}, sf::Color::White),
                        sf::Vertex({points[i + 1].x(), points[i + 1].y()}, sf::Color::White)
                    });
                }
            }

            void LoadData_(std::string_view path) {
                const auto p = path.data() + std::string(".h5");
                const HighFive::File file(p, HighFive::File::ReadOnly);
                const auto dataset = file.getDataSet("points");
                auto data = dataset.read<std::vector<std::array<float, 2> > >();
                AddPoints_(data);
            }

            void ComputeHull_() {
                std::vector<math::Point2f> points;
                std::ranges::transform(shapes_, std::back_inserter(points), [](const auto &shape) -> math::Point2f {
                    return {shape->getPosition().x, shape->getPosition().y};
                });
                const std::vector<math::Point2f> hull = alg::ConvexHull2D(points);
                AddLines_(hull);
            }

            void SetupImGui_() {
                bool loadData = false;
                ImGui::SFML::Update(window_, clock_.restart());
                ImGui::Begin("Convex Hull Points");
                const auto preview = (convex_hull_points_ >= 0)
                                         ? convex_hull_data_files[convex_hull_points_]
                                         : "Choose an option";

                if (ImGui::BeginCombo("Filename", preview.data())) {
                    for (int n = 0; n < IM_ARRAYSIZE(convex_hull_data_files); ++n) {
                        if (const auto filename = convex_hull_data_files[n]; ImGui::Selectable(
                            filename.data(), convex_hull_points_ == n)) {
                            convex_hull_points_ = n;
                        }
                    }
                    ImGui::EndCombo();
                }
                if (ImGui::Button("Generate points") && convex_hull_points_ >= 0) LoadData_(
                    convex_hull_data_files[convex_hull_points_]);
                ImGui::SameLine();
                if (ImGui::Button("Create Hull")) ComputeHull_();
                ImGui::SameLine();
                if (ImGui::Button("Clear")) Clear_();
                ImGui::End();
            }

            void Update() {
                SetupImGui_();
                window_.clear();
                for (const auto &shape: shapes_) window_.draw(*shape);
                for (const auto &line: lines_) window_.draw(line.data(), line.size(), sf::PrimitiveType::Lines);

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
            std::vector<std::unique_ptr<sf::Shape> > shapes_;
            std::vector<std::array<sf::Vertex, 2> > lines_;

            sf::RenderWindow window_;
            sf::Clock clock_;
            int convex_hull_points_;
        };
    }
} // namespace geom

#endif // WINDOW_H
