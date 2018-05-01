#pragma once

#include "SaveManager.hh"
// #include "miscutils.h"
#include "Memory.hh"


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <initializer_list>
#include <type_traits>


namespace ExpUtils{
  
  namespace{
    
    template <typename T>
    struct identity
    {
      typedef T type;
    };

  }

  struct Signal{
    int i;
    std::string s;
  };

  //! \class ReporteeBase
  //! \brief Used as the base class for Reportee for storage purposes
  class ReporteeBase{
  public:

    virtual ~ReporteeBase() = default;
    
    // virtual void init() = 0;
    virtual void update() = 0;
    virtual void signal(const Signal&) = 0;
    virtual std::ostream& print(std::ostream&) const = 0;
    virtual void clear() = 0;

  };

  inline std::ostream& operator<<(std::ostream& os, const ReporteeBase& reportee){
    return reportee.print(os);
  }
  
  template<typename DataType, typename StorageType = DataType>
  class Reportee: public ReporteeBase{
  public:
    Reportee(const DataType& dataRef):m_dataRef(dataRef){}

    Reportee(
      const DataType& dataRef,
      std::function<void (Reportee<DataType, StorageType>*)> _init,
      std::function<bool (Reportee<DataType, StorageType>*)> _toStore,
      std::function<void (Reportee<DataType, StorageType>* reportee, const DataType& data)> _store,
      std::function<void (Reportee<DataType, StorageType>* reportee, const Signal& s)> _signal,
      std::function<void (const Reportee<DataType,StorageType>*,std::ostream&)> _print
    ): m_dataRef(dataRef), m_init(_init),m_toStore(_toStore), m_signal(_signal), m_store(_store), m_print(_print){
      m_init(this);
    }
    

    Reportee(Reportee&& reportee):
      m_steps(std::move(reportee.m_steps)), m_data(std::move(reportee.m_data)), m_dataRef(std::move(reportee.m_dataRef)),
      m_init(std::move(reportee.m_init)),m_toStore(std::move(reportee.m_toStore)),m_signal(std::move(reportee.m_signal)),
      m_store(std::move(reportee.m_store)),m_print(std::move(reportee.m_print)){}
    Reportee(const Reportee& reportee):
      m_steps(reportee.m_steps), m_data(reportee.m_data), m_dataRef(reportee.m_dataRef),
      m_init(reportee.m_init),m_toStore(reportee.m_toStore),m_signal(reportee.m_signal),
      m_store(reportee.m_store),m_print(reportee.m_print){}
    ~Reportee(){}

    void update() final override{
      if(m_toStore(this)){
        m_store(this,m_dataRef);
      }
    }
    void signal(const Signal& _signal) final override{ m_signal(this, _signal); }
    std::ostream& print(std::ostream& os) const final override{
      m_print(this, os);
      return os;
    }
    void clear() final override{
      m_data.clear();
      std::fill(m_steps.begin(), m_steps.end(), 0);
    }
    
    void init(){ m_init(this); }

    std::vector<size_t> m_steps;
    std::vector<StorageType> m_data;
    const DataType& m_dataRef;

    std::function<void (Reportee<DataType, StorageType>*)> m_init = [](Reportee<DataType, StorageType>*){};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    std::function<bool (Reportee<DataType, StorageType>*)> m_toStore = [](Reportee<DataType, StorageType>* reportee){ return true; };

    std::function<void (Reportee<DataType, StorageType>* reportee, const Signal& s)> m_signal = [](Reportee<DataType, StorageType>* reportee, const Signal& s){};
#pragma clang diagnostic pop
    
    std::function<void (Reportee<DataType,StorageType>* r, const DataType& data)> m_store = [](Reportee<DataType,StorageType>* r, const DataType& data){
      r->m_data.push_back(data);
    };

    std::function<void (const Reportee<DataType,StorageType>*,std::ostream&)> m_print = [](const Reportee<DataType,StorageType>* r, std::ostream& os){
      for(size_t i = 0; i < r->m_data.size(); i++){
        os<<r->m_data[i]<<'\n';
      }
    };
  };


          

  class ReporterManager{
    friend SaveManager& operator<<(SaveManager& sm, const ReporterManager& rm);
    friend std::ostream& operator<<(std::ostream& os, const ReporterManager& rm);
  public:
    

    // Reportee Needs to resolve to a ReporteeBase.
    template <typename _Reportee>
    void registerReportee(std::string id, _Reportee&& reportee){
      //Reportee must resolve to ReporteeBase
      static_assert(std::is_base_of<ReporteeBase, _Reportee>::value, "Registering a Reportee requires reportee to be derived from ExpUtils::ReporteeBase");
      //Reportee must be move constructable!
      static_assert(std::is_move_constructible<_Reportee>::value, "Reportee must be move constructable");
      //Assign reportee to map of reportees from map[id] = reportee;
      reporteeMap[id] = ExpUtils::Memory::make_unique<_Reportee>(std::move(reportee));
    }

    template <typename _Reportee>
    void registerReportee(std::string id, const _Reportee& reportee){
      static_assert(std::is_base_of<ReporteeBase, _Reportee>::value, "Registering a Reportee requires reportee to be derived from ExpUtils::ReporteeBase");
      static_assert(std::is_copy_constructible<_Reportee>::value, "Reportee must be copy constructable");
      reporteeMap[id] = ExpUtils::Memory::make_unique<_Reportee>(reportee);
    }
    // 
    template <typename _Reportee, typename ...Args>
    void registerReportee(std::string id, Args&... args){
      static_assert(std::is_base_of<ReporteeBase, _Reportee>::value, "Registering a Reportee requires to be derived from ExpUtils::ReporteeBase");
      reporteeMap[id] = ExpUtils::Memory::make_unique<_Reportee>(std::forward<Args>(args)...);
    }

    void registerReportee(std::string id, std::unique_ptr<ReporteeBase>&& reportee){
      reporteeMap[id] = std::move(reportee);
    }


    void update(){
      for(auto& reportee: reporteeMap) reportee.second->update();
    }

    void clear(){
      for(auto& reportee: reporteeMap) reportee.second->clear();
    }

    void signal(const Signal& _signal){
      for(auto& reportee: reporteeMap) reportee.second->signal(_signal);
    }

    void setFileType(std::string ft){fileType = ft;}
    



  private:

    std::map<std::string, std::unique_ptr<ReporteeBase>> reporteeMap;
    std::string fileType = ".dat";
  };
  
  inline std::ostream& operator<<(std::ostream& os, const ReporterManager& rm){
    for(auto& _reportee: rm.reporteeMap){
      os<<_reportee.first<<'\n'<<*_reportee.second;
    }
    return os<<std::flush;
    // for(auto& p: rm.reporteeMap){
    //   sm<<(p.first + rm.fileType), *p.second;
    // }
    // return sm;
  }
  inline SaveManager& operator<<(SaveManager& sm, const ReporterManager& rm){
    for(auto& _reportee: rm.reporteeMap){
      sm<<(_reportee.first + rm.fileType), *_reportee.second;
    }
    return sm;
  }
  //Specialization of ReporterManager::registerReportee for already cast pointers.
  template<>
  inline void ReporterManager::registerReportee<ReporteeBase*>(std::string id, ReporteeBase* && reportee){
    reporteeMap[id] = std::unique_ptr<ReporteeBase>(std::move(reportee));
  }

}
