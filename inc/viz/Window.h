#ifndef WINDOW_H
#define WINDOW_H

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <highfive/highfive.hpp>

#include "../alg/ConvexHull.hpp"
#include "../math/Line.hpp"
#include "../math/Point.hpp"

namespace geom {
    namespace viz {
        typedef std::vector<std::array<float, 2>> PointList;
        typedef std::vector<std::array<float, 4>> LineList;

        class Renderer {
        public:
            Renderer() : convex_hull_points_(-1), line_segments_(-1) {
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

            void AddPoints_(const PointList &points) {
                for (const auto &coords: points) {
                    auto c = sf::CircleShape(1.0);
                    c.setFillColor(sf::Color::Green);
                    float x = coords[0] * kWidth;
                    float y = kHeight - coords[1] * kHeight;
                    c.setPosition({x, y});
                    shapes_.push_back(std::make_unique<sf::CircleShape>(c));
                }
            }

            void AddLines_(const PointList &points) {
                for (int i = 0; i < points.size() - 1; i+=2) {
                    lines_.emplace_back(std::array{
                        sf::Vertex({points[i][0], points[i][1]}, sf::Color::White),
                        sf::Vertex({points[i + 1][0], points[i + 1][1]}, sf::Color::White)
                    });
                }
            }

            void AddConvex_(const PointList &points) {
                sf::ConvexShape convex;
                convex.setFillColor(sf::Color::Transparent);
                convex.setOutlineThickness(1.f);
                convex.setOutlineColor(sf::Color::Cyan);
                convex.setPointCount(points.size());
                for (int i = 0; i < points.size(); ++i) {
                    convex.setPoint(i, {points[i][0], points[i][1]});
                }
                shapes_.emplace_back(std::make_unique<sf::ConvexShape>(convex));
            }

            static void LoadPoints_(
                const std::string_view path,
                const std::function<void(const PointList&)> &adder
                ) {
                const auto p = path.data() + std::string(".h5");
                const HighFive::File file(p, HighFive::File::ReadOnly);
                const auto dataset = file.getDataSet("points");
                const auto data = dataset.read<PointList>();
                adder(data);
            }

            void ComputeHull_() {
                // TODO: This is gross, either create an interface or switch to SFML data structures
                std::vector<math::Point2f> points;
                PointList hull_list;
                std::ranges::transform(shapes_, std::back_inserter(points), [](const auto &shape) -> math::Point2f {
                    return {shape->getPosition().x, shape->getPosition().y};
                });
                const std::vector<math::Point2f> hull = alg::ConvexHull2D(points);
                std::ranges::transform(hull, std::back_inserter(hull_list), [](const auto &point) -> std::array<float, 2> {
                    return {point.x(), point.y()};
                });
                AddConvex_(hull_list);
            }

            void ComputeIntersection_() {
                // TODO: This is gross, either create an interface or switch to SFML data structures
                std::vector<math::Line<float, 2>> lines;
                std::ranges::transform(lines_, std::back_inserter(lines), [](const auto &line) -> math::Line<float, 2> {
                    return {
                        {line[0].position.x, line[0].position.y},
                        {line[1].position.x, line[1].position.y},
                        };
                });
                if (!lines[0].Intersects(lines[1])) return;
                const math::Point2f intersection = math::GetIntersection(lines[0], lines[1]);
                AddPoints_({{intersection.x() / kWidth, (kHeight - intersection.y()) / kHeight}});
            }

            void CreateAlgorithmBar(
                    const std::string_view name,
                    const std::vector<std::string_view> &filenames,
                    int& file_idx,
                    const std::function<void()>& algorithm,
                    const std::function<void(const PointList&)>& data_adder
                    ) {
                if (ImGui::BeginTabItem(name.data())) {
                    const auto preview = (file_idx >= 0)
                                             ? filenames[file_idx]
                                             : "Choose an file";

                    if (ImGui::BeginCombo("Filename", preview.data())) {
                        for (int n = 0; n < filenames.size(); ++n) {
                            if (const auto filename = filenames[n]; ImGui::Selectable(
                                filename.data(), file_idx == n)) {
                                file_idx = n;
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::Button("Generate data") && file_idx >= 0) LoadPoints_(filenames[file_idx], data_adder);
                    ImGui::SameLine();
                    if (ImGui::Button("Run")) algorithm();
                    ImGui::SameLine();
                    if (ImGui::Button("Clear")) Clear_();
                    ImGui::EndTabItem();
                }

            }

            void SetupImGui_() {
                ImGui::SFML::Update(window_, clock_.restart());
                ImGui::Begin("Algorithms");
                if (ImGui::BeginTabBar("Alg_Bar")) {
                    CreateAlgorithmBar(
                        "Convex Hull",
                        {std::begin(convex_hull_data_files), std::end(convex_hull_data_files)},
                        convex_hull_points_,
                        [this] () {this->ComputeHull_();},
                        [this] (const PointList& p) {this->AddPoints_(p);}
                        );
                    CreateAlgorithmBar(
                        "Line Segment Intersection",
                        {std::begin(line_segment_data_files), std::end(line_segment_data_files)},
                        line_segments_,
                        [this] () {this->ComputeIntersection_();},
                        [this] (const PointList& p) {this->AddLines_(p);}
                        );
                    ImGui::EndTabBar();
                }
                ImGui::End();
            }

            void Update() {
                SetupImGui_();
                window_.clear();
                for (const auto &line: lines_) window_.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                for (const auto &shape: shapes_) window_.draw(*shape);

                ImGui::SFML::Render(window_);
                window_.display();
            }

            [[nodiscard]]  bool IsRunning() const {
                return window_.isOpen();
            }

            ~Renderer() {
                ImGui::SFML::Shutdown();
            }

        public:
            static constexpr int              kWidth                    = 640;
            static constexpr int              kHeight                   = 640;
            static constexpr std::string_view convex_hull_data_files[]  = {"res/points"};
            static constexpr std::string_view line_segment_data_files[] = {"res/lines"};

        private:
            std::vector<std::unique_ptr<sf::Shape> > shapes_;
            std::vector<std::array<sf::Vertex, 2> > lines_;

            sf::RenderWindow window_;
            sf::Clock clock_;
            int convex_hull_points_;
            int line_segments_;
        };
    }
} // namespace geom

#endif // WINDOW_H
