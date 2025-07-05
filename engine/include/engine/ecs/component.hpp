#pragma once

#include <SDL2/SDL.h>

#include <cstdint>
#include <typeindex>
#include <unordered_map>

#include <memory>
 
  
namespace engine{
  using Entity = uint32_t;

  template<typename T>
  class ComponentStorage {
  public:
      std::unordered_map<Entity, T> components;

      void add(Entity entity, const T& component); 

      void remove(Entity entity) ;

      T& get(Entity entity) ;

      bool has(Entity entity);
  };
  class ComponentManager {
  private:
      std::unordered_map<std::type_index, std::shared_ptr<void>> storages;

  public:
      template<typename T>
      void registerComponent();

      template<typename T>
      ComponentStorage<T>& getStorage(); 
  };
  //MANAGER
  template<typename T>
  void ComponentManager::registerComponent(){
    std::type_index type = std::type_index(typeid(T));
    storages[type] = std::make_shared<ComponentStorage<T>>();
  }

  template<typename T>
  ComponentStorage<T>& ComponentManager::getStorage(){
    std::type_index type = std::type_index(typeid(T));
    return *std::static_pointer_cast<ComponentStorage<T>>(storages[type]);

  }

  //STORAGE
  //
  template<typename T>
  void ComponentStorage<T>::add(Entity entity, const T& component) {
      components[entity] = component;
  }

  template<typename T>
  void ComponentStorage<T>::remove(Entity entity) {
      components.erase(entity);
  }

  template<typename T>
  T& ComponentStorage<T>::get(Entity entity) {
      return components.at(entity);
  }

  template<typename T>
  bool ComponentStorage<T>::has(Entity entity) {
      return components.find(entity) != components.end();
  }


} 
