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
}
