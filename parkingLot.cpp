#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>
using namespace std;

class vehicle;
class parkingSpot;


enum class parkingSpotType
{
COMPACT, LARGE,MOTORCYCLE
};

enum class vehicleType
{MOTORBIKE, TRUCK, CAR 
};

class vehicle{
protected:
  string licensePlate;
  vehicleType
 type;
public:
  vehicle(string plate, vehicleType
   type) : licensePlate(plate), type(type) {};
  virtual ~vehicle() = default;
  string getLicensePlate() {
    return licensePlate;
  }
  vehicleType
 getVehicleType(){
    return type;
  }
  virtual parkingSpotType getParkingSpotType() const = 0;
};
 
class car : public vehicle{
public : 
  car(string plate) : vehicle(plate,vehicleType
  :: CAR) {};
  parkingSpotType getParkingSpotType() const override {
    return parkingSpotType::COMPACT;
  }
};

class truck: public vehicle{

public:
  truck(string plate) : vehicle(plate, vehicleType
  ::TRUCK) {};
  parkingSpotType getParkingSpotType() const override {
    return parkingSpotType::LARGE;
  }
};

class parkingSpot {
protected:
  int spotNumber;
  bool occupied;
  parkingSpotType type;

public:
  parkingSpot(int number, parkingSpotType type) : spotNumber(number), occupied(false), type(type) {};
  int getSpotNumber() { return spotNumber; }
  bool isOccuped() { return occupied; }
  parkingSpotType getParkingSpotType() { return type; }

  void park() { occupied = true; }
  void unPark() { occupied = false; }
};

class compactSpot : public parkingSpot{
  public:
    compactSpot(int number) : parkingSpot(number, parkingSpotType::COMPACT) {};
};

class largeSpot : public parkingSpot{
  public:
    largeSpot(int number) : parkingSpot(number, parkingSpotType::LARGE) {};
};

class motorCycleSpot : public parkingSpot{
  public:
    motorCycleSpot(int number) : parkingSpot(number, parkingSpotType::MOTORCYCLE) {};
};

class ticket{
private:
  int ticketId;
  vehicle * ticketVehicle;
  parkingSpot * ticketParkingSpot;
  chrono::system_clock::time_point entryTime;

  public: 
  ticket(int id, vehicle* v, parkingSpot * p): ticketId(id), ticketVehicle(v),ticketParkingSpot(p){
    entryTime = chrono::system_clock::now();
  }
  int getId() { return ticketId; }
  vehicle *getTicketVehicle() { return ticketVehicle; }
  parkingSpot *getParkingSpot() { return ticketParkingSpot; }
  chrono::system_clock::time_point getEntryTime() { return entryTime;  }
};

class parkingLot{
  private:
    map<parkingSpotType, vector<parkingSpot *>> availableSpots;
    map<int, ticket *> activeTickets;
    int nextTicketId = 1;
  public:
  ~parkingLot() {
    for (auto it = availableSpots.begin(); it != availableSpots.end();it++){
      for(auto sp: it->second){
        delete sp;
      }
    }
    for (auto it = activeTickets.begin(); it != activeTickets.end();it++){
      delete it->second;
    }
  }
  void addParkingSpot(parkingSpot* parkSpot){
    availableSpots[parkSpot->getParkingSpotType()].push_back(parkSpot);
   }

   // okay so when you are parking a vehicle you willl be getting a ticket for that parking
  ticket*  parkVehicle(vehicle* v){
      parkingSpotType requiredType = v->getParkingSpotType();
      if(availableSpots[requiredType].empty()){
        cout << "no available spots for vehicle type" << endl;
        return nullptr;
      }
      parkingSpot *spot = availableSpots[requiredType].back();
      availableSpots[requiredType].pop_back();
      spot->park();
      ticket *tkt = new ticket(nextTicketId++, v, spot);
      activeTickets[tkt->getId()] = tkt;
      cout << "vehicle : " << v->getLicensePlate() << "parking in spot " << spot->getSpotNumber() << " . Ticket ID " << tkt->getId() << endl;
      return tkt;
    }


    // when you are unparking the vehicle you will be getting the price of the parking;
  double unparkVehicle(int ticketId) {
     if(activeTickets.find(ticketId) == activeTickets.end()){
       cout << "your tickets has not been found inside the database" << endl;
       return 0.0;
     }
     ticket *tkt = activeTickets[ticketId];
     parkingSpot* spot  = tkt->getParkingSpot();
     spot->unPark();
     availableSpots[spot->getParkingSpotType()].push_back(spot);
     double fee = calculateFee(tkt);
     cout << "Vehicle " << tkt->getTicketVehicle()->getLicensePlate() << " unparked from spot "
             << spot->getSpotNumber() << ". Fee: $" << fee << endl;
     activeTickets.erase(ticketId);
     delete tkt;
     return fee;
  }
  double calculateFee(ticket * tkt){
    auto exitTime = chrono::system_clock::now();
    auto entryTime = tkt->getEntryTime();
    auto duration = chrono::duration_cast<chrono::seconds>(exitTime - entryTime);
    double hours = (duration.count() / 3600.0);
    return hours * 10.0;
  }
};


// now we will implement the class code to see whether our classes are working or not
int main(){
  parkingLot myParkingLot;
  myParkingLot.addParkingSpot(new compactSpot(100));
  myParkingLot.addParkingSpot(new compactSpot(101));
  myParkingLot.addParkingSpot(new compactSpot(102));
  myParkingLot.addParkingSpot(new largeSpot(103));
  myParkingLot.addParkingSpot(new largeSpot(104));
  myParkingLot.addParkingSpot(new motorCycleSpot(105));
  myParkingLot.addParkingSpot(new motorCycleSpot(106));

  cout << "parking lot initialized" << endl;
  cout << "-----------------------" << endl;
  // creation of vehicles; 
  car myCar("MP04-AB1234");
  truck myTruck("sadda-punjab");

  
  ticket* carTicket = myParkingLot.parkVehicle(&myCar);
  ticket *truckTicket = myParkingLot.parkVehicle(&myTruck);
  cout << "vehicles are parked" << endl;
  if(carTicket){
    myParkingLot.unparkVehicle(carTicket->getId());
    
  }
  if(truckTicket){
    myParkingLot.unparkVehicle(truckTicket->getId());
  }
  cout << "parking long operations completed" << endl;
}