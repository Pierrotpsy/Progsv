/*!
 * HeadlessApplication
 */


#include "HeadlessApplication.hpp"
#include <Animal/Gerbil.hpp>
#include <Stats/StatsAccessor.hpp>


IMPLEMENT_MAIN(HeadlessApplication)


void HeadlessApplication::run() {
  mEnvPPS = new Environment();

  onRun();
  onSimulationStart();


  StatsAccessor accessor;

  getEnv().addGenerator(new FoodGenerator());

  for (int i = 0; i < 50; i++) {
    getEnv().addEntity(new Gerbil(Vec2d(
            uniform(0, getConfig().simulation_world_size),
            uniform(0, getConfig().simulation_world_size))));
  }

  unsigned int gerbilAddIndex(0);
  sf::Time gerbilAddTime(sf::Time::Zero);

  sf::Time dt = sf::seconds(.1);
  sf::Time endTime = sf::seconds(1e4);

  for (sf::Time time = sf::Time::Zero; time < endTime; time += dt) {
    getEnv().update(dt);
    onUpdate(dt);

    if (gerbilAddIndex < 10 && time >= gerbilAddTime) {
      getEnv().addEntity(new Gerbil(Vec2d(
              uniform(0, getConfig().simulation_world_size),
              uniform(0, getConfig().simulation_world_size))));

      gerbilAddIndex++;
      gerbilAddTime = time + sf::seconds(3);
    }


    auto data = accessor.updateData();

    std::cout << " " << time.asSeconds()
      << " " << data.food.number
      << " " << data.gerbil.number
      << " " << data.gerbil.energyMean
      << " " << data.gerbil.ageMean
      << " " << data.gerbil.femaleProportion
      << " " << data.gerbil.matableFemaleProportion
      << " " << data.gerbil.matableMaleProportion
      << "\n";

    if (data.gerbil.number < 1) {
      std::cerr << "\r " << time.asSeconds() << "/" << endTime.asSeconds();
      break;
    }
  }

  std::cout << "\n";
}

