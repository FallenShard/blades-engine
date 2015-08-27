#pragma once

#include <memory>
#include <set>

#include "OpenGL.h"

namespace fsi
{
    struct Event;

    namespace gui
    {
        class Component : public std::enable_shared_from_this<Component>
        {
        public:
            using ComponentPtr = std::shared_ptr<Component>;
            

            Component(glm::vec3 position = glm::vec3(0.f), glm::vec2 size = glm::vec2(1.f));
            virtual ~Component();

            virtual void handleEvents(const Event& event);
            virtual void render(const glm::mat4& P) = 0;

            virtual void setPosition(const glm::vec3& position);
            glm::vec3 getPosition() const;
            glm::vec3 getAbsolutePosition() const;
            
            void setSize(const glm::vec2& size);
            glm::vec2 getSize() const;

            virtual glm::vec4 getBounds() const;

            void addComponent(ComponentPtr component);

        protected:
            void setParent(Component* component);
            virtual void updateModelMatrix();
            
            glm::vec3 m_position;
            glm::vec2 m_size;

            glm::mat4 m_modelMat;

            Component* m_parent;

            std::set<ComponentPtr> m_children;
        };
    }
}
