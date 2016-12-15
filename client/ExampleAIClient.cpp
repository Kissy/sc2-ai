#include <BWAPI.h>
#include <BWAPI/Client.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using namespace BWAPI;

void reconnect() {
    while (!BWAPIClient.connect()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }
}

int main(int argc, const char* argv[]) {
    std::cout << "Connecting..." << std::endl;;
    reconnect();
    while (true) {
        std::cout << "waiting to enter match" << std::endl;
        while (!Broodwar->isInGame()) {
            BWAPI::BWAPIClient.update();
            if (!BWAPI::BWAPIClient.isConnected()) {
                std::cout << "Reconnecting..." << std::endl;;
                reconnect();
            }
        }
        std::cout << "starting match!" << std::endl;
        // Enable some cheat flags
        Broodwar->enableFlag(Flag::UserInput);
        // Uncomment to enable complete map information
        //Broodwar->enableFlag(Flag::CompleteMapInformation);

        //send each worker to the mineral field that is closest to it
        Unitset units = Broodwar->self()->getUnits();
        Unitset minerals = Broodwar->getMinerals();
        for (auto& u : units) {
            if (u->getType().isWorker()) {
                Unit closestMineral = nullptr;

                for (auto& m : minerals) {
                    if (!closestMineral || u->getDistance(m) < u->getDistance(closestMineral))
                        closestMineral = m;
                }
                if (closestMineral) {
                    u->rightClick(closestMineral);
                }
            }
            else if (u->getType().isResourceDepot()) {
                //if this is a center, tell it to build the appropiate type of worker
                u->train(Broodwar->self()->getRace().getWorker());
            }
        }

        while (Broodwar->isInGame()) {
            BWAPI::BWAPIClient.update();

            for (auto& e : Broodwar->getEvents()) {
                switch (e.getType()) {
                    case EventType::MatchEnd: break;
                    case EventType::ReceiveText: break;
                    case EventType::PlayerLeft: break;
                    case EventType::NukeDetect: break;
                    case EventType::UnitCreate: break;
                    case EventType::UnitDestroy:  break;
                    case EventType::UnitMorph: break;
                    case EventType::UnitShow: break;
                    case EventType::UnitHide: break;
                    case EventType::UnitRenegade: break;
                    case EventType::SaveGame: break;
                    case EventType::MatchStart: break;
                    case EventType::MatchFrame: break;
                    case EventType::MenuFrame: break;
                    case EventType::UnitDiscover: break;
                    case EventType::UnitEvade: break;
                    case EventType::UnitComplete: break;
                    case EventType::SendText: break;
                    case EventType::None: break;
                    default: break;
                }
            }

            Broodwar->drawTextScreen(300, 0, "FPS: %f", Broodwar->getAverageFPS());
        }
        std::cout << "Game ended" << std::endl;
    }
}

