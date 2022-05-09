/*!
 * Stats
 */


#include "Stats.hpp"
#include <Application.hpp>


class OrganicEntity;

void Stats::addGraph(int index, std::string const& title,
    std::vector<std::string> const& series,
    double min, double max, Vec2d size) {
  std::unique_ptr<Graph> ptr(new Graph(series, size, min, max));

  graphs[index] = std::move(ptr);
  titles[index] = title;

  setActive(index);
}

void Stats::draw(sf::RenderTarget& targetWindow) const {
  if (activeGraphIndex >= 0) {
    graphs.at(activeGraphIndex)->draw(targetWindow);
  }
}

std::unordered_map<std::string, double> Stats::fetchData(std::string title) {
  auto data = accessor.updateData();

  if (title == s::GENERAL) {
    return {
      { "Gerbils", data.gerbil.number },
      { "Scorpions", data.scorpion.number },
      { "Food sources", data.food.number }
    };
  } if (title == s::WAVES) {
    return {
      { "Waves", data.wave.number }
    };
  } if (title == s::GERBILS) {
    return {
      { "Gerbils", data.gerbil.number },
      { "Mean age", data.gerbil.ageMean },
      { "Mean energy", data.gerbil.energyMean }
    };
  }

  return {};
}

void Stats::focusOn(std::string targetTitle) {
  for (auto& title : titles) {
    if (title.second == targetTitle) {
      setActive(title.first);
      break;
    }
  }
}

// This method is used for navigating among the graphs:
//  * delta = +1 -> focus on next graph
//  * delta = -1 -> focus on previous graph
void Stats::focusOnRelativeGraph(int delta) {
  activeGraphIndex = (activeGraphIndex + delta) % (int) graphs.size();

  // The % operator maps negative numbers to [1 - graphs.size(), 0], by
  // adding graphs.size() we map them to [1, graphs.size()]. The upper bound
  // is never reached because 0 is not initially strictly negative.
  if (activeGraphIndex < 0) {
    activeGraphIndex += graphs.size();
  }
}

void Stats::reset() {
  for (auto& graph : graphs) {
    graph.second->reset();
  }
}

void Stats::setActive(int index) {
  activeGraphIndex = index;
}

void Stats::update(sf::Time dt) {
  for (auto& graph : graphs) {
    std::string title = titles.at(graph.first);
    graph.second->updateData(dt, fetchData(title));
  }
}

