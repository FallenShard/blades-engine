#include "Input/Event.h"

#include "GUI/Component.h"

namespace fsi
{
    namespace gui
    {
        Component::Component(glm::vec3 position, glm::vec2 size)
            : m_position(position)
            , m_size(size)
            , m_modelMat(glm::translate(m_position) * glm::scale(glm::vec3(m_size, 1.f)))
            , m_parent(nullptr)
        {
        }

        Component::~Component()
        {
        }

        void Component::handleEvents(const Event& event)
        {
        }

        glm::vec3 Component::getPosition() const
        {
            return m_position;
        }

        glm::vec3 Component::getAbsolutePosition() const
        {
            glm::vec3 parentAbsPos(0.f);
            if (m_parent)
                parentAbsPos = m_parent->getAbsolutePosition();

            return m_position + parentAbsPos;
        }

        void Component::setPosition(const glm::vec3& position)
        {
            m_position = position;
            updateModelMatrix();
        }

        void Component::setSize(const glm::vec2& size)
        {
            m_size = size;
            updateModelMatrix();
        }

        glm::vec2 Component::getSize() const
        {
            return m_size;
        }

        glm::vec4 Component::getBounds() const
        {
            glm::vec3 absPos = getAbsolutePosition();
            glm::vec4 bounds;
            bounds.x = absPos.x;
            bounds.y = absPos.y;
            bounds.z = absPos.x + m_size.x;
            bounds.w = absPos.y + m_size.y;
            return bounds;
        }

        void Component::addComponent(ComponentPtr component)
        {
            component->setParent(this);
            m_children.insert(component);
        }

        void Component::setParent(Component* component)
        {
            m_parent = component;
            updateModelMatrix();
        }

        void Component::updateModelMatrix()
        {
            glm::vec3 parentPos(0.f);
            if (m_parent)
                parentPos = m_parent->getAbsolutePosition();

            m_modelMat = glm::translate(m_position + parentPos) * glm::scale(glm::vec3(m_size, 1.f));

            for (auto& child : m_children)
                child->updateModelMatrix();
        }
    }
}