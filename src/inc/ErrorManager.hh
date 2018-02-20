#pragma once


#include "SaveManager.hh"
#include "Memory.hh"

#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

namespace ExpUtils{

  template<typename TruthType>
  class ErrorBase{
    template<typename _>
    friend class ErrorManager;
  public:
    virtual void operator()(const TruthType& truth) = 0;
    virtual void clear() = 0;
    virtual std::ostream& print(std::ostream& os) const = 0;
  };

  

  template<typename TruthType, typename PredType, typename ErrType>
  class ErrorFunction: public ErrorBase<TruthType>{
    template<typename _>
    friend class ErrorManager;
  public:
    ErrorFunction(
      const PredType& measuredRef, 
      std::function<ErrType (const ErrorFunction<TruthType, PredType, ErrType>*,const PredType& pred, const TruthType& truth)> calcError,
      int experimentSteps,
      int trackSteps
    ): m_measuredReference(measuredRef), m_calcError(calcError),m_experimentSteps(experimentSteps), m_currStep(0)
    {
      m_error.resize(experimentSteps);
    }
    
    ErrorFunction(
      const PredType& measuredRef, 
      std::function<ErrType (const ErrorFunction<TruthType, PredType, ErrType>*,const PredType& pred, const TruthType& truth)> calcError, 
      std::function<void (const ErrorFunction<TruthType, PredType, ErrType>*, std::ostream&)> p_print, 
      int experimentSteps, 
      int trackSteps
    ): m_measuredReference(measuredRef), m_calcError(calcError), m_print(p_print),m_experimentSteps(experimentSteps), m_currStep(0)
    {
      m_error.resize(experimentSteps);
    }
      
    void operator()(const TruthType& truth) override{
      m_error[m_currStep++] = m_calcError(this, m_measuredReference, truth);
    }

    void clear() override{
      m_clear(this);
    }
    
    std::ostream& print(std::ostream& os) const override{
      m_print(this,os);
      return os;
    }
      
    // private:
    
    std::vector<ErrType> m_error;
    const PredType& m_measuredReference;
    // const T& truth;
    
    std::function<ErrType (const ErrorFunction<TruthType, PredType, ErrType>*, const PredType& pred, const TruthType& truth)> m_calcError
    = [](const ErrorFunction<TruthType, PredType, ErrType>* ef, const PredType& pred, const TruthType& truth){
      std::cerr<<"m_CalcError Is not assigned. Please assign."<<std::endl;
    };
    std::function<void (const ErrorFunction<TruthType, PredType, ErrType>*, std::ostream&)> m_print = [](const ErrorFunction<TruthType, PredType, ErrType>* ef, std::ostream& os){
      for(const auto& v: ef->m_error){
        os<<v<<"\n";
      }
      os<<std::flush;
    };
    std::function<void (ErrorFunction<TruthType, PredType, ErrType>*)> m_clear = [](ErrorFunction<TruthType, PredType, ErrType>* ef){
      ef->m_currStep = 0;
      ef->m_error.clear();
      ef->m_error.resize(ef->m_experimentSteps);
    };
    
    int m_experimentSteps;
    int m_currStep = 0;
    
  };

  template<typename T>
  std::ostream& operator<<(std::ostream& os, const ErrorBase<T>& eb){
    return eb.print(os);
  }
  
  
  template<typename TruthType>
  class ErrorManager{
  public:
    
    std::map<std::string, std::unique_ptr<ErrorBase<TruthType>>> errorsMap;
    std::string fileType = ".dat";
    
    void setFileType(std::string ft){fileType = ft;}
    
    template<typename PredType, typename ErrType>
    void registerErrorFunction(
      std::string id,
      const PredType& measuredRef,
      std::function<ErrType (const ErrorFunction<TruthType, PredType, ErrType>*,const PredType&, const TruthType&)> calcError, 
      int experimentSteps, 
      int trackSteps)
    {
      errorsMap[id] = Memory::make_unique<ErrorFunction<TruthType, PredType, ErrType>>(measuredRef, calcError, experimentSteps, trackSteps);
    }

    template<typename PredType, typename ErrType>
    void registerErrorFunction(
      std::string id, 
      const PredType& measuredRef, 
      std::function<ErrType (const ErrorFunction<TruthType, PredType, ErrType>*,const PredType&, const TruthType&)> calcError, 
      std::function<void (const ErrorFunction<TruthType, PredType, ErrType>*, std::ostream&)> p_print, 
      int experimentSteps, 
      int trackSteps)
    {
      errorsMap[id] = Memory::make_unique<ErrorFunction<TruthType, PredType, ErrType>>(measuredRef, calcError, p_print, experimentSteps, trackSteps);
    }

    // void registerErrorFunction(std::string id, ErrorBase<TruthType>* &error){
    //   errorsMap[id] = std::unique_ptr<ErrorBase<TruthType>>(error);
    //   error = nullptr;
    // }

    void registerErrorFunction(std::string id, ErrorBase<TruthType>* error){
      errorsMap[id] = std::unique_ptr<ErrorBase<TruthType>>(error);
      // error = nullptr;
    }

    void update(const TruthType& truth){
      for(auto& errPair: errorsMap){
        errPair.second->operator()(truth);
      }
    }
    
    
    void save(SaveManager& smi, std::string dir, std::string fileType = ".dat"){
      if(dir != "" && dir.back() != '/'){
        dir.push_back('/');
      }
      for(auto& p: errorsMap){
        smi<<(dir + p.first + fileType),*p.second;
      }
    }
    
    void clear(){
      for(auto& p: errorsMap){
        p.second->clear();
      }
    }
  };

  template<typename truthType>
  SaveManager& operator<<(SaveManager& sm, const ErrorManager<truthType>& em){
    for(auto& p: em.errorsMap){
      sm<<(p.first+em.fileType),*p.second;
    }
    return sm;
  }
}
