#pragma once
#include <iostream>
#include <vector>
#include "Debug.h"

namespace SpaRcle {
	namespace Helper {
		class EventsManager {
		public:
			enum class Event {
				None, Exit, Error
			};
		private:
			inline static std::vector<Event>	 m_events		= std::vector<Event>();
			inline static unsigned long			 m_count_events = 0;
		private:
			EventsManager()  { };
			~EventsManager() { };
		public:
			static void PushEvent(Event _event) {
				EventsManager::m_count_events++;
				EventsManager::m_events.push_back(_event);
			}
			static Event PopEvent() {
				if (m_count_events > 0) {
					try {
						Event ev = m_events[m_count_events - 1];

						EventsManager::m_count_events--;
						m_events.pop_back();

						return ev;
					}
					catch (...) {
						Debug::Error("EventsManager::PopEvent() : unknown error!");
						return Event::None;
					}
				}
				else return Event::None;
			};
		};
	}
}

