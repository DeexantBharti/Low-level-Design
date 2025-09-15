#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<map>
#include<algorithm>

using namespace std;
// forward declaration of classess;
class Appointment;
class User;
class Doctor;

class Address{

  string line1, line2, city;

public:
  Address(string l1, string l2, string city, string country) : line1(l1), line2(l2), city(city) {};
};

class TimeSlot{
  string date, time;
  public:
    TimeSlot(string date, string time) : date(date), time(time) {};
    string getDate() { return date; }
    string getTime() { return time; }
    bool operator==(TimeSlot& timeslot){
      return timeslot.date == this->date && timeslot.time == this->time;
    }
};

class Person{
  string name, email, hashedPassword;
  Address address;
  int id;
  static int nextId;

public:
  Person(string name, string email, string hashedPassword, Address address) : name(name), email(email), hashedPassword(hashedPassword), address(address),id(nextId++) {};
  virtual ~Person(){};
  string getName() { return name; }
  int getId() { return id; }
  virtual void displayDetails() {
    cout << "person name  is : " << name << " person email is : " << email << endl;
  }
};
int Person::nextId = 1;
class Appointment{
public:
  enum class Status
  {
    SCHEDULED,
    COMPLETED,
    CANCELLED
  };
  private:
    int id;
    User *patient;
    Doctor* doctor;
    TimeSlot timeslot;
    Status status;
    static int nextId;
  public:
    Appointment(User *p, Doctor *d, TimeSlot ts) : id(nextId++), patient(p), doctor(d), timeslot(ts),  status(Status::SCHEDULED){};
    void displayDetails();
};
int Appointment::nextId = 1001;





// core-classess;


class User: public Person{
  string phone;
  vector<Appointment *> appointments;
  public:
    User(string name, string email, string hpass, Address add, string phone) : Person(name, email, hpass, add), phone(phone) {};
    void addAppointment(Appointment *app){
      appointments.push_back(app);
    }
    void displayDetails() override{
      Person::displayDetails();
      cout << endl
           << "phone number is " << phone << endl;
    }
    void viewAppointments(){
      if(appointments.empty()){
        cout<<"sorry no appointments are there for this user"<<endl;
        return;
      }
      for(auto apt : appointments ){
        apt->displayDetails();
      }
    }

};

class Doctor: public Person{
  string speciality;
  double fees;
  bool isAvailable;
  vector<Appointment *> appointments;
  map<string, vector<string>> slots_booked; // date -> vector of time slot of each half hour;
  public:
    Doctor(string name, string e, string hp, Address add, string spec, double fees) : Person(name, e, hp, add), speciality(spec), fees(fees), isAvailable(true) {};
    void addAppointment(Appointment* app ){
      appointments.push_back(app);
    }
    bool isSlotAvailable(TimeSlot & timeslot){
      if(isAvailable == false)
        return false;
      if(slots_booked.find(timeslot.getDate()) == slots_booked.end())
        return true;

      auto times = slots_booked[timeslot.getDate()];
      // return find(slots_booked[timeslot.getDate()].begin(), slots_booked[timeslot.getDate()].end(), timeslot.getTime()) == slots_booked[timeslot.getDate()].end();
      return find(times.begin(), times.end(), timeslot.getTime()) == times.end();
    }
    void bookSlot(TimeSlot & timeslot){
      slots_booked[timeslot.getDate()].push_back(timeslot.getTime());
    }
    void displayDetails()override{
      Person::displayDetails();
      cout << endl;
       std::cout << "Speciality: " << speciality << "\nFees: $" << fees << std::endl;
        std::cout << "Availability: " << (isAvailable ? "Available" : "Not Available") << std::endl;
    }
    string getSpeciality() {
      return speciality;
    }
    void viewAppointments(){
      if(appointments.empty()){
        cout << "there are no appointments for this particular doctor " << endl;
        return;
      }
      for(auto app: appointments){
        app->displayDetails();
      }
    }

};

    void Appointment::displayDetails(){
      cout << "appointment Id : " << id << endl;
      cout << "patient name" << patient->getName() << endl;
      cout << "doctors name" << doctor->getName() << endl;
      std::cout << "  Status of this appointment: ";
                switch(status){
                    case Status::SCHEDULED: std::cout << "Scheduled"; break;
                    case Status::COMPLETED: std::cout << "Completed"; break;
                    case Status::CANCELLED: std::cout << "Cancelled"; break;
                }
    }



// controller system or orchestrator class
class DoceasySystem{
  vector<User *> users;
  vector<Doctor*> doctors;
  vector<Appointment* > appointments;
public: 
void registerUser(string name, string em , string hpass, Address address, string phone){
  User *us = new User(name, em, hpass, address, phone);
  users.push_back(us);
}
void registerDoctor(string name, string email, string hpass, Address address, string speci, double fees) {
  Doctor *dc = new Doctor(name, email, hpass, address, speci, fees);
  doctors.push_back(dc);
}
User* getUserById(int id){
  for(const auto & user: users){
    if(user->getId() == id)
      return user;
  }
  return NULL;
}
Doctor* getDoctorById(int id){
  for(const auto & doc: doctors){
    if(doc->getId() == id)
      return doc;
  }
  return NULL;
}
void displayDoctorsBySpeciality(string spec){
  cout << "doctors list of speciality : " << spec << " :";
  int count = 1;
  for(const auto & doc: doctors){
    if(doc->getSpeciality() == spec){
      cout << "doctor no. " << count++ << endl;
      doc->displayDetails();
    }
  }
  }
  bool createBooking(int userId, int doctorId, TimeSlot timeslot){
    auto doctor = getDoctorById(doctorId);
    auto user = getUserById(userId);
    if(!doctor || !user){
      cout << "doctor or user doesn't exist " << endl;
      return false;
    }
    if(doctor->isSlotAvailable(timeslot)){
      doctor->bookSlot(timeslot);
      Appointment *newAppointment = new Appointment(user, doctor, timeslot);
      user->addAppointment(newAppointment);
      doctor->addAppointment(newAppointment);
      cout << "booking successfull " << endl;
      newAppointment->displayDetails();
      return true;
    }
    else{
      cout << "booking unsuccessfull" << endl;
      return false;
    }
  }
};


int main(){
  DoceasySystem system;
  system.registerUser("Deexant Bharti", "deexantbharti4321@gmail.com", "password123", Address("crockroad", "ritchmond road", "Bhopal", "India"), "911-xxxxxxxx");
  system.registerUser("Divy Pareek" , "divy1234@gmail.com", "password234" , Address("ritchmond orad" , "jaipur road" , "Bhopal" , "India") , "9111-xxxxxxx");

  system.registerDoctor("Dr. Kartik Patel", "drKartik12345@gmail.com", "passwrod 1245", Address("kamla nagar", "police station", "Indore", "India"), "Cardiology",40);
  system.registerDoctor("Dr. Manish Sharma", "manish12345@gmail.com", "passwrod 1245", Address("ravi nagar", "hospital sadan ", "Indore", "India"), "Cardiology",40);
  system.registerDoctor("Dr. Radha Bishnoi", "rradha12345@gmail.com", "passwrod 1245", Address("purushottam nagar", "manuvihar", "Indore", "India"), "Dermatology ",40);

  system.displayDoctorsBySpeciality("Cardiology");

  
    std::cout << "\n>>> Deexant is booking an appointment with Dr. kartik" << std::endl;
  TimeSlot slot1("15_09_2025", "10:00 AM");
  system.createBooking(1, 3, slot1);

  std::cout << "\n>>> Divy is booking an appointment with Dr. Radha" << std::endl;
   TimeSlot slot2("15_09_2025", "10:30 AM");
  system.createBooking(2, 5, slot2 );

  auto deexant = system.getUserById(1);
  if(deexant){
    deexant->viewAppointments();
  }
  auto divy = system.getUserById(2);
  if(divy) {
    divy->viewAppointments();
  }
  return 0;
}