//
// Created by admin1 on 19-12-2023.
//

#include <iostream>
#include "../include/Network.h"

Network::Network() {
    this->airportSet.clear();
    this->airlineMap.clear();
    this->flightCount = 0;
}

const std::unordered_map<std::string, std::shared_ptr<AirportVertex>>& Network::getAirportSet() const {
    return this->airportSet;
}


const std::unordered_map<std::string, std::shared_ptr<Airline>>& Network::getAirlineMap() const {
    return this->airlineMap;
}

bool Network::addAirline(std::shared_ptr<Airline> airline) {
    this->airlineMap.insert(std::pair<std::string, std::shared_ptr<Airline>  >(airline->getCode(), airline));
    return false;
}

bool Network::addAirport(std::shared_ptr<AirportVertex> airport) {
    if(this->airportSet.find(airport->getAirportCode()) != this->airportSet.end()){return false;}

    if(this->cityMap.find(airport->getCity()) == this->cityMap.end()){
        this->cityMap.emplace(airport->getCity(), std::vector<std::shared_ptr<AirportVertex>>());
    }

    this->cityMap.find(airport->getCity())->second.push_back(airport);
    this->airportSet.emplace(airport->getAirportCode(), airport);
    return true;
}

void Network::addFlight(const std::string &source, const std::string &target, const std::string &airline) {
    std::shared_ptr<AirportVertex> airportSource = this->findVertex(source);
    std::shared_ptr<AirportVertex> airportTarget = this->findVertex(target);
    if(airportSource != nullptr && airportTarget != nullptr){
        airportSource->addFlight(airportTarget, airline);
        this->increaseFlightCount();
    }
}

std::shared_ptr<AirportVertex> Network::findVertex(const std::string &info) {
    if(this->airportSet.find(info) != this->airportSet.end()){
        return this->airportSet[info];
    }
    return nullptr;
}



int Network::getFlightCount() const {
    return this->flightCount;
}

void Network::increaseFlightCount() {
    this->flightCount += 1;
}

int Network::outDegree(const std::string &airportCode) const {
    if(this->airportSet.find(airportCode) == this->airportSet.end()) return -1;
    return this->airportSet.find(airportCode)->second->getFlights().size();
}

const std::vector<Flight>& Network::getFlightsFromAirport(const std::string &airportCode) const{
    static const std::vector<Flight> emptyFlightVector;
    if(this->airportSet.find(airportCode) == this->airportSet.end()) return emptyFlightVector;
    return this->airportSet.find(airportCode)->second->getFlights();
}

std::vector<std::shared_ptr<AirportVertex>> Network::airportsInCity(const std::string &city) {
    if(this->cityMap.find(city) == this->cityMap.end()) return {};
    return this->cityMap.find(city)->second;
}

void Network::resetFlags() {
    for(std::pair<std::string, std::shared_ptr<AirportVertex>> airportVertex: this->airportSet){
        airportVertex.second->setVisited(false);
    }
}

std::shared_ptr<AirportVertex> Network::findAirportByName(const std::string &name) {
    for(const auto& pair: this->airportSet){
        if(pair.second->getName() == name){
            return pair.second;
        }
    }
    return nullptr;
}

void Network::clearData() {
    this->flightCount = 0;
    this->airportSet.clear();
    this->airlineMap.clear();
    this->cityMap.clear();
}
