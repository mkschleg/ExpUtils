#include "gtest/gtest.h"
#include "ReporterManager.hh"

#include <sstream>

// class ReporterManagerTest: public ::testing::Test{
//
// protected:
//   virtual void SetUp(){}
//   virtual void TearDown(){}
//
//
//
// };

class TestReportee: public ExpUtils::ReporteeBase{
// class TestReportee{
public:
  TestReportee(const int& i):data(i){}
  TestReportee(const TestReportee& r): data(r.data), storage(r.storage.begin(),r.storage.end()){}
  // TestReportee(TestReportee&& r): data(r.data), storage(r.storage.begin(),r.storage.end()) {r.storage.clear();}

  // virtual ~ReporteeBase() = default;

  void update() override final{
    storage.push_back(data);
    // std::cout<<data<<std::endl;
  }
  void signal(const ExpUtils::Signal& _signal) override final{
    if(_signal.i == 100){
      storage.push_back(30);
    }
  }
  std::ostream& print(std::ostream& os) const override final{
    for(const auto& _dat: storage){
      os<<_dat<<" ";
    }
    os<<std::flush;
    return os;
  }
  void clear() override final{
    storage.clear();
  }
// private:

  const int& data;
  std::vector<int> storage;

};


TEST(ReporterManagerTest, TemplateConstructor){
  ExpUtils::ReporterManager rm;
  int i = 0;
  
  rm.registerReportee<TestReportee>("TestReportee", i);
  
  for(int j = 0; j < 10; j++){
    i = i + j;
    rm.update();
  }
  ExpUtils::Signal s;
  s.i = 100;
  rm.signal(s);

  std::stringstream ss;
  
  ss<<rm;

  std::string str = ss.str();

  EXPECT_EQ(str, "TestReportee\n0 1 3 6 10 15 21 28 36 45 30 ");

}

TEST(ReporterManagerTest, PointerConstructor){
  ExpUtils::ReporterManager rm;
  int i = 0;

  rm.registerReportee("TestReportee", TestReportee(i));

  for(int j = 0; j < 10; j++){
    i = i + j;
    rm.update();
  }
  ExpUtils::Signal s;
  s.i = 100;
  rm.signal(s);
  
  std::stringstream ss;

  ss<<rm;

  std::string str = ss.str();

  EXPECT_EQ(str, "TestReportee\n0 1 3 6 10 15 21 28 36 45 30 ");

}



TEST(ReporterManagerTest, reportee){
  
  ExpUtils::ReporterManager rm;
  int i = 0;
  
  rm.registerReportee<ExpUtils::Reportee<int>>("Reportee", i);

  for(int j = 0; j < 10; j++){
    i = i + j;
    rm.update();
  }
  std::stringstream ss;
  ss<<rm;
  std::string str = ss.str();
  EXPECT_EQ(str, "Reportee\n0\n1\n3\n6\n10\n15\n21\n28\n36\n45\n");

}

TEST(ReporterManagerTest, multipleReportees){

  ExpUtils::ReporterManager rm;
  std::pair<int,int> p{0,0};



  rm.registerReportee<ExpUtils::Reportee<int>>("Reportee", p.first);

  
  //Due to my lack of knowledge as of now you have to declare functionals before initializing with the templates....
  std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*)> init = [](ExpUtils::Reportee<std::pair<int,int>,int>*) {return;};
  std::function<bool (ExpUtils::Reportee<std::pair<int,int>,int>*)> toStore = [](ExpUtils::Reportee<std::pair<int,int>,int>*) -> bool {return true;};
  std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*, const std::pair<int,int>&)> store = [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const std::pair<int,int>& data) -> void{reportee->m_data.push_back(data.second);};
  std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*, const ExpUtils::Signal&)> signal = [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const ExpUtils::Signal& s) -> void{};
  std::function<void (const ExpUtils::Reportee<std::pair<int,int>,int>*,std::ostream&)> print = [](const ExpUtils::Reportee<std::pair<int,int>,int>* reportee,std::ostream& os) -> void{ for(const auto& d : reportee->m_data) os<<d<<" ";};
  rm.registerReportee<ExpUtils::Reportee<std::pair<int,int>,int>>("Reportee2", p, init, toStore, store, signal, print);

  for(int j = 0; j < 10; j++){
    p.second++;
    p.first = p.first + j;
    rm.update();
  }
  std::stringstream ss;
  ss<<rm;
  std::string str = ss.str();
  EXPECT_EQ(str, "Reportee\n0\n1\n3\n6\n10\n15\n21\n28\n36\n45\nReportee2\n1 2 3 4 5 6 7 8 9 10 ");

}

ExpUtils::ReporteeBase* func(std::pair<int,int>& p){
  return new ExpUtils::Reportee<std::pair<int,int>,int>{
    p,
    [](ExpUtils::Reportee<std::pair<int,int>,int>*) {return;},
    [](ExpUtils::Reportee<std::pair<int,int>,int>*) -> bool {return true;},
    [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const std::pair<int,int>& data) -> void{reportee->m_data.push_back(data.second);},
    [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const ExpUtils::Signal& s) -> void{},
    [](const ExpUtils::Reportee<std::pair<int,int>,int>* reportee,std::ostream& os) -> void{ for(const auto& d : reportee->m_data) os<<d<<" ";}
  };
}



TEST(ReporterManagerTest, reporteeFromFunction){

  ExpUtils::ReporterManager rm;
  std::pair<int,int> p{0,0};



  // rm.registerReportee<ExpUtils::Reportee<int>>("Reportee", p.first);
  // 
  // 
  // //Due to my lack of knowledge as of now you have to declare functionals before initializing with the templates....
  // std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*)> init = [](ExpUtils::Reportee<std::pair<int,int>,int>*) {return;};
  // std::function<bool (ExpUtils::Reportee<std::pair<int,int>,int>*)> toStore = [](ExpUtils::Reportee<std::pair<int,int>,int>*) -> bool {return true;};
  // std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*, const std::pair<int,int>&)> store = [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const std::pair<int,int>& data) -> void{reportee->m_data.push_back(data.second);};
  // std::function<void (ExpUtils::Reportee<std::pair<int,int>,int>*, const ExpUtils::Signal&)> signal = [](ExpUtils::Reportee<std::pair<int,int>,int>* reportee, const ExpUtils::Signal& s) -> void{};
  // std::function<void (const ExpUtils::Reportee<std::pair<int,int>,int>*,std::ostream&)> print = [](const ExpUtils::Reportee<std::pair<int,int>,int>* reportee,std::ostream& os) -> void{ for(const auto& d : reportee->m_data) os<<d<<" ";};

  rm.registerReportee("Reportee", func(p));

  for(int j = 0; j < 10; j++){
    p.second++;
    p.first = p.first + j;
    rm.update();
  }
  std::stringstream ss;
  ss<<rm;
  std::string str = ss.str();
  EXPECT_EQ(str, "Reportee\n1 2 3 4 5 6 7 8 9 10 ");

}
