/*!
 * Stats
 */


#pragma once

#include <iostream>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>
#include <Stats/StatsAccessor.hpp>


class Stats : public Drawable, public Updatable {
 public:
  void addGraph(int index, std::string const& title,
      std::vector<std::string> const& series,
      double min, double max, Vec2d size);
	
	//It calls the setActive method for the targetTitle.
  void focusOn(std::string targetTitle);
  void focusOnRelativeGraph(int delta);
  void reset();
  //It marks the graph number index as active, 
  //and as such, as drawable.
  void setActive(int index);

  void draw(sf::RenderTarget& targetWindow) const override;
  void update(sf::Time dt) override;

 private:
  StatsAccessor accessor;
  int activeGraphIndex = -1;

  std::unordered_map<int, std::unique_ptr<Graph>> graphs;
  std::unordered_map<int, std::string> titles;

  std::unordered_map<std::string, double> fetchData(std::string title);
};

