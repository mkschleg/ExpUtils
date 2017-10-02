#pragma once

#include "SaveManager.h"
// #include "miscutils.h"
#include "Memory.h"


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

    virtual void store() = 0;
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
      std::function<bool (Reportee<DataType, StorageType>*)> _toStore,
      std::function<void (Reportee<DataType, StorageType>* reportee, const DataType& data)> _store,
      std::function<void (Reportee<DataType, StorageType>* reportee, const Signal& s)> _signal,
      std::function<void (const Reportee<DataType,StorageType>*,std::ostream&)> _print
    ): m_dataRef(dataRef), m_toStore(_toStore), m_signal(_signal), m_store(_store), m_print(_print){}
    

    Reportee(Reportee&& reportee):m_dataRef(std::move(reportee.m_dataRef)){}
    Reportee(const Reportee& reportee){}
    ~Reportee(){}

    void store() final override{
      if(m_toStore(this)){
        m_store(this,m_dataRef);
      }
    }
    void signal(const Signal& _signal) final override{
      m_signal(this, _signal);
    }
    std::ostream& print(std::ostream& os) const final override{
      m_print(this, os);
      return os;
    }
    void clear() final override{
      m_data.clear();
    }

    void initializeSize(size_t size){ m_data.resize(size); }

    // void set_print(std::function<void (const Reportee<DataType,StorageType>*,std::ostream&)> _func) { m_print = _func; }
    // void set_toStore(std::function<bool (Reportee<DataType, StorageType>*)> _func) { m_toStore = _func; }
    // void set_store(std::function<void (const Reportee<DataType,StorageType>* r, const DataType& data)> _func) { m_store = _func; }
    // void set_signal(std::function<void (Reportee<DataType, StorageType>* reportee, const Signal& s)> _func) { m_signal = _func; }

  // protected:
    std::vector<size_t> m_steps;
    std::vector<StorageType> m_data;
    const DataType& m_dataRef;

     std::function<bool (Reportee<DataType, StorageType>*)> m_toStore = [](Reportee<DataType, StorageType>* reportee){ return true; };

     std::function<void (Reportee<DataType, StorageType>* reportee, const Signal& s)> m_signal = [](Reportee<DataType, StorageType>* reportee, const Signal& s){};

     std::function<void (Reportee<DataType,StorageType>* r, const DataType& data)> m_store = [](Reportee<DataType,StorageType>* r, const DataType& data){
       r->m_data.push_back(data);
     };

     std::function<void (const Reportee<DataType,StorageType>*,std::ostream&)> m_print = [](const Reportee<DataType,StorageType>* r, std::ostream& os){
       for(int i = 0; i < r->m_data.size(); i++){
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
      static_assert(std::is_base_of<ReporteeBase, _Reportee>::value, "Registering a Reportee requires reportee to be derived from ExpUtils::ReporteeBase");
      static_assert(std::is_move_constructible<_Reportee>::value, "Reportee must be move constructable");
      reporteeMap[id] = ExpUtils::Memory::make_unique<_Reportee>(std::move(reportee));
    }
    // 
    template <typename _Reportee>
    void registerReportee(std::string id, const _Reportee& reportee){
      static_assert(std::is_base_of<ReporteeBase, _Reportee>::value, "Registering a Reportee requires reportee to be derived from ExpUtils::ReporteeBase");
      static_assert(std::is_copy_constructible<_Reportee>::value, "Reportee must be move constructable");
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
      for(auto& reportee: reporteeMap) reportee.second->store();
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

}
