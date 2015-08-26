#pragma once

#include <memory>
#include <set>

#include "OpenGL.h"

namespace fsi
{
    namespace gui
    {
        class Component : public std::enable_shared_from_this<Component>
        {
        public:
            using ComponentPtr = std::shared_ptr<Component>;
            

            Component(glm::vec3 position = glm::vec3(0.f), glm::vec2 size = glm::vec2(1.f));
            virtual ~Component();

            virtual void render(const glm::mat4& P) = 0;

            glm::vec3 getPosition() const;
            glm::vec3 getAbsolutePosition() const;
            void setPosition(const glm::vec3& position);
            void setSize(const glm::vec2& size);

            void addComponent(ComponentPtr component);
            

        protected:
            using ComponentWeakPtr = std::weak_ptr<Component>;

            void setParent(ComponentPtr component);
            void updateModelMatrix();
            
            glm::vec3 m_position;
            glm::vec2 m_size;

            glm::mat4 m_modelMat;

            ComponentWeakPtr m_parent;

            std::set<ComponentPtr> m_children;
        };
    }
}
