#include "StateMachine.hpp"
#include <stdexcept>

namespace LaserWave
{

StateMachine::StateMachine(GameDataRef data) : m_data(std::move(data)) {}

void StateMachine::processRequests()
{
    for (auto &&request : this->m_requests)
      this->processRequest(request);
    if (!this->m_requests.empty() && !this->empty()) {
      this->getTopState().asTopState();
      this->getTopState().resume();
    }
    this->m_requests.clear();
}

void StateMachine::processRequest(const StateRequest &request)
{
    request(*this);
}

} // namespace LaserWave