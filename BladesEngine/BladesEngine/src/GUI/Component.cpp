#include "GUI/Component.h"

namespace fsi
{
    namespace gui
    {
        Component::Component(glm::vec3 position, glm::vec2 size)
            : m_position(position)
            , m_size(size)
            , m_modelMat(glm::translate(m_position) * glm::scale(glm::vec3(m_size, 1.f)))
        {
        }

        Component::~Component()
        {
        }

        glm::vec3 Component::getPosition() const
        {
            return m_position;
        }

        glm::vec3 Component::getAbsolutePosition() const
        {
            glm::vec3 parentAbsPos(0.f);
            if (auto parent = m_parent.lock())
                parentAbsPos = parent->getAbsolutePosition();

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

        void Component::addComponent(ComponentPtr component)
        {
            component->setParent(shared_from_this());
            m_children.insert(component);
        }

        void Component::setParent(ComponentPtr component)
        {
            m_parent = component;
            updateModelMatrix();
        }

        void Component::updateModelMatrix()
        {
            glm::vec3 parentPos(0.f);
            if (auto parent = m_parent.lock())
                parentPos = parent->getAbsolutePosition();

            m_modelMat = glm::translate(m_position + parentPos) * glm::scale(glm::vec3(m_size, 1.f));

            for (auto& child : m_children)
                child->updateModelMatrix();
        }
    }
}