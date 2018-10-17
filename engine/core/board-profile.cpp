/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "board-profile.h"
#include "paths.h"
#include "shapes.h"

namespace MonkeyCAM {

BoardProfile::BoardProfile(MCFixed noseThickness, MCFixed centerThickness,
                           MCFixed tailThickness, End nose, End tail,
                           BoardShape& shape)
    : m_noseThickness(noseThickness),
      m_centerThickness(centerThickness),
      m_tailThickness(tailThickness),
      m_nose(nose),
      m_tail(tail),
      m_debugPathSet("Profile") {
  auto eeHalf = shape.eeLength() / 2;
  auto eeCenterX = eeHalf + shape.noseLength();
  auto noseLandX = -eeHalf * m_nose.taperEnd + eeCenterX;
  auto cpNosePullStart = -eeHalf * m_nose.pullStart + eeCenterX;
  auto cpNosePullEnd = -eeHalf * m_nose.pullEnd + eeCenterX;
  auto noseDropX = -eeHalf * m_nose.taperStart + eeCenterX;
  auto tailLandX = eeHalf * m_tail.taperEnd + eeCenterX;
  auto tailDropX = eeHalf * m_tail.taperStart + eeCenterX;
  auto cpTailPullStart = eeHalf * m_tail.pullStart + eeCenterX;
  auto cpTailPullEnd = eeHalf * m_tail.pullEnd + eeCenterX;

  m_path.push_back(Point(0, m_noseThickness));
  BezierPath np{Point(noseLandX, m_noseThickness),
                Point(cpNosePullEnd, m_noseThickness),
                Point(cpNosePullStart, m_centerThickness),
                Point(noseDropX, m_centerThickness)};
  m_path.push_back_path(np);
  BezierPath tp{Point(tailDropX, m_centerThickness),
                Point(cpTailPullStart, m_centerThickness),
                Point(cpTailPullEnd, m_tailThickness),
                Point(tailLandX, m_tailThickness)};
  m_path.push_back_path(tp);
  m_path.push_back(Point(shape.overallLength(), m_tailThickness));

  // It's more work to visualize the profile than to create it :)
  Path ep = m_path;
  std::transform(ep.begin(), ep.end(), ep.begin(),
                 [&](const Point& p) { return Point(p.X, p.Y * 5); });
  m_debugPathSet.addDescription(
      "<p>The thickness profile is shown below, as viewed from the side. It is "
      "formed with flat sections at the nose, center, and tail joined by a "
      "Bezier curve between each. The transition points are defined as a "
      "percentage of the distance from the center of the effective edge to the "
      "nose or tail. Starting from the center and moving towards the nose, the "
      "profile will be flat until reaching the taper start point. The Bezier "
      "will provide a smooth transition (pulled by the start handle and end "
      "handle) to the taper end point, where the minimum thickness is reached. "
      "The core remains flat all the way to the end of the nose.</p>");
  m_debugPathSet.addDescription(
      "<p>Note that these percentages can by any valid value between 0%% and "
      "100%%, but they should be increasing values. </p>");
  m_debugPathSet.addPath([&] {
    return DebugPath{
        ep,
        DebugAnnotationDesc{
            "Profile shape",
            "The shape of the profile, as viewed from the side. <b>Note: the "
            "height is exaggerated 5x for better visibility into the shape of "
            "the taper.</b>"}};
  });
  MCFixed taperY = m_centerThickness * -5 - 1;
  MCFixed controlY = taperY;
  MCFixed insertsY = m_centerThickness * -5;
  auto pointMarker = [&](DebugAnnotation& a, Point p, double len) {
    auto ls = (len < 0 ? -0.75 : 0.75) / 2;
    a.addSvgFormat(R"(<path d="M%f %f l%f 0"/>)", p.X.dbl() + ls, p.Y.dbl(),
                   len);
    a.addSvgCircle(Point(p.X, -p.Y), 0.75);
    const char* anchor = len < 0 ? "end" : "start";
    a.addSvgFormat(R"(<text x="%f" y="%f" dy="-0.5" text-anchor="%s" )"
                   R"(font-size="1" stroke="none" fill="black">)"
                   R"(%.2f</text>)",
                   p.X.dbl() + len, p.Y.dbl(), anchor, (p.X - eeCenterX).dbl());
  };
  m_debugPathSet.addAnnotation([&] {
    auto a = DebugAnnotation{DebugAnnotationDesc{
        "Taper begin and end points",
        "Circles at each point where the taper beings and ends, with a short "
        "line pointing in the direction of the associate Bezier control "
        "point.",
        "green", true}};
    pointMarker(a, Point(noseLandX, taperY), 1);
    pointMarker(a, Point(noseDropX, taperY), -1);
    pointMarker(a, Point(tailDropX, taperY), 1);
    pointMarker(a, Point(tailLandX, taperY), -1);
    return a;
  });
  m_debugPathSet.addAnnotation([&] {
    auto a = DebugAnnotation{DebugAnnotationDesc{
        "Taper control points",
        "Circles at each control point, with a short line pointing in the "
        "direction of the associated taper begin/end point.",
        "red", true}};
    pointMarker(a, Point(cpNosePullStart, controlY), 1);
    pointMarker(a, Point(cpNosePullEnd, controlY), -1);
    pointMarker(a, Point(cpTailPullStart, controlY), -1);
    pointMarker(a, Point(cpTailPullEnd, controlY), 1);
    return a;
  });
  m_debugPathSet.addAnnotation([&] {
    auto a = DebugAnnotation{
        DebugAnnotationDesc{"Nose, centers, tail", "", "blue", true}};
    auto guideWidth = (shape.minWidth() / 2 - 2).dbl();
    auto wMarker = [&](MCFixed x, double width) {
      a.addSvgFormat(R"(<path d="M%f %f L%f %f"/>)", x.dbl(), -width, x.dbl(),
                     width);
    };
    wMarker(shape.noseLength(), guideWidth);
    wMarker(shape.overallLength() / 2, guideWidth / 2);
    wMarker(eeCenterX, guideWidth);
    wMarker(shape.noseLength() + shape.eeLength(), guideWidth);
    return a;
  });
  m_debugPathSet.addAnnotation([&] {
    auto a = DebugAnnotation{DebugAnnotationDesc{"Inserts", "", "blue"}};
    Path ip = shape.insertsPath();
    auto newEnd = std::unique(ip.begin(), ip.end(),
                              [](Point p1, Point p2) { return p1.X == p2.X; });
    ip.resize(std::distance(ip.begin(), newEnd));
    for (auto& p : ip) {
      if (p.Y != 0) {  // The pins are in here too, so remove them.
        a.addSvgCircle({p.X, -insertsY}, 0.75);
      }
    }
    return a;
  });
}

}  // namespace MonkeyCAM
